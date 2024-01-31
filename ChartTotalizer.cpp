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
// ChartTotalizer.cpp : implementation file
//

#include "pch.h"
#include "wmuser.h"
#include "ChartTotalizer.h"
#include "ConfigItem.h"
#include "global.h"
#include "StringConvert.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartTotalizer

IMPLEMENT_DYNCREATE(CChartTotalizer, CEasyGraphView)

CChartTotalizer::CChartTotalizer()
	: CEasyGraphView(CChartTotalizer::IDD)
	, m_KeySelected (0)
{
	//{{AFX_DATA_INIT(CChartTotalizer)

}

CChartTotalizer::~CChartTotalizer()
{
}

void CChartTotalizer::DoDataExchange(CDataExchange* pDX)
{
	CEasyGraphView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartTotalizer)
	DDX_Control(pDX, IDC_TOTALIZER, m_wndChart);
	DDX_Control(pDX, IDC_TOTALIZER_LINE, m_LineWnd);
	DDX_Control(pDX, IDC_TOTALIZER_DATE, m_DateWnd);
	DDX_Text(pDX, IDC_TOTALIZER_LINE, m_szLine);
	DDX_Text(pDX, IDC_TOTALIZER_DATE, m_szDate);
	DDX_Control(pDX, IDC_TOTALIZER_KEYSELECT, m_KeySelectWnd);
	DDX_CBIndex(pDX, IDC_TOTALIZER_KEYSELECT, m_KeySelected);


	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartTotalizer, CEasyGraphView)
	//{{AFX_MSG_MAP(CChartTotalizer)
	ON_MESSAGE(WM_NEWDATE, OnNewDate)
	ON_CBN_SELENDOK(IDC_TOTALIZER_KEYSELECT, OnKeySelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartTotalizer diagnostics

#ifdef _DEBUG
void CChartTotalizer::AssertValid() const
{
	CEasyGraphView::AssertValid();
}

void CChartTotalizer::Dump(CDumpContext& dc) const
{
	CEasyGraphView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChartTotalizer message handlers
void CChartTotalizer::SetupSelectCombo(CComboBox* pComboBox, int nSel /*= -1*/)
{
	ASSERT_VALID(pComboBox);

	pComboBox->ResetContent();
	pComboBox->AddString(_T("QMNummer"));
	pComboBox->AddString(_T("Produktname"));

	if (nSel >= 0)
	{
		pComboBox->SetCurSel(nSel);
	}
}


void CChartTotalizer::OnInitialUpdate() 
{
	CEasyGraphView::OnInitialUpdate();
	
	if (m_bIsReady)
	{
		return;
	}

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

	m_bIsReady = TRUE;
	SetupSelectCombo(&m_KeySelectWnd, m_KeySelected);

	//SetupShapeCombo(&m_wndMarkerShape, m_nMarkerShape);
	SendMessage(WM_NEWDATE);
}

void CChartTotalizer::OnUpdateChart()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	// for 3D bars label position can't be changed and is always center
	BOOL bCanChangeLabelPos = FALSE;

	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);

	pChart->ShowDataLabels(TRUE);

	BCGPChartDataLabelOptions dataLabelOptions = pChart->GetDataLabelOptions();
	dataLabelOptions.m_position = BCGPChartDataLabelOptions::LabelPosition::LP_OUTSIDE_END;
	//dataLabelOptions.m_strDataFormat = CString("%3.1 [kg/h]");

	CBCGPBrush::BCGP_GRADIENT_TYPE type = CBCGPBrush::BCGP_NO_GRADIENT;
	for (int i = 0; i < pChart->GetSeriesCount(); i++)
	{
		CBCGPChartBarSeries* pSeries = DYNAMIC_DOWNCAST(CBCGPChartBarSeries, pChart->GetSeries(i));
		ASSERT_VALID(pSeries);
		pSeries->SetDefaultFillGradientType(type);
		BCGPChartFormatSeries style = pSeries->GetSeriesFormat();

		if (i == 0 )
		{
			pSeries->SetBarShape(CBCGPChartBarSeries::BarShape::BS_BOX);
		}
		pSeries->EnableAutoColorDataPoints();
	}
	pChart->SetThemeOpacity(100);

	pChart->SetDirty();
	pChart->Redraw();
}


LRESULT CChartTotalizer::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	auto m_Feature = theConfig.m_Feature;

	pChart->CleanUpChartData(-1, TRUE);
	pChart->SetChartType(BCGPChartColumn);

	CBCGPChartSeries* pSeries1 = DYNAMIC_DOWNCAST(CBCGPChartBarSeries, pChart->CreateSeries(_T("Totalisator")));
	pChart->SetChartTitle(_T("Totalisator"));

	const auto& rMap = g_Statistics.GetQMTotalizerMap();
	const auto& rBase = g_Statistics.GetProductDatabase();
	const auto& rBaseMap = rBase.getMap();
	for (const auto& rItem : rMap)
	{
		CString szTemp;
		if (m_KeySelected == 0)
		{
			szTemp = CString(std::to_string(rItem.first).c_str());
		}
		else
		{
			auto it = rBaseMap.find(rItem.first);
			if (it != rBaseMap.cend())
			{
				szTemp = toCString(it->second.c_str());
			}
			else
			{
				szTemp = "??";
			}
		}
		if (! szTemp.IsEmpty())
		{
			pSeries1->AddDataPoint(szTemp, rItem.second);
		}
	}
	pSeries1->SetMinValue(0.0);
	pSeries1->SetMaxValue(rMap.GetMax());


	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);

	pYAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pYAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	m_szLine = g_Statistics.GetHeaderLine();
	m_szDate = g_Statistics.GetHeaderDateTime();

	OnUpdateChart();
	UpdateData(FALSE);

	return 0L;
}



void CChartTotalizer::OnKeySelect()
{
	auto m_KeySelect = m_KeySelectWnd.GetCurSel();
	SendMessage(WM_NEWDATE);
}

void CChartTotalizer::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
	if ( bActivate )
		SendMessage(WM_NEWDATE);
	CEasyGraphView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

