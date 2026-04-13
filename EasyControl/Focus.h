//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ScaleBoxDlg
///
/// @file   ScaleBoxDlg.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "EasyControl.h"


class CFocus
{
	CWnd* m_pWnd;
	BOOL  m_bSaveLock;
	BOOL& m_bWindowLocked;

	CFocus() = delete;

public:
	explicit CFocus(CWnd* pWnd, BOOL& m_bWindowLocked) :
		m_pWnd{ pWnd }
		, m_bSaveLock{ m_bWindowLocked }
		, m_bWindowLocked{ m_bWindowLocked }
	{
		m_bWindowLocked = TRUE;
		AfxGetMainWnd()->SetFocus();
	}
	~CFocus()
	{
		if (m_pWnd)
		{
			m_pWnd->SetFocus();
		}
		m_bWindowLocked = m_bSaveLock;
	}
};
