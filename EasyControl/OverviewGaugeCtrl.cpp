//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module OverviewGaugeCtrl
///
/// @file   OverviewGaugeCtrl.h
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
#include "OverviewGaugeCtrl.h"


//*************************************************************************************************************
//*************************************************************************************************************
COverviewGaugeCtrl::COverviewGaugeCtrl() : CBCGPLinearGaugeCtrl()
, m_Setpoint(-1.0F)
, m_Massflow(-1.0F)
, m_kMaxSetpoint(-1)
{
	Init();
}
//******************************************************************************************************
//******************************************************************************************************
int32_t COverviewGaugeCtrl::GetMaxTicks(const int32_t maxvalue)
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
//*************************************************************************************************************
//*************************************************************************************************************
int32_t COverviewGaugeCtrl::GetMaxSetpointScale(const float32_t value)
{
	int32_t kMax = NEXTLONG(value);
	int32_t divisor = 100;
	if (kMax <= 500)
	{
		divisor = 100;
	}
	else if (kMax <= 1000)
	{
		divisor = 200;
	}
	else if (kMax <= 3000)
	{
		divisor = 250;
	}
	else
	{
		divisor = 500;
	}
	int32_t quot = kMax / divisor;
	return (quot + 1) * divisor;

}
//*************************************************************************************************************
//*************************************************************************************************************
void COverviewGaugeCtrl::SetSetpoint(const float32_t value)
{
	if (_F32(fabs(value - m_Setpoint)) > epsilon)
	{
		m_Setpoint = value;
		auto pGauge = GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetValue(m_Setpoint, 0, 0, TRUE);
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
void COverviewGaugeCtrl::SetMassflow(const float32_t value)
{
	if (_F32(fabs(value - m_Massflow)) > epsilon)
	{
		m_Massflow = value;
		auto pGauge = GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetLevelBarValue(m_Massflow, 0, 0, TRUE);
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
void COverviewGaugeCtrl::SetMaxSetpoint(const float32_t value)
{
	const int32_t kMax  = GetMaxSetpointScale(value);
	if (kMax != m_kMaxSetpoint)
	{
		m_kMaxSetpoint = kMax;
		auto pGauge = GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetRange(0, m_kMaxSetpoint);
		const int32_t majorstep = GetMaxTicks(m_kMaxSetpoint);
		const int32_t minorstep = majorstep / 5U;
		pGauge->SetMajorTickMarkStep((minorstep > 0) ? _F32(majorstep / minorstep) : 1.0F);
		pGauge->SetStep(minorstep);
		pGauge->Redraw();
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
void COverviewGaugeCtrl::Init()
{
	CBCGPLinearGaugeImpl* pGauge = GetGauge();
	ASSERT_VALID(pGauge);

	pGauge->SetColors(CBCGPLinearGaugeColors::BCGP_LINEAR_GAUGE_VISUAL_MANAGER);
	pGauge->SetFrameSize(1);
	ASSERT(pGauge->IsVerticalOrientation() == FALSE);
	pGauge->SetVerticalOrientation(FALSE);
	pGauge->RemoveAllLevelBars();
	pGauge->RemoveAllColoredRanges();
	ASSERT(pGauge->GetPointersCount() == 0);

	const COLORREF clrFill(RGB(77, 161, 211));
	CBCGPBrush brFrame(CBCGPColor::Silver);
	CBCGPBrush brFill(CBCGPColor::Gainsboro, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_HORIZONTAL, .2);
	CBCGPBrush brValue(clrFill);
	const double dblLevelBarSize = 14.;
	const int nScale = 0;
	const int nFrameSize = 1;

	CBCGPBrush brValue2(clrFill, CBCGPDrawManager::ColorMakeLighter(clrFill, .5), CBCGPBrush::BCGP_GRADIENT_HORIZONTAL);

	double yOffsetFromFrame = 30.0;
	pGauge->AddLevelBar(brFill, brFrame, brValue2, nScale, dblLevelBarSize, yOffsetFromFrame, CBCGPGaugeLevelBarObject::BCGP_GAUGE_LEVEL_OBJECT_ROUNDED_RECT);


	CBCGPLinearGaugePointer pointer(CBCGPLinearGaugePointer::BCGP_GAUGE_NEEDLE_TRIANGLE, 18.0, 18.0);
	pGauge->AddPointer(pointer);
}
