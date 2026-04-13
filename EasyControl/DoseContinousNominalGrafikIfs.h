//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousNominalGrafikIfs
///
/// @file   DoseContinousNominalGrafikIfs.h
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



class CDoseContinousNominalGrafikIfs : public CDoseBaseNominalGrafik
{
	using CBaseClass = CDoseBaseNominalGrafik;

protected:
	void DrawPercentage(CDC*) override
	{}

private:
	CDoseContinousNominalGrafikIfs(void) = delete;

public:
	CDoseContinousNominalGrafikIfs(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseContinousNominalGrafikIfs(void) override = default;
	CDoseContinousNominalGrafikIfs(const CDoseContinousNominalGrafikIfs&) = delete;
	CDoseContinousNominalGrafikIfs& operator = (const CDoseContinousNominalGrafikIfs&) = delete;
};










