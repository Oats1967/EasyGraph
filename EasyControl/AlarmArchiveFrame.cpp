//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmArchiveFrame.cpp
///
/// @file   AlarmArchiveFrame.cpp
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
#include "AlarmArchiveFrame.h"
#include "DateTimePicker.h"
#include "BASE/include/DiagramConfig.h"


IMPLEMENT_DYNCREATE(CAlarmArchiveFrame, CMDIChildWndEx)


//*************************************************************************************
//*************************************************************************************
BEGIN_MESSAGE_MAP(CAlarmArchiveFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_TB_ALARMARCHIVEFRAME_TIMESELECT, OnAlarmArchiveFrameTimeSelect)
	ON_UPDATE_COMMAND_UI(ID_TB_ALARMARCHIVEFRAME_TIMESELECT, OnAlarmArchiveFrameTimeSelect)
	ON_WM_DESTROY()
END_MESSAGE_MAP()
//*************************************************************************************
//*************************************************************************************
BOOL CAlarmArchiveFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    ASSERT(cs.style & WS_CHILD);
	cs.style &= ~(WS_MAXIMIZE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
//*************************************************************************************
//*************************************************************************************
int CAlarmArchiveFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;

}
//*************************************************************************************
//*************************************************************************************
void CAlarmArchiveFrame::OnDestroy()
{
	CMDIChildWndEx::OnDestroy();
}
//*************************************************************************************
//*************************************************************************************
void CAlarmArchiveFrame::ActivateFrame(int nCmdShow)
{
	nCmdShow = SW_SHOWMAXIMIZED;
	CMDIChildWndEx::ActivateFrame(nCmdShow);
}


void CAlarmArchiveFrame::OnAlarmArchiveFrameTimeSelect()
{
	CDateTimePickerDlg m_wndDateTime;
	m_wndDateTime.DoModal();
	auto date = m_wndDateTime.GetDate();
	auto newTime = base::eTimeMode::TG_DATE;
	CView* pActiveView = GetActiveView();
	ASSERT(pActiveView);
	pActiveView->SendMessage(WM_LOGTIME, WPARAM(newTime), LPARAM(date));
}


void CAlarmArchiveFrame::OnAlarmArchiveFrameTimeSelect(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

