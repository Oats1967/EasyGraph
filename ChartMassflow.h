#pragma once

#include "ChartLineView.h"


class CChartMassflow : public CChartLineView
{
protected:
	CChartMassflow() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartMassflow)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWMASSFLOW;	}

	const CString GetTitle() const override
	{	return CString("Durchsatz");	}


	// Implementation
protected:
	virtual ~CChartMassflow() {}

//	DECLARE_MESSAGE_MAP()
};

