#pragma once

#include "ChartLineView.h"


class CChartLineInterpolation : public CChartLineView
{
protected:
	CChartLineInterpolation() : CChartLineView() 
	{
		m_nZoomType = BCGPChartMouseConfig::ZoomScrollOptions::ZSO_SELECT;
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartLineInterpolation)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWWEIGHT;	}

	const CString GetTitle() const override
	{	return CString("Lineinterpolation");	}


	// Implementation
protected:
	virtual ~CChartLineInterpolation() {}

//	DECLARE_MESSAGE_MAP()
};

