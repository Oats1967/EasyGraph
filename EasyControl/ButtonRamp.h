//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonRamp
///
/// @file   ButtonRamp.h
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

class CRampButton : public CBCGPButton
{
public:
	CRampButton() : CBCGPButton()
	{
		m_bVisualManagerStyle = TRUE;
		m_bDrawFocus = FALSE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
	}
};


class CRampUpButton : public CRampButton
{
public:
	CRampUpButton() : CRampButton()
	{
		SetImage(IDB_BTPLUSUP, IDB_BTPLUSUP, IDB_BTPLUSUP, IDB_BTPLUSDOWN);
	}
};

class CRampDownButton : public CRampButton
{
public:
	CRampDownButton() : CRampButton()
	{
		SetImage(IDB_BTMINUSUP, IDB_BTMINUSUP, IDB_BTMINUSUP, IDB_BTMINUSDOWN);
	}
};

