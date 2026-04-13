//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonContinousGrafikLcl
///
/// @file   DoseButtonContinousGrafikLcl.h
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

#include "DoseButtonContinousGrafik.h"



class CDoseButtonContinousGrafikLcl : public CDoseButtonContinousGrafik
{
	using CBaseClass = CDoseButtonContinousGrafik;

protected:

private:
	CDoseButtonContinousGrafikLcl(void) = delete;

public:
	CDoseButtonContinousGrafikLcl(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseButtonContinousGrafikLcl(void) override = default;
	CDoseButtonContinousGrafikLcl(const CDoseButtonContinousGrafikLcl&) = delete;
	CDoseButtonContinousGrafikLcl& operator = (const CDoseButtonContinousGrafikLcl&) = delete;

	BOOL IsVisible() const override
	{	return FALSE;	}

	BOOL IsButtonRun() const override
	{ return FALSE; }
};







