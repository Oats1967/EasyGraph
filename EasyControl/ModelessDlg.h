//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CModelessDlg
///
/// @file   ModelessDlg.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "EasyControl.h"


class CModelessDlg : public CBCGPDialog
{
	using CBaseClass = CBCGPDialog;

protected:
	BOOL			m_bWindowLocked;

	void LockWindow()
	{	m_bWindowLocked = TRUE;	}

	void UnlockWindow()
	{	m_bWindowLocked = FALSE;	}

	BOOL IsWindowLocked() const
	{	return m_bWindowLocked;	}

	virtual void OnActivateWnd()   {}
	virtual void OnDeactivateWnd() {}

	virtual void OnSetFocus(void);
	virtual void ShowHeader(const CString&);
	virtual void Move(const CPoint& top);
	virtual void Shrink(const CSize& rSize);
	virtual void Shrink(const int32_t dy);
	virtual void DestroyControls(const std::vector<int32_t>& ids);
	virtual void RemoveBottomControls(std::vector<int32_t>& ) {};

	BOOL OnInitDialog() override;
public:
	CModelessDlg() : CBCGPDialog()
		, m_bWindowLocked{ TRUE }
	{}

	CModelessDlg(UINT IDD, CWnd* pParent = NULL) : CBCGPDialog(IDD, pParent)
		, m_bWindowLocked{ TRUE }
	{}

	virtual ~CModelessDlg()
	{}

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimised);
	LRESULT OnLockWindow(WPARAM, LPARAM);
	LRESULT OnHelpInfoClosed(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

