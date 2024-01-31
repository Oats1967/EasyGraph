#pragma once

//#include "pch.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "VirtualSeries.h"
#include "Statistics.h"
#include "resource.h"


class CChartLineView : public CEasyGraphView
{
protected:
	CChartLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartLineView)

// Form Data
protected:
	//{{AFX_DATA(CChartLineView)
	enum { IDD = IDD_CHART };
	CBCGPScrollBar	m_wndScrollBarVert;
	CBCGPScrollBar	m_wndScrollBarHorz;
	CBCGPStatic		m_LineWnd;
	CBCGPStatic		m_DateWnd;
	CString			m_szLine;
	CString			m_szDate;
	CFont			m_Font;
	CBCGPChartCtrl	m_wndChart;

	int		 m_nZoomType;
	CString	 m_strInfo;
	int		 m_nChartCategory;
	BOOL	 m_bExternalScrollBar;
	BOOL	 m_bInScroll;
	CBCGPColor::BCGP_COLOR m_LineColor;

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


	CBCGPChartSeries* CreateSeries(const base::eMassflowSelect select, const int32_t index);

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
	virtual void SetupLayout();
	virtual void OnSetLineColor(CBCGPColor::BCGP_COLOR rColor) override;


	virtual const base::eMassflowSelect GetSelection() const
	{ return base::eMassflowSelect::eVIEWMAX;	}

	virtual const CString GetTitle() const
	{	return CString("EasyGraph");	}

protected:
	virtual ~CChartLineView();
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
	afx_msg LRESULT OnNewDate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


