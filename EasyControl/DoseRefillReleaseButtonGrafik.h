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

#include "DoseButtonGrafikContext.h"



class CDoseRefillReleaseButtonGrafik : public CDoseButtonGrafikContext
{
	using CBaseClass = CDoseButtonGrafikContext;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bRefillReleaseChanged : 1;
			uint32_t bRefillActiveChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};

private:
	const POINT			cDoseRefillReleaseButtonPoint;
	DosierBaseStatus	m_StatusFlags;

protected:
	void OnRedraw(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNew) override;

	virtual void DrawStatus(CDC* pDC);

private:
	CDoseRefillReleaseButtonGrafik(void) = delete;

public:
	static CDoseRefillReleaseButtonGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseRefillReleaseButtonGrafik(const CRect& rRect);
	~CDoseRefillReleaseButtonGrafik(void) override = default;
	CDoseRefillReleaseButtonGrafik(const CDoseRefillReleaseButtonGrafik&) = delete;
	CDoseRefillReleaseButtonGrafik& operator = (const CDoseRefillReleaseButtonGrafik&) = delete;

	BOOL IsVisible() const override;
};










