//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SingleUserDlg.cpp
///
/// @file   SingleUserDlg.cpp
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
#include "SingleUserDlg.h"
#include "FormatMetric.h"


//***************************************************************************************************************
//***************************************************************************************************************
IMPLEMENT_DYNAMIC(CSingleUserDlg, CDialog)

CSingleUserDlg::CSingleUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSingleUserDlg::IDD, pParent)
	, m_szUserName(_T(""))
{
}
//***************************************************************************************************************
//***************************************************************************************************************
void CSingleUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_TextN(pDX, IDC_USER_NAME, m_szUserName, 25);
}
//***************************************************************************************************************
//***************************************************************************************************************
BEGIN_MESSAGE_MAP(CSingleUserDlg, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
//***************************************************************************************************************
//***************************************************************************************************************
void CSingleUserDlg::OnClose()
{
	// TODO: Fügen Sie hier Ihren Meldungsbehandlungscode ein, und/oder benutzen Sie den Standard.
	UpdateData(TRUE);
	m_szUserName.TrimLeft();
	m_szUserName.TrimRight();
	CDialog::OnClose();
}

