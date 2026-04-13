//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperWbm
///
/// @file   HopperWbm.h
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

#include "SquareHopper.h"

class CHopperWbm : public CSquareHopper
{
private:
	static const uint16_t X0 = 16U;
	static const uint16_t Y0 = 53U;
	static const uint16_t X2 = 89U;
	static const uint16_t Y2 = 64U;

public:
	explicit CHopperWbm() :
		CSquareHopper { base::IPoint { X0, Y0 },  base::IPoint { X2, Y2  } }
	{}

	CHopperWbm(const CHopperWbm&) = default;
	CHopperWbm& operator = (const CHopperWbm&) = default;
};
