//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperLcl
///
/// @file   HopperLcl.h
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


class CHopperLcl : public CSquareHopper
{
	static const uint16_t FDX = 54U;
	static const uint16_t FDY = 39U;
	static const uint16_t FX0 = 23U;
	static const uint16_t FX1 = (FX0 + FDX);
	static const uint16_t FX2 = FX1;

	static const uint16_t FY0 = 27U;
	static const uint16_t FY1 = FY0;
	static const uint16_t FY2 = (FY1 + FDY);

	//							  { FX0/FX4, FY0/FY4 } |            | { FX1, FY1 }
	//                                                 |            |
	//                                                 |            |
	//                                                 |            |
	//                                  { FX3, FY3 }   |            | { FX2, FY2 }
	//                                                         

public:
	CHopperLcl() :
		CSquareHopper(base::IPoint{ FX0, FY0 }, base::IPoint{ FX2, FY2 })
	{}
	CHopperLcl(const CHopperLcl& points) = default;
	CHopperLcl& operator = (const CHopperLcl& points) = default;
};



