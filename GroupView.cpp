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
// GroupView.cpp : implementation file
//

#include "pch.h"
#include "MainFrm.h"
#include "EasyGraph.h"
#include "EasyGraphView.h"
//#include "PrintChartView.h"
#include "GroupView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CSize szTile(220, 200);
static const CSize szThumbnail(170, 140);

/////////////////////////////////////////////////////////////////////////////
// CGroupView

IMPLEMENT_DYNCREATE(CGroupView, CEasyGraphView)

CGroupView::CGroupView()
	: CEasyGraphView(CGroupView::IDD)
{
	//{{AFX_DATA_INIT(CGroupView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nGroupID = -1;

	CBCGPWinUITiles* pTiles = m_wndTiles.GetWinUITiles();
	ASSERT_VALID(pTiles);
	
	pTiles->SetHorizontalLayout(FALSE);
	pTiles->SetVisualManagerTheme();
}

CGroupView::~CGroupView()
{
}

void CGroupView::SetupGroup(int nGroupID)
{
	CBCGPWinUITiles* pTiles = m_wndTiles.GetWinUITiles();
	ASSERT_VALID(pTiles);
	
	if (m_nGroupID == nGroupID && !pTiles->GetTilesList().IsEmpty() || nGroupID < 0)
	{
		return;
	}

	m_nGroupID = nGroupID;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	
	CWorkspaceBar& workspace = pMainFrame->GetWorkspace();
	workspace.GetGroupName(m_nGroupID, m_strTitle);
	
	UpdateData(FALSE);

	pTiles->RemoveAll();

	CList<CBCGPMultiViewData*, CBCGPMultiViewData*> lstFeatures;
	workspace.GetGroupViews(m_nGroupID, lstFeatures);
	
	for (POSITION pos = lstFeatures.GetHeadPosition(); pos != NULL;)
	{
		CDemoFeature* pFeature = (CDemoFeature*)lstFeatures.GetNext(pos);
		
		CBCGPWinUITile* pTile = new CBCGPWinUITile(pFeature->GetName(), CBCGPWinUITile::BCGP_TILE_REGULAR);

		HBITMAP hbmpChart = NULL;
		CView* pView = pMainFrame->GetFeatureView(pFeature);
		
		CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, pView);
		if (pChartView != NULL)
		{
			hbmpChart = pChartView->CreateThumbnail(globalUtils.ScaleByDPI(szThumbnail, this), pMainFrame->IsDarkTheme());
		}
#if 0
		else if (DYNAMIC_DOWNCAST(CPrintChartView, pView) != NULL)
		{
			hbmpChart = theApp.GetThumbnail(pMainFrame->IsDarkTheme() ? 5 : 4);
		}
#endif

		if (hbmpChart != NULL)
		{
			CBCGPImage image(hbmpChart);
			pTile->SetImage(image);
		}

		pTile->SetBorderWidth(1.0);
		pTile->SetUserData((DWORD_PTR)pFeature->m_Feature);
		
		pTiles->Add(pTile);
	}

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}

void CGroupView::DoDataExchange(CDataExchange* pDX)
{
	CEasyGraphView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupView)
	DDX_Control(pDX, IDC_GROUP, m_wndTiles);
	//}}AFX_DATA_MAP
}

void CGroupView::SetupLayout()
{
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL)
	{
		pLayout->AddAnchor(IDC_GROUP_TITLE, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeHorz);
		pLayout->AddAnchor(IDC_GROUP, CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
}

void CGroupView::GetTitle(CString& /*strTitle*/)
{
}

BEGIN_MESSAGE_MAP(CGroupView, CEasyGraphView)
	//{{AFX_MSG_MAP(CGroupView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_ON_CLICK_WINUI_UI_TILE, OnClickTile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupView diagnostics

#ifdef _DEBUG
void CGroupView::AssertValid() const
{
	CEasyGraphView::AssertValid();
}

void CGroupView::Dump(CDumpContext& dc) const
{
	CEasyGraphView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGroupView message handlers

void CGroupView::OnInitialUpdate() 
{
	CEasyGraphView::OnInitialUpdate();

	if (m_bIsReady)
	{
		return;
	}

	CBCGPWinUITiles* pTiles = m_wndTiles.GetWinUITiles();
	ASSERT_VALID(pTiles);

	pTiles->SetSquareSize(globalUtils.ScaleByDPI(szTile, this));

#ifndef _BCGSUITE_INC_
	pTiles->SetRoundedShapes(TRUE, 5.0);
#endif
}

LRESULT CGroupView::OnClickTile(WPARAM /*wp*/, LPARAM lp)
{
	CBCGPWinUITile* pClickedTile = (CBCGPWinUITile*)lp;
	ASSERT_VALID(pClickedTile);
	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	pMainFrame->SelectView((int)pClickedTile->GetUserData());

	return 0l;	
}

void CGroupView::UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme)
{
	const int nGroupID = m_nGroupID;
	m_nGroupID = -1;

	SetupGroup(nGroupID);
	CEasyGraphView::UpdateChartColorTheme(nTheme, bIsDarkTheme);
}
