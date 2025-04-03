#pragma once

#include "ChartLineView.h"


class CChartDriveCommand : public CChartLineView
{
	CString m_Title;

protected:
	CChartDriveCommand() : CChartLineView() 
	{
		VERIFY(m_Title.LoadString(IDS_WB_DRIVECOMMAND));
	}
	// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChartDriveCommand)

public:
	const base::eMassflowSelect GetSelection() const override
	{	return base::eMassflowSelect::eVIEWDRIVECOMMAND;	}

	const CString GetTitle() const override
	{	return m_Title;	}

	// Implementation
protected:
	virtual ~CChartDriveCommand() {}

//	DECLARE_MESSAGE_MAP()
};

