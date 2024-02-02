#pragma once

#include "pch.h"
#include <vector>


//************************************************************
// Custom chart that adjusts plot area to accommodate space for third custom axis
//************************************************************
class CCustomChart : public CBCGPChartVisualObject
{
	DECLARE_DYNCREATE(CCustomChart)


public:
	CCustomChart()
	{
		m_nCustomAxisID = -1;
		m_bInsideAdjustLayout = FALSE;
		m_bFirstAdjust = TRUE;
	}

	CBCGPChartAxis* AddCustomAxis();
	virtual BOOL OnGetPlotAreaRect(CBCGPRect& rectPlotArea) const;

	virtual void AdjustLayout(CBCGPGraphicsManager* pGM);
	virtual void OnScaleRatioChanged(const CBCGPSize& sizeScaleRatioOld)
	{
		CBCGPChartVisualObject::OnScaleRatioChanged(sizeScaleRatioOld);
		m_bFirstAdjust = TRUE;
	}

	virtual void SetThumbnailMode(BOOL bSet = TRUE, UINT nThumbnailFlags = 0)
	{
		CBCGPChartVisualObject::SetThumbnailMode(bSet, nThumbnailFlags);
		m_bFirstAdjust = TRUE;
	}

	void RemoveCustomAxis()
	{
		for (auto& rItem : m_CustomAxis)
		{
			CBCGPChartVisualObject::RemoveCustomAxis(rItem, FALSE);
		}
		m_CustomAxis.clear();
	}

protected:
	int m_nCustomAxisID;
	BOOL m_bInsideAdjustLayout;
	CBCGPRect m_rectCustomPlotArea;
	BOOL m_bFirstAdjust;
	std::vector < CBCGPChartAxis* > m_CustomAxis;

};

