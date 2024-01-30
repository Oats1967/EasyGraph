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
	m_nDataPoints = 0;
	m_nZoomType = 0;
	m_strInfo = _T("");
	m_nChartCategory = 0;
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
#if 0
	DDX_Control(pDX, IDC_ZOOM_TYPE, m_wndZoomType);
	DDX_CBIndex(pDX, IDC_DATA_POINTS, m_nDataPoints);
	DDX_CBIndex(pDX, IDC_ZOOM_TYPE, m_nZoomType);
	DDX_Text(pDX, IDC_INFO, m_strInfo);
	DDX_CBIndex(pDX, IDC_CHART_CATEGORY, m_nChartCategory);
	DDX_Check(pDX, IDC_EXTERNAL_SCROLLBAR, m_bExternalScrollBar);
#endif
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartLineView, CChartLineView)
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

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetLegendPosition(BCGPChartLayout::LP_NONE);
	pChart->SetThemeOpacity(70);

	OnUpdateChartCategory();
	OnUpdateChart();
	OnUpdateZoom();

	SetDefaultLineWidth();

	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateChart() 
{
	CWaitCursor wait;

	UpdateData();

	int nDataPoints = 0;

	switch (m_nDataPoints)
	{
	case 0:
		nDataPoints = 1000;
		break;

	case 1:
		nDataPoints = 10000;
		break;

	case 2:
		nDataPoints = 100000;
		break;

	case 3:
		nDataPoints = 1000000;
		break;
	}

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->CleanUpChartData ();

	double dblMaxVal = 0;
	double dblVal = 0;

	CBCGPDoubleArray arValues;
	arValues.SetSize(nDataPoints);

	for (int i = 0; i < nDataPoints; i++)
	{
		double dblDelta = dblVal > 500 ? Rand(-500, 500) : Rand(0, 1000);
		dblVal += dblDelta;

		arValues[i] = dblVal;

		dblMaxVal = max(dblMaxVal, dblVal);
	}

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	CBCGPChartAxis* pAxisY = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);

	CString strLabel;
	pAxisY->GetDisplayedLabel(dblMaxVal, strLabel);
 	pAxisY->m_strMaxDisplayedLabel = strLabel;

	pAxisX->GetDisplayedLabel(nDataPoints, strLabel);
	pAxisX->m_strMaxDisplayedLabel = strLabel;

	pChart->AddDataPointsOptimized(arValues);
	pChart->GetSeries(0)->SetCurveType(BCGPChartFormatSeries::CCT_SPLINE);

	pChart->SetDirty(TRUE, TRUE);

	UpdateScrollBars();

	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateChartCategory()
{
	UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	
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

	pChart->SetChartType(category, BCGP_CT_SIMPLE, FALSE, FALSE);

	if (bIsFixedIntervalWidth)
	{
		pAxisX->SetFixedIntervalWidth(nFixedIntervalWidth, nValuesPerInterval);
	}

	pAxisX->SetFixedDisplayRange(dblMinDisplayedValueX, dblMaxDisplayedValueX);

	pAxisY->SetScrollRange(dblMinScrollValueY, dblMaxScrollValueY);
	pAxisY->SetFixedDisplayRange(dblMinDisplayedValueY, dblMaxDisplayedValueY);

	pChart->SetDirty(TRUE, TRUE);

	UpdateScrollBars();

	m_wndChart.SetFocus();
}

void CChartLineView::OnUpdateZoom() 
{
	UpdateData();

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

	UpdateData(FALSE);
	m_wndChart.SetFocus();
}

void CChartLineView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CEasyGraphView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		m_wndChart.SetFocus();
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
	UpdateData();

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
