//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Polygon.h
///
/// @file   Polygon.h
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

#include "BASE/types.h"
#include "BASE/Utils/public/Memory.h"

class CPolygon
{
	POINT p[5];

public:
	CPolygon	(const CPoint&, const CPoint&, const CPoint&, const CPoint&);
	CPolygon	(const POINT [], const int n = 4);
	CPolygon()
	{
		base::utils::baseZeroMemory(p);
	}
	CPolygon(const CPolygon&) = default;
	CPolygon& operator =(const CPolygon&) = default;

	CPolygon&   operator += (const CPoint& p);
	void		Shrink (int32_t n);
	void		SetTopLine(const int32_t sHeight, const int32_t sWidth);

	int32_t		GetHeight (void) const
	{ return int32_t(p[3].y-p[0].y); }
	void Draw ( CDC* );

	int32_t GetWidthBottom() const 
	{	return abs(p[3].x - p[2].x);}

	int32_t GetWidthTop() const
	{	return abs(p[1].x - p[0].x);	}

	const POINT& GetPoint(int32_t index) const
	{ return p[index]; }
};



//-------------------------------------------------------------------------
inline	CPolygon ::	CPolygon (const POINT pp[], const int n)
{
	ASSERT(n >= 4 && n <= 5);
	p[0] = pp[0];
	p[1] = pp[1];
	p[2] = pp[2];
	p[3] = pp[3];
	p[4] = (n == 4) ? pp[0] : pp[4];
}



//-------------------------------------------------------------------------
inline	CPolygon ::	CPolygon (const CPoint& p0, const CPoint& p1, const CPoint& p2, const CPoint& p3)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
	p[4] = p0;
}


//-------------------------------------------------------------------------
inline CPolygon& CPolygon :: operator += (const CPoint& op)
{
	p[0].x += op.x;
	p[0].y += op.y;
	p[1].x += op.x;
	p[1].y += op.y;
	p[2].x += op.x;
	p[2].y += op.y;
	p[3].x += op.x;
	p[3].y += op.y;
	p[4].x += op.x;
	p[4].y += op.y;
	return *this;
}


inline	void CPolygon :: SetTopLine (const int32_t sHeight, const int32_t sWidth)
{
	const int32_t width = GetWidthTop();
	p[4].y = p[0].y = p[3].y - sHeight;
	p[1].y = p[2].y - sHeight;
	int32_t delta = (width - sWidth) / 2;
	p[4].x = p[0].x = p[0].x + delta;
	p[1].x = p[0].x + sWidth;
}


//-------------------------------------------------------------------------
inline	void CPolygon ::	Draw (CDC *pDC)
{
	pDC->Polygon(p, 5);
}


//-------------------------------------------------------------------------
inline void CPolygon :: Shrink (int32_t n)
{
	p[0].x += n;
	p[0].y += n;
	p[1].x -= n;
	p[1].y += n;
	p[2].x -= n;
	p[2].y -= n;
	p[3].x += n;
	p[3].y -= n;
	p[4].x += n;
	p[4].y += n;
}

