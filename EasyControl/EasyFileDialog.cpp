//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeView.cpp
///
/// @file   RecipeView.cpp
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
#include "afxdlgs.h"
#include "EasyControl.h"
#include "EasyFileDialog.h"
#include "RemoteControl.h"
#include "wmuser.h"

IMPLEMENT_DYNAMIC(CEasyFileDialog, CFileDialog)

BEGIN_MESSAGE_MAP(CEasyFileDialog, CFileDialog)
	ON_MESSAGE(MYWM_POSTINIT, OnPostInit)
END_MESSAGE_MAP()


LRESULT CEasyFileDialog::OnPostInit(WPARAM wp, LPARAM lp)
{
	SetListView(ODM_VIEW_DETAIL); // this will succeed
	return 0;
}

BOOL CEasyFileDialog::SetListView(LISTVIEWCMD cmd)
{
	TRACE(_T("CMyOpenDlg::SetListView: "));

	// note that real dialog is my parent, not me
	CWnd* pshell = GetParent()->GetDlgItem(lst2);
	if (pshell) 
	{
		TRACE(_T("hwnd=%p.\n"), m_wndList.GetSafeHwnd());
		pshell->SendMessage(WM_COMMAND, cmd);
		return TRUE;
	}
	TRACE(_T("failed.\n"), m_wndList.GetSafeHwnd());
	return FALSE;
}

