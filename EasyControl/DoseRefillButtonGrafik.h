//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseRefillButtonGrafik
///
/// @file   DoseRefillButtonGrafik.h
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

#include "DoseButtonGrafikContext.h"



class CDoseRefillButtonGrafik : public CDoseButtonGrafikContext
{
	using CBaseClass = CDoseButtonGrafikContext;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bRefillReleaseChanged	: 1;
			uint32_t bRefillActiveChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};

private:
	const POINT			cDoseRefillButtonPoint;
	DosierBaseStatus	m_StatusFlags;

protected:
	void OnRedraw(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNew) override;

	virtual void DrawStatus(CDC* pDC);

private:
	CDoseRefillButtonGrafik(void) = delete;

public:
	static CDoseRefillButtonGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseRefillButtonGrafik(const CRect& rRect);
	~CDoseRefillButtonGrafik(void) override = default;
	CDoseRefillButtonGrafik(const CDoseRefillButtonGrafik&) = delete;
	CDoseRefillButtonGrafik& operator = (const CDoseRefillButtonGrafik&) = delete;

	BOOL IsVisible() const override;
};










