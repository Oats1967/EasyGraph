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
#pragma once

#include <vector>


struct CDemoFeature : public CBCGPMultiViewData
{
	enum Feature
	{
		BCGP_Unknown = -1,
		BCGP_Massflow,
		BCGP_Setpoint,
		BCGP_DriveCommand,
		BCGP_RotSpeed,
		BCGP_DosePerformance,
		BCGP_Weight,
		BCGP_Table,
		BCGP_OverallView,
		BCGP_Statistics,
	};

	CDemoFeature(int nGroup, Feature feature, CRuntimeClass* pRTI, int nStatus = 0);

	const int		m_nStatus;
	const Feature	m_Feature;
};

struct CGroupData : public CBCGPMultiViewData
{
	CGroupData(UINT id, CRuntimeClass* pRTI) : CBCGPMultiViewData(CString(), pRTI)
	{
		VERIFY(m_strName.LoadString(id));
	}
};


class CWorkspaceBar : public CBCGPDockingControlBar,
					  public CBCGPMultiViewsCollection
{
	std::vector < CDemoFeature > features;
	std::vector < CGroupData > groups;

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


