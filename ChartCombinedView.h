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
	void OnUpdateSeries() override;
	void OnUpdateChart() override;
	void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;


	void OnColorThemeUpdated();
	void SetupAxis();

	// Implementation
protected:
	virtual ~CChartCombinedView();
};

