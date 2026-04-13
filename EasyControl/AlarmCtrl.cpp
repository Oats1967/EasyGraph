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
#include "AlarmCtrl.h"
#include "AlarmMap.h"
#include "HelpInfoDlg.h"
#include "ToolTipDlg.h"
#include "MFCMacros.h"



BOOL CAlarmCtrl::m_bLargeView = FALSE;


//*****************************************************************************************************
//*****************************************************************************************************
CAlarmCtrl::CAlarmCtrl(CWnd* parent) : CListCtrl()
, c_CateorySC{ _T("AM"), _T("FL"), _T("S"), _T("SD"), _T("RA") }
, m_ImageList{}
, m_pParent{ parent }
{
	ASSERT(m_pParent);
}
//*****************************************************************************************************
//*****************************************************************************************************
int32_t CAlarmCtrl::GetColumnWidth()
{
	CRect aRect;
	GetWindowRect(aRect);
	CSize rSize = aRect.Size();
	ASSERT((rSize.cx > 0) && (rSize.cy > 0));
	const int32_t cx = _S32(rSize.cx) - 40 - (base::eAlarmCategoryMax * 40) - 36 - 24;
	return (m_bLargeView) ? _S32(rSize.cx * 0.8F) : __max(cx, 80);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlarmCtrl::BuildHeadLine()
{
	LV_COLUMN lvc;

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES);
	SetImageList(&m_ImageList, LVSIL_SMALL);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.cx = 40;
	lvc.fmt = LVCFMT_RIGHT;
	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_ALARMNR));
	lvc.pszText = (LPTSTR)LPCTSTR(szTemp);
	lvc.cchTextMax = (int32_t)szTemp.GetLength();
	InsertColumn(0, &lvc);

	VERIFY(szTemp.LoadString(IDS_ALARMMELDUNG));
	lvc.cx = GetColumnWidth();
	lvc.iSubItem = 1;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR)LPCTSTR(szTemp);
	lvc.cchTextMax = szTemp.GetLength();
	InsertColumn(1, &lvc);


	int32_t subitem = OFFSETCATAGORY;
	for (int32_t i = 0; i < base::eAlarmCategoryMax; i++)
	{
		lvc.cx = 40;
		lvc.iSubItem = subitem;
		lvc.fmt = LVCFMT_CENTER;
		lvc.pszText = (LPTSTR)LPCTSTR(c_CateorySC[i]);
		lvc.cchTextMax = c_CateorySC[i].GetLength();
		InsertColumn(subitem, &lvc);
		subitem++;
	}
	szTemp = "I";
	lvc.cx = 36;
	lvc.iSubItem = subitem;
	lvc.fmt = LVCFMT_CENTER;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	InsertColumn(subitem, &lvc);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmCtrl::CreateHelpInfoBox(const CPoint& cp, const base::eAlarmError alarmerror)
{
	CRect aRect;
	GetWindowRect(aRect);
	CPoint p0 = aRect.TopLeft();
	p0.Offset(cp.x + 30, cp.y - 50);
	ASSERT(m_pParent);
	CHelpInfoDlg::CreateInfo(m_pParent, p0, CAlarmMap::GetAlarmString(alarmerror), CAlarmMap::GetAlarmInfo(alarmerror));
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmCtrl::CreateHelpInfoBox(const CPoint& cp, const base::eAlarmCategory category)
{
	CRect aRect;
	GetWindowRect(aRect);
	CPoint p0 = aRect.TopLeft();
	p0.Offset(cp);
	CString sz;
	sz.Format(_T("%s = %s"), LPCTSTR(c_CateorySC[_S32(category)]), LPCTSTR(CAlarmMap::GetCategoryString(category)));;
	ASSERT(m_pParent);
	CHelpInfoDlg::CreateInfo(m_pParent, p0, sz, CAlarmMap::GetCategoryInfo(category));
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmCtrl::CreateToolTip(const CPoint& cp, const base::eAlarmError alarmerror)
{
	CreateToolTip(cp, CAlarmMap::GetAlarmString(alarmerror));
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmCtrl::CreateToolTip(const CPoint& cp, const CString& szMessage)
{
	CRect aRect;
	GetWindowRect(aRect);
	CPoint p0 = aRect.TopLeft();
	p0.Offset(cp.x, cp.y - 20);
	ASSERT(m_pParent);
	CToolTipDlg::CreateInfo(m_pParent, p0, szMessage);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmCtrl::OnColumnclickAlarmList(NMHDR* pNMHDR)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA->iSubItem >= CAlarmCtrl::OFFSETCATAGORY)
	{
		int32_t iSubItem = pNMIA->iSubItem - CAlarmCtrl::OFFSETCATAGORY;
		if (iSubItem < base::eAlarmCategoryMax)
		{
			CreateHelpInfoBox(CPoint(170 + (iSubItem + 1) * 40, 0), static_cast<base::eAlarmCategory>(iSubItem));
		}
	}
	else if (pNMIA->iSubItem == 1)
	{
		m_bLargeView = !m_bLargeView;
		SetColumnWidth(1, GetColumnWidth());
	}
}





