  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CModelessDlg
///
/// @file   ModelessDlg.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "ModelessDlg.h"

//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CModelessDlg, CBaseClass)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_HELPINFOCLOSED, OnHelpInfoClosed)
	ON_MESSAGE(WM_NOTIFYHELPINFOBOX, OnLockWindow)
	ON_MESSAGE(WM_NOTIFYEDITBOX, OnLockWindow)
	ON_MESSAGE(WM_NOTIFYSCALEBOX, OnLockWindow)
	ON_MESSAGE(WM_NOTIFYSPEEDBOX, OnLockWindow)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CModelessDlg::Move(const CPoint& top)
{
	SetWindowPos(NULL, top.x, top.y, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
}
//***********************************************************************************************
//***********************************************************************************************
void CModelessDlg::Shrink(const CSize& rSize)
{
	SetWindowPos(NULL, -1, -1, rSize.cx, rSize.cy, SWP_NOMOVE | SWP_NOZORDER);
}
//***********************************************************************************************
//***********************************************************************************************
void CModelessDlg::Shrink(const int32_t dy)
{
	CRect aRect;
	GetWindowRect(aRect);
	Shrink(CSize(aRect.Width(), __max(aRect.Height() - dy, 0)));
}
//**************************************************************************************
//**************************************************************************************
void CModelessDlg::DestroyControls(const std::vector<int32_t>& ids)
{
	CRect wRect;
	GetWindowRect(wRect);
	int32_t top = wRect.bottom;
	for (const auto& rItem : ids)
	{
		auto pWnd = GetDlgItem(rItem);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(aRect);
		pWnd->EnableWindow(FALSE);
		if (aRect.top < top)
		{
			top = aRect.top;
		}
	}
	int32_t dy = wRect.bottom - top;
	if (dy > 0)
	{
		Shrink(dy - 5);
	}
}
//**************************************************************************************
//**************************************************************************************
BOOL CModelessDlg::OnInitDialog()
{
	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		std::vector<int32_t> ids;
		RemoveBottomControls(ids);
		DestroyControls(ids);
	}
	return result;
}
//*************************************************************************************
//*************************************************************************************
void CModelessDlg::ShowHeader(const CString& value)
{
	SetWindowText(value);
}
//***********************************************************************************************
//***********************************************************************************************
void CModelessDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseClass::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
	{
		OnDeactivateWnd();
		if (!IsWindowLocked())
		{
			DestroyWindow();
		}
	}
	else if (nState == WA_ACTIVE)
	{
		OnActivateWnd();
	}
}

//***************************************************************************************
//***************************************************************************************
LRESULT CModelessDlg::OnLockWindow(WPARAM, LPARAM)
{
	LockWindow();
	return 0L;
}
//**************************************************************************************
//**************************************************************************************
LRESULT CModelessDlg::OnHelpInfoClosed(WPARAM, LPARAM)
{
	OnSetFocus();
	return 0L;
}

//**************************************************************************************
//**************************************************************************************
void CModelessDlg::OnSetFocus()
{
	UnlockWindow();
	CWnd* pWnd = GetFocus();
	if (pWnd != this)
	{
		SetFocus();
	}
}

