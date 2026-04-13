//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonContinousGrafik
///
/// @file   DoseButtonContinousGrafik.h
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

#include "DoseButtonGrafik.h"



class CDoseButtonContinousGrafik : public CDoseButtonGrafik
{
	using CBaseClass = CDoseButtonGrafik;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bSetpointChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};

	DosierBaseStatus m_StatusFlags;

protected:
	BOOL IsBitmapUpdate(void) const override;
	BOOL GetReady() const  override;
	void OnSetNewData(const base::CDoseDataCache&);

private:
	CDoseButtonContinousGrafik(void) = delete;

public:
	CDoseButtonContinousGrafik(const CRect& rRect);
	~CDoseButtonContinousGrafik(void) override = default;
	CDoseButtonContinousGrafik(const CDoseButtonContinousGrafik&) = delete;
	CDoseButtonContinousGrafik& operator = (const CDoseButtonContinousGrafik&) = delete;
};










