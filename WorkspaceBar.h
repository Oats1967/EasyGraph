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

struct CDemoFeature : public CBCGPMultiViewData
{
	enum Feature
	{
		BCGP_Unknown = -1,
		BCGP_ChartElements,
		BCGP_Table,
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

class CWorkspaceBar :	public CBCGPMultiViewsCollection
{
public:
	CWorkspaceBar();
	virtual ~CWorkspaceBar();

// Operations
private:
	int Init();
};

