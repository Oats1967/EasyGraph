
#pragma once

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
// ChartLineView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChartLineView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "resource.h"

class CChartLineView : public CEasyGraphView
{
protected:
	CChartLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartLineView)

// Form Data
public:
	//{{AFX_DATA(CChartLineView)
	enum { IDD = IDD_CHART };
	CBCGPChartCtrl	m_wndChart;
	int		m_nLineStyle;
	int		m_nLineWidth;
	BOOL	m_bShowDataMarkers;
	int		m_nMarkerSize;
	int		m_nMarkerShape;
	BOOL	m_bShowDataLabels;
	int		m_nDataLabelAngle;
	BOOL	m_bDisplayShadow;
	//}}AFX_DATA

protected:

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartLineView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual CBCGPChartVisualObject* GetChart()
	{
		return m_wndChart.GetChart();
	}

	virtual CBCGPBaseVisualCtrl* GetCtrl()
	{
		return &m_wndChart;
	}

// Implementation
protected:
	virtual ~CChartLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	LRESULT OnNewDate(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CChartLineView)
	afx_msg void OnUpdateChart();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

