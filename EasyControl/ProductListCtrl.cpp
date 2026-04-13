//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductListCtrl
///
/// @file   ProductListCtrl.cpp
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
#include "ProductListCtrl.h"
#include "MFCMacros.h"


#define ICON_SIZEX 16
#define ICON_SIZEY 32
#define IMAGE_MAX 2

//*****************************************************************************************************
//*****************************************************************************************************
CProductListCtrl::CProductListCtrl(CWnd* pParent, base::utils::CProductItemList& rProductList) : m_rProductList{ rProductList }, m_pParent{ pParent }, m_bInvers{ FALSE }
{
	CreateImageList();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CProductListCtrl::BuildHeadLine(BOOL bInvers)
{
	LV_COLUMN lvc;

	CRect aRect;
	GetWindowRect(aRect);
	CSize rSize = aRect.Size();
	int32_t hw = ::GetSystemMetrics(SM_CXVSCROLL) + 7;
	ASSERT((rSize.cx > 0) && (rSize.cy > 0));
	const int32_t cx = rSize.cx - hw;
	const int32_t cxName = _S32(cx * 6 / 10);
	const int32_t cxKey  = cx - cxName;

	m_bInvers = bInvers;
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES);
	SetImageList(&m_ImageList, LVSIL_SMALL);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_PRODUCTKEY));
	lvc.fmt = LVCFMT_CENTER;
	lvc.cx = cxKey; //  GetColumnWidth();
	lvc.iSubItem = m_bInvers;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = szTemp.GetLength();
	InsertColumn(m_bInvers, &lvc);

	lvc.iSubItem = !m_bInvers;
	lvc.cx = cxName;
	VERIFY(szTemp.LoadString(IDS_PRODUCTNAME));
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = (int32_t)szTemp.GetLength();
	InsertColumn(! m_bInvers, &lvc);

}

//*****************************************************************************************************
//*****************************************************************************************************
void CProductListCtrl::CreateImageList()
{
	m_ImageList.Create(ICON_SIZEX, ICON_SIZEY, ILC_COLOR, 0, IMAGE_MAX); // 16, 32 for large icons
	HICON aIcon = AfxGetApp()->LoadIcon(IDI_ICONPRODUCTDATABASE);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONALARMINFO);
	ASSERT(aIcon);
	m_ImageList.Add(aIcon);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CProductListCtrl::Fill(const int32_t selectitemID)
{
    DeleteAllItems();
	int32_t selectedIndex = -1;
	const auto count = _S32(m_rProductList.GetCount());
	for (int32_t index = 0; index < count; index++)
	{
		const auto& rItem = m_rProductList.GetItem(index);
		CString szFormat[2];

		CString szQMNUmber;
		szQMNUmber.Format(_T("%lld"), rItem.GetQMNumber());
		szFormat[m_bInvers != FALSE] = szQMNUmber;
		szFormat[m_bInvers == FALSE] = toCString(rItem.GetProductName());;

		LVITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE | LVIF_PARAM;
		lvi.iItem = index;
		lvi.iSubItem = 0;
		lvi.pszText = LPTSTR(LPCTSTR(szFormat[0]));
		lvi.iImage = 0;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.cchTextMax = szFormat[0].GetLength();
		lvi.state = INDEXTOSTATEIMAGEMASK(1);
		lvi.lParam = LPARAM(&rItem);
		InsertItem(&lvi);

		SetItemText(index, 1, szFormat[1]);

		if (rItem.GetID() == selectitemID)
		{
			selectedIndex = index;
		}
	}
	SetItemState(-1, 0, LVIS_SELECTED);
	if (selectedIndex >= 0)
	{
		SetItemState(selectedIndex, LVIS_SELECTED, LVIS_SELECTED);
		SetSelectionMark(selectedIndex);
	}
}