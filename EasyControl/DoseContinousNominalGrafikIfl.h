//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousNominalGrafikIfl
///
/// @file   DoseContinousNominalGrafikIfl.h
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

#include "DoseBaseNominalGrafik.h"



class CDoseContinousNominalGrafikIfl : public CDoseBaseNominalGrafik
{
	using CBaseClass = CDoseBaseNominalGrafik;

protected:
	void DrawPercentage(CDC*) override
	{}

private:
	CDoseContinousNominalGrafikIfl(void) = delete;

public:
	CDoseContinousNominalGrafikIfl(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseContinousNominalGrafikIfl(void) override = default;
	CDoseContinousNominalGrafikIfl(const CDoseContinousNominalGrafikIfl&) = delete;
	CDoseContinousNominalGrafikIfl& operator = (const CDoseContinousNominalGrafikIfl&) = delete;
};










