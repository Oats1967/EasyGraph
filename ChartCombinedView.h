#pragma once

#include "ChartLineView.h"


class CChartCombinedView : public CChartLineView
{
protected:
	CChartCombinedView();

	std::map <base::eMassflowSelect, CBCGPChartSeries*> m_pSeries;

	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartCombinedView)

protected:
	virtual void OnUpdateChart();
	void OnColorThemeUpdated();
	void SetupAxis();

public:
	virtual void OnInitialUpdate();

	// Implementation
protected:
	virtual ~CChartCombinedView();
};

