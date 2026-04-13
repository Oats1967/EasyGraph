//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousActualGrafikLcl
///
/// @file   DoseContinousActualGrafikLcl.h
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

#include "DoseBaseActualGrafik.h"



class CDoseContinousActualGrafikLcl : public CDoseBaseActualGrafik
{
	using CBaseClass = CDoseBaseActualGrafik;

protected:
	void OnRedraw(CDC* ) override
	{}

	void OnUpdate(CDC* ) override
	{}

	BOOL IsTextUpdate(void) const override
	{	return FALSE;	}

	BOOL IsBitmapUpdate(void) const override
	{ return FALSE; 	}


private:
	CDoseContinousActualGrafikLcl(void) = delete;

public:
	CDoseContinousActualGrafikLcl(const CRect& rRect) : CBaseClass(rRect)
	{}
	~CDoseContinousActualGrafikLcl(void) override = default;
	CDoseContinousActualGrafikLcl(const CDoseContinousActualGrafikLcl&) = delete;
	CDoseContinousActualGrafikLcl& operator = (const CDoseContinousActualGrafikLcl&) = delete;
};










