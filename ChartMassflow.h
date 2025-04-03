#pragma once

#include "ChartLineView.h"


class CChartMassflow : public CChartLineView
{
	CString m_Title;
protected:
	CChartMassflow() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_MASSFLOW));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartMassflow)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWMASSFLOW;	}

	const CString GetTitle() const override
	{	return m_Title;	}


	// Implementation
protected:
	virtual ~CChartMassflow() {}

//	DECLARE_MESSAGE_MAP()
};

