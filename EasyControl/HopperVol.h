//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperVol
///
/// @file   HopperVol.h
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
class CHopperVol : public CStandardHopper
{
	static const uint16_t FDX  = 38U;
	static const uint16_t FDXZ = 4U;
	static const uint16_t XOFF = ((FDX - FDXZ) / 2U);
	static const uint16_t FX0  = 43U;
	static const uint16_t FX1  = (FX0 + FDX);
	static const uint16_t FX2  = FX1;
	static const uint16_t FX3  = (FX2 - XOFF);
	static const uint16_t FX4  = (FX0 + XOFF);
	static const uint16_t FX5  = FX0;

	static const uint16_t FDY  = 36U;
	static const uint16_t FDYZ = 18U;
	static const uint16_t FY0 = 25U;
	static const uint16_t FY1  = FY0;
	static const uint16_t FY2 = (FY1 + FDY);
	static const uint16_t FY3 = (FY2 + FDYZ);
	static const uint16_t FY4 = FY3;
	static const uint16_t FY5 = FY2;

public:
	CHopperVol() : CStandardHopper( { base::IPoint{ FX0, FY0 },
								      base::IPoint{ FX1, FY1 },
									  base::IPoint{ FX2, FY2 },
									  base::IPoint{ FX3, FY3 },
									  base::IPoint{ FX4, FY4 },
									  base::IPoint{ FX5, FY5 } })
	{}
	CHopperVol(const CHopperVol& ) = default;
	CHopperVol& operator = (const CHopperVol& ) = default;
};
