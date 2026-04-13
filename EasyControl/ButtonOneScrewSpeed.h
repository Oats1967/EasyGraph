//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonOneScrewSpeed
///
/// @file   ButtonOneScrewSpeed.h
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

class CButtonOneScrewSpeed : public CSpeedButton
{
public:
	CButtonOneScrewSpeed() : CSpeedButton(IDB_BTSPEEDUP, IDB_BTSPEEDUP, IDB_BTSPEEDDISABLE)
	{}
};
