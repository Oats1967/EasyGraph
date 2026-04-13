//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductDatabaseBox
///
/// @file   ProductDatabaseBox.cpp
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
#include "WrapperBase.h"
#include "ProductDatabaseBox.h"
#include "MFCMacros.h"



#define TIMERID 1

CProductDatabaseBox* CProductDatabaseBox::g_ProductDatabaseBox = nullptr;


//***********************************************************************************************
//***********************************************************************************************
// CProductDatabaseBox-Dialogfeld
IMPLEMENT_DYNAMIC(CProductDatabaseBox, CBaseClass)
CProductDatabaseBox::CProductDatabaseBox()
	: CBaseClass()
	, m_wndProductListCtrl(this, PRODUCTDATABASEREF)
	, m_bInvers(FALSE)
	, m_pParent{ nullptr }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CProductDatabaseBox::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRODUCTDATABASE_LIST, m_wndProductListCtrl);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CProductDatabaseBox, CBaseClass)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_PRODUCTDATABASE_LIST, &CProductDatabaseBox::OnLvnItemActivateList)
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
void CProductDatabaseBox::SetValue(void)
{
	UpdateData(FALSE);
}
//***************************************************************************************
//***************************************************************************************
void CProductDatabaseBox::OnTimer(UINT_PTR)
{
	UnlockWindow();
	SetFocus();
	KillTimer(TIMERID);
}
//***************************************************************************************
//***************************************************************************************
BOOL CProductDatabaseBox::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		SetValue();
		m_wndProductListCtrl.BuildHeadLine(m_bInvers);
		m_wndProductListCtrl.Fill();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CProductDatabaseBox::Create(CWnd* pParent, BOOL bInvers, const CPoint& top)
{
	m_bInvers = bInvers;
	m_pParent = pParent;
	auto result = CBaseClass::Create(IDD, pParent);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 1000, NULL);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CProductDatabaseBox::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_PRODUCTDATABASEBOXCLOSED);
	}
	delete this;
}
//***********************************************************************************************
//***********************************************************************************************
void CProductDatabaseBox::CreateProductDatabaseBox(CWnd* pWnd, BOOL bInvers, const CPoint& top)
{
	if (!g_ProductDatabaseBox)
	{
		CEasyControlApp::LoadProductDatabase();
		g_ProductDatabaseBox = new CProductDatabaseBox;
		g_ProductDatabaseBox->Create(pWnd, bInvers, top);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CProductDatabaseBox::DestroyProductDatabaseBox(void)
{
	if (g_ProductDatabaseBox)
	{
		g_ProductDatabaseBox->PostMessageW(WM_DESTROY);
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CProductDatabaseBox::OnLvnItemActivateList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	auto pItem = (base::utils::CProductItem*)(m_wndProductListCtrl.GetItemData(pNMIA->iItem));

	*pResult = 0;
	if (m_pParent)
	{
		m_pParent->PostMessage(WM_PRODUCTKEY, WPARAM(pItem->GetID()), LPARAM(m_bInvers) );
	}
	else
	{
		DestroyWindow();
	}
}





