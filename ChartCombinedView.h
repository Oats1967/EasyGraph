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
	LRESULT OnChartMouseUp(WPARAM /*wp*/, LPARAM lp);
	LRESULT OnChartMouseDown(WPARAM /*wp*/, LPARAM lp);
	LRESULT OnAfterDraw(WPARAM /*wp*/, LPARAM lp);



	void OnColorThemeUpdated();
	void SetupAxis();

	// Implementation
protected:
	virtual ~CChartCombinedView();

public:
	afx_msg void OnMenuSelect(UINT id);
	afx_msg void OnUpdateMenuSelect(CCmdUI* pCmdUI);


public:
	DECLARE_MESSAGE_MAP()

};

