//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CHopperGwb
///
/// @file   CHopperGwb.h
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

#include "HopperLwb.h"

class CHopperGwb : public CHopperLwb
{
public:
	CHopperGwb(const base::C3DHopper::HopperPoints& points) :
		CHopperLwb(points)
	{}

	CHopperGwb() = delete;
	CHopperGwb(const CHopperGwb& points) = default;
	CHopperGwb& operator = (const CHopperGwb& points) = default;

	void DrawFilling(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t ) override
	{ CHopperLwb::DrawFilling(pDC, pBrush, pPen, 1.0F);	}
};



