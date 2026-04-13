//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousNominalGrafik
///
/// @file   DoseContinousNominalGrafik.h
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

#include "DoseBaseNominalGrafik.h"



class CDoseContinousNominalGrafik : public CDoseBaseNominalGrafik
{
	using CBaseClass = CDoseBaseNominalGrafik;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bSetpointChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

private:
	const POINT cSetpointPoint;

protected:
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNew) override;


protected:
	virtual void DrawSetpoint(CDC* pDC);

private:
	CDoseContinousNominalGrafik(void) = delete;

public:
	CDoseContinousNominalGrafik(const CRect& rRect);
	~CDoseContinousNominalGrafik(void) override = default;
	CDoseContinousNominalGrafik(const CDoseContinousNominalGrafik&) = delete;
	CDoseContinousNominalGrafik& operator = (const CDoseContinousNominalGrafik&) = delete;
};










