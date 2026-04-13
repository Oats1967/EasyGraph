//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonEmptyFeederSpeed
///
/// @file   ButtonEmptyFeederSpeed.h
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

class CButtonEmptyFeederSpeed : public CSpeedButton
{
public:
	CButtonEmptyFeederSpeed() : CSpeedButton(IDB_BTSPEEDUP, IDB_BTSPEEDUP, IDB_BTSPEEDDISABLE)
	{}
};
