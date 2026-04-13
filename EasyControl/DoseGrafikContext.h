//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseRefillReleaseButtonGrafik
///
/// @file   DoseRefillReleaseButtonGrafik.h
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


#include "DoseBase.h"
#include "GrafikContext.h"



class CDoseGrafikContext : public CDoseBase, public CGrafikContext
{
private:
	CDoseGrafikContext(void) = delete;

public:
	CDoseGrafikContext(const int32_t id, const CRect& rRect) : CDoseBase(id), CGrafikContext{ rRect }
	{}
	CDoseGrafikContext(const CRect& rRect) : CDoseBase(), CGrafikContext{ rRect }
	{}
	~CDoseGrafikContext(void) override = default;
	CDoseGrafikContext(const CDoseGrafikContext&) = delete;
	CDoseGrafikContext& operator = (const CDoseGrafikContext&) = delete;

	virtual BOOL IsVisible() const
	{ return TRUE;	}
};










