//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2023 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__435B2ABB_C4AB_4400_B80C_7EE3520CBFEA__INCLUDED_)
#define AFX_WORKSPACEBAR_H__435B2ABB_C4AB_4400_B80C_7EE3520CBFEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CDemoFeature : public CBCGPMultiViewData
{
	enum Feature
	{
		BCGP_Unknown = -1,
		BCGP_ChartElements,
		BCGP_ChartLegend,
		BCGP_ChartAdvancedLegend,
		BCGP_ChartDataTable,
		BCGP_ChartDataBinding,
		BCGP_ClusteredColumn2D,
		BCGP_StackedColumn2D,
		BCGP_StackedColumn1002D,
		BCGP_StackedColumnSideBySide2D,
		BCGP_RangeColumn,
		BCGP_ClusteredColumn3D,
		BCGP_Column3D,
		BCGP_StackedColumn3D,
		BCGP_StackedColumn1003D,
		BCGP_StackedColumnSideBySide3D,
		BCGP_StackedColumnManhattan3D,
		BCGP_Line,
		BCGP_StackedLine,
		BCGP_StackedLine100,
		BCGP_StackedLineSpline,
		BCGP_StepLine,
		BCGP_Line3D,
		BCGP_Pie,
		BCGP_Pie3D,
		BCGP_Doughnut,
		BCGP_DoughnutNested,
		BCGP_Doughnut3D,
		BCGP_Torus3D,
		BCGP_PieGroupSmallerSlices,
		BCGP_MultiplePies,
		BCGP_ClusteredBar2D,
		BCGP_StackedBar2D,
		BCGP_StackedBar1002D,
		BCGP_StackedBarSideBySide2D,
		BCGP_RangeBar,
		BCGP_ClusteredBar3D,
		BCGP_Bar3D,
		BCGP_StackedBar3D,
		BCGP_StackedBar1003D,
		BCGP_StackedBarSideBySide3D,
		BCGP_Area,
		BCGP_StackedArea,
		BCGP_StackedArea100,
		BCGP_StackedAreaSpline,
		BCGP_RangeArea,
		BCGP_StepArea,
		BCGP_HistoricalAreaSpline,
		BCGP_Area3D,
		BCGP_RangeArea3D,
		BCGP_Stock,
		BCGP_StockRealTime,
		BCGP_Points,
		BCGP_Bubble,
		BCGP_Bubble3D,
		BCGP_Polar,
		BCGP_PolarBar,
		BCGP_Radar,
		BCGP_Pyramid,
		BCGP_Pyramid3D,
		BCGP_PyramidGroupSmallerSlices,
		BCGP_Funnel,
		BCGP_Funnel3D,
		BCGP_FunnelGroupSmallerSlices,
		BCGP_BoxPlot,
		BCGP_BoxPlotNotched,
		BCGP_ErrorBars,
		BCGP_Surface3DBasic,
		BCGP_Surface3DIntersect,
		BCGP_Surface3DAdvanced,
		BCGP_HugeData,
		BCGP_HugeAverageData,
		BCGP_Virtual,
		BCGP_Historical,
		BCGP_RealTime,
		BCGP_TernaryBasic,
		BCGP_TernaryAdvanced,
		BCGP_FormulaTransition,
		BCGP_FormulaTrend,
		BCGP_CombinedCharts,
		BCGP_CombinedCharts3D,
		BCGP_MultiCharts,
		BCGP_VirtualSeries,
		BCGP_SecondaryAxis,
		BCGP_SmartLabels,
		BCGP_ScaleBreaks,
		BCGP_LogarithmicAxis,
		BCGP_CustomAxis,
		BCGP_ChartObjects,
		BCGP_InteractiveChart,
		BCGP_ChartAndGrid,
		BCGP_ChartView,
		BCGP_ChartThumbnails,
	};

	CDemoFeature(int nGroup, Feature feature, LPCTSTR lpszName, CRuntimeClass* pRTI, int nStatus = 0) :
		CBCGPMultiViewData(lpszName, pRTI, nGroup),
		m_nStatus(nStatus),
		m_Feature(feature)
	{
	}

	const int		m_nStatus;
	const Feature	m_Feature;
};

class CWorkspaceBar :	public CBCGPDockingControlBar,
						public CBCGPMultiViewsCollection
{
public:
	CWorkspaceBar();

// Attributes
public:
	int GetSelected() const
	{
		return m_nLastSelectedItem;
	}

protected:
	CBCGPTreeCtrlEx		m_wndTree;
	int					m_nLastSelectedItem;

// Operations
public:
	BOOL SelectGroup(int nIndex);
	BOOL SelectView(int nIndex);

	void AdjustLayout();

// Overrides
	virtual BOOL IsGroupAvailable(int nIndex) const;
	virtual BOOL IsViewAvailable(int nIndex) const;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnSelectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnGridFilterBarChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	void AddFeatureGroup(CBCGPMultiViewData& group);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__435B2ABB_C4AB_4400_B80C_7EE3520CBFEA__INCLUDED_)
