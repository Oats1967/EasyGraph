#pragma once

#include "ChartLineView.h"


class CChartRotSpeed : public CChartLineView
{
	CString m_Title;

protected:
	CChartRotSpeed() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_ROTSPEED));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartRotSpeed)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWROTSPEED;	}

	const CString GetTitle() const override
	{	return m_Title;	}

	// Implementation
protected:
	virtual ~CChartRotSpeed() {}

//	DECLARE_MESSAGE_MAP()
};

