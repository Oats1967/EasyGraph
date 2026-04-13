//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseBaseNominalGrafik.h
///
/// @file   DoseBaseNominalGrafik.h
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


class CDoseBaseNominalGrafik : public CDoseGrafikContext
{
	using CBaseClass = CDoseGrafikContext;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bLocalModeChanged : 1;
			uint32_t bRecipeValueChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

private:
	const POINT cRecipeValuePoint;


protected:
	void OnRedraw(CDC* ) override {}
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

protected:
	virtual void DrawPercentage(CDC* pDC);

private:
	CDoseBaseNominalGrafik(void) = delete;

public:
	static CDoseBaseNominalGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseBaseNominalGrafik(const CRect& rRect);
	~CDoseBaseNominalGrafik(void) override = default;
	CDoseBaseNominalGrafik(const CDoseBaseNominalGrafik&) = delete;
	CDoseBaseNominalGrafik& operator = (const CDoseBaseNominalGrafik&) = delete;
};










