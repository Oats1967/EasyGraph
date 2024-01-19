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
// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "pch.h"
#include "WorkspaceBar.h"
#include "EasyGraph.h"
#include "MainFrm.h"
#include "EasyGraphView.h"
#include "ChartLineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int idTree = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	Init();
}

CWorkspaceBar::~CWorkspaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers
#if 0
static CBCGPMultiViewData groups[] =
{
	CBCGPMultiViewData(_T("Chart"), RUNTIME_CLASS(CChartLineView)),
	CBCGPMultiViewData(_T("Table"), RUNTIME_CLASS(CChartLineView)),
};
#endif
static CDemoFeature features[] =
{
	CDemoFeature(0, CDemoFeature::BCGP_ChartElements, _T("Chart"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(1, CDemoFeature::BCGP_Table, _T("Chart Legend"), RUNTIME_CLASS(CChartLineView))
};

int CWorkspaceBar::Init()
{
#if 0
	int nCount = sizeof(groups) / sizeof(groups[0]);

	for (int i = 0; i < nCount; i++)
	{
		AddGroup(&groups[i]);
	}
#endif
	int nCount = sizeof(features) / sizeof(features[0]);
	for (int i = 0; i < nCount; i++)
	{
		AddView(&features[i]);
	}
	return 0;
}

