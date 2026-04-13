//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonAgitatorSpeed
///
/// @file   ButtonAgitatorSpeed.h
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

class CButtonAgitatorSpeed : public CBCGPButton
{
public:
	CButtonAgitatorSpeed() : CBCGPButton()
	{
		m_bIsLeftText = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_3D;

		SetImage(IDB_BTAGITATORUP, IDB_BTAGITATORUP, IDB_BTAGITATORDISABLE);
	}
};
