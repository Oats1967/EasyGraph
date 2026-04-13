//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HopperLwb
///
/// @file   HopperLwb.h
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

#include "BASE/include/3DHopper.h"
#include "IGrafikHopper.h"

class CHopperLwb : public IGrafikHopper
{
public:
	const COLORREF colorblue = RGB(166, 202, 240);
	const COLORREF lightblue = RGB(214, 230, 248);
	base::C3DHopper	c_hopper;
	CPen	m_LightBluekPen;
	CBrush  m_LightBlueBrush;

public:
	CHopperLwb(const base::C3DHopper::HopperPoints& points) :
		c_hopper{ base::C3DHopper::HopperPoints{
				  base::IPoint { points[0].x, Mirror(points[0].y)  },
				  base::IPoint { points[1].x, Mirror(points[1].y)  },
				  base::IPoint { points[2].x, Mirror(points[2].y)  },
				  base::IPoint { points[3].x, Mirror(points[3].y)  },
				  base::IPoint { points[4].x, Mirror(points[4].y)  } } }
		, m_LightBluekPen{ PS_SOLID, 1, lightblue }
		, m_LightBlueBrush { lightblue }
	{}

	CHopperLwb() = delete;
	CHopperLwb(const CHopperLwb& points) = default;
	CHopperLwb& operator = (const CHopperLwb& points) = default;

	void DrawFilling(CDC* pDC, CBrush*, CPen*, const float32_t filling) override;
};
//**********************************************************************************************************************
inline	void CHopperLwb ::DrawFilling(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t filling)
{
	const int32_t fy = 4;

	uint32_t count = 0;
	base::C3DHopper::HopperPoints points;
	c_hopper.GetFilling(filling, points, count);

	if (count > 0)
	{
		POINT polypoints[base::C3DHopper::c_MaxPoints];
		for (uint32_t i = 0; i < count; i++)
		{
			polypoints[i] = { points[i].x, Mirror(points[i].y) };
		}
		//int32_t oldbk = pDC->SetBkMode(OPAQUE);
		CPen* oldPen = pDC->SelectObject(pPen);
		CBrush* oldBrush = pDC->SelectObject(pBrush);
		CRect rRect{ CPoint(polypoints[0].x + 1, polypoints[0].y), CPoint(polypoints[2].x, polypoints[2].y) };
		pDC->Rectangle(rRect);
		CRect eRect{ CPoint(polypoints[0].x + 1, -fy), CPoint(polypoints[2].x, fy) };
		{
			pDC->SelectObject(&m_LightBluekPen);
			pDC->SelectObject(&m_LightBlueBrush);
			CRect topRect{ eRect };
			topRect.OffsetRect(0, polypoints[0].y);
			pDC->Ellipse(topRect);
		}
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
	}
}



