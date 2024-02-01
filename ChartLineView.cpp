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
	, m_LineColor{ CBCGPColor::BCGP_COLOR::Blue }
	, m_nChartCategory{ BCGPChartCategory::BCGPChartLine }
{
	//{{AFX_DATA_INIT(CChartLineView)
	m_nZoomType = 0;
	m_strInfo = _T("");
	
	m_bExternalScrollBar = FALSE;
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
	DDX_Control(pDX, IDC_SCROLLBAR_VERT, m_wndScrollBarVert);
	DDX_Control(pDX, IDC_SCROLLBAR_HORZ, m_wndScrollBarHorz);
	DDX_Control(pDX, IDC_CHART, m_wndChart);
	DDX_Control(pDX, IDC_CHART_LINE, m_LineWnd);
	DDX_Control(pDX, IDC_CHART_DATE, m_DateWnd);
	CBCGPComboBox	m_KeySelectWnd;

	DDX_Text(pDX, IDC_CHART_LINE, m_szLine);
	DDX_Text(pDX, IDC_CHART_DATE, m_szDate);

#if 0
	DDX_Control(pDX, IDC_ZOOM_TYPE, m_wndZoomType);
	DDX_CBIndex(pDX, IDC_ZOOM_TYPE, m_nZoomType);
	DDX_Text(pDX, IDC_INFO, m_strInfo);
	DDX_CBIndex(pDX, IDC_CHART_CATEGORY, m_nChartCategory);
	DDX_Check(pDX, IDC_EXTERNAL_SCROLLBAR, m_bExternalScrollBar);
#endif
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartLineView, CEasyGraphView)
	//{{AFX_MSG_MAP(CChartLineView)
#if 0
	ON_CBN_SELENDOK(IDC_DATA_POINTS, OnUpdateChart)
	ON_CBN_SELENDOK(IDC_CHART_CATEGORY, OnUpdateChartCategory)
	ON_CBN_SELENDOK(IDC_ZOOM_TYPE, OnUpdateZoom)
	ON_BN_CLICKED(IDC_EXTERNAL_SCROLLBAR, OnExternalScrollbar)
#endif
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ON_CHART_AXIS_SCROLLED, OnAxisScrolled)
	ON_REGISTERED_MESSAGE(BCGM_ON_CHART_AXIS_ZOOMED, OnAxisZoomed)
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

	OnUpdateChartCategory();
	OnUpdateChart();
	OnUpdateZoom();
	OnExternalScrollbar();
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

		CString szFormat;
		szFormat.Format("Dosierung : % d", index+1);

		pSeries = pChart->CreateSeries(szFormat, CBCGPColor(), BCGPChartType::BCGP_CT_SIMPLE, m_nChartCategory);
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
		}
	}
	return pSeries;
}



void CChartLineView::OnUpdateChart() 
{
	CWaitCursor wait;

	//UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->CleanUpChartData(-1, TRUE);
	if (g_Statistics.GetActiveFeeder() < 0)
	{
		for (int32_t k = 0; k < g_Statistics.GetFeederCount(); k++)
		{
			CreateSeries(GetSelection(), k);
		}
	}
	else
	{
		CreateSeries(GetSelection(), g_Statistics.GetActiveFeeder());
	}
	SetDefaultLineWidth();
	SetSeriesLineColor(&m_LineColor, 1);

	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE, FALSE, FALSE);
	pChart->SetCurveType(BCGPChartFormatSeries::CCT_LINE);
	pChart->SetChartTitle(GetTitle());
	//pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
	//pChart->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);
	pChart->ShowDataLabels(FALSE);

	//int32_t nDataPoints = pSeries->GetDataPointCount();

	//double dblMaxVal = 0;
	//double dblVal = 0;

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);

#if 0
	CBCGPChartAxis* pAxisY = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	CString strLabel;
	pAxisY->GetDisplayedLabel(dblMaxVal, strLabel);
 	pAxisY->m_strMaxDisplayedLabel = strLabel;

	pAxisX->GetDisplayedLabel(nDataPoints, strLabel);
	pAxisX->m_strMaxDisplayedLabel = strLabel;
#endif
	//pAxisX->m_bFormatAsDate = TRUE;
	//pAxisX->UseApproximation(FALSE);
	//pAxisX->SetFixedDisplayRange(pSeries->GetFirstDate(), pSeries->GetLastDate());
	pAxisX->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);
#if 0
	for (auto& rItem : pVec)
	{
		pChart->AddSeries(rItem);
	}
	pVec.clear();
	auto c = pChart->GetSeriesCount();
	if (c > 0)
	{
		auto pSeries = pChart->GetSeries(0);
		uint32_t dp = pSeries->GetDataPointCount();
	}


	pSeries->SetChartType(BCGPChartLine);
	auto format = pSeries->GetSeriesFormat();
	format.m_curveType = BCGPChartFormatSeries::CCT_LINE;
	pSeries->SetSeriesFormat(format);
#endif
	pChart->SetThemeOpacity(100);
	pChart->SetDirty(TRUE, FALSE);

	//pChart->EnableLegendCustomPosition(FALSE); // clear custom state
	pChart->SetLegendPosition(BCGPChartLayout::LegendPosition::LP_BOTTOM);
	pChart->EnableDrawLegendShape(FALSE);

	UpdateScrollBars();
	pChart->Redraw();

	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateChartCategory()
{
	//UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
#if 0	
	BCGPChartCategory category = BCGPChartLine;
	switch(m_nChartCategory)
	{
	case 0:
		category = BCGPChartLine;
		break;
		
	case 1:
		category = BCGPChartArea;
		break;
		
	case 2:
		category = BCGPChartColumn;
		break;
		
	case 3:
		category = BCGPChartBar;
		break;
		
	case 4:
		category = BCGPChartHistogram;
		break;
	}
#endif
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

	pChart->SetChartType(m_nChartCategory, BCGP_CT_SIMPLE, FALSE, FALSE);

	if (bIsFixedIntervalWidth)
	{
		pAxisX->SetFixedIntervalWidth(nFixedIntervalWidth, nValuesPerInterval);
	}

	pAxisX->SetFixedDisplayRange(dblMinDisplayedValueX, dblMaxDisplayedValueX);

	pAxisY->SetScrollRange(dblMinScrollValueY, dblMaxScrollValueY);
	pAxisY->SetFixedDisplayRange(dblMinDisplayedValueY, dblMaxDisplayedValueY);

	pChart->SetDirty(TRUE, false);

	UpdateScrollBars();

	pChart->Redraw();

	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateZoom() 
{
	//UpdateData();

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

LRESULT CChartLineView::OnAxisScrolled(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_bExternalScrollBar || m_bInScroll)
	{
		return 0;
	}

	UpdateScrollBars();
	return 0;
}

LRESULT CChartLineView::OnAxisZoomed(WPARAM /*wp*/, LPARAM /*lp*/)
{
	if (!m_bExternalScrollBar)
	{
		return 0;
	}
	
	UpdateScrollBars();	
	return 0;
}

void CChartLineView::OnExternalScrollbar() 
{
	//UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	m_wndScrollBarHorz.ShowWindow(m_bExternalScrollBar ? SW_SHOWNOACTIVATE : SW_HIDE);
	m_wndScrollBarVert.ShowWindow(m_bExternalScrollBar ? SW_SHOWNOACTIVATE : SW_HIDE);

	UpdateScrollBars();
}

void CChartLineView::SetupLayout()
{
	CEasyGraphView::SetupLayout();
	
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(IDC_SCROLLBAR_HORZ, CBCGPStaticLayout::e_MoveTypeVert, CBCGPStaticLayout::e_SizeTypeHorz);
		pLayout->AddAnchor(IDC_SCROLLBAR_VERT, CBCGPStaticLayout::e_MoveTypeHorz, CBCGPStaticLayout::e_SizeTypeVert);
	}
}

void CChartLineView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->GetSafeHwnd() == m_wndScrollBarHorz.GetSafeHwnd())
	{
		DoScroll(m_wndScrollBarHorz, nSBCode);
		return;
	}
	
	CEasyGraphView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChartLineView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->GetSafeHwnd() == m_wndScrollBarVert.GetSafeHwnd())
	{
		DoScroll(m_wndScrollBarVert, nSBCode);
		return;
	}
	
	CEasyGraphView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CChartLineView::UpdateScrollBars()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	
	if (!m_bExternalScrollBar)
	{
		pChart->ShowScrollBars();
		return;
	}

	for (int i = 0; i < 2; i++)
	{
		BOOL bIsHorizontal = (i == 0);

		CBCGPScrollBar& wndScrollBar = bIsHorizontal ? m_wndScrollBarHorz : m_wndScrollBarVert;

		CBCGPChartAxis* pAxis = GetChartAxis(bIsHorizontal);
		ASSERT_VALID(pAxis);
		
		if (pAxis->CanBeScrolled())
		{
			SCROLLINFO si;
			ZeroMemory (&si, sizeof (SCROLLINFO));
			
			si.cbSize = sizeof (SCROLLINFO);
			si.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;

			si.nPage = (int)(pAxis->GetMaxDisplayedValue() - pAxis->GetMinDisplayedValue());
			si.nMin = (int)pAxis->GetMinScrollValue();
			si.nMax = (int)pAxis->GetMaxScrollValue();
			si.nPos = bIsHorizontal ? (int)pAxis->GetMinDisplayedValue() : (int)pAxis->GetMaxScrollValue();
			
			wndScrollBar.SetScrollInfo(&si, TRUE);
			wndScrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
			wndScrollBar.EnableWindow();
		}
		else
		{
			wndScrollBar.SetScrollRange(0, 0);
			wndScrollBar.EnableScrollBar(ESB_DISABLE_BOTH);
		}
	}

	pChart->ShowScrollBars(FALSE);
}

void CChartLineView::DoScroll(CBCGPScrollBar& wndScrollBar, UINT nSBCode)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	BOOL bIsHorizontal = wndScrollBar.GetSafeHwnd() == m_wndScrollBarHorz.GetSafeHwnd();
	CBCGPChartAxis* pAxis = GetChartAxis(bIsHorizontal);
	ASSERT_VALID(pAxis);
	
	SCROLLINFO si;
	ZeroMemory(&si, sizeof(SCROLLINFO));
	
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL;
	
	wndScrollBar.GetScrollInfo (&si);

	int iOffset = si.nPos;
	int nPage = si.nPage;
	
	switch (nSBCode)
	{
	case SB_LINEUP:
		iOffset--;
		break;
		
	case SB_LINEDOWN:
		iOffset++;
		break;
		
	case SB_TOP:
		iOffset = si.nMin;
		break;
		
	case SB_BOTTOM:
		iOffset = si.nMax;
		break;
		
	case SB_PAGEUP:
		iOffset -= nPage;
		break;
		
	case SB_PAGEDOWN:
		iOffset += nPage;
		break;
		
	case SB_THUMBPOSITION:
		iOffset = si.nPos;
		break;

	case SB_THUMBTRACK:
		iOffset = si.nTrackPos;
		break;
		
	default:
		return;
	}
	
	iOffset = min (max (0, iOffset), si.nMax);
	
	wndScrollBar.SetScrollPos (iOffset);
	wndScrollBar.GetScrollInfo (&si);
	
	m_bInScroll = TRUE;

	double dScrollValue;
	if (!bIsHorizontal)
	{
		dScrollValue = pAxis->GetMaxScrollValue() - iOffset - si.nPage / 2;
	}
	else
	{
		dScrollValue = iOffset + si.nPage / 2;
	}
	pAxis->ScrollTo(dScrollValue);

	pChart->SetDirty(TRUE, TRUE);

	
	m_bInScroll = FALSE;
}

CBCGPChartAxis* CChartLineView::GetChartAxis(BOOL bIsHorizontal)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);

	if (bIsHorizontal)
	{
		return pXAxis->IsVertical() ? pYAxis : pXAxis;
	}

	return pYAxis->IsVertical() ? pYAxis : pXAxis;
}


LRESULT CChartLineView::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	m_szLine = g_Statistics.GetHeaderLine();
	m_szDate = g_Statistics.GetHeaderDateTime();
	OnUpdateChartCategory();
	OnUpdateChart();
	OnUpdateZoom();
	UpdateData(FALSE);
	return 0L;
}


void CChartLineView::OnSetLineColor(const CBCGPColor& rColor)
{
	m_LineColor = rColor;
	SetSeriesLineColor(&m_LineColor, 1);
	UpdateScrollBars();
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->Redraw();
}

void CChartLineView::OnSetCategory(const BCGPChartCategory& rCategory)
{
	m_nChartCategory = rCategory;
	//OnUpdateChartCategory();
	OnUpdateChart();
	OnUpdateZoom();
	UpdateData(FALSE);
}

