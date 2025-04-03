#pragma once

#include "ChartLineView.h"


class CChartSetpoint : public CChartLineView
{
	CString m_Title;

protected:
	CChartSetpoint() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_SETPOINT));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartSetpoint)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWSETPOINT;	}

	const CString GetTitle() const override
	{	return m_Title;	}



	// Implementation
protected:
	virtual ~CChartSetpoint() {}

//	DECLARE_MESSAGE_MAP()
};

