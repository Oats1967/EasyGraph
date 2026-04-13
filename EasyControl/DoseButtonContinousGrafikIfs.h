//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonContinousGrafikIfs
///
/// @file   DoseButtonContinousGrafikIfs.h
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



class CDoseButtonContinousGrafikIfs : public CDoseButtonContinousGrafik
{
	using CBaseClass = CDoseButtonContinousGrafik;

protected:

private:
	CDoseButtonContinousGrafikIfs(void) = delete;

public:
	CDoseButtonContinousGrafikIfs(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseButtonContinousGrafikIfs(void) override = default;
	CDoseButtonContinousGrafikIfs(const CDoseButtonContinousGrafikIfs&) = delete;
	CDoseButtonContinousGrafikIfs& operator = (const CDoseButtonContinousGrafikIfs&) = delete;

	BOOL IsVisible() const override;
	BOOL IsButtonRun() const override;
};

//***********************************************************************************************************
//***********************************************************************************************************
inline BOOL CDoseButtonContinousGrafikIfs::IsVisible() const
{
	const auto bLocalMode			= GetLocalMode();
	const auto bIOStartInputern		= GetIOStartInput();
	const auto bIOLineStartInput	= GetIOLineStartInput();
	const auto bIOLogoStart			= GetIOLogoStart();
	const auto bReady				= GetReady(); // override
	BOOL bVisible = bIOLogoStart && (!bIOStartInputern) && ((!bIOLineStartInput) || bLocalMode);
	return bVisible;
}
//***********************************************************************************************************
//***********************************************************************************************************
inline BOOL CDoseButtonContinousGrafikIfs::IsButtonRun() const
{ 
	return GetReady() && GetStart(); 
}








