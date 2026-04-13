//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmCtrl
///
/// @file   AlarmCtrl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "AlarmConfigCtrl.h"
#include "AlarmMap.h"
#include "MFCMacros.h"


#define ICON_SIZEX 16
#define ICON_SIZEY 32
#define IMAGE_MAX 2

//*****************************************************************************************************
//*****************************************************************************************************
CAlarmConfigCtrl::CAlarmConfigCtrl(CWnd* pParent, base::CAlarmField& rAlarmField) : CAlarmCtrl(pParent), m_rAlarmField{ rAlarmField }
{
	CreateImageList();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlarmConfigCtrl::CreateImageList()
{
	m_ImageList.Create(ICON_SIZEX, ICON_SIZEY, ILC_COLOR, 0, IMAGE_MAX); // 16, 32 for large icons
	HICON aIcon = AfxGetApp()->LoadIcon(IDI_ICONALARMBIG);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONALARMINFO);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlarmConfigCtrl::FillAlarmCtrl(const base::eAlarmErrorBits& rMask)
{
    DeleteAllItems();
    int32_t count = 0;
    DECLARE_ALARMERROR(alarmfield);
    for (const auto& alarmerror : alarmfield)
    {
        if (rMask.IsAlarmErrorBits(alarmerror))
        {
			CString szFormat;
            szFormat.Format(_T("%d"), _S32(alarmerror) + 1);

            LVITEM lvi;
            lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
            lvi.iItem = count;
            lvi.iSubItem = 0;
            lvi.pszText = LPTSTR(LPCTSTR(szFormat));
            lvi.iImage = 0;
            lvi.stateMask = LVIS_STATEIMAGEMASK;
            lvi.cchTextMax = szFormat.GetLength();
            lvi.state = INDEXTOSTATEIMAGEMASK(1);
            lvi.lParam = _S32(alarmerror);
            InsertItem(&lvi);

            auto szMessage = CAlarmMap::GetAlarmString(alarmerror);
            SetItemText(count, 1, szMessage);

            int32_t pos = OFFSETCATAGORY;
            DECLARE_ALARMCATEGORY(categoryfield)
            for (const auto& category : categoryfield)
            {
                BOOL bA = m_rAlarmField.Get(alarmerror, category);
                SetItemText(count, pos, (bA) ? SZMARKED : SZEMPTY);
                pos++;
            }
            lvi.iImage = 1;
			lvi.mask = LVIF_IMAGE  | LVIF_STATE;
            lvi.iSubItem = pos;
            SetItem(&lvi);
            count++;
        }
    }
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CAlarmConfigCtrl::SetAlarm(const base::eAlarmError alarmerror, base::eAlarmCategory category, const BOOL value)
{
	m_rAlarmField.Set(alarmerror, category, value);
	m_rAlarmField.SetModified(TRUE);
	m_Change.alarmerror = alarmerror;
	m_Change.category = category;
	m_Change.value = value;
	ASSERT(m_pParent);
	m_pParent->SendMessage(WM_NOTIFY_ALARMERROR, WPARAM(&m_Change));
	return TRUE;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmConfigCtrl::Set(const int32_t item, const base::eAlarmError alarmerror, const base::eAlarmCategory category, const BOOL value)
{
	const int32_t subitem = _S32(category) + CAlarmCtrl::OFFSETCATAGORY;
	SetAlarm(alarmerror, category, value);
	SetItemText(item, subitem, (value) ? SZMARKED : SZEMPTY);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CAlarmConfigCtrl::OnLvnItemActivateAlarmList(NMHDR* pNMHDR)
{
	BOOL modified = FALSE;
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	auto alarmerror = static_cast<base::eAlarmError>(GetItemData(pNMIA->iItem));
	if (pNMIA->iSubItem >= CAlarmCtrl::OFFSETCATAGORY)
	{
		if (pNMIA->iSubItem < CAlarmCtrl::OFFSETCATAGORY + base::eAlarmCategoryMax)
		{
			if (m_PermissionModify && (alarmerror != base::eAlarmError::ERROR_DOSE_SHUTDOWN && (alarmerror != base::eAlarmError::ERROR_DOSE_ALARMSTOP)))
			{
				auto category = static_cast<base::eAlarmCategory>(pNMIA->iSubItem - CAlarmCtrl::OFFSETCATAGORY);
				BOOL bActiveAM = m_rAlarmField.Get(alarmerror, base::eAlarmCategory::categoryAM);
				if (category == base::eAlarmCategory::categoryAM)
				{
					bActiveAM = !bActiveAM;
					Set(pNMIA->iItem, alarmerror, category, bActiveAM);
					if (!bActiveAM)
					{
						for (int32_t i = 1; i < base::eAlarmCategoryMax; i++)
						{
							Set(pNMIA->iItem, alarmerror, static_cast<base::eAlarmCategory>(i), FALSE);
						}
					}
				}
				else
				{
					if (bActiveAM)
					{
						BOOL bActive = m_rAlarmField.Get(alarmerror, category);
						bActive = !bActive;
						Set(pNMIA->iItem, alarmerror, category, bActive);
						if ((category == base::eAlarmCategory::categoryS) && bActive)
						{
							Set(pNMIA->iItem, alarmerror, base::eAlarmCategory::categorySD, FALSE);
						}
						else if ((category == base::eAlarmCategory::categorySD) && bActive)
						{
							Set(pNMIA->iItem, alarmerror, base::eAlarmCategory::categoryS, FALSE);
						}
					}
				}
			}
		}
		else
		{
			CreateHelpInfoBox(CPoint(pNMIA->ptAction), alarmerror);
		}
	}
	else
	{
		CreateToolTip(CPoint(0, pNMIA->ptAction.y - 40), alarmerror);
	}
	return modified;
}
//****************************************************************************************************
//****************************************************************************************************
void CAlarmConfigCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//for this notification, the structure is actually a
	// NMLVCUSTOMDRAW that tells you what's going on with the custom
	// draw action. So, we'll need to cast the generic pNMHDR pointer.
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	//int32_t index = lplvcd->

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;          // ask for subitem notifications.
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM: // recd when CDRF_NOTIFYSUBITEMDRAW is returned in
	{                                    // response to CDDS_ITEMPREPAINT.
		int32_t iCategory = lplvcd->iSubItem - 2;
		if (iCategory < 0)
		{
			*pResult = CDRF_DODEFAULT;
		}
		else if (iCategory >= base::eAlarmCategoryMax)
		{
			lplvcd->clrTextBk = RGBCOLORWHITE;
			lplvcd->clrText = RGBCOLORWHITE;
			*pResult = CDRF_NEWFONT;
		}
		else
		{
			BOOL bDisable = ! m_PermissionModify;
			if ( ( ! bDisable) && (iCategory > 0))
			{
				const auto item = _S32(lplvcd->nmcd.dwItemSpec);
				auto alarmerror = static_cast<base::eAlarmError>(GetItemData(item));
				bDisable = (alarmerror == base::eAlarmError::ERROR_DOSE_SHUTDOWN) || (alarmerror == base::eAlarmError::ERROR_DOSE_ALARMSTOP);
				if ( ! bDisable)
				{
					bDisable = !m_rAlarmField.Get(alarmerror, base::eAlarmCategory::categoryAM);
				}
			}
			if ( bDisable )
			{
				lplvcd->clrTextBk = DISABLEBKCOLOR;
				lplvcd->clrText = DISABLETEXTCOLOR;
				*pResult = CDRF_NEWFONT;
			}
			else
			{
				*pResult = CDRF_DODEFAULT;
			}
		}
	}
	break;

	default:// it wasn't a notification that was interesting to us.
		*pResult = CDRF_DODEFAULT;
		break;
	}
}







