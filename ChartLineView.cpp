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
#include "ChartLineView.h"
#include "ConfigItem.h"

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
#if 0
	DDX_Control(pDX, IDC_SERIES_SHADOW, m_wndDisplayShadow);
	DDX_Control(pDX, IDC_DATA_MARKERS, m_wndShowDataMarkers);
	DDX_Control(pDX, IDC_DATA_LABEL_ANGLE, m_wndDataLabelAngle);
	DDX_Control(pDX, IDC_DATA_MARKER_SHAPE, m_wndMarkerShape);
	DDX_Control(pDX, IDC_DATA_MARKER_SIZE, m_wndMarkerSize);
#endif
	DDX_Control(pDX, IDC_CHART, m_wndChart);
#if 0
	DDX_CBIndex(pDX, IDC_DATA_MARKER_SHAPE, m_nMarkerShape);
	DDX_CBIndex(pDX, IDC_LINE_STYLE, m_nLineStyle);
	DDX_CBIndex(pDX, IDC_LINE_WIDTH, m_nLineWidth);
	DDX_Check(pDX, IDC_DATA_MARKERS, m_bShowDataMarkers);
	DDX_CBIndex(pDX, IDC_DATA_MARKER_SIZE, m_nMarkerSize);
	DDX_Check(pDX, IDC_DATA_LABELS, m_bShowDataLabels);
	DDX_CBIndex(pDX, IDC_DATA_LABEL_ANGLE, m_nDataLabelAngle);
	DDX_Check(pDX, IDC_SERIES_SHADOW, m_bDisplayShadow);
#endif
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
#endif
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

/////////////////////////////////////////////////////////////////////////////
// CChartLineView message handlers

void CChartLineView::OnInitialUpdate() 
{
	CEasyGraphView::OnInitialUpdate();
	
	if (m_bIsReady)
	{
		return;
	}

	m_bIsReady = TRUE;

	//SetupShapeCombo(&m_wndMarkerShape, m_nMarkerShape);

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	auto m_Feature = theConfig.m_Feature;
	BCGPChartType type = 
		(m_Feature == CDemoFeature::BCGP_StackedLine || m_Feature == CDemoFeature::BCGP_StackedLineSpline) ? BCGP_CT_STACKED :
		(m_Feature == CDemoFeature::BCGP_StackedLine100) ? 
		BCGP_CT_100STACKED : BCGP_CT_SIMPLE;

	pChart->SetChartType(BCGPChartLine, type);

	pChart->SetChartTitle(_T("Furniture Sales"));

	CBCGPChartSeries* pSeries1 = pChart->CreateSeries(_T("Tables"));
	CBCGPChartSeries* pSeries2 = pChart->CreateSeries(_T("Chairs"));
	CBCGPChartSeries* pSeries3 = pChart->CreateSeries(_T("Cabinets"));

	pSeries1->AddDataPoint(m_arYears[4], 5.);
	pSeries1->AddDataPoint(m_arYears[3], 8.);
	pSeries1->AddDataPoint(m_arYears[2], 12.);
	pSeries1->AddDataPoint(m_arYears[1], 13.);
	pSeries1->AddDataPoint(m_arYears[0], 15.);

	pSeries2->AddDataPoint(9.);
	pSeries2->AddDataPoint(15.);
	pSeries2->AddDataPoint(19.);
	pSeries2->AddDataPoint(21.);
	pSeries2->AddDataPoint(23.);

	pSeries3->AddDataPoint(2.);
	pSeries3->AddDataPoint(3.);
	pSeries3->AddDataPoint(16.);
	pSeries3->AddDataPoint(19.);
	pSeries3->AddDataPoint(19.);

	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);

	pYAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pYAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	if (m_wndChart.GetGraphicsManager()->GetType() == CBCGPGraphicsManager::BCGP_GRAPHICS_MANAGER_GDI)
	{
		//m_wndDisplayShadow.EnableWindow(FALSE);
		//m_wndDisplayShadow.ShowWindow(SW_HIDE);

		m_bDisplayShadow = FALSE;
	}

	OnUpdateChart();
}

void CChartLineView::OnUpdateChart() 
{
	UpdateData();

#if 0
	m_wndMarkerShape.EnableWindow(m_bShowDataMarkers);
	m_wndMarkerSize.EnableWindow(m_bShowDataMarkers);

	m_wndDataLabelAngle.EnableWindow(m_bShowDataLabels);
#endif

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


		style.m_curveType = (theConfig.m_Feature == CDemoFeature::BCGP_StackedLineSpline) ?
			BCGPChartFormatSeries::CCT_SPLINE : 
			(theConfig.m_Feature == CDemoFeature::BCGP_StepLine) ?
			BCGPChartFormatSeries::CCT_STEP :
			BCGPChartFormatSeries::CCT_LINE;

		style.SetSeriesOutlineDashStyle((CBCGPStrokeStyle::BCGP_DASH_STYLE)m_nLineStyle);
		style.SetSeriesLineWidth(m_nLineWidth + 1);

		style.m_shadowType.m_bDisplayShadow = m_bDisplayShadow;

		pSeries->SetSeriesFormat(style);
	}
	
	pChart->Redraw();
}
