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
#include "BASE/include/MassflowSelect.h"
#include "resource.h"

#define HOURS_NUM 24
#define ROTATE_BY_MOUSE_INFOTIP _T("Rotate by mouse\nWhen this option is checked, you may press and hold left mouse button and move the mouse to rotate the chart. Using mouse wheel you may change the perspective.")

#if 0
inline double Rand (double dblStart, double dblFinish)
{
	double minVal = __min(dblStart, dblFinish);
	double maxVal = __max(dblStart, dblFinish);

	return (maxVal - minVal) * (double)rand() / (RAND_MAX + 1) + minVal;
}
#endif

class CChartSliderCtrl : public CBCGPSliderCtrl
{
public:
	CChartSliderCtrl()
	{
		m_bDrawFocus = FALSE;
		EnableZoomButtons();
	}
};


class CEasyGraphDoc;

class CEasyGraphView : public CBCGPFormView
{
	friend class CMainFrame;


	enum { IDD = IDD_FORM };


protected:
	CBCGPStatic	m_wndTitle;
	CString	m_strTitle;
	BOOL								m_bIsReady;
	BOOL								m_bIsDarkBackground;
	BOOL								m_bIsTexturedTheme;
	BOOL								m_bIsFlatTheme;
	UINT								m_nThumbnailFlags;
	CBCGPCircularProgressIndicatorCtrl	m_wndProgress;
	const std::array<const CString, base::cMassflowSelectMax> c_SelectString;


protected: // create from serialization only
	CEasyGraphView(UINT id = 0);
	DECLARE_DYNCREATE(CEasyGraphView)

// Form Data

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

	virtual void OnSetCategory(const BCGPChartCategory&) {}


	virtual void OnUpdateLineColor(const base::eMassflowSelect) {}
	virtual void OnUpdateCategory(const base::eMassflowSelect) {}
	virtual void OnUpdateLineWidth(const base::eMassflowSelect) {}
	virtual void OnUpdateVisible(const base::eMassflowSelect) {}

	virtual BOOL IsFillGradientAvailable() const
	{
		return !m_bIsTexturedTheme && !m_bIsFlatTheme;
	}

	void RotateChart(CBCGPRotationObject::RotationElement hit, double xDelta = 10., double yDelta = 10., double persperctiveDelta = 0.1);
	void SetLineWidth(int32_t m_LineWidth);
	void SetSeriesFillColors(const CBCGPColor::BCGP_COLOR* pColors, int32_t count);
	void SetSeriesLineColor(const CBCGPColor* pColors, int32_t count);


	virtual BOOL IsGroupView() const
	{
		return FALSE;
	}

	virtual HBITMAP CreateThumbnail(const CSize& size, BOOL bIsDarkTheme);
	virtual void Repos() {}

#if 0
	void SetupShapeCombo(CComboBox* pComboBox, int nSel = -1);
	void SetupShapeCombo(UINT nID, int nSel = -1);
#endif
	
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

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in EasyGraphView.cpp
inline CEasyGraphDoc* CEasyGraphView::GetDocument()
   { return (CEasyGraphDoc*)m_pDocument; }
#endif

