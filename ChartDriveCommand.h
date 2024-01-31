#pragma once

#include "ChartLineView.h"


class CChartDriveCommand : public CChartLineView
{
protected:
	CChartDriveCommand() : CChartLineView() 
	{}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartDriveCommand)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWDRIVECOMMAND;	}

	const CString GetTitle() const override
	{	return CString("Stellbefehl");	}



	// Implementation
protected:
	virtual ~CChartDriveCommand() {}

//	DECLARE_MESSAGE_MAP()
};

