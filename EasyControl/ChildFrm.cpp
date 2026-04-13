//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ChildFrm.cpp
///
/// @file   ChildFrm.cpp
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
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

CChildFrame::CChildFrame() noexcept
{
}
//****************************************************************************************************************
//****************************************************************************************************************
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    ASSERT(cs.style & WS_CHILD);
	cs.style &= ~WS_SYSMENU;
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
//****************************************************************************************************************
//****************************************************************************************************************
#ifdef _DEBUG
//------------------------------------------------------------------------------------
///  @brief   AssertValid
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CChildFrame
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG
