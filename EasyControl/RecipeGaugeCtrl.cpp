//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeGaugeCtrl
///
/// @file   RecipeGaugeCtrl.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <cmath>
#include "RecipeGaugeCtrl.h"


//*************************************************************************************************************
//*************************************************************************************************************
CRecipeGaugeCtrl::CRecipeGaugeCtrl() : CBCGPLinearGaugeCtrl()
	, m_BrushField({ 
					 CBCGPBrush(CBCGPColor::Gray),
					 CBCGPBrush(CBCGPColor::Red), 
					 CBCGPBrush(CBCGPColor::Yellow),
					 CBCGPBrush(CBCGPColor::YellowGreen),
					 CBCGPBrush(CBCGPColor::Green)
		})
	, m_BrushStep  { eBrushStep ::eBrushNone }
	, m_Percentage { -1.0F }
#if _DEBUG
	, m_bInit{ FALSE }
#endif

{}
//******************************************************************************************************
//******************************************************************************************************
int32_t CRecipeGaugeCtrl::GetMaxTicks(const int32_t maxvalue)
{
	int32_t count = maxvalue / 5;
	int32_t maxticks = 0;
	if (count <= 5)
		maxticks = 5;
	else if (count <= 10)
		maxticks = 10;
	else if (count <= 20)
		maxticks = 20;
	else if (count <= 50)
		maxticks = 50;
	else if (count <= 100)
		maxticks = 100;
	else if (count <= 200)
		maxticks = 200;
	else if (count <= 500)
		maxticks = 500;
	else maxticks = 1000;
	return maxticks;
}
//******************************************************************************************************
//******************************************************************************************************
CRecipeGaugeCtrl::eBrushStep CRecipeGaugeCtrl::GetBrushStep(const float32_t value)
{
	eBrushStep brushStep = eBrushStep::eBrushNone;

	if (( value < 10.0F) || (value > 100.0F))
		brushStep = eBrushStep::eBrushRed;
	else if (value < 50.0F)
		brushStep = eBrushStep::eBrushYellow;
	else if (value < 100.0F)
		brushStep = eBrushStep::eBrushYellowgreen;
	else
		brushStep = eBrushStep::eBrushGreen;
	return brushStep;
}


	//*************************************************************************************************************
//*************************************************************************************************************
void CRecipeGaugeCtrl::SetPercentage(const float32_t value)
{
	ASSERT(m_bInit);
	if (_F32(fabs(value - m_Percentage)) > epsilon)
	{
		m_Percentage = value;
		auto pGauge = GetGauge();
		ASSERT_VALID(pGauge);
		auto brushstep = GetBrushStep(value);
		auto pLevelBar = pGauge->GetLevelBar(0);
		if (brushstep != m_BrushStep)
		{
			m_BrushStep = brushstep;
			pLevelBar->SetFillValueBrush(m_BrushField[_S32(m_BrushStep)]);
		}
		pLevelBar->SetValue(RANGE(m_Percentage, 0.0F, 100.0F));
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
void CRecipeGaugeCtrl::Init()
{
	ASSERT(! m_bInit);
	CBCGPLinearGaugeImpl* pGauge = GetGauge();
	ASSERT_VALID(pGauge);

	pGauge->SetColors(CBCGPLinearGaugeColors::BCGP_LINEAR_GAUGE_VISUAL_MANAGER);
	pGauge->SetFrameSize(1);
	pGauge->SetVerticalOrientation(TRUE);
	pGauge->SetRange(0, 100.0F);
	const int32_t majorstep = GetMaxTicks(_S32(100.0F));
	const int32_t minorstep = majorstep / 5U;
	pGauge->SetMajorTickMarkStep((minorstep > 0) ? _F32(majorstep / minorstep) : 1.0F);
	pGauge->SetStep(minorstep);


	const double xOffsetFromFrame = 45.0;
	const double dblLevelBarSize = 8.;
	const int nScale = 0;
	m_BrushStep = eBrushStep::eBrushNone;
	m_Percentage = -1.0F;

	pGauge->RemoveAllLevelBars();
	CBCGPBrush brFillVert(CBCGPColor::Gainsboro, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_VERTICAL, .2);
	CBCGPBrush brFrame(CBCGPColor::Silver);
	pGauge->AddLevelBar(brFillVert, brFrame, m_BrushField[_S32(eBrushStep::eBrushNone)], nScale, dblLevelBarSize, xOffsetFromFrame, CBCGPGaugeLevelBarObject::BCGP_GAUGE_LEVEL_OBJECT_THERMOMETER);
#if _DEBUG
	m_bInit = TRUE;
#endif

}
