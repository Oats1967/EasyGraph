#pragma once

#include "pch.h"

static const int AXIS_GAP = 20;
static const int ADDITIONAL_GAP = 40;


//************************************************************
// Custom axis to handle label formatting
//************************************************************
class CCustomYAxisBase : public CBCGPChartAxisY
{
	DECLARE_DYNCREATE(CCustomYAxisBase)

public:
	CCustomYAxisBase()
	{}

	CCustomYAxisBase(int nAxisID, CBCGPChartAxis::AxisDefaultPosition position, CBCGPChartVisualObject* pChartCtrl) :
		CBCGPChartAxisY(nAxisID, position, pChartCtrl)
	{}

	virtual void GetDisplayedLabel(double dblValue, CString& strLabel);
};

//************************************************************
// Custom axis to be displayed outside of the Plot Area
//************************************************************
class CCustomAxisY : public CCustomYAxisBase
{
public:
	CCustomAxisY(int nAxisID, CBCGPChartAxis::AxisDefaultPosition position, CBCGPChartVisualObject* pChartCtrl) :
		CCustomYAxisBase(nAxisID, position, pChartCtrl)
	{}

	virtual void CalcAxisPos(const CBCGPRect& rectDiagram, BOOL bStoreAxisPos);
};

