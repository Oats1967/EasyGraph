//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2022 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// DiagramBaseView.cpp : implementation of the CDiagramBaseView class
//

#include "stdafx.h"
#include "EasyControl.h"
#include "DiagramDoc.h"
#include "DiagramBaseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramBaseView

IMPLEMENT_DYNCREATE(CDiagramBaseView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CDiagramBaseView, CBCGPFormView)
	//{{AFX_MSG_MAP(CDiagramBaseView)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramBaseView construction/destruction

CDiagramBaseView::CDiagramBaseView(UINT id)
	: CBCGPFormView(id)
{
	EnableVisualManagerStyle();
	EnableLayout();

	SetWhiteBackground();
	SetControlsDefaultDlgBackground();

	m_bIsReady = FALSE;
	InitChart();
}

CDiagramBaseView::~CDiagramBaseView()
{
}

void CDiagramBaseView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagramBaseView)
	//}}AFX_DATA_MAP
}

BOOL CDiagramBaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CBCGPFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDiagramBaseView diagnostics

#ifdef _DEBUG
void CDiagramBaseView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CDiagramBaseView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CDiagramDoc* CDiagramBaseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiagramDoc)));
	return (CDiagramDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDiagramBaseView message handlers

LRESULT CDiagramBaseView::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CBCGPFormView::HandleInitDialog (wParam, lParam);

	if (GetLayout() != NULL)
	{
		UpdateData(FALSE);
		SetupLayout();
	}

	return lRes;
}
//*************************************************************************************************************************
//*************************************************************************************************************************
void CDiagramBaseView::OnInitialUpdate()
{
	CBCGPFormView::OnInitialUpdate();

	if (m_bIsReady)
	{
		return;
	}
	
	GetParentFrame()->RecalcLayout();

	if (AfxGetMainWnd() == NULL)
	{
		return;
	}

	UpdateData(FALSE);

	UpdateChartColorTheme(CBCGPChartTheme::CBCGPChartTheme::CT_FLAT_2014_1, FALSE);

	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart != NULL)
	{
		pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS);
	}
}
//*************************************************************************************************************************
//*************************************************************************************************************************
void CDiagramBaseView::SetupLayout()
{
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL && GetCtrl() != NULL)
	{
		pLayout->AddAnchor(GetCtrl()->GetDlgCtrlID(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
}
//************************************************************************************************************************************************
//************************************************************************************************************************************************
void CDiagramBaseView::UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}
	pChart->SetColors((CBCGPChartTheme::ChartTheme)nTheme, bIsDarkTheme);
}
//************************************************************************************************************************************************
//************************************************************************************************************************************************
void CDiagramBaseView::OnNcPaint()
{
	if (!CBCGPVisualManager::GetInstance()->OnDrawViewBorder(this))
	{
		Default();
	}
}
//************************************************************************************************************************************************
//************************************************************************************************************************************************
static void InitSeriesColors(BCGPSeriesColors& sc, const CBCGPColor& color)
{
	CBCGPColor colorGradient = color;
	colorGradient.MakeLighter(.5);

	sc.m_brElementFillColor.SetColors(color, colorGradient, (CBCGPBrush::BCGP_GRADIENT_TYPE)-1);
	CBCGPColor colorOutline = color;
	colorOutline.MakeDarker(.1);

	sc.m_brElementLineColor.SetColor(colorOutline);
	sc.m_brMarkerFillColor.SetColors(color, colorGradient, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
	sc.m_brMarkerFillColor.MakeLighter(.15);

	sc.m_brMarkerLineColor.SetColor(colorOutline);
	sc.m_brDataLabelLineColor.SetColor(colorOutline);

	sc.m_brDataLabelFillColor.SetColor(color);
	sc.m_brDataLabelFillColor.MakePale();

	CBCGPColor colorText = color;
	colorText.MakeDarker(.2);

	sc.m_brDataLabelTextColor.SetColor(colorText);
}
//************************************************************************************************************************************************
//************************************************************************************************************************************************
void CDiagramBaseView::InitChart()
{
	CBCGPChartTheme	m_CustomColorTheme{ CBCGPChartTheme::ChartTheme::CT_FLAT_2014_1 };

	// Setup custom color theme:
	CBCGPChartTheme::InitChartColors(m_CustomColorTheme,
		CBCGPColor::White,		// Fill color
		CBCGPColor::Chocolate,	// Outline color
		CBCGPColor::Maroon,		// Text color
		CBCGPColor(),
		CBCGPColor::SeaShell, .08);

	InitSeriesColors(m_CustomColorTheme.m_seriesColors[0], CBCGPColor::Crimson);
	InitSeriesColors(m_CustomColorTheme.m_seriesColors[1], CBCGPColor::BlueViolet);
	InitSeriesColors(m_CustomColorTheme.m_seriesColors[2], CBCGPColor::CornflowerBlue);
	InitSeriesColors(m_CustomColorTheme.m_seriesColors[3], CBCGPColor::DarkGreen);
	InitSeriesColors(m_CustomColorTheme.m_seriesColors[4], CBCGPColor::Coral);
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramBaseView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		LOGDEBUG("Activate CDiagramBaseView !");

		AfxGetMainWnd()->SendMessage(WM_ACTIVATE_VIEW, WPARAM(eViewSelect::eDiagramView), 0);
		BringWindowToTop();
	}
	else
	{
	}
	CBCGPFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
