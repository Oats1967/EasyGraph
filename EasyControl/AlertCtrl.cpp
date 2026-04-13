//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlertCtrl
///
/// @file   AlertCtrl.cpp
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
#include "EasyControl.h"
#include "AlertCtrl.h"
#include "AlarmMap.h"
#include "MFCMacros.h"



#define ICON_SIZEX 16
#define ICON_SIZEY 32
#define IMAGE_MAX 4

//*****************************************************************************************************
//*****************************************************************************************************
CAlertCtrl::CAlertCtrl(CWnd* pParent, const base::utils::CLogItemList& rLoglist) : CAlarmCtrl(pParent)
	, c_rLoglist { rLoglist }
{
	CreateImageList();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlertCtrl::CreateImageList()
{
	m_ImageList.Create(ICON_SIZEX, ICON_SIZEY, ILC_COLOR, 0, IMAGE_MAX); // 16, 32 for large icons
	HICON aIcon = AfxGetApp()->LoadIcon(IDI_ICONWARNINGBIG);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONWARNINGBIG);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONALARMBIG);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONALARMINFO);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
}
//*****************************************************************************************************
//*****************************************************************************************************
CString CAlertCtrl::CreateAlarmString(const std::string& szMessage, const time_t& t0)
{
	CTime aTime(t0);
	CString szTime = aTime.Format("%H:%M:%S; ");
	szTime += toCString(szMessage);
	return std::move(szTime);
}

//*****************************************************************************************************
//*****************************************************************************************************
void CAlertCtrl::FillAlertCtrl(const int32_t sItem )
{
	const uint32_t Count = c_rLoglist.GetCount();
	int32_t count = 0;
	DeleteAllItems();
	for (uint32_t aPos = 0; aPos < Count; aPos++)
	{
		const auto& aItem = c_rLoglist.GetItem(aPos);
		if ((aItem.GetItem() != sItem) || (aItem.GetChecked()))
		{
			continue;
		}

		CString szFormat;
		szFormat.Format(_T("%d"), count + 1);

		LVITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
		lvi.iItem = count;
		lvi.iSubItem = 0;
		lvi.pszText = LPTSTR(LPCTSTR(szFormat));
		lvi.iImage = _S32(aItem.GetAlarmClass()); // 0, 1, 2
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.cchTextMax = szFormat.GetLength();
		lvi.state = INDEXTOSTATEIMAGEMASK(1);
		lvi.lParam = _S32(aPos);

		InsertItem(&lvi);

		SetItemText(count, 1, toCString(aItem.GetMessage()));

		int32_t pos = OFFSETCATAGORY;
		DECLARE_ALARMCATEGORY(categoryfield)
		for (const auto& category : categoryfield)
		{
			BOOL bA = FALSE;
			REMOTEREF.getDoseAlarmCfgBit(sItem, category, aItem.GetAlarm(), bA);
			if (bA)
			{
				SetItemText(count, pos, SZMARKED);
			}
			pos++;
		}
		lvi.iImage = 3;
		lvi.mask = LVIF_IMAGE | LVIF_STATE;
		lvi.iSubItem = pos;
		SetItem(&lvi);
		count++;
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CAlertCtrl::OnLvnItemActivateAlarmList(NMHDR* pNMHDR)
{
	BOOL modified = FALSE;
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	auto pos = _U32(GetItemData(pNMIA->iItem));
	const uint32_t Count = c_rLoglist.GetCount();
	if (pos < Count)
	{
		const auto& rItem = c_rLoglist.GetItem(pos);
		if (pNMIA->iSubItem >= CAlarmCtrl::OFFSETCATAGORY)
		{
			if (pNMIA->iSubItem >= CAlarmCtrl::OFFSETCATAGORY + base::eAlarmCategoryMax)
			{
				CreateHelpInfoBox(CPoint(pNMIA->ptAction), rItem.GetAlarm());
			}
		}
		else
		{
			CreateToolTip(CPoint(0, pNMIA->ptAction.y - 40), CreateAlarmString(rItem.GetMessage(), rItem.GetTime()));
		}
	}
	return modified;
}





