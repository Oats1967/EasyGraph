#pragma once

#include "ChartLineView.h"


class CChartRotSpeed : public CChartLineView
{
protected:
	CChartRotSpeed() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartRotSpeed)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWROTSPEED;	}

	const CString GetTitle() const override
	{	return CString("Drehzahl");	}



	// Implementation
protected:
	virtual ~CChartRotSpeed() {}

//	DECLARE_MESSAGE_MAP()
};

