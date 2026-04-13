//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of interface IGrafikHopper
///
/// @file   IGrafikHopper.h
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


class IGrafikHopper
{
protected:
	static const int32_t cImageheight = 113;

	static int32_t Mirror(const int32_t y)
	{ return cImageheight - y;	}

public:
	IGrafikHopper() = default;
	virtual ~IGrafikHopper() = 0 {};
	IGrafikHopper(const IGrafikHopper&) = delete;
	IGrafikHopper& operator=(const IGrafikHopper&) = delete;

	virtual void DrawFilling(CDC* pDC, CBrush*, CPen*, const float32_t filling) = 0;
};



