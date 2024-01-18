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
// EasyGraphView.h : interface of the CEasyGraphView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "pch.h"
#include "resource.h"

#define YEARS_NUM 5
#define ROTATE_BY_MOUSE_INFOTIP _T("Rotate by mouse\nWhen this option is checked, you may press and hold left mouse button and move the mouse to rotate the chart. Using mouse wheel you may change the perspective.")

inline double Rand (double dblStart, double dblFinish)
{
	double minVal = min(dblStart, dblFinish);
	double maxVal = max(dblStart, dblFinish);

	return (maxVal - minVal) * (double)rand() / (RAND_MAX + 1) + minVal;
}

class CChartSliderCtrl : public CBCGPSliderCtrl
{
public:
	CChartSliderCtrl()
	{
		m_bDrawFocus = FALSE;
		EnableZoomButtons();
	}
};


class CEasyGraphView : public CBCGPFormView
{
	friend class CMainFrame;

protected: // create from serialization only
	CEasyGraphView(UINT id = 0);
	DECLARE_DYNCREATE(CEasyGraphView)

// Form Data
public:
	//{{AFX_DATA(CEasyGraphView)
	enum { IDD = IDD_FORM };
	CBCGPStatic	m_wndTitle;
	CString	m_strTitle;
	//}}AFX_DATA

// Attributes
public:
	CEasyGraphDoc* GetDocument();

// Operations
public:
	void SetProgress(int nCurr, int nTotal);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyGraphView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEasyGraphView();
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

	virtual void GetTitle(CString& strTitle);
	virtual void SetupLayout();
	virtual void UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme);
	virtual void OnChangeBackgroundDarkness() {}

	virtual void OnChartAnimation();
	virtual BOOL IsAnimationAvailable() const
	{
		return TRUE;
	}

	virtual BOOL IsFillGradientAvailable() const
	{
		return !m_bIsTexturedTheme && !m_bIsFlatTheme;
	}

	void RotateChart(CBCGPRotationObject::RotationElement hit, double xDelta = 10., double yDelta = 10., double persperctiveDelta = 0.1);
	void SetDefaultLineWidth();

	virtual BOOL IsGroupView() const
	{
		return FALSE;
	}

	virtual HBITMAP CreateThumbnail(const CSize& size, BOOL bIsDarkTheme);
	virtual void Repos() {}

	void SetupShapeCombo(CComboBox* pComboBox, int nSel = -1);
	void SetupShapeCombo(UINT nID, int nSel = -1);
	
	int GetMarkerSize(int nSel);

	virtual int GetAxisSplitGap() const;

// Generated message map functions
protected:
	//{{AFX_MSG(CEasyGraphView)
	afx_msg void OnChartCopy();
	afx_msg void OnChartExport();
	afx_msg void OnNcPaint();
	afx_msg void OnUpdateChartCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChartExport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#if 0
	afx_msg void OnAnimateChart();
	afx_msg void OnUpdateAnimateChart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAnimationStyleCombo(CCmdUI* pCmdUI);
#endif
	DECLARE_MESSAGE_MAP()

	//CDemoFeature::Feature				m_Feature;
	BOOL								m_bIsReady;
	CString								m_arYears[YEARS_NUM];
	BOOL								m_bIsDarkBackground;
	BOOL								m_bIsTexturedTheme;
	BOOL								m_bIsFlatTheme;
	UINT								m_nThumbnailFlags;
	CBCGPCircularProgressIndicatorCtrl	m_wndProgress;
};

#ifndef _DEBUG  // debug version in EasyGraphView.cpp
inline CEasyGraphDoc* CEasyGraphView::GetDocument()
   { return (CEasyGraphDoc*)m_pDocument; }
#endif

