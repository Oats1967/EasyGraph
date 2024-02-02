#include "pch.h"
#include "CustomYAxis.h"
#include "CustomChart.h"


IMPLEMENT_DYNCREATE(CCustomChart, CBCGPChartVisualObject)

//------------------------------------------------------
// Adds custom Y axis and stores its index
//------------------------------------------------------
CBCGPChartAxis* CCustomChart::AddCustomAxis()
{
	// we could store a pointer to custom axis, but storing ID will gracefully handle removing (just in case)
	// in OnGetPlotAreaRect
	// CBCGPChartAxis::ADP_RIGHT tells the axis show labels at the right side by default
	CBCGPChartAxis* pAxis = new CCustomAxisY(GetNextCustomAxisID(), CBCGPChartAxis::ADP_RIGHT, this);
	m_nCustomAxisID = pAxis->m_nAxisID;
	m_CustomAxis.push_back(pAxis);

	// add the custom axis to collection
	AddAxis(pAxis);
	return pAxis;
}

//------------------------------------------------------
// Adjust layout must be called 2 times when the chart is displayed for the first time,
// because plot area rectangle is based on other elements
//------------------------------------------------------
void CCustomChart::AdjustLayout(CBCGPGraphicsManager* pGM)
{
	m_bInsideAdjustLayout = TRUE;
	CBCGPChartVisualObject::AdjustLayout(pGM);

	if (m_bFirstAdjust)
	{
		m_bFirstAdjust = FALSE;
		CBCGPChartVisualObject::AdjustLayout(pGM);
	}
	m_bInsideAdjustLayout = FALSE;

	// store the plot area rect to return it without additional calculations
	// when the rect is requested from methods other than AdjustLayout
	m_rectCustomPlotArea = m_rectPlotArea;
}

//------------------------------------------------------
// Returns plot area rectangle adjusted to make additional space for third axis
//------------------------------------------------------
BOOL CCustomChart::OnGetPlotAreaRect(CBCGPRect& rectPlotArea) const
{
	if (!m_bInsideAdjustLayout && !m_rectCustomPlotArea.IsRectEmpty())
	{
		rectPlotArea = m_rectCustomPlotArea;
		return TRUE;
	}

	CBCGPRect rectLegend = m_rectLegendArea;

	if (m_bFirstAdjust || m_rectTitleArea.IsRectEmpty() || m_nCustomAxisID == -1 || m_rect.IsRectEmpty())
	{
		// AdjustLayout has not been called or custom axis has not been created
		return FALSE;
	}
	
	if (m_CustomAxis.size() == 0 )
	{
		return FALSE;
	}

	CBCGPRect rectPlot = m_rect;
	rectPlot.DeflateRect(m_chartAreaFormat.GetContentPadding(TRUE));

	if (!m_chartLayout.m_bTitleOverlapsChart)
	{
		rectPlot.top = m_rectTitleArea.bottom;
	}
	// if the legend is at right or top right and does not overlaps chart!
	if (!m_bIsThumbnailMode && !m_chartLayout.m_bLegendOverlapsChart)
	{
		CBCGPSize szLegendAreaPadding = GetLegendAreaPadding(TRUE);

		if (m_chartLayout.m_legendPosition == BCGPChartLayout::LP_RIGHT ||
			m_chartLayout.m_legendPosition == BCGPChartLayout::LP_TOPRIGHT)
		{
			rectPlot.right = m_rect.right - szLegendAreaPadding.cx - rectLegend.Width();
		}
		else if (m_chartLayout.m_legendPosition == BCGPChartLayout::LP_LEFT)
		{
			rectPlot.left = rectLegend.right + szLegendAreaPadding.cx;
		}
		else if (m_chartLayout.m_legendPosition == BCGPChartLayout::LP_TOP)
		{
			rectPlot.top = rectLegend.bottom + szLegendAreaPadding.cy;
		}
		else if (m_chartLayout.m_legendPosition == BCGPChartLayout::LP_BOTTOM)
		{
			rectPlot.bottom = rectLegend.top - szLegendAreaPadding.cy;
		}
	}
	double dblAxisGap = (double)(AXIS_GAP + ADDITIONAL_GAP) * (m_CustomAxis.size() + 1);
	if (m_bIsThumbnailMode)
	{
		dblAxisGap = (ADDITIONAL_GAP - AXIS_GAP) * 0.5;
	}
	rectPlot.right -= dblAxisGap;
	rectPlotArea = rectPlot;

	return TRUE;
}
