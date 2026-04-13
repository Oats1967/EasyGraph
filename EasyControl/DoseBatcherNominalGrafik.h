//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseBatcherNominalGrafik.h
///
/// @file   DoseBatcherNominalGrafik.h
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



class CDoseBatcherNominalGrafik : public CDoseBaseNominalGrafik
{
	using CBaseClass = CDoseBaseNominalGrafik;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bSetpointChanged : 1;
		} flags;
		uint64_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

private:
	const POINT cSetpointPoint;

protected:
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

private:
	CDoseBatcherNominalGrafik(void) = delete;

public:
	CDoseBatcherNominalGrafik(const CRect& rRect);
	~CDoseBatcherNominalGrafik(void) override = default;
	CDoseBatcherNominalGrafik(const CDoseBatcherNominalGrafik&) = delete;
	CDoseBatcherNominalGrafik& operator = (const CDoseBatcherNominalGrafik&) = delete;
};










