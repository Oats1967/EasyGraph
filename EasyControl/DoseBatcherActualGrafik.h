//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseBatcherActualGrafik
///
/// @file   DoseBatcherActualGrafik.h
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



class CDoseBatcherActualGrafik : public CDoseBaseActualGrafik
{
	using CBaseClass = CDoseBaseActualGrafik;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bTotalizerChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus	m_StatusFlags;

private:
	const POINT cFeederTotalizerPoint;

protected:
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

private:
	CDoseBatcherActualGrafik(void) = delete;

public:
	CDoseBatcherActualGrafik(const CRect& rRect);
	~CDoseBatcherActualGrafik(void) override = default;
	CDoseBatcherActualGrafik(const CDoseBatcherActualGrafik&) = delete;
	CDoseBatcherActualGrafik& operator = (const CDoseBatcherActualGrafik&) = delete;
};










