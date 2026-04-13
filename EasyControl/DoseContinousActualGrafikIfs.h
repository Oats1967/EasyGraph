//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousActualGrafik
///
/// @file   DoseContinousActualGrafik.h
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



class CDoseContinousActualGrafikIfs : public CDoseBaseActualGrafik
{
	using CBaseClass = CDoseBaseActualGrafik;

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
	bool GetRunning() const override
	{
		return CBaseClass::GetRunning() || CBaseClass::GetStart();
	}

	void DrawFeedingType(CDC*) override
	{}

	void OnUpdate(CDC* pDC) override;

	BOOL IsTextUpdate(void) const override;

	void OnSetNewData(const base::CDoseDataCache& rNew) override;

	virtual void DrawActualSetpoint(CDC*);


private:
	CDoseContinousActualGrafikIfs(void) = delete;

public:
	CDoseContinousActualGrafikIfs(const CRect& rRect) : CBaseClass(rRect)
		, cSetpointPoint{ 0, 46 }
	{}
	~CDoseContinousActualGrafikIfs(void) override = default;
	CDoseContinousActualGrafikIfs(const CDoseContinousActualGrafikIfs&) = delete;
	CDoseContinousActualGrafikIfs& operator = (const CDoseContinousActualGrafikIfs&) = delete;
};










