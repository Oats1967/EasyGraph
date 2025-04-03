#pragma once

#include "ChartLineView.h"


class CChartDoseperformance : public CChartLineView
{
	CString m_Title;

protected:
	CChartDoseperformance() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_DOSEPERFORMANCE));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartDoseperformance)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWDOSEPERFORMANCE;	}

	const CString GetTitle() const override
	{	return m_Title;	}


	// Implementation
protected:
	virtual ~CChartDoseperformance() {}

//	DECLARE_MESSAGE_MAP()
};

