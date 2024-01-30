#pragma once

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
	CBCGPScrollBar	m_wndScrollBarVert;
	CBCGPScrollBar	m_wndScrollBarHorz;
#if 0
	CBCGPComboBox	m_wndSelectionType;
	CBCGPComboBox	m_wndZoomType;
#endif
	CBCGPChartCtrl	m_wndChart;

	int		m_nDataPoints;
	int		m_nZoomType;
	int		m_nSelectionType;
	CString	m_strInfo;
	int		m_nChartCategory;
	BOOL	m_bExternalScrollBar;
	BOOL	m_bInScroll;
	//}}AFX_DATA

// Attributes
public:

	virtual CBCGPChartVisualObject* GetChart()
	{
		return m_wndChart.GetChart();
	}

	virtual CBCGPBaseVisualCtrl* GetCtrl()
	{
		return &m_wndChart;
	}

	virtual BOOL IsAnimationAvailable() const
	{
		return FALSE;
	}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartLineView)
public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

	virtual void SetupLayout();

// Implementation
protected:
	virtual ~CChartLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CBCGPChartAxis* GetChartAxis(BOOL bIsHorizontal);
	void UpdateScrollBars();
	void DoScroll(CBCGPScrollBar& wndScrollBar, UINT nSBCode);

	// Generated message map functions
	//{{AFX_MSG(CChartLineView)
	afx_msg void OnUpdateChart();
	afx_msg void OnUpdateChartCategory();
	afx_msg void OnUpdateZoom();
	afx_msg void OnExternalScrollbar();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg LRESULT OnAxisScrolled(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnAxisZoomed(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};
