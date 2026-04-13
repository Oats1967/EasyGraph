//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsProductListPage
///
/// @file   SettingsProductListPage.cpp
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
#include "ECMessageBox.h"
#include "BASE/Base.def"
#include "RemoteControl.h"
#include "SettingsProductListPage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CSettingsProductListPage, _func)


IMPLEMENT_DYNAMIC(CSettingsProductListPage, CBCGPPropertyPage)
CSettingsProductListPage::CSettingsProductListPage()
	: CBCGPPropertyPage(CSettingsProductListPage::IDD)
	, m_aListCtrl{ this, PRODUCTDATABASEREF }
	, m_szProduct(_T(""))
	, m_bModified(FALSE)
	, m_QMNumber (0)
	, m_SelectedItemID(-1)
	, m_szProductModified(FALSE)
	, m_QMNumberModified(FALSE)
	, m_EditMap({
			EDITITEM(IDC_SETTINGS_PRODUCTLIST_NAME,	OnNotifyProductName),
			EDITITEM(IDC_SETTINGS_PRODUCTLIST_KEY, OnNotifyProductKey)
		})

{
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS_PRODUCTLIST_LIST, m_aListCtrl);
	DDX_Text(pDX, IDC_SETTINGS_PRODUCTLIST_NAME, m_szProduct);
	DDX_Text(pDX, IDC_SETTINGS_PRODUCTLIST_KEY, m_QMNumber);
}
//*********************************************************************************************************
//*********************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsProductListPage, CBCGPPropertyPage)
	ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, &CSettingsProductListPage::OnPageChanged)
	ON_MESSAGE(WM_NOTIFYEDIT, &CSettingsProductListPage::OnNotifyEdit)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_SETTINGS_PRODUCTLIST_LIST, &CSettingsProductListPage::OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_SETTINGS_PRODUCTLIST_ACCEPT, &CSettingsProductListPage::OnBnClickedSettingsAccept)
	ON_BN_CLICKED(IDC_SETTINGS_PRODUCTLIST_DELETE, &CSettingsProductListPage::OnBnClickedSettingsDelete)
	ON_BN_CLICKED(IDC_SETTINGS_PRODUCTLIST_MODIFY, &CSettingsProductListPage::OnBnClickedSettingsModify)
	ON_STN_CLICKED(IDC_SETTINGS_PRODUCTLIST_NAME, &CSettingsProductListPage::OnStnClickedProductName)
	ON_STN_CLICKED(IDC_SETTINGS_PRODUCTLIST_KEY, &CSettingsProductListPage::OnStnClickedKey)
END_MESSAGE_MAP()
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::SaveItems()
{
	auto result = CEasyControlApp::SaveProductDatabase();
	if (!result)
	{
		ECMessageBox(IDS_ERROR_PRODUCTDATABASE_SAVE, MB_ICONSTOP | MB_OK);
	}
}
//***************************************************************************************
//***************************************************************************************
LRESULT CSettingsProductListPage::OnPageChanged(WPARAM, LPARAM)
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CSettingsProductListPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = m_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			m_bModified = TRUE;
			SetValue();
		}
	}
	return 0;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CSettingsProductListPage::OnNotifyProductName()
{
	auto bModified = CEditCtrlEx::GetString(m_szProduct);
	if (bModified)
	{
		m_szProductModified = TRUE;
		SetValue();
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CSettingsProductListPage::OnNotifyProductKey()
{
	auto bModified = CEditCtrlEx::GetUint64(m_QMNumber);
	if (bModified)
	{
		m_QMNumberModified = TRUE;
		SetValue();
	}
	return bModified;
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsProductListPage::GetValue()
{
	UpdateData(TRUE);
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsProductListPage::SetValue()
{
	UpdateData(FALSE);
	ENABLE_ID(IDC_SETTINGS_PRODUCTLIST_ACCEPT, (m_QMNumber > 0) && ( m_szProductModified && m_QMNumberModified));
	ENABLE_ID(IDC_SETTINGS_PRODUCTLIST_MODIFY, (m_QMNumber > 0) && ( m_SelectedItemID >= 0) && (m_szProductModified ^ m_QMNumberModified));
	ENABLE_ID(IDC_SETTINGS_PRODUCTLIST_DELETE, (m_QMNumber > 0) && (m_SelectedItemID  >= 0));

	if (m_szProductModified && m_QMNumberModified)
	{
		m_aListCtrl.SetItemState(-1, 0, LVIS_SELECTED);
	}
	SetModified(m_bModified);
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CSettingsProductListPage::OnApply()
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	return CBCGPPropertyPage::OnApply();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsProductListPage::OnOK()
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	CBCGPPropertyPage::OnOK();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::Init()
{
	m_aListCtrl.Fill();
	SetModified(m_bModified = FALSE);
	m_szProduct = _T("");
	m_bModified = FALSE;
	m_QMNumber = 0;
	m_SelectedItemID = -1;
	m_szProductModified = FALSE;
	m_QMNumberModified = FALSE;
	SetValue();
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CSettingsProductListPage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		CEasyControlApp::LoadProductDatabase();
		m_aListCtrl.BuildHeadLine(TRUE);
		Init();
	}
	return bResult;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::OnLvnItemActivateList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	const auto pItem = (base::utils::CProductItem*)(m_aListCtrl.GetItemData(pNMIA->iItem));
	m_QMNumber			= pItem->GetQMNumber();
	m_szProduct			= toCString(pItem->GetProductName());
	m_SelectedItemID	= pItem->GetID();
	m_szProductModified = FALSE;
	m_QMNumberModified = FALSE;
	SetValue();
	*pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CSettingsProductListPage::CheckValidItem(const uint64_t& key, const std::string& szProduct )
{
	BOOL result = TRUE;

	if (result && m_szProductModified)
	{
		if (PRODUCTDATABASEREF.FindItem(szProduct))
		{
			ECMessageBox(IDS_ERROR_PRODUCTNAME_EXIST, MB_OK);
			result = FALSE;
		}
	}
	if (result && m_QMNumberModified)
	{
		if ( PRODUCTDATABASEREF.FindItem(key) )
		{
			ECMessageBox(IDS_ERROR_QMNUMBER_EXIST, MB_OK);
			result = FALSE;
		}
	}
	return result;
}

//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::OnBnClickedSettingsAccept()
{
	GetValue();
	ASSERT(m_szProductModified && m_QMNumberModified);
	const auto szProduct = toStdString(m_szProduct);
	BOOL result = CheckValidItem(m_QMNumber, szProduct);
	if (result)
	{
		PRODUCTDATABASEREF.AddItem(base::utils::CProductItem { -1, szProduct, m_QMNumber  });
		PRODUCTDATABASEREF.SetChanged(TRUE);
		const auto pItem = PRODUCTDATABASEREF.FindItem(m_QMNumber);
		ASSERT(pItem->GetProductName()  == szProduct);
		ASSERT(pItem->GetQMNumber()		== m_QMNumber);
		m_bModified = TRUE;
		m_SelectedItemID = pItem->GetID();;
		m_aListCtrl.Fill(m_SelectedItemID);
		m_QMNumberModified = FALSE;
		m_szProductModified = FALSE;

		SetValue();
		SetFocus();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::OnBnClickedSettingsModify()
{
	GetValue();

	auto szProduct = toStdString(m_szProduct);
	ASSERT(m_SelectedItemID >= 0);
	BOOL result = CheckValidItem(m_QMNumber, szProduct);
	if (result)
	{
		if (m_szProductModified || m_QMNumberModified)
		{
			PRODUCTDATABASEREF.ReplaceItem(m_SelectedItemID, (base::utils::CProductItem{ -1, szProduct, m_QMNumber }));
			PRODUCTDATABASEREF.SetChanged(TRUE);
			m_szProductModified = FALSE;
			m_QMNumberModified = FALSE;
			m_bModified = TRUE;
		}
		m_aListCtrl.Fill(m_SelectedItemID);
		SetValue();
		SetFocus();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsProductListPage::OnBnClickedSettingsDelete()
{
	GetValue();

	if (m_SelectedItemID >= 0)
	{
		PRODUCTDATABASEREF.DeleteItem(m_SelectedItemID);
		PRODUCTDATABASEREF.SetChanged(TRUE);
		m_bModified = TRUE;
		m_QMNumber = 0;
		m_szProduct = _T("");
		m_SelectedItemID = -1;
		m_aListCtrl.Fill(m_SelectedItemID);
		m_QMNumberModified = FALSE;
		m_szProductModified = FALSE;
		SetValue();
		SetFocus();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CSettingsProductListPage::OnSetActive()
{
	GetParent()->SendMessage(WM_PROPERTYPAGE_CHANGED, WPARAM(this));
	Init();
	return CBCGPPropertyPage::OnSetActive();
}
//*********************************************************************************************************
//*********************************************************************************************************
void  CSettingsProductListPage::OnStnClickedProductName()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_SETTINGS_PRODUCTLIST_NAME);
}
//*********************************************************************************************************
//*********************************************************************************************************
void  CSettingsProductListPage::OnStnClickedKey()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_UINT64CTRL, IDC_SETTINGS_PRODUCTLIST_KEY);
}

