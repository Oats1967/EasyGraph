#pragma once

#include "ChartLineView.h"


class CMassflowChart : public CChartLineView
{
protected:
	CMassflowChart() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMassflowChart)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWMASSFLOW;	}


	// Implementation
protected:
	virtual ~CMassflowChart() {}

//	DECLARE_MESSAGE_MAP()
};

