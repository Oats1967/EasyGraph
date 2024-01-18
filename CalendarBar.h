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
// CalendarBar.h : interface of the CCalendarBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CCalendarBar : public CBCGPDockingControlBar
{
	friend class CMainFrame;

public:
	CCalendarBar();

// Attributes
protected:
	CBCGPCalendar m_wndCalendars;

// Operations
public:
	void UpdateCalendar ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalendarBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCalendarBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCalendarBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnSelChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
