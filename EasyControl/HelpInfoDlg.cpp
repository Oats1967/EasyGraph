//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module HelpInfoDlg
///
/// @file   HelpInfoDlg.cpp
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
#include "HelpInfoDlg.h"
#include "MFCMacros.h"



#define CLR_DEFAULT             0xFF000000L
#define CLR_DEFAULTTEXTCOLOR    COLORREF(RGB(100, 100, 100))
#define CLR_FRAMECOLOR			COLORREF(RGB(100, 100, 100))
#define CLR_CAPTIONCOLOR		COLORREF(RGB(100, 100, 100))
#define CLR_BARCOLOR			COLORREF(RGB(38, 213, 82))
#define TIMERID 1

CHelpInfoDlg* CHelpInfoDlg:: g_HelpInfoDlg = NULL;


//***********************************************************************************************
//***********************************************************************************************
// CHelpInfoDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CHelpInfoDlg, CBaseClass)
CHelpInfoDlg::CHelpInfoDlg()
	: CBaseClass()
	, m_wndHelpInfoDlg()
	, m_pParent{ nullptr }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HELPINFO_TEXT, m_wndHelpInfoDlg);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CHelpInfoDlg, CBaseClass)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
BOOL CHelpInfoDlg::OnInitDialog()
{
	const int32_t m_nColorBarWidth = 7;

	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		m_wndHelpInfoDlg.m_clrBackground = CLR_DEFAULT;
		m_wndHelpInfoDlg.m_clrForeground = CLR_DEFAULTTEXTCOLOR;
		m_wndHelpInfoDlg.m_clrFrame = CLR_FRAMECOLOR;
		m_wndHelpInfoDlg.m_clrCaption = CLR_CAPTIONCOLOR;
		m_wndHelpInfoDlg.m_clrColorBar = CLR_BARCOLOR;
		m_wndHelpInfoDlg.SetLink(_T(""));
		m_wndHelpInfoDlg.ModifyStyle(WS_TABSTOP, 0);

		m_wndHelpInfoDlg.SetIcon(NULL);
		ASSERT(m_wndHelpInfoDlg.m_nIconIndex == -1);
		m_wndHelpInfoDlg.m_bIsColorBarOnLeft = TRUE;
		m_wndHelpInfoDlg.m_nColorBarWidth = m_nColorBarWidth;
		m_wndHelpInfoDlg.m_bDrawShadow = TRUE;
		m_wndHelpInfoDlg.m_bRoundedCorners = FALSE;
		m_wndHelpInfoDlg.m_bFixedFrameHeight = FALSE;
		m_wndHelpInfoDlg.m_bFixedFrameWidth = FALSE;
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::CreateInfo(CWnd* pWnd, const CPoint& top, const CString& szCaption, const CString& szMessage)
{
	if (!g_HelpInfoDlg)
	{
		g_HelpInfoDlg = new CHelpInfoDlg;
		g_HelpInfoDlg->Create(pWnd, top, szCaption, szMessage);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::CreateInfo(CWnd* pWnd, const CPoint& top, const UINT caption, const UINT id)
{
	CString szMessage;
	if (id != _U32(-1))
	{
		VERIFY(szMessage.LoadString(id));
	}
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateInfo(pWnd, top, szCaption, szMessage);
}
//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::Create(CWnd* pParent, const CPoint& top, const CString& szCaption, const CString& szMessage)
{
	m_pParent = pParent;

	mfcmacros::SendMessage(pParent, WM_NOTIFYHELPINFOBOX);

	auto result = CBaseClass::Create(IDD, pParent);
	ASSERT(result);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ASSERT(m_wndHelpInfoDlg.m_nIconIndex == -1);
		m_wndHelpInfoDlg.SetWindowText(szMessage);
		m_wndHelpInfoDlg.SetCaption(szCaption);
		m_wndHelpInfoDlg.SetWindowOrg();
		m_wndHelpInfoDlg.SizeToContent();
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 1000, NULL);
		ASSERT(IsWindowLocked());
	}
}

//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::OnTimer(UINT_PTR )
{
	SetFocus();
	KillTimer(TIMERID);
	UnlockWindow();
}
//***********************************************************************************************
//***********************************************************************************************
void CHelpInfoDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_HELPINFOCLOSED);
	}
	delete this;
}



