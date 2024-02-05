#pragma once

//#include "pch.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "Statistics.h"
#include "resource.h"


class CChartLineView : public CEasyGraphView
{
protected:
	CChartLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartLineView)

// Form Data
protected:
	enum { IDD = IDD_CHART };

	CBCGPStatic		m_LineWnd;
	CBCGPStatic		m_DateWnd;
	CString			m_szLine;
	CString			m_szDate;
	CFont			m_Font;
	CBCGPChartCtrl	m_wndChart;

	int					m_nZoomType;
	CString				m_strInfo;
	BOOL				m_bInScroll;
	std::array<CBCGPColor::BCGP_COLOR, GLOBALDOSEMAXCOUNT> c_Colors;

protected:
	virtual void OnInitialUpdate() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;
	virtual CBCGPChartSeries* CreateSeries(const base::eMassflowSelect select, const int32_t index);
	virtual void AddLogItems();

	virtual void OnUpdateLineColor(const base::eMassflowSelect) override;
	virtual void OnUpdateCategory(const base::eMassflowSelect) override;
	virtual void OnUpdateLineWidth(const base::eMassflowSelect) override;
	virtual void OnUpdateVisible(const base::eMassflowSelect) override;


	virtual const base::eMassflowSelect GetSelection() const
	{ return base::eMassflowSelect::eVIEWMAX;	}

	virtual const CString GetTitle() const
	{	return CString("EasyGraph");	}

	virtual void OnUpdateSeries();
	virtual void OnUpdateChart();
	virtual void OnUpdateChartCategory();
	virtual void OnUpdateZoom();
	//virtual void OnRedraw();
	void UpdateScrollBars();

protected:
	virtual ~CChartLineView();

public:
	virtual CBCGPChartVisualObject* GetChart()
	{	return m_wndChart.GetChart();	}

	virtual CBCGPBaseVisualCtrl* GetCtrl()
	{	return &m_wndChart;	}

	virtual BOOL IsAnimationAvailable() const
	{	return FALSE;	}

	afx_msg LRESULT OnNewDate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


