//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonContinousGrafikNone
///
/// @file   DoseButtonContinousGrafikNone.h
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

#include "DoseButtonGrafik.h"



class CDoseButtonContinousGrafikNone : public CDoseButtonGrafik
{
	using CBaseClass = CDoseButtonGrafik;

private:
	CDoseButtonContinousGrafikNone(void) = delete;

public:
	CDoseButtonContinousGrafikNone(const CRect& rRect) : CBaseClass(rRect)
	{}

	~CDoseButtonContinousGrafikNone(void) override = default;
	CDoseButtonContinousGrafikNone(const CDoseButtonContinousGrafikNone&) = delete;
	CDoseButtonContinousGrafikNone& operator = (const CDoseButtonContinousGrafikNone&) = delete;

	BOOL IsBitmapUpdate(void) const final
	{ return FALSE;	}

	BOOL GetReady() const  final
	{	return FALSE;	}

	void FlashButton(CDC* ) final
	{}

	BOOL IsVisible() const final
	{	return FALSE;	}


	BOOL IsButtonRun() const final
	{	return FALSE;	}

	BOOL IsButtonReady() const final
	{	return FALSE;	}

	BOOL IsButtonStart() const final
	{	return FALSE;	}
};
