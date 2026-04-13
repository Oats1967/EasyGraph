//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseButtonBatcherGrafik
///
/// @file   DoseButtonBatcherGrafik.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "DoseButtonBatcherGrafik.h"


/////////////////////////////////////////////////////////////////////////////
// CDoseButtonBatcherGrafik command target
//***********************************************************************************************************
//***********************************************************************************************************
CDoseButtonBatcherGrafik::CDoseButtonBatcherGrafik(const CRect& rRect) : CDoseButtonGrafik(rRect)
{}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonBatcherGrafik::GetReady() const
{
	const BOOL bReady = (GetSetpoint() > 0.01F);
	return bReady;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonBatcherGrafik::IsBitmapUpdate(void) const
{
	auto bUpdate = CBaseClass::IsBitmapUpdate();
	if (!bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bSetpointChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonBatcherGrafik::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNewData.m_fSetpoint);
}

