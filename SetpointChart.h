#pragma once

#include "ChartLineView.h"


class CSetpointChart : public CChartLineView
{
protected:
	CSetpointChart() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSetpointChart)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWSETPOINT;	}


	// Implementation
protected:
	virtual ~CSetpointChart() {}

//	DECLARE_MESSAGE_MAP()
};

