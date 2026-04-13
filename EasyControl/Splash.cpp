//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Splash.cpp
///
/// CG: This file was added by the Splash Screen component.
/// Splash.cpp : implementation file
///
/// @file   Splash.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"  // e. g. stdafx.h
#include "EasyControl.h"
#include "EasyControl_DEU/resource.h"

#include "Splash.h"  // e.g. splash.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CStartupWnd::c_bShowSplashWnd;
CStartupWnd* CStartupWnd::c_pSplashWnd;
CStartupWnd::CStartupWnd()
{
}


CStartupWnd::~CStartupWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CStartupWnd, CWnd)
	//{{AFX_MSG_MAP(CStartupWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CStartupWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CStartupWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CStartupWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CStartupWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}


BOOL CStartupWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	VERIFY(m_bitmap.LoadBitmap(IDB_EASYCONTROLSPLASH));

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

void CStartupWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	CWnd *pWnd = AfxGetMainWnd();
	if ( pWnd )
		pWnd->UpdateWindow();
}

void CStartupWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

//------------------------------------------------------------------------------------
///  @brief   OnCreate
///
///           The framework calls this member function when the window is first created
///
///  @author  Detlef Hafer
///
///  @class        CStartupWnd
///
///  @param[in] lpCreateStruct , LPCREATESTRUCT
///  @return int
///
//------------------------------------------------------------------------------------
int CStartupWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 2750, NULL);

	return 0;
}

//------------------------------------------------------------------------------------
///  @brief   OnPaint
///
///           The framework calls this member function when Windows or an application 
///           makes a request to repaint a portion of an application's window
///
///  @author  Detlef Hafer
///
///  @class        CStartupWnd
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void CStartupWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CStartupWnd::OnTimer(UINT_PTR )
{
	// Destroy the splash screen window.
	HideSplashScreen();
}

