//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousNominalGrafikLcl
///
/// @file   DoseContinousNominalGrafikLcl.h
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



class CDoseContinousNominalGrafikLcl : public CDoseBaseNominalGrafik
{
	using CBaseClass = CDoseBaseNominalGrafik;

protected:
	void OnUpdate(CDC* ) override
	{}

	BOOL IsTextUpdate(void) const override
	{ return FALSE; }

	BOOL IsBitmapUpdate(void) const override
	{ return FALSE; }

	void OnSetNewData(const base::CDoseDataCache& ) override
	{}

private:
	CDoseContinousNominalGrafikLcl(void) = delete;

public:
	CDoseContinousNominalGrafikLcl(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseContinousNominalGrafikLcl(void) override = default;
	CDoseContinousNominalGrafikLcl(const CDoseContinousNominalGrafikLcl&) = delete;
	CDoseContinousNominalGrafikLcl& operator = (const CDoseContinousNominalGrafikLcl&) = delete;
};










