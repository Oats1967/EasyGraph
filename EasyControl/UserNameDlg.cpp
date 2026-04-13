//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module UserNameDlg.cpp
///
/// @file   UserNameDlg.cpp
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
#include "UserNameDlg.h"
#include "FormatMetric.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



IMPLEMENT_DYNAMIC(CUserNameDlg, CDialog)
CUserNameDlg::CUserNameDlg(CWnd* pParent /*=NULL*/, base::utils::eUserClass currentUser)
	: CDialog(CUserNameDlg::IDD, pParent)
	, m_szUserName(_T(""))
	, m_NewUserClass(base::utils::eUserClass::eCLASSUSER)
	, m_currentUserClass(currentUser)
	, m_bUserNameAdmin(FALSE)
	, m_bUserNameSupervisor(FALSE)
	, m_bUserNameStandard(FALSE)
	, m_bUserNameUser(FALSE)
{
	static_assert(_S32(base::utils::eUserClass::eCLASSSUPERVISOR) == 0, "Invalid array indices!");
	static_assert(_S32(base::utils::eUserClass::eCLASSADMINISTRATOR) == 1, "Invalid array indices!");
	static_assert(_S32(base::utils::eUserClass::eCLASSSTANDARDUSER) == 2, "Invalid array indices!");
	static_assert(_S32(base::utils::eUserClass::eCLASSUSER) == 3, "Invalid array indices!");
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_TextN(pDX, IDC_USERNAME, m_szUserName, 15);
	DDX_Check(pDX, IDC_USERNAME_ADMIN,	m_bUserNameAdmin);
	DDX_Check(pDX, IDC_USERNAME_STANDARD, m_bUserNameStandard);
	DDX_Check(pDX, IDC_USERNAME_USER,		m_bUserNameUser);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BEGIN_MESSAGE_MAP(CUserNameDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_STN_CLICKED(IDC_USERNAME, OnStnClickedBenutzername)
	ON_BN_CLICKED(IDC_USERNAME_ADMIN, OnBnClickedUserNameAdmin)
	ON_BN_CLICKED(IDC_USERNAME_STANDARD, OnBnClickedUserNameStandard)
	ON_BN_CLICKED(IDC_USERNAME_USER, OnBnClickedUserNameUser)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
END_MESSAGE_MAP()
//*******************************************************************************************************************
//*******************************************************************************************************************
LRESULT CUserNameDlg::OnNotifyEdit(WPARAM id, LPARAM)
{
	switch(id)
	{
		case IDC_USERNAME				:	CEditCtrlEx :: GetString(m_szUserName);
											ENABLE_ID(IDOK, !m_szUserName.IsEmpty());
											break;
		default:
					ASSERT(FALSE);
					break;
	}
	UpdateData(FALSE);
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::SetUserLevel (const base::utils::eUserClass newUser)
{
	m_NewUserClass = newUser;
	m_bUserNameSupervisor	= (newUser == base::utils::eUserClass::eCLASSSUPERVISOR);
	m_bUserNameAdmin		= (newUser == base::utils::eUserClass::eCLASSADMINISTRATOR);
	m_bUserNameStandard		= (newUser == base::utils::eUserClass::eCLASSSTANDARDUSER);
	m_bUserNameUser			= (newUser == base::utils::eUserClass::eCLASSUSER);
	UpdateData(FALSE);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::OnStnClickedBenutzername()
{
   	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_USERNAME);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
BOOL CUserNameDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	ENABLE_ID(IDC_USERNAME_ADMIN,	 ( _S32(m_currentUserClass) < _S32(base::utils::eUserClass::eCLASSADMINISTRATOR)));
	ENABLE_ID(IDC_USERNAME_STANDARD, (_S32(m_currentUserClass) < _S32(base::utils::eUserClass::eCLASSSTANDARDUSER)));
	ENABLE_ID(IDC_USERNAME_USER,	 (_S32(m_currentUserClass) < _S32(base::utils::eUserClass::eCLASSUSER)));
	ENABLE_ID(IDOK, FALSE);

	SetUserLevel (base::utils::eUserClass::eCLASSUSER);
    return FALSE;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::OnBnClickedUserNameAdmin()
{
	SetUserLevel (base::utils::eUserClass::eCLASSADMINISTRATOR);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::OnBnClickedUserNameStandard()
{
	SetUserLevel (base::utils::eUserClass::eCLASSSTANDARDUSER);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::OnBnClickedUserNameUser()
{
	SetUserLevel(base::utils::eUserClass::eCLASSUSER);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CUserNameDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	OnOK();
}

