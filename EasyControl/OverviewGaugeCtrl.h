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
#pragma once

#include "EasyControl.h"
#include "BASE/types.h"

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class COverviewGaugeCtrl : public CBCGPLinearGaugeCtrl
{
	float32_t m_Setpoint;
	float32_t m_Massflow;
	int32_t m_kMaxSetpoint;

	const float32_t epsilon = 1e-06F;

private:
	static int32_t GetMaxTicks(const int32_t maxvalue);
	static int32_t GetMaxSetpointScale(const float32_t value);

	void Init();

public:
	COverviewGaugeCtrl();

	void SetSetpoint(const float32_t value);
	void SetMassflow(const float32_t value);
	void SetMaxSetpoint(float32_t value);
};

