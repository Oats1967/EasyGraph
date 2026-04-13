//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeGaugeCtrl
///
/// @file   RecipeGaugeCtrl.h
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

#include <array>
#include "EasyControl.h"
#include "BASE/types.h"

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CRecipeGaugeCtrl : public CBCGPLinearGaugeCtrl
{
	enum class eBrushStep : uint32_t
	{
		eBrushNone,
		eBrushRed,
		eBrushYellow,
		eBrushYellowgreen,
		eBrushGreen,
		eBrushMax,
	};
	const float32_t epsilon = 1e-06F;

	std::array<CBCGPBrush, _S32(eBrushStep::eBrushMax)> m_BrushField;
	eBrushStep m_BrushStep;
	float32_t m_Percentage;
#if _DEBUG
	BOOL m_bInit;
#endif


private:
	static int32_t GetMaxTicks(const int32_t maxvalue);
	static eBrushStep CRecipeGaugeCtrl::GetBrushStep(const float32_t value);

public:
	CRecipeGaugeCtrl();

	void SetPercentage(const float32_t value);
	void Init();
};

