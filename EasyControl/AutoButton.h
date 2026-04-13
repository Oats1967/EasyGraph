  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AutoButton.h
///
/// @file   AutoButton.h
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

#include "ButtonEC.h"

class CAutoButton : public CButtonEC
{
public:
	CAutoButton() :
		CButtonEC(
			{ {
				{ IDB_BTAUTO_UP,		IDB_BTAUTO_UP,		IDB_BTAUTO_DISABLE },
				{ IDB_BTAUTO_DOWN,		IDB_BTAUTO_DOWN,	IDB_BTAUTO_DOWN },
			} }
			)
	{
		m_bRighImage = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
		m_bTransparent = TRUE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_FLAT;
	}

	void SetAuto(BOOL bAuto)
	{
		SetCheck(bAuto);
	}
};

