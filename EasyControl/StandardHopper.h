//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayVol
///
/// @file   FeederDisplayVol.cpp
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

#include "BASE/include/2DHopper.h"
#include "IGrafikHopper.h"
#include "Colors.def"


class CStandardHopper : public IGrafikHopper
{
protected:
	base::C2DHopper	c_hopper;

public:
	CStandardHopper(const base::C2DHopper::HopperPoints& aP) :
		c_hopper{ { base::IPoint { aP[0].x, Mirror(aP[0].y)  },
					base::IPoint { aP[1].x, Mirror(aP[1].y)  },
					base::IPoint { aP[2].x, Mirror(aP[2].y)  },
					base::IPoint { aP[3].x, Mirror(aP[3].y)  },
					base::IPoint { aP[4].x, Mirror(aP[4].y)  },
					base::IPoint { aP[5].x, Mirror(aP[5].y)  } } }
	{}
	CStandardHopper(const CStandardHopper&) = default;
	CStandardHopper& operator = (const CStandardHopper&) = default;

	void DrawFilling(CDC* pDC, CBrush*, CPen*, const float32_t filling) override;
};
//**********************************************************************************************************************
//**********************************************************************************************************************
inline	void CStandardHopper::DrawFilling(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t filling)
{
	uint32_t count = 0;
	base::C2DHopper::HopperPoints points;
	c_hopper.GetFilling(filling, points, count);

	POINT polypoints[base::C2DHopper::c_MaxPoints];
	for (uint32_t i = 0; i < count; i++)
	{
		polypoints[i] = { points[i].x, Mirror(points[i].y) };
	}
	auto oldclr = pDC->SetBkColor(COLORBLUE);
	auto oldBrush = pDC->SelectObject(pBrush);
	auto oldPen = pDC->SelectObject(pPen);
	auto oldbk = pDC->SetBkMode(OPAQUE);
	pDC->Polygon(polypoints, count);
	pDC->SetBkMode(oldbk);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SetBkColor(oldclr);
}
