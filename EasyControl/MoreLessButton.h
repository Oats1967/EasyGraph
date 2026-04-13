//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeView.h
///
/// @file   RecipeView.h
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


class CMoreLessButton : public CButtonEC
{
public:
	CMoreLessButton() : 
		CButtonEC( 
					{ {   
						{ IDB_ARROW_DOWN,		IDB_ARROW_DOWN,		IDB_ARROW_DOWN },
						{ IDB_ARROW_UP,		IDB_ARROW_UP,		IDB_ARROW_UP },
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

