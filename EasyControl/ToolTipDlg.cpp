//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ToolTipDlg
///
/// @file   ToolTipDlg.cpp
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
#include "ToolTipDlg.h"
#include "MFCMacros.h"



#define CLR_DEFAULT             0xFF000000L
#define CLR_DEFAULTTEXTCOLOR    COLORREF(RGB(100, 100, 100))
#define CLR_FRAMECOLOR			COLORREF(RGB(100, 100, 100))
#define CLR_CAPTIONCOLOR		COLORREF(RGB(100, 100, 100))
#define CLR_BARCOLOR			COLORREF(RGB(38, 82, 213))
#define TIMERID 1

CToolTipDlg* CToolTipDlg:: g_ToolTipDlg = NULL;


//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::CreateInfo(CWnd* pWnd, const CPoint& top, const CString& szCaption)
{
	if (!g_ToolTipDlg)
	{
		g_ToolTipDlg = new CToolTipDlg;
		g_ToolTipDlg->Create(pWnd, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::CreateInfo(CWnd* pWnd, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateInfo(pWnd, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::DestroyInfo()
{
	if (g_ToolTipDlg && g_ToolTipDlg->GetSafeHwnd())
	{
		g_ToolTipDlg->PostMessageW(WM_DESTROY);
	}
}
//***********************************************************************************************
//***********************************************************************************************
// CToolTipDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CToolTipDlg, CBaseClass)
CToolTipDlg::CToolTipDlg()
	: CBaseClass()
	, m_wndToolTipDlg()
{
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLTIP_BOX, m_wndToolTipDlg);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CToolTipDlg, CBaseClass)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
BOOL CToolTipDlg::OnInitDialog()
{
	const int32_t m_nColorBarWidth = 7;

	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	ASSERT(result);
	if (result)
	{
		m_wndToolTipDlg.m_clrBackground = CLR_DEFAULT;
		m_wndToolTipDlg.m_clrForeground = CLR_DEFAULTTEXTCOLOR;
		m_wndToolTipDlg.m_clrFrame = CLR_FRAMECOLOR;
		m_wndToolTipDlg.m_clrCaption = CLR_CAPTIONCOLOR;
		m_wndToolTipDlg.m_clrColorBar = CLR_BARCOLOR;
		m_wndToolTipDlg.SetLink(_T(""));
		m_wndToolTipDlg.ModifyStyle(WS_TABSTOP, 0);

		m_wndToolTipDlg.SetIcon(NULL);
		ASSERT(m_wndToolTipDlg.m_nIconIndex == -1);
		m_wndToolTipDlg.m_bIsColorBarOnLeft = TRUE;
		m_wndToolTipDlg.m_nColorBarWidth = m_nColorBarWidth;
		m_wndToolTipDlg.m_bDrawShadow = TRUE;
		m_wndToolTipDlg.m_bRoundedCorners = TRUE;
		m_wndToolTipDlg.m_bFixedFrameHeight = TRUE;
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::Create(CWnd* pParent, const CPoint& top, const CString& szCaption)
{
	auto result = CBaseClass::Create(IDD, pParent);
	ASSERT(result);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ASSERT(m_wndToolTipDlg.m_nIconIndex == -1);
		m_wndToolTipDlg.SetCaption(szCaption);
		m_wndToolTipDlg.SetWindowText(_T(" "));
		m_wndToolTipDlg.SetWindowOrg();
		m_wndToolTipDlg.SizeToContent();
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 2000, NULL);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::OnTimer(UINT_PTR )
{
	UnlockWindow();
	KillTimer(TIMERID);
	DestroyWindow();
}
//***********************************************************************************************
//***********************************************************************************************
void CToolTipDlg::PostNcDestroy()
{
	delete this;
}



