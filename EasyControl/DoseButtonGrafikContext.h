//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonGrafik
///
/// @file   DoseButtonGrafik.h
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

#include "DoseGrafikContext.h"


class CDoseButtonGrafikContext : public CDoseGrafikContext
{
	using CBaseClass = CDoseGrafikContext;

private:
	CDoseButtonGrafikContext(void) = delete;

public:
	CDoseButtonGrafikContext(const CRect& rRect) : CDoseGrafikContext{ rRect }
	{}

	~CDoseButtonGrafikContext(void) override = default;
	CDoseButtonGrafikContext(const CDoseButtonGrafikContext&) = delete;
	CDoseButtonGrafikContext& operator = (const CDoseButtonGrafikContext&) = delete;

	virtual void FlashButton(CDC* pDC);
};











