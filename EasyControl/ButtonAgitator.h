//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonAgitator
///
/// @file   ButtonAgitator.h
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

#include "SpeedButton.h"

class CButtonAgitator : public CSpeedButton
{
public:
	CButtonAgitator() : CSpeedButton(IDB_BTAGITATORUP, IDB_BTAGITATORUP, IDB_BTAGITATORDISABLE)
	{}
};
