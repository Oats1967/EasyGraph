
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
// ChartTotalizer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChartTotalizer form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "EasyGraph.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"


class CChartTotalizer : public CEasyGraphView
{
protected:
	CChartTotalizer();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartTotalizer)

private:
	void SetupSelectCombo(CComboBox* pComboBox, int nSel /*= -1*/);


// Form Data
public:
	//{{AFX_DATA(CChartTotalizer)
	enum { IDD = IDD_TOTALIZER };
	CBCGPChartCtrl	m_wndChart;
#if 0
	int		m_nLineStyle;
	int		m_nLineWidth;
	BOOL	m_bShowDataMarkers;
	int		m_nMarkerSize;
	int		m_nMarkerShape;
	BOOL	m_bShowDataLabels;
	int		m_nDataLabelAngle;
	BOOL	m_bDisplayShadow;
#endif
	CFont m_Font;
	CBCGPComboBox	m_KeySelectWnd;
	CBCGPStatic m_LineWnd;
	CBCGPStatic m_DateWnd;
	CBCGPStatic m_ANWnd;
	CString m_szLine;
	CString m_szDate;
	CString m_szANNumber;
	int32_t m_KeySelected;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartTotalizer)
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
	virtual ~CChartTotalizer();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	LRESULT OnNewDate(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CChartTotalizer)
	afx_msg void OnUpdateChart();
	afx_msg void OnKeySelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
};
