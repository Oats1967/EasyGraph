//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseProductNameGrafik
///
/// @file   DoseProductNameGrafik.h
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


class CDoseProductNameGrafik : public CDoseGrafikContext
{
	using CBaseClass = CDoseGrafikContext;

private:
	union DosierBaseStatus
	{
		struct
		{
			uint32_t bNameChanged : 1;
		} flags;
		uint64_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

protected:
	void OnRedraw(CDC* ) override {};
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override
	{	return FALSE;	}

	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

private:
	CDoseProductNameGrafik(void) = delete;

public:
	static CDoseProductNameGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseProductNameGrafik(const CRect& rRect) : CDoseGrafikContext{ rRect }
	{}

	~CDoseProductNameGrafik(void) override = default;
	CDoseProductNameGrafik(const CDoseProductNameGrafik&) = delete;
	CDoseProductNameGrafik& operator = (const CDoseProductNameGrafik&) = delete;
};

	











