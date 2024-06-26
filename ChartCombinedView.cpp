#include "pch.h"
#include "wmuser.h"
#include "CustomChart.h"
#include "CustomYAxis.h"
#include "ChartCombinedView.h"




BEGIN_MESSAGE_MAP(CChartCombinedView, CChartLineView)
	//{{AFX_MSG_MAP(CChartLineView)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ON_CHART_MOUSE_UP, OnChartMouseUp)
	ON_REGISTERED_MESSAGE(BCGM_ON_CHART_MOUSE_DOWN, OnChartMouseDown)
	ON_REGISTERED_MESSAGE(BCGM_ON_CHART_AFTER_DRAW, OnAfterDraw)
END_MESSAGE_MAP()

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


void CChartCombinedView::OnUpdateSeries()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	if (g_Statistics.GetActiveFeeder() < 0)
	{
		g_Statistics.SetActiveFeeder(0);
	}
	pChart->CleanUpChartData(-1, TRUE);
	m_pSeries.clear();
	int32_t iCount = 0;
	DECLARE_MASSFLOWSELECT(field);
	for (const auto& rItem : field)
	{
		if (rItem != base::eMassflowSelect::eVIEWTOTALIZER)
		{
			const auto& lineAttrib = g_Statistics.GetLineAttribute(rItem);
			if (lineAttrib.m_Visible)
			{
				auto pSeries = CreateSeries(rItem, g_Statistics.GetActiveFeeder());
				if (pSeries)
				{
					m_pSeries.insert({ rItem, pSeries });
				}
			}
		}
	}
}

void CChartCombinedView::OnUpdateChart()
{
	SetupAxis();
	AddLogItems();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	pChart->SetChartType(BCGPChartLine, BCGP_CT_SIMPLE, FALSE, FALSE);
	pChart->SetCurveType(BCGPChartFormatSeries::CCT_LINE);
	CString szTemp;
	szTemp.Format(_T("Dosierung : %d"), g_Statistics.GetActiveFeeder() + 1);
	pChart->SetChartTitle(szTemp);
	pChart->ShowDataLabels(FALSE);

	CBCGPChartAxis* pAxisX = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	pAxisX->m_axisLabelsFormat.m_textFormat.SetDrawingAngle(90);

	pChart->SetThemeOpacity(100);

	//pChart->EnableLegendCustomPosition(FALSE); // clear custom state
	pChart->SetLegendPosition(BCGPChartLayout::LegendPosition::LP_TOP);
	pChart->EnableDrawLegendShape(FALSE);

	// Sync series and axis colors
	OnColorThemeUpdated();
	UpdateScrollBars();
	pChart->SetDirty(TRUE, FALSE);
	m_wndChart.SetFocus();
}


void CChartCombinedView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		if (g_Statistics.GetActiveFeeder() < 0)
		{
			g_Statistics.SetActiveFeeder(0);
		}
	}
	CChartLineView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


static BOOL bZoomStart = FALSE;
static BOOL bMarkerSet = FALSE;


LRESULT CChartCombinedView::OnChartMouseUp(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

#if 0
	BCGPChartHitInfo* pHitInfo = (BCGPChartHitInfo*)lp;
	if (pHitInfo->m_nMouseButton != 0 ||
		(pHitInfo->m_hitInfo != BCGPChartHitInfo::HIT_DATA_POINT && pHitInfo->m_hitInfo != BCGPChartHitInfo::HIT_DATA_TABLE))
	{
		return 0;
	}
#endif
	auto nZoomType = pChart->GetZoomScrollConfig();
	if (nZoomType == BCGPChartMouseConfig::ZoomScrollOptions::ZSO_SELECT)
	{
		if (bZoomStart)
		{
			bZoomStart = FALSE;
			bMarkerSet = TRUE;
		}
	}
	return 0L;
}

LRESULT CChartCombinedView::OnChartMouseDown(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	auto nZoomType = pChart->GetZoomScrollConfig();
	if (nZoomType == BCGPChartMouseConfig::ZoomScrollOptions::ZSO_SELECT)
	{
		bZoomStart = TRUE;
	}
	return 0L;
}


LRESULT CChartCombinedView::OnAfterDraw(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	auto nZoomType = pChart->GetZoomScrollConfig();

	if (nZoomType == BCGPChartMouseConfig::ZoomScrollOptions::ZSO_SELECT)
	{
		int k = 0;
	}
	return 0L;
}

