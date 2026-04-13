//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module PasswordDlg
///
/// @file   PasswordDlg.cpp
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
#include "PasswordDlg.h"


//************************************************************************************************
//************************************************************************************************
BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//************************************************************************************************
//************************************************************************************************
IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)
CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
	, m_szPassword(_T(""))
{
}
//************************************************************************************************
//************************************************************************************************
void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASSWORD_PASSWORD, m_szPassword);
}
//************************************************************************************************
//************************************************************************************************
void CPasswordDlg::OnClose()
{
	UpdateData(TRUE);

	m_szPassword.TrimLeft();
	m_szPassword.TrimRight();

	CDialog::OnClose();
}

