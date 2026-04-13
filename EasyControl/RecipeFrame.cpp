//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeFrame.cpp
///
/// @file   RecipeFrame.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "RecipeTypen.h"
#include "RecipeFrame.h"
#include "RemoteControl.h"



IMPLEMENT_DYNCREATE(CRecipeFrame, CMDIChildWndEx)
//*************************************************************************************
//*************************************************************************************

CRecipeFrame::CRecipeFrame()
{
}
//*************************************************************************************
//*************************************************************************************
BEGIN_MESSAGE_MAP(CRecipeFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()
//*************************************************************************************
//*************************************************************************************
int CRecipeFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
	return -1;

    EnableScrollBarCtrl(SB_HORZ, FALSE);
	return 0;
}

//*************************************************************************************
//*************************************************************************************
BOOL CRecipeFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style &= ~(WS_MAXIMIZE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_HSCROLL  );
	return CMDIChildWndEx::PreCreateWindow(cs);
}
//*************************************************************************************
//*************************************************************************************
void CRecipeFrame::ActivateFrame(int nCmdShow)
{
    nCmdShow = SW_SHOW;

    CMDIChildWndEx::ActivateFrame(nCmdShow);
}

