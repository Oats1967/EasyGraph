//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonContinousGrafikIfl
///
/// @file   DoseButtonContinousGrafikIfl.h
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


class CDoseButtonContinousGrafikIfl : public CDoseButtonContinousGrafik
{
	using CBaseClass = CDoseButtonContinousGrafik;

private:
	CDoseButtonContinousGrafikIfl(void) = delete;

public:
	CDoseButtonContinousGrafikIfl(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseButtonContinousGrafikIfl(void) override = default;
	CDoseButtonContinousGrafikIfl(const CDoseButtonContinousGrafikIfl&) = delete;
	CDoseButtonContinousGrafikIfl& operator = (const CDoseButtonContinousGrafikIfl&) = delete;

	BOOL IsVisible() const override;
	BOOL IsButtonRun() const override;
};


//***********************************************************************************************************
//***********************************************************************************************************
inline BOOL CDoseButtonContinousGrafikIfl::IsVisible() const
{
	const auto bLocalMode			= GetLocalMode();
	const auto bIOStartInputern		= GetIOStartInput();
	const auto bIOLineStartInput	= GetIOLineStartInput();
	const auto bIOLogoStart			= GetIOLogoStart();
	const auto bReady				= GetReady(); // override
	const auto bTaraState			= GetTaring(); // override
	BOOL bVisible = (!bTaraState) && bIOLogoStart && (!bIOStartInputern) && ((!bIOLineStartInput) || bLocalMode);
	return bVisible;
}
//***********************************************************************************************************
//***********************************************************************************************************
inline BOOL CDoseButtonContinousGrafikIfl::IsButtonRun() const
{ 
	return GetReady() && GetStart();
}








