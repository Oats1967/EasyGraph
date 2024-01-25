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
#include "MainFrm.h"
#include "WorkspaceBar.h"
#include "EasyGraph.h"
//#include "MainFrm.h"
#include "EasyGraphView.h"
#include "ChartLineView.h"
#include "TotalizerView.h"
#include "GroupView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



const int idTree = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, idTree, OnSelectTree)
	ON_REGISTERED_MESSAGE(BCGM_GRID_FILTERBAR_CHANGED, OnGridFilterBarChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	m_nLastSelectedItem = -1;
}

CWorkspaceBar::~CWorkspaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

static CBCGPMultiViewData groups[] =
{
	CBCGPMultiViewData(_T("Analyse"), RUNTIME_CLASS(CGroupView)),
	CBCGPMultiViewData(_T("Statistik"), RUNTIME_CLASS(CGroupView)),
};


/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers
static CDemoFeature features[] =
{
	CDemoFeature(0, CDemoFeature::BCGP_Massflow, _T("Durchsatz"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(0, CDemoFeature::BCGP_Setpoint, _T("Sollwert"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(0, CDemoFeature::BCGP_DriveCommand, _T("Stellbefehl"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(0, CDemoFeature::BCGP_RotSpeed, _T("Drehzahl"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(0, CDemoFeature::BCGP_DosePerformance, _T("Dosierperformanz"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(0, CDemoFeature::BCGP_Weight, _T("Gewicht"), RUNTIME_CLASS(CChartLineView)),
	CDemoFeature(1, CDemoFeature::BCGP_Statistics, _T("Totalisator"), RUNTIME_CLASS(CTotalizerView)),
};


int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tree window:
	m_wndTree.SetVisualManagerColorTheme();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_TRACKSELECT |
		TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, idTree))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	BCGP_GRID_FILTERBAR_OPTIONS filterBarOptions(_T("Search Chart Views"));
	filterBarOptions.m_bIncludeGroups = TRUE;
	m_wndTree.EnableFilterBar(TRUE, filterBarOptions);

	m_wndTree.SetOutOfFilterLabel(_T("No items match your search."));


	const int nCount = sizeof(groups) / sizeof(CBCGPMultiViewData);

	for (int i = 0; i < nCount; i++)
	{
		AddFeatureGroup(groups[i]);
	}

	SelectView(0);
	return 0;
}

void CWorkspaceBar::AddFeatureGroup(CBCGPMultiViewData& group)
{
	HTREEITEM hFolder = m_wndTree.InsertItem(group.GetName(), 0);

	const int nGroupIndex = AddGroup(&group, (DWORD_PTR)hFolder);
	m_wndTree.SetItemData(hFolder, nGroupIndex + 1);

	const int nCount = sizeof(features) / sizeof(CDemoFeature);

	for (int i = 0; i < nCount; i++)
	{
		CDemoFeature& feature = features[i];

		if (feature.GetGroup() == nGroupIndex)
		{
			HTREEITEM hTreeItem = m_wndTree.InsertItem(feature.GetName(), 1, 1, hFolder);

			const int nIndex = AddView(&feature, nGroupIndex, (DWORD_PTR)hTreeItem);
			m_wndTree.SetItemData(hTreeItem, nIndex + 1);

			if (feature.m_nStatus != 0)
			{
				const CString strBadge = (feature.m_nStatus == 1) ? _T("New") : _T("Upd");
				const COLORREF clrText = (feature.m_nStatus == 1) ? RGB(0, 100, 0) : RGB(160, 82, 45);
				const COLORREF clrFill = (feature.m_nStatus == 1) ? RGB(152, 251, 152) : RGB(255, 218, 185);

				m_wndTree.SetItemNotificationBadge(hTreeItem, strBadge, 0, clrText, clrFill, TRUE, FALSE);
			}
		}
	}
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();

	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	if (pFrame != NULL)
	{
		pFrame->AdjustDockingLayout();
	}
}

void CWorkspaceBar::OnSelectTree(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	HTREEITEM hTreeItem = m_wndTree.GetSelectedItem();
	if (hTreeItem != NULL)
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		if (pMainFrame != NULL)
		{
			if (m_wndTree.GetParentItem(hTreeItem) == NULL)
			{
				// Group is selected
				int nGroup = (int)m_wndTree.GetItemData(hTreeItem) - 1;
				if (nGroup >= 0 && AfxGetMainWnd() != NULL)
				{
					pMainFrame->SelectGroup(nGroup);
				}
			}
			else
			{
				int nView = (int)m_wndTree.GetItemData(hTreeItem) - 1;
				if (nView >= 0 && AfxGetMainWnd() != NULL)
				{
					pMainFrame->SelectView(nView);
				}
			}
		}
	}

	*pResult = 0;
}

BOOL CWorkspaceBar::SelectView(int nIndex)
{
	HTREEITEM hItem = (HTREEITEM)GetViewData(nIndex);
	if (hItem != NULL)
	{
		if (m_wndTree.SelectItem(hItem))
		{
			m_wndTree.Expand(m_wndTree.GetParentItem(hItem), TVE_EXPAND);
			m_wndTree.EnsureVisible(hItem);

			m_nLastSelectedItem = nIndex;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWorkspaceBar::SelectGroup(int nIndex)
{
	HTREEITEM hItem = (HTREEITEM)GetGroupData(nIndex);
	if (hItem != NULL)
	{
		if (m_wndTree.SelectItem(hItem))
		{
			m_wndTree.Expand(m_wndTree.GetParentItem(hItem), TVE_EXPAND);
			m_wndTree.EnsureVisible(hItem);

			m_nLastSelectedItem = nIndex;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWorkspaceBar::IsViewAvailable(int nIndex) const
{
	if (!CBCGPMultiViewsCollection::IsViewAvailable(nIndex))
	{
		return FALSE;
	}

	HTREEITEM hItem = (HTREEITEM)GetViewData(nIndex);
	if (hItem != NULL)
	{
		return m_wndTree.IsRowVisible((CBCGPGridRow*)hItem);
	}

	return FALSE;
}

BOOL CWorkspaceBar::IsGroupAvailable(int nIndex) const
{
	if (!CBCGPMultiViewsCollection::IsGroupAvailable(nIndex))
	{
		return FALSE;
	}

	HTREEITEM hItem = (HTREEITEM)GetGroupData(nIndex);
	if (hItem != NULL)
	{
		return m_wndTree.IsRowVisible((CBCGPGridRow*)hItem);
	}

	return FALSE;
}

void CWorkspaceBar::AdjustLayout()
{
	CRect rectClient;
	GetClientRect(rectClient);

	m_wndTree.SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT CWorkspaceBar::OnGridFilterBarChanged(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPTreeCtrlEx* pSender = (CBCGPTreeCtrlEx*)lp;

	if (m_wndTree.GetSafeHwnd() == pSender->GetSafeHwnd() && m_nLastSelectedItem >= 0)
	{
		m_wndTree.EnableTreeCtrlNotifications(FALSE);
		SelectView(m_nLastSelectedItem);
		m_wndTree.EnableTreeCtrlNotifications();
	}

	return 0L;
}


#if 0
int CWorkspaceBar::Init()
{
	int nCount = sizeof(features) / sizeof(features[0]);
	for (int i = 0; i < nCount; i++)
	{
		AddView(&features[i]);
	}
	return 0;
}


int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0, WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tree window:
	m_wndTree.SetVisualManagerColorTheme();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_TRACKSELECT |
		TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;

	if (!m_wndTree.Create(dwViewStyle, rectDummy, this, idTree))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	BCGP_GRID_FILTERBAR_OPTIONS filterBarOptions(_T("Search Chart Views"));
	filterBarOptions.m_bIncludeGroups = TRUE;
	m_wndTree.EnableFilterBar(TRUE, filterBarOptions);

	m_wndTree.SetOutOfFilterLabel(_T("No items match your search."));

	const int nCount = sizeof(groups) / sizeof(CBCGPMultiViewData);

	for (int i = 0; i < nCount; i++)
	{
		AddFeatureGroup(groups[i]);
	}

	SelectView(0);
	return 0;
}

#endif
