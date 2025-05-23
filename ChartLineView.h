#pragma once

#include "EasyGraph.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "Statistics.h"


struct MarkerInfo
{
	BOOL	m_ShowDataLabel = FALSE;
	int32_t m_DataLabelAngle = 0;
	BOOL	m_ShowMarker = FALSE;
	int32_t m_MarkerSize = 4;
	BOOL	m_DataLabelDropLineToMarker = FALSE;
};


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
	CBCGPStatic		m_ANWnd;
	CString			m_szLine;
	CString			m_szDate;
	CString			m_szANNumber;
	CFont			m_Font;
	CBCGPChartCtrl	m_wndChart;

	BCGPChartMouseConfig::ZoomScrollOptions	m_nZoomType;
	CString				m_strInfo;
	BOOL				m_bInScroll;
	std::array<CBCGPColor::BCGP_COLOR, GLOBALDOSEMAXCOUNT> c_Colors;

protected:
	virtual void OnInitialUpdate() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;
	virtual CBCGPChartSeries* CreateSeries(const base::eMassflowSelect select, const int32_t index, const MarkerInfo* pMarkerInfo = nullptr);
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

	afx_msg void OnMenuSelect(UINT id);
	afx_msg void OnUpdateMenuSelect(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};


