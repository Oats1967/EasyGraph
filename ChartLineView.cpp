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
#include "wmuser.h"
#include "Statistics.h"

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
	, c_Colors{ CBCGPColor::BCGP_COLOR::DeepSkyBlue,
				CBCGPColor::BCGP_COLOR::Brown,
				CBCGPColor::BCGP_COLOR::Red,
				CBCGPColor::BCGP_COLOR::Violet,
				CBCGPColor::BCGP_COLOR::Gold,
				CBCGPColor::BCGP_COLOR::DarkGray,
				CBCGPColor::BCGP_COLOR::Green,
				CBCGPColor::BCGP_COLOR::Cornsilk,
				CBCGPColor::BCGP_COLOR::Cyan,
				CBCGPColor::BCGP_COLOR::DarkOliveGreen,
				CBCGPColor::BCGP_COLOR::ForestGreen,
				CBCGPColor::BCGP_COLOR::Yellow,
				CBCGPColor::BCGP_COLOR::Ivory,
				CBCGPColor::BCGP_COLOR::LightCyan,
				CBCGPColor::BCGP_COLOR::MediumOrchid,
				CBCGPColor::BCGP_COLOR::MediumPurple,
				CBCGPColor::BCGP_COLOR::MediumSeaGreen,
				CBCGPColor::BCGP_COLOR::MediumSlateBlue,
				CBCGPColor::BCGP_COLOR::MediumSpringGreen,
				CBCGPColor::BCGP_COLOR::MediumTurquoise,
				CBCGPColor::BCGP_COLOR::Tan,
				CBCGPColor::BCGP_COLOR::Teal,
				CBCGPColor::BCGP_COLOR::Thistle,
				CBCGPColor::BCGP_COLOR::Tomato,
				CBCGPColor::BCGP_COLOR::Turquoise,
				CBCGPColor::BCGP_COLOR::DarkSlateGray,
				CBCGPColor::BCGP_COLOR::DarkGoldenrod,
				CBCGPColor::BCGP_COLOR::DeepSkyBlue,
				CBCGPColor::BCGP_COLOR::DimGray,
				CBCGPColor::BCGP_COLOR::Gainsboro,
				CBCGPColor::BCGP_COLOR::Indigo,
				CBCGPColor::BCGP_COLOR::LightCoral }
{
	//{{AFX_DATA_INIT(CChartLineView)
	m_nZoomType = 0;
	m_strInfo = _T("");
	
	//}}AFX_DATA_INIT

	m_bInScroll = FALSE;
}

CChartLineView::~CChartLineView()
{
}

void CChartLineView::DoDataExchange(CDataExchange* pDX)
{
	CEasyGraphView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartLineView)
	DDX_Control(pDX, IDC_CHART, m_wndChart);
	DDX_Control(pDX, IDC_CHART_LINE, m_LineWnd);
	DDX_Control(pDX, IDC_CHART_DATE, m_DateWnd);
	CBCGPComboBox	m_KeySelectWnd;

	DDX_Text(pDX, IDC_CHART_LINE, m_szLine);
	DDX_Text(pDX, IDC_CHART_DATE, m_szDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartLineView, CEasyGraphView)
	//{{AFX_MSG_MAP(CChartLineView)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_NEWDATE, OnNewDate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartLineView diagnostics



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

	VERIFY(m_Font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	m_DateWnd.SetFont(&m_Font);
	m_DateWnd.m_clrText = COLORREF(RGB(0, 0, 0));
	m_LineWnd.SetFont(&m_Font);
	m_LineWnd.m_clrText = COLORREF(RGB(0, 0, 0));


	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChart->SetThemeOpacity(70);

	OnUpdateChart();
	OnUpdateZoom();
	UpdateScrollBars();
	OnUpdateChartCategory();
	m_wndChart.SetFocus();
}


CBCGPChartSeries* CChartLineView::CreateSeries( const base::eMassflowSelect select, const int32_t index)
{
	CBCGPChartSeries* pSeries = nullptr;

	const auto& rRecDayList = g_Statistics.GetRecDaysList();
	int32_t nDataPointCount = rRecDayList.GetCount();
	if (nDataPointCount > 0)
	{
		CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
		ASSERT_VALID(pChart);

		const auto& rLineAttrib = g_Statistics.GetLineAttribute(select);
		pSeries = pChart->CreateSeries(c_SelectString[_S32(select)], rLineAttrib.m_Color, BCGPChartType::BCGP_CT_SIMPLE, rLineAttrib.m_Category);
		if (pSeries)
		{
			const auto& cTimeSpan = g_Statistics.GetDateToShow();
			COleDateTime dtStartTime{ cTimeSpan.dateStart };

			for (int i = 0; i < nDataPointCount; i++)
			{
				const auto& rItem = rRecDayList.GetItem(i);

				COleDateTime dtTime{ rItem.GetTime() };
				auto szTime = dtTime.Format("%d.%m.%y %H:%M:%S");
				pSeries->AddDataPoint(szTime, rItem.Get(select, index));
			}
			BCGPChartFormatSeries style = pSeries->GetSeriesFormat();
			style.SetSeriesLineWidth(rLineAttrib.m_LineWidth);
			pSeries->SetSeriesFormat(style);
		}
	}
	return pSeries;
}



void CChartLineView::OnUpdateSeries()
{
	CWaitCursor wait;

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->CleanUpChartData(-1, TRUE);
	if (g_Statistics.GetActiveFeeder() < 0)
	{
		for (int32_t k = 0; k < g_Statistics.GetFeederCount(); k++)
		{
			auto pSeries = CreateSeries(GetSelection(), k);
			if (pSeries)
			{
				CString szName;
				szName.Format("Dosierung : %d", k + 1);
				pSeries->m_strSeriesName = szName;
				pSeries->SetSeriesLineColor(CBCGPBrush(c_Colors[k]));
			}
		}
	}
	else
	{
		auto pSeries = CreateSeries(GetSelection(), g_Statistics.GetActiveFeeder());
		if (pSeries)
		{
			CString szName;
			szName.Format("Dosierung : %d", g_Statistics.GetActiveFeeder() + 1);
			pSeries->m_strSeriesName = szName;
		}
	}
}


void CChartLineView::OnUpdateChart()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetCurveType(BCGPChartFormatSeries::CCT_LINE);
	pChart->SetChartTitle(GetTitle());
	pChart->ShowDataLabels(FALSE);

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	pAxisX->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);
	pChart->SetThemeOpacity(100);

	pChart->SetLegendPosition(BCGPChartLayout::LegendPosition::LP_BOTTOM);
	pChart->EnableDrawLegendShape(FALSE);
	UpdateScrollBars();
	pChart->SetDirty(TRUE, TRUE);
	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateZoom()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	switch (m_nZoomType)
	{
	case 0:
		// Default - "Mouse wheel and pan" mode
		m_strInfo = _T("Use mouse wheel to zoom chart. If the chart is zoomed, you can pan it using left mouse button.");
		pChart->SetZoomScrollConfig(BCGPChartMouseConfig::ZSO_WHEEL_PAN);
		break;

	case 1:
		m_strInfo = _T("Left mouse button - zoom-in; Right mouse button - zoom out. Mouse wheel - scroll horizontally.");
		pChart->SetZoomScrollConfig(BCGPChartMouseConfig::ZSO_MAGNIFY);
		break;
	}
	//m_wndChart.SetFocus();
}


void CChartLineView::OnUpdateChartCategory()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	CBCGPChartAxis* pAxisY = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);

	const double dblMinDisplayedValueX = pAxisX->GetMinDisplayedValue();
	const double dblMaxDisplayedValueX = pAxisX->GetMaxDisplayedValue();
	const BOOL bIsFixedIntervalWidth = pAxisX->IsFixedIntervalWidth();
	const int nValuesPerInterval = pAxisX->GetValuesPerInterval();
	const int nFixedIntervalWidth = pAxisX->GetFixedIntervalWidth();

	const double dblMinDisplayedValueY = pAxisY->GetMinDisplayedValue();
	const double dblMaxDisplayedValueY = pAxisY->GetMaxDisplayedValue();
	const double dblMinScrollValueY = pAxisY->GetMinScrollValue();
	const double dblMaxScrollValueY = pAxisY->GetMaxScrollValue();

	BCGPChartCategory  category = BCGPChartCategory::BCGPChartLine;
	auto select = GetSelection();
	if (select != base::eMassflowSelect::eVIEWMAX)
	{
		category = g_Statistics.GetLineAttribute(select).m_Category;
	}
	pChart->SetChartType(category, BCGP_CT_SIMPLE, FALSE, FALSE);

	if (bIsFixedIntervalWidth)
	{
		pAxisX->SetFixedIntervalWidth(nFixedIntervalWidth, nValuesPerInterval);
	}

	pAxisX->SetFixedDisplayRange(dblMinDisplayedValueX, dblMaxDisplayedValueX);

	pAxisY->SetScrollRange(dblMinScrollValueY, dblMaxScrollValueY);
	pAxisY->SetFixedDisplayRange(dblMinDisplayedValueY, dblMaxDisplayedValueY);

	UpdateScrollBars();
	pChart->SetDirty(TRUE, TRUE);
	m_wndChart.SetFocus();
}



void CChartLineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CEasyGraphView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		PostMessage(WM_NEWDATE);
	}
}


void CChartLineView::UpdateScrollBars()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->ShowScrollBars();
}

LRESULT CChartLineView::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	m_szLine = g_Statistics.GetHeaderLine();
	m_szDate = g_Statistics.GetHeaderDateTime();
	OnUpdateSeries();
	OnUpdateChart();
	OnUpdateZoom();
	OnUpdateChartCategory();
	UpdateData(FALSE);
	return 0L;
}



void CChartLineView::OnUpdateCategory(const base::eMassflowSelect select)
{
	SendMessage(WM_NEWDATE);
}


void CChartLineView::OnUpdateLineColor(const base::eMassflowSelect select)
{
	SendMessage(WM_NEWDATE);
}

void CChartLineView::OnUpdateLineWidth(const base::eMassflowSelect select)
{
	SendMessage(WM_NEWDATE);
}


void CChartLineView::OnUpdateVisible(const base::eMassflowSelect select)
{
	SendMessage(WM_NEWDATE);
}



