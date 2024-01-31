#pragma once

#include "ChartLineView.h"


class CChartSetpoint : public CChartLineView
{
protected:
	CChartSetpoint() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartSetpoint)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWSETPOINT;	}

	const CString GetTitle() const override
	{	return CString("Sollwert");	}



	// Implementation
protected:
	virtual ~CChartSetpoint() {}

//	DECLARE_MESSAGE_MAP()
};

