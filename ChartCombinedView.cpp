#include "pch.h"
#include "CustomChart.h"
#include "CustomYAxis.h"
#include "ChartCombinedView.h"




/////////////////////////////////////////////////////////////////////////////
// CChartCombinedView

IMPLEMENT_DYNCREATE(CChartCombinedView, CChartLineView)

CChartCombinedView::CChartCombinedView()
	: CChartLineView()
{
	m_wndChart.CreateCustomChart(RUNTIME_CLASS(CCustomChart));
}

CChartCombinedView::~CChartCombinedView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CChartCombinedView diagnostics

void CChartCombinedView::SetupAxis()
{
	CCustomChart* pChart = DYNAMIC_DOWNCAST(CCustomChart, m_wndChart.GetChart());
	ASSERT_VALID(pChart);
	pChart->RemoveCustomAxis();
	std::array<BOOL, base::cMassflowSelectMax> select;
	select.fill(FALSE);

	int32_t k = 0;
	for (auto& rItem : m_pSeries)
	{
		select[_S32(rItem.first)] = TRUE;
		k++;
	}
	if (k > 0)
	{
		if (select[_S32(base::eMassflowSelect::eVIEWMASSFLOW)])
		{
			select[_S32(base::eMassflowSelect::eVIEWMASSFLOW)] = FALSE;
			CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
			m_pSeries[base::eMassflowSelect::eVIEWMASSFLOW]->SetRelatedAxis(pYAxis, CBCGPChartSeries::AI_Y);
			k--;
		}
	}
	if (k > 0)
	{
		if (select[_S32(base::eMassflowSelect::eVIEWSETPOINT)])
		{
			select[_S32(base::eMassflowSelect::eVIEWSETPOINT)] = FALSE;
			CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
			m_pSeries[base::eMassflowSelect::eVIEWSETPOINT]->SetRelatedAxis(pYAxis, CBCGPChartSeries::AI_Y);
			k--;
		}
	}
	if (k > 0)
	{
		BOOL bReplace = TRUE;
		for (int32_t i = 0; i < base::cMassflowSelectMax; i++)
		{
			if (select[i])
			{
				select[i] = FALSE;
				CBCGPChartAxis* pYAxis = nullptr;
				if (bReplace)
				{
					bReplace = FALSE;
					pYAxis = pChart->ReplaceDefaultAxis(BCGP_CHART_Y_SECONDARY_AXIS, RUNTIME_CLASS(CCustomYAxisBase));
				}
				else
				{
					pYAxis = pChart->AddCustomAxis();
				}
				if (pYAxis)
				{
					m_pSeries[base::eMassflowSelect(i)]->SetRelatedAxis(pYAxis, CBCGPChartSeries::AI_Y);
				}
				if (--k == 0)
					break;
			}
		}
	}
	ASSERT(k == 0);
}


void CChartCombinedView::OnColorThemeUpdated()
{
	CCustomChart* pChart = DYNAMIC_DOWNCAST(CCustomChart, m_wndChart.GetChart());
	ASSERT_VALID(pChart);

	for (int i = 0; i < pChart->GetSeriesCount(); i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i, FALSE);

		if (pSeries == NULL)
		{
			continue;
		}

		BCGPSeriesColorsPtr seriesColors;

		// get base series colors
		pSeries->GetColors(seriesColors, -1);

		// set axis color to series color
		CBCGPChartAxis* pAxis = pSeries->GetRelatedAxis(CBCGPChartSeries::AI_Y);

		pAxis->m_axisLineFormat.m_brLineColor = *seriesColors.m_pBrElementLineColor;
		pAxis->m_axisLabelsFormat.m_brTextColor = *seriesColors.m_pBrElementLineColor;
	}
}


void CChartCombinedView::OnInitialUpdate()
{
	CChartLineView::OnInitialUpdate();

	CCustomChart* pChart = DYNAMIC_DOWNCAST(CCustomChart, m_wndChart.GetChart());
	ASSERT_VALID(pChart);

#if 0
	CBCGPChartSeries* pSeries1 = pChart->CreateSeries(_T("Column"), CBCGPColor(), BCGP_CT_DEFAULT, BCGPChartColumn);
	CBCGPChartSeries* pSeries2 = pChart->CreateSeries(_T("Line1"), CBCGPColor(), BCGP_CT_DEFAULT, BCGPChartLine);
	CBCGPChartSeries* pSeries3 = pChart->CreateSeries(_T("Line2"), CBCGPColor(), BCGP_CT_DEFAULT, BCGPChartLine);

	CBCGPChartAxis* pPrimaryXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	CBCGPChartAxis* pPrimaryYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);

	// replace default Y axis to handle custom labels
	CBCGPChartAxis* pSecondaryYAxis = pChart->ReplaceDefaultAxis(BCGP_CHART_Y_SECONDARY_AXIS, RUNTIME_CLASS(CCustomYAxisBase));

	// create custom third Y axis
	CBCGPChartAxis* pCustomAxis = pChart->AddCustomAxis();

	pPrimaryXAxis->m_axisLineFormat.m_dblWidth = 2.;
	pPrimaryYAxis->m_axisLineFormat.m_dblWidth = 2.;
	pSecondaryYAxis->m_axisLineFormat.m_dblWidth = 2.;
	pCustomAxis->m_axisLineFormat.m_dblWidth = 2.;

	pSeries1->AddDataPoint(_T("Item 1"), 35.);
	pSeries1->AddDataPoint(_T("Item 2"), 40.);
	pSeries1->AddDataPoint(_T("Item 3"), 85.);
	pSeries1->AddDataPoint(_T("Item 4"), 37);
	pSeries1->AddDataPoint(_T("Item 5"), 55.);
	pSeries1->AddDataPoint(_T("Item 6"), 48.);

	pSeries2->AddDataPoint(2000.);
	pSeries2->AddDataPoint(7500.);
	pSeries2->AddDataPoint(9900.);
	pSeries2->AddDataPoint(600.);
	pSeries2->AddDataPoint(3000.);
	pSeries2->AddDataPoint(12000.);

	pSeries3->AddDataPoint(40000000.);
	pSeries3->AddDataPoint(60000000.);
	pSeries3->AddDataPoint(71000000.);
	pSeries3->AddDataPoint(33000000.);
	pSeries3->AddDataPoint(28000000.);
	pSeries3->AddDataPoint(73000000.);

	// display second series on the secondary Y axis
	pSeries2->SetRelatedAxis(pSecondaryYAxis, CBCGPChartSeries::AI_Y);

	// display third series on the custom third axis
	pSeries3->SetRelatedAxis(pCustomAxis, CBCGPChartSeries::AI_Y);

	// set fixed display ranges with fixed major unit, because automatic ranges 
	// will look a bit different
	pPrimaryYAxis->SetFixedDisplayRange(0., 150., 25.);
	pSecondaryYAxis->SetFixedDisplayRange(0., 30000., 10000.);
	pCustomAxis->SetFixedDisplayRange(0., 90000000, 10000000.);

	// setup marker visibility
	pSeries1->ShowMarker(FALSE);
	pSeries2->ShowMarker(TRUE);
	pSeries2->SetMarkerShape(BCGPChartMarkerOptions::MS_CIRCLE);
	pSeries3->ShowMarker(TRUE);
	pSeries3->SetMarkerShape(BCGPChartMarkerOptions::MS_RECTANGLE);

	// set second series to be SPLINE with width 2.
	BCGPChartFormatSeries style = pSeries2->GetSeriesFormat();

	style.m_curveType = BCGPChartFormatSeries::CCT_SPLINE;
	style.SetSeriesLineWidth(2.);
	pSeries2->SetSeriesFormat(style);

	// set width for the third series
	pSeries3->SetSeriesLineWidth(2.);

	// relocate the legend at the top
	pChart->SetLegendPosition(BCGPChartLayout::LP_TOPRIGHT);

	// set title font to be smaller
	pChart->GetTitleLabelFormat().m_textFormat.SetFontSize(18.);

	// do not display vertical interlaces
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, FALSE);

	// Sync series and axis colors
	OnColorThemeUpdated();

	SetDefaultLineWidth();
#endif
}


void CChartCombinedView::OnUpdateChart()
{
	CWaitCursor wait;


	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->CleanUpChartData(-1, TRUE);
	if (g_Statistics.GetActiveFeeder() >= 0)
	{
		m_pSeries.clear();
		int32_t iCount = 0;
		DECLARE_MASSFLOWSELECT(field);
		for ( const auto& rItem : field)
		{
			if (rItem != base::eMassflowSelect::eVIEWTOTALIZER)
			{
				auto pSeries = CreateSeries(rItem, g_Statistics.GetActiveFeeder());
				if (pSeries)
				{
					m_pSeries.insert({ rItem, pSeries });
				}
			}
		}
	}
	SetupAxis();

	SetDefaultLineWidth();

	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE, FALSE, FALSE);
	pChart->SetCurveType(BCGPChartFormatSeries::CCT_LINE);
	CString szTemp;
	szTemp.Format("Dosierung : %d", g_Statistics.GetActiveFeeder() + 1);
	pChart->SetChartTitle(szTemp);
	pChart->ShowDataLabels(FALSE);

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	pAxisX->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);

	pChart->SetThemeOpacity(100);
	pChart->SetDirty(TRUE, FALSE);

	//pChart->EnableLegendCustomPosition(FALSE); // clear custom state
	pChart->SetLegendPosition(BCGPChartLayout::LegendPosition::LP_TOP);
	pChart->EnableDrawLegendShape(FALSE);

	// Sync series and axis colors
	OnColorThemeUpdated();

	UpdateScrollBars();
	pChart->Redraw();

	m_wndChart.SetFocus();
}

