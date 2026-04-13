//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2022 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// DiagramBaseView.h : interface of the CDiagramBaseView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCGPCHARTEXAMPLEVIEW_H__CB2F5435_9A0A_418C_8145_C809FAB94156__INCLUDED_)
#define AFX_BCGPCHARTEXAMPLEVIEW_H__CB2F5435_9A0A_418C_8145_C809FAB94156__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "WorkspaceBar.h"

#define YEARS_NUM 5
#define ROTATE_BY_MOUSE_INFOTIP _T("Rotate by mouse\nWhen this option is checked, you may press and hold left mouse button and move the mouse to rotate the chart. Using mouse wheel you may change the perspective.")


class CChartSliderCtrl : public CBCGPSliderCtrl
{
public:
	CChartSliderCtrl()
	{
		m_bDrawFocus = FALSE;
		EnableZoomButtons();
	}
};

class CDiagramBaseView : public CBCGPFormView
{
protected: // create from serialization only
	CDiagramBaseView(UINT id = 0);
	DECLARE_DYNCREATE(CDiagramBaseView)

// Form Data
public:
	//{{AFX_DATA(CDiagramBaseView)
	//}}AFX_DATA

private:
	void InitChart();

// Attributes
public:
	CDiagramDoc* GetDocument();


protected:
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void DoDataExchange(CDataExchange* pDX) override;
	void OnInitialUpdate() override;
	void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;

	
protected:

// Implementation
public:
	virtual ~CDiagramBaseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual CBCGPChartVisualObject* GetChart()
	{
		return NULL;
	}

	virtual CBCGPBaseVisualCtrl* GetCtrl()
	{
		return NULL;
	}

	//virtual void GetTitle(CString& strTitle);
	virtual void SetupLayout();
	virtual void UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme);

	virtual BOOL IsGroupView() const
	{
		return FALSE;
	}

	virtual void Repos() {}

// Generated message map functions
protected:
	//{{AFX_MSG(CDiagramBaseView)
	afx_msg void OnNcPaint();

	
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

	BOOL								m_bIsReady;
};

#ifndef _DEBUG  // debug version in DiagramBaseView.cpp
inline CDiagramDoc* CDiagramBaseView::GetDocument()
   { return (CDiagramDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCGPCHARTEXAMPLEVIEW_H__CB2F5435_9A0A_418C_8145_C809FAB94156__INCLUDED_)
