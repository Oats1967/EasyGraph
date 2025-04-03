#pragma once

#include "ChartLineView.h"


class CChartNetweight : public CChartLineView
{
	CString m_Title;

protected:
	CChartNetweight() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_NETWEIGHT));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartNetweight)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWWEIGHT;	}

	const CString GetTitle() const override
	{	return m_Title;	}


	// Implementation
protected:
	virtual ~CChartNetweight() {}

//	DECLARE_MESSAGE_MAP()
};

