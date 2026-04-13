//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeEditDlg
///
/// @file   RecipeEditDlg.cpp
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
#include "RecipeEditDlg.h"
#include "ProductDatabaseBox.h"
#include "MFCMacros.h"


#define TIMERID 1
CRecipeEditDlg* CRecipeEditDlg::g_RecipeEditDlg = nullptr;

//***********************************************************************************************
//***********************************************************************************************
// CRecipeEditDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CRecipeEditDlg, CBaseClass)
CRecipeEditDlg::CRecipeEditDlg() : CBaseClass()
, m_szText(L"")
, m_pParent{ nullptr }
, m_bProductName{ FALSE }
, m_ID(_U32(-1))
{}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITDLG_EDIT, m_szText);
	DDX_Control(pDX, IDC_EDITDLG_BT, m_Button);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CRecipeEditDlg, CBaseClass)
	ON_MESSAGE(WM_PRODUCTKEY, &CRecipeEditDlg::OnNewProductKey)
	ON_MESSAGE(WM_NOTIFYEDIT, &CRecipeEditDlg::OnNotifyEdit)
	ON_MESSAGE(WM_PRODUCTDATABASEBOXCLOSED, &CRecipeEditDlg::OnGainFocus)
	ON_BN_CLICKED(IDC_EDITDLG_BT, &CRecipeEditDlg::OnClickNameBt)
	ON_BN_CLICKED(IDC_EDITDLG_EDIT, &CRecipeEditDlg::OnClickName)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//**************************************************************************************
//**************************************************************************************
LRESULT CRecipeEditDlg::OnGainFocus(WPARAM, LPARAM)
{
	OnSetFocus();
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CRecipeEditDlg::OnNewProductKey(WPARAM id, LPARAM bInvers)
{
	OnSetFocus();
	auto pItem = PRODUCTDATABASEREF.GetItemID(_S32(id));
	if (pItem)
	{
		if (m_bProductName)
		{
			m_szText = toCString(pItem->GetProductName());
		}
		else
		{
			m_szText.Format(L"%I64d", pItem->GetQMNumber());
		}
		UpdateData(FALSE);
		if (m_pParent)
		{
			m_pParent->PostMessage(WM_PRODUCTKEY, id, bInvers);
		}
	}
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CRecipeEditDlg::OnNotifyEdit(WPARAM , LPARAM bChanged)
{
	ASSERT(IsWindowLocked());

	OnSetFocus();
	if (m_pParent)
	{
		m_pParent->SendMessage(WM_NOTIFYEDIT, WPARAM(m_ID), bChanged);
	}
	return 0;
}
//***************************************************************************************
//***************************************************************************************
BOOL CRecipeEditDlg::OnInitDialog()
{
	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		BOOL bProductListAvail = !(PRODUCTDATABASEREF.GetCount() == 0);
		ENABLE_SHOW_ID(IDC_EDITDLG_BT, bProductListAvail, bProductListAvail)
		UpdateData(FALSE);
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::CreateEditDlg(CWnd* pWnd, const CPoint& top, const CString& szName, UINT nID)
{
	if (!g_RecipeEditDlg)
	{
		g_RecipeEditDlg = new CRecipeEditDlg;
		g_RecipeEditDlg->Create(pWnd, top, szName, TRUE, nID);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::CreateEditDlg(CWnd* pWnd, const CPoint& top, const uint64_t& QMNUmber, UINT nID)
{
	if (!g_RecipeEditDlg)
	{
		g_RecipeEditDlg = new CRecipeEditDlg;
		CString szValue;
		szValue.Format(L"%I64d", QMNUmber);
		g_RecipeEditDlg->Create(pWnd, top, szValue, FALSE, nID);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::Create(CWnd* pParent, const CPoint& top, const CString& szName, const BOOL bProductName, UINT nID)
{
	m_pParent = pParent;
	m_bProductName = bProductName;
	m_szText = szName;
	m_ID = nID;

	auto result = CBaseClass::Create(IDD, pParent);
	ASSERT(result);
	if (result)
	{
		ShowWindow(SW_HIDE);
		Move(top);
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 500, NULL);
		ASSERT(IsWindowLocked());
	}
}

//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::OnTimer(UINT_PTR )
{
	KillTimer(TIMERID);
	OnSetFocus();
}
//***********************************************************************************************
//***********************************************************************************************
void CRecipeEditDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_RECIPEEDITLOSED);
	}
	delete this;
}
//************************************************************************************
//************************************************************************************
void CRecipeEditDlg::OnClickNameBt()
{
	if (IsWindowLocked()) // sonst gewt gleich das EditCtrl auf und der Timer schliesst es wieder
		return;

	LockWindow();
	CPoint aP;
	{
		auto pWnd = GetDlgItem(IDC_EDITDLG_BT);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(aRect);
		aP.x = aRect.right + 10;
		aP.y = aRect.top;
	}

	auto DesktopWnd = CWnd::GetDesktopWindow();
	CRect rcScreen;
	DesktopWnd->GetWindowRect(&rcScreen);
	if (aP.x + 220 > rcScreen.right)
	{
		auto pWnd = GetDlgItem(IDC_EDITDLG_EDIT);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(aRect);
		aP.x = aRect.left;
		aP.y = aRect.bottom + 20;
	}
	CProductDatabaseBox::CreateProductDatabaseBox(this, m_bProductName, aP);
}
//************************************************************************************
//************************************************************************************
void CRecipeEditDlg::OnClickName()
{
	if (IsWindowLocked()) // sonst geht gleich das EditCtrl auf und der Timer schliesst es wieder
		return;

	CEditCtrl::GetInput(this, (m_bProductName) ? E_TYPCTRL::E_STRINGCTRL : E_TYPCTRL::E_UINT64CTRL, IDC_EDITDLG_EDIT);
}
