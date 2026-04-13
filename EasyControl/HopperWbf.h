//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperWbf
///
/// @file   HopperWbf.h
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

#include "IGrafikHopper.h"
#include "BASE/include/2DTrapeze.h"

class CHopperWbf : public IGrafikHopper
{
public:
	const COLORREF fillcolor = RGB(166, 202, 240);

private:
	base::C2DTrapeze c_hopper;

public:
	explicit CHopperWbf(const base::C2DTrapeze::TrapezePoints& points) :
				c_hopper{ base::C2DTrapeze::TrapezePoints {
						  base::IPoint { points[0].x, Mirror(points[0].y)  },
						  base::IPoint { points[1].x, Mirror(points[1].y)  },
						  base::IPoint { points[2].x, Mirror(points[2].y)  },
						  base::IPoint { points[3].x, Mirror(points[3].y)  } } }
	{}

	CHopperWbf() = delete;

	void DrawFilling(CDC* pDC, CBrush*, CPen*, const float32_t filling) override;
};
//**********************************************************************************************************************
//**********************************************************************************************************************
inline	void CHopperWbf ::DrawFilling(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t filling)
{
	uint32_t count = 0;
	base::C2DTrapeze::TrapezePoints points;
	c_hopper.GetFilling(filling, points, count);
	if (count > 0)
	{
		POINT polypoints[base::C2DTrapeze::c_MaxPoints];
		for (uint32_t i = 0; i < count; i++)
		{
			polypoints[i] = { points[i].x, Mirror(points[i].y) };
		}
		auto oldclr = pDC->SetBkColor(fillcolor);
		auto oldBrush = pDC->SelectObject(pBrush);
		auto oldPen = pDC->SelectObject(pPen);
		auto oldbk = pDC->SetBkMode(OPAQUE);
		pDC->Polygon(polypoints, count);
		pDC->SetBkColor(oldclr);
		pDC->SetBkMode(oldbk);
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
	}
}



