//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperLwp
///
/// @file   HopperLwp.h
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

#include "StandardHopper.h"


//							  { FX0/FX6, FY0/FY6 } |            | { FX1, FY1 }
//                                                 |            |
//                                                 |            |
//                                                 |            |
//                                  { FX5, FY5 }   |            | { FX2, FY2 }
//													\		   /
//													  \		 /
//													    \  / 
//                                          { FX4, FY4 }    { FX3, FY3 }
//                                                         

//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************
class CHopperLwp : public CStandardHopper
{
	static const uint16_t FDX   = 39U;
	static const uint16_t FDXZ  = 1U;
	static const uint16_t XOFF  = ((FDX - FDXZ) / 2U);
	static const uint16_t FX0   = 44U;
	static const uint16_t FX1	 = (FX0 + FDX);
	static const uint16_t FX2	 = FX1;
	static const uint16_t FX3	 = (FX2 - XOFF);
	static const uint16_t FX4   = (FX0 + XOFF - 1U);
	static const uint16_t FX5   = FX0;
	static const uint16_t FX6   = FX0;

	static const uint16_t FDY	 = 25U;
	static const uint16_t FDYZ  = 14U;

	static const uint16_t FY0 = 17U;
	static const uint16_t FY1 = FY0;
	static const uint16_t FY2 = (FY1 + FDY - 1U);
	static const uint16_t FY3 = (FY2 + FDYZ);
	static const uint16_t FY4 = FY3;
	static const uint16_t FY5 = FY2;
	static const uint16_t FY6 = FY0;

public:
	CHopperLwp() : CStandardHopper({  base::IPoint{ FX0, FY0 },
									  base::IPoint{ FX1, FY1 },
									  base::IPoint{ FX2, FY2 },
									  base::IPoint{ FX3, FY3 },
									  base::IPoint{ FX4, FY4 },
									  base::IPoint{ FX5, FY5 } })
	{}
	CHopperLwp(const CHopperLwp& ) = default;
	CHopperLwp& operator = (const CHopperLwp& ) = default;
};
