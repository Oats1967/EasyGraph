//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SquareHopper
///
/// @file   SquareHopper.h
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

#include "BASE/include/2DRectangle.h"
#include "IGrafikHopper.h"


class CSquareHopper : public IGrafikHopper
{
	base::C2DRectangle c_hopper;

public:
	CSquareHopper(const base::IPoint& topleft, const base::IPoint& bottomright) :
		c_hopper{ { topleft, bottomright } }
	{}

	CSquareHopper(const CSquareHopper&) = default;
	CSquareHopper& operator = (const CSquareHopper&) = default;

	void DrawFilling(CDC* pDC, CBrush*, CPen*, const float32_t filling) override;
};
//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************
inline	void CSquareHopper::DrawFilling(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t filling)
{
	uint32_t count = 0;
	base::C2DRectangle::HopperPoints points;
	c_hopper.GetFilling(filling, points, count);

	if (count > 1)
	{
		CRect rRect{ CPoint(points[0].x + 1, points[0].y), CPoint(points[1].x, points[1].y) };
		auto oldBrush = pDC->SelectObject(pBrush);
		auto oldPen = pDC->SelectObject(pPen);
		pDC->Rectangle(rRect);
		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
	}
}
