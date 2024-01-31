#pragma once

#include "ChartLineView.h"


class CChartNetweight : public CChartLineView
{
protected:
	CChartNetweight() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartNetweight)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWWEIGHT;	}

	const CString GetTitle() const override
	{	return CString("Nettogewicht");	}


	// Implementation
protected:
	virtual ~CChartNetweight() {}

//	DECLARE_MESSAGE_MAP()
};

