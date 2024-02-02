#include "pch.h"
#include "CustomYAxis.h"


IMPLEMENT_DYNCREATE(CCustomYAxisBase, CBCGPChartAxisY)



//------------------------------------------------------
// Displays label in format n,nnn.nn
//------------------------------------------------------
void CCustomYAxisBase::GetDisplayedLabel(double dblValue, CString& strLabel)
{
	strLabel.Format(_T("%.2f"), dblValue);

	int nPointIndex = strLabel.Find('.');

	for (int nIndex = nPointIndex - 3; nIndex > 0; nIndex -= 3)
	{
		if (nPointIndex <= 0)
		{
			break;
		}

		strLabel.Insert(nIndex, _T(","));
	}
}

//------------------------------------------------------
// Calculates custom axis X offset based on the legend position
//------------------------------------------------------
void CCustomAxisY::CalcAxisPos(const CBCGPRect& rectDiagram, BOOL bStoreAxisPos)
{
	if (m_pChart == NULL)
	{
		return;
	}

	CBCGPChartAxisY::CalcAxisPos(rectDiagram, bStoreAxisPos);

	// assume we align axis relative to legend, which is located at the right
	// if the legend position doesn't matter use rectDiagram
	CBCGPRect rectLegend = rectDiagram; //  m_pChart->GetLegendArea();

	double dblAxisGap = (double)(AXIS_GAP + ADDITIONAL_GAP) * (m_nAxisID - BCGP_CHART_FIRST_CUSTOM_ID + 1);
	if (m_bThumbnailMode)
	{
		dblAxisGap = (ADDITIONAL_GAP - AXIS_GAP) * 0.5;
	}

	// assumes the axis is vertical
	if ((m_pChart->m_chartLayout.m_legendPosition == BCGPChartLayout::LP_RIGHT ||
		m_pChart->m_chartLayout.m_legendPosition == BCGPChartLayout::LP_TOPRIGHT) &&
		!m_pChart->m_chartLayout.m_bLegendOverlapsChart && !m_bThumbnailMode)
	{
		m_ptAxisStart.x = m_ptAxisEnd.x = rectLegend.left - ADDITIONAL_GAP - m_szMaxLabelSize.cx;
	}
	else
	{
		// rectDiagram contains current plot area rect
		//CBCGPChartAxis* pSecondaryAxis = m_pChart->GetChartAxis(BCGP_CHART_Y_SECONDARY_AXIS);
		m_ptAxisStart.x = m_ptAxisEnd.x = rectDiagram.right + dblAxisGap;
	}
}

