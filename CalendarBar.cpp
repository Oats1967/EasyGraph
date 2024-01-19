//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2023 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// CalendarBar.cpp : implementation of the CCalendarBar class
//

#include "pch.h"
#include "CalendarBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 10;

/////////////////////////////////////////////////////////////////////////////
// CCalendarBar

BEGIN_MESSAGE_MAP(CCalendarBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CCalendarBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_CALENDAR_ON_SELCHANGED, OnSelChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalendarBar construction/destruction

CCalendarBar::CCalendarBar()
{
}

CCalendarBar::~CCalendarBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCalendarBar message handlers

int CCalendarBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	m_wndCalendars.Create (WS_VISIBLE | WS_CHILD, rectDummy, this, 1);
	m_wndCalendars.EnableMutipleSelection (TRUE, 35 /* Max sel days */, TRUE);
	m_wndCalendars.EnableWeekNumbers ();
	m_wndCalendars.EnableVisualManagerStyle();

	UpdateCalendar ();
	return 0;
}

void CCalendarBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	if (m_wndCalendars.GetSafeHwnd () != NULL)
	{
		m_wndCalendars.SetWindowPos (NULL,
					nBorderSize, nBorderSize, 
					cx - 2 * nBorderSize, cy - 2 * nBorderSize,
					SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CCalendarBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rectClient;
	GetClientRect (rectClient);

	CBCGPCalendarColors colors;
	CBCGPVisualManager::GetInstance()->GetCalendarColors(&m_wndCalendars, colors);

	dc.FillSolidRect(rectClient, colors.clrBackground == (COLORREF)-1 ? globalData.clrWindow : colors.clrBackground);
}

LRESULT CCalendarBar::OnSelChanged(WPARAM, LPARAM)
{
	static DateToShow g_Date;

	CMainFrame* pFrame = DYNAMIC_DOWNCAST (CMainFrame, GetTopLevelFrame ());
	if (pFrame != NULL)
	{
		CList<DATE, DATE&> lstDates;
		m_wndCalendars.GetSelectedDates (lstDates);

		COleDateTime date1;
		COleDateTime date2;

		CBCGPCalendar::GetMinMaxSelection (lstDates, date1, date2);

		g_Date.dateStart = date1;
		g_Date.dateEnd = date2;

		pFrame->SendMessage(WM_NEWDATE, WPARAM(&g_Date));
	}
	return 0;
}

void CCalendarBar::UpdateCalendar ()
{
#if 0
	int nFirstDayOfWeek = theApp.m_Options.m_WorkFirstDayOfWeek == -1
		? CBCGPPlannerManagerCtrl::GetSystemFirstDayOfWeek (TRUE)
		: theApp.m_Options.m_WorkFirstDayOfWeek;
	if (nFirstDayOfWeek != m_wndCalendars.GetFirstDayOfWeek ())
	{
		m_wndCalendars.SetFirstDayOfWeek (nFirstDayOfWeek);
	}

	const int nFirstWeekOfYear = theApp.m_Options.m_WorkFirstWeekOfYear == -1
		? CBCGPPlannerManagerCtrl::GetSystemFirstWeekOfYear ()
		: theApp.m_Options.m_WorkFirstWeekOfYear;
	if (nFirstWeekOfYear != m_wndCalendars.GetFirstWeekOfYear ())
	{
		m_wndCalendars.SetFirstWeekOfYear (nFirstWeekOfYear);
	}

	m_wndCalendars.EnableWeekNumbers (theApp.m_Options.m_CalendarWeekNumbers);
	m_wndCalendars.EnableGradientFillCaption (theApp.m_Options.m_CalendarGradientFill);
#endif
}
