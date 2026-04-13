//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module NumericTotalizerCtrl
///
/// @file   NumericTotalizerCtrl.h
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

#include "EasyControl.h"
#include "BASE/types.h"

#define ANIMATION_MIN	150
#define ANIMATION_MAX	1000

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CNumericTotalizerCtrl : public CBCGPNumericIndicatorImpl
{
	float32_t m_Value;
	const int32_t	c_nAmimationTime = 0;
	const float32_t epsilon = 1e-06F;

public:
	CNumericTotalizerCtrl(CBCGPVisualContainer* pContainer = NULL) : CBCGPNumericIndicatorImpl(CBCGPNumericIndicatorImpl::BCGP_NUMERIC_INDICATOR_CLASSIC, pContainer)
		, m_Value(-10.0F)
	{}

	void SetValue(const float32_t value)
	{
		if (_F32(fabs(value - m_Value)) > epsilon)
		{
			m_Value = value;
			CBCGPNumericIndicatorImpl::SetValue(m_Value, TRUE, c_nAmimationTime);
			Redraw();
		}
	}
	void Init(const int32_t cells = 9, const int32_t decimals = 1)
	{
		const bool cDrawLeadingZeros = TRUE;
		const bool cDrawDecimalPoint = (decimals > 0);
		const bool cShowSeparators = TRUE;

		SetCells(cells);
		SetDecimals(decimals);
		SetTextFormatAutoSize();
		SetDrawLeadingZeros(cDrawLeadingZeros);
		SetDrawDecimalPoint(cDrawDecimalPoint);
		SetSeparatorWidth(cShowSeparators ? 1 : 0);

	}


};

