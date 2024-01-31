#pragma once

#include "ChartLineView.h"


class CChartDoseperformance : public CChartLineView
{
protected:
	CChartDoseperformance() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartDoseperformance)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWDOSEPERFORMANCE;	}

	const CString GetTitle() const override
	{	return CString("Dosierperformanz");	}


	// Implementation
protected:
	virtual ~CChartDoseperformance() {}

//	DECLARE_MESSAGE_MAP()
};

