//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseButtonContinousGrafik
///
/// @file   DoseButtonContinousGrafik.cpp
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
#include "DoseButtonContinousGrafik.h"


/////////////////////////////////////////////////////////////////////////////
// CDoseButtonContinousGrafik command target
//***********************************************************************************************************
//***********************************************************************************************************
CDoseButtonContinousGrafik::CDoseButtonContinousGrafik(const CRect& rRect) : CBaseClass(rRect)
{}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonContinousGrafik::GetReady() const
{
	const BOOL bReady = (GetSetpoint() > 0.05F);
	return bReady;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonContinousGrafik::IsBitmapUpdate(void) const
{
	auto bUpdate = CBaseClass::IsBitmapUpdate();
	if (! bUpdate)
	{
		bUpdate = m_StatusFlags.flags.bSetpointChanged;
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonContinousGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNew.m_fSetpoint);
}

