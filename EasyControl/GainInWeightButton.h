//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GainInWeightButton
///
/// @file   GainInWeightButton.h
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


class CGainInWeightButton : public CButtonEC
{
public:
	CGainInWeightButton() : 
		CButtonEC( 
					{ {   
						{ IDB_LOSSINWEIGHT,		IDB_LOSSINWEIGHT,		IDB_LOSSINWEIGHT },
						{ IDB_GAININWEIGHT,		IDB_GAININWEIGHT,		IDB_GAININWEIGHT },
					} }
				)
	{
		m_bRighImage = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
	}

	void SetCheck(BOOL bLess)
	{		
		CButtonEC::SetCheck(bLess);	
	}

};

