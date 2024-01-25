#if !defined(AFX_GROUPVIEW_H__0DFA01C1_E192_4B25_87B6_0F30EF6355D8__INCLUDED_)
#define AFX_GROUPVIEW_H__0DFA01C1_E192_4B25_87B6_0F30EF6355D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
// GroupView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EasyGraphDoc.h"
#include "EasyGraphView.h"

class CChartUITilesCtrl : public CBCGPWinUITilesCtrl
{
public:
	CChartUITilesCtrl()
	{
		m_nDlgCode = DLGC_WANTALLKEYS;	// To catch Enter key
	}
};

class CGroupView : public CEasyGraphView
{
	friend class CMainFrame;

protected:
	CGroupView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGroupView)

// Form Data
public:
	//{{AFX_DATA(CGroupView)
	enum { IDD = IDD_GROUP };
	CChartUITilesCtrl	m_wndTiles;
	//}}AFX_DATA

// Attributes
protected:
	int		m_nGroupID;
	CString m_strGroupName;

// Operations
public:
	void SetupGroup(int nGroupID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual void SetupLayout();
	virtual void GetTitle(CString& strTitle);

	virtual BOOL IsGroupView() const
	{
		return TRUE;
	}

	virtual void UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme);

// Implementation
protected:
	virtual ~CGroupView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGroupView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnClickTile(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPVIEW_H__0DFA01C1_E192_4B25_87B6_0F30EF6355D8__INCLUDED_)
