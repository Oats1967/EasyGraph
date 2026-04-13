//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module NewPasswordDlg.cpp
///
/// @file   NewPasswordDlg.cpp
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
#include "NewPasswordDlg.h"
#include "FormatMetric.h"



//************************************************************************************************************
//************************************************************************************************************
IMPLEMENT_DYNAMIC(CNewPasswordDlg, CDialog)
CNewPasswordDlg::CNewPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPasswordDlg::IDD, pParent)
	, m_szPassword(_T(""))
	, m_szPasswordConfirm(_T(""))
{
}
//************************************************************************************************************
//************************************************************************************************************
void CNewPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_TextN(pDX, IDC_NEWPASSWORD_PASSWORD, m_szPassword, 25);
	DDX_Text(pDX, IDC_NEWPASSWORD_PASSWORD_CONFIRM, m_szPasswordConfirm);
}
//************************************************************************************************************
//************************************************************************************************************
BEGIN_MESSAGE_MAP(CNewPasswordDlg, CDialog)
	ON_EN_CHANGE(IDC_NEWPASSWORD_PASSWORD, OnEnChangeNewPasswordPassword)
	ON_EN_CHANGE(IDC_NEWPASSWORD_PASSWORD_CONFIRM, OnEnChangeNewPasswordPasswordConfirm)
END_MESSAGE_MAP()
//************************************************************************************************************
//************************************************************************************************************
void CNewPasswordDlg::OnEnChangeNewPasswordPassword()
{
	UpdateData(TRUE);
	m_szPasswordConfirm = "";

	CWnd *pWnd = (CWnd*) GetDlgItem(IDOK);
	ASSERT(pWnd);
	pWnd->EnableWindow(FALSE);
}
//************************************************************************************************************
//************************************************************************************************************
void CNewPasswordDlg::OnEnChangeNewPasswordPasswordConfirm()
{
	UpdateData(TRUE);

	CWnd *pWnd = (CWnd*) GetDlgItem(IDOK);
	ASSERT(pWnd);
	pWnd->EnableWindow((!m_szPassword.IsEmpty()) && (m_szPassword == m_szPasswordConfirm));
	
}
//************************************************************************************************************
//************************************************************************************************************
void CNewPasswordDlg::OnOK()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}
//************************************************************************************************************
//************************************************************************************************************
BOOL CNewPasswordDlg::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();

	CEdit *pWnd = (CEdit *) GetDlgItem(IDC_NEWPASSWORD_PASSWORD);
	ASSERT(pWnd);
	pWnd->SetFocus();
	return result;
}

