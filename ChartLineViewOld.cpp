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
// ChartLineView.cpp : implementation file
//

#include "pch.h"
#include "wmuser.h"
#include "ChartLineView.h"
#include "ConfigItem.h"
#include "global.h"
#include "VirtualSeries.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartLineView
IMPLEMENT_DYNCREATE(CChartLineView, CEasyGraphView)

CChartLineView::CChartLineView()
	: CEasyGraphView(CChartLineView::IDD)
{
	//{{AFX_DATA_INIT(CChartLineView)
	m_nLineStyle = 0;
	m_nLineWidth = 0;
	m_bShowDataMarkers = TRUE;
	m_nMarkerSize = 1;
	m_nMarkerShape = 0;
	m_bShowDataLabels = TRUE;
	m_nDataLabelAngle = 3;
	m_bDisplayShadow = TRUE;
	//}}AFX_DATA_INIT

	m_nLineWidth = theConfig.GetDefaultLineWidth() - 1;
}

CChartLineView::~CChartLineView()
{
}

void CChartLineView::DoDataExchange(CDataExchange* pDX)
{
	CEasyGraphView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartLineView)
	DDX_Control(pDX, IDC_CHART, m_wndChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartLineView, CEasyGraphView)
	//{{AFX_MSG_MAP(CChartLineView)
#if 0
	ON_CBN_SELENDOK(IDC_LINE_STYLE, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_LINE_WIDTH, OnUpdateChart)
	ON_BN_CLICKED(IDC_DATA_LABELS, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_DATA_MARKER_SHAPE, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_DATA_MARKER_SIZE, OnUpdateChart)
	ON_BN_CLICKED(IDC_DATA_MARKERS, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_DATA_LABEL_ANGLE, OnUpdateChart)
	ON_BN_CLICKED(IDC_SERIES_SHADOW, OnUpdateChart)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
#endif
	ON_MESSAGE(WM_NEWDATE, OnNewDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartLineView diagnostics

#ifdef _DEBUG
void CChartLineView::AssertValid() const
{
	CEasyGraphView::AssertValid();
}

void CChartLineView::Dump(CDumpContext& dc) const
{
	CEasyGraphView::Dump(dc);
}
#endif //_DEBUG


void CChartLineView::OnInitialUpdate() 
{
	CEasyGraphView::OnInitialUpdate();
	
	if (m_bIsReady)
	{
		return;
	}
	m_bIsReady = TRUE;
	SendMessage(WM_NEWDATE);
}

void CChartLineView::OnUpdateChart() 
{
#if 0
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->ShowDataMarkers(m_bShowDataMarkers, GetMarkerSize(m_nMarkerSize), 
		(BCGPChartMarkerOptions::MarkerShape)m_nMarkerShape);

	double dblDataLabelAngle = (m_nDataLabelAngle - 2) * 45;

	pChart->ShowDataLabels(m_bShowDataLabels, TRUE, TRUE, dblDataLabelAngle);

	for (int i = 0; i < pChart->GetSeriesCount(); i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i);
		ASSERT_VALID(pSeries);

		BCGPChartFormatSeries style = pSeries->GetSeriesFormat();

		style.m_curveType = BCGPChartFormatSeries::CCT_SPLINE;
#if 0
	 	(theConfig.m_Feature == CDemoFeature::BCGP_StackedLineSpline) ?
			BCGPChartFormatSeries::CCT_SPLINE : 
			(theConfig.m_Feature == CDemoFeature::BCGP_StepLine) ?
			BCGPChartFormatSeries::CCT_STEP :
			BCGPChartFormatSeries::CCT_LINE;
#endif
		style.SetSeriesOutlineDashStyle((CBCGPStrokeStyle::BCGP_DASH_STYLE)m_nLineStyle);
		style.SetSeriesLineWidth(m_nLineWidth + 1);

		style.m_shadowType.m_bDisplayShadow = m_bDisplayShadow;

		pSeries->SetSeriesFormat(style);
	}

	pChart->Redraw();
#endif
}


LRESULT CChartLineView::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->CleanUpChartData(-1, TRUE);
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS);
	pChart->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE, FALSE, FALSE);

	CVirtualSeries* pVirtualSeries = new CVirtualSeries(pChart);
	pVirtualSeries->CreateData(g_Statistics.GetDoseSelect(), 0);
//	pChart->AddDataPointsOptimized(pVirtualSeries->m_arXValues);
	pVirtualSeries->SetCurveType(BCGPChartFormatSeries::CCT_SPLINE);

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	pXAxis->m_bFormatAsDate = TRUE;
	pXAxis->UseApproximation(FALSE);
	pXAxis->SetFixedDisplayRange(pVirtualSeries->GetFirstDate(), pVirtualSeries->GetLastDate());
	pXAxis->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);
	pChart->AddSeries(pVirtualSeries);
	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE, FALSE, FALSE);

	SetDefaultLineWidth();
	pChart->RecalcMinMaxValues();
	pChart->SetDirty(TRUE, TRUE);

#if 0

	pChart->CleanUpChartData(-1, TRUE);
	pChart->SetLegendPosition(BCGPChartLayout::LP_NONE);

	CVirtualSeries* pVirtualSeries = new CVirtualSeries(pChart);
	pVirtualSeries->CreateData(365);
	pVirtualSeries->SetCurveType(BCGPChartFormatSeries::CCT_SPLINE);

	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE);

	//m_DoseSelect
	auto pSeries1 = pChart->CreateSeries(_T("Valuelist"));
	//CBCGPChartSeries* pSeries1 = DYNAMIC_DOWNCAST(CBCGPChartBarSeries, pChart->CreateSeries(_T("Valuelist")));
	pChart->SetChartTitle(_T("ValueList"));
	if ( ! g_Statistics.GetRecDaysList().empty())
	{
		const base::utils::CRecItemList& rFirst = g_Statistics.GetRecDaysList().begin()->second;
		const base::eMassflowSelect& rDoseSelect = g_Statistics.GetDoseSelect();;
		const int32_t ActiveFeeder = 0;;

		for (int32_t i = 0; i < rFirst.GetCount(); i++)
		{
			//float32_t x = g_RecList
			const auto& rItem = rFirst.GetItem(i);
			const auto val = rItem.Get(rDoseSelect, ActiveFeeder);
			const auto rTime = COleDateTime(rItem.GetTime());
			int32_t h = rTime.GetHour();
			int32_t m = rTime.GetMinute();
			int32_t s = rTime.GetSecond();
			float32_t hf = _F32(h) + (_F32(m) * 60.0F + _F32(s)) / 3600.0F;

			pSeries1->AddDataPoint(val, hf);
		}
	}
	pSeries1->SetMinValue(0.0);
	//pSeries1->SetMaxValue(rMap.GetMax());

	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);
	pYAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pYAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->SetFixedIntervalWidth(1.0);
	//pXAxis->ShowScrollBar(TRUE);
	OnUpdateChart();
	UpdateData(FALSE);

#endif

	return 0L;
}


void CChartLineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
	if (bActivate)
		SendMessage(WM_NEWDATE);
	CEasyGraphView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


