//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MoreButton
///
/// @file   MoreButton.h
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


class CMoreButton : public CButtonEC
{
public:
	CMoreButton() : 
		CButtonEC( 
					{ {   
						{ IDB_ARROW_UP,		IDB_ARROW_UP,		IDB_ARROW_UP_DOWN_DISABLE },
						{ IDB_ARROW_UP,		IDB_ARROW_UP,		IDB_ARROW_UP_DOWN_DISABLE },
					} }
				)
	{
		m_bRighImage = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
	}

	void ShowLess(BOOL bLess)
	{	SetCheck(bLess);	}

};

