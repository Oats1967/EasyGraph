//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseContinousNominalGrafik.cpp
///
/// @file   DoseContinousNominalGrafik.cpp
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
#include "DoseContinousNominalGrafik.h"
#include "FormatMetric.h"

//***********************************************************************************************************
//***********************************************************************************************************
CDoseContinousNominalGrafik::CDoseContinousNominalGrafik(const CRect& rRect) : CBaseClass(rRect)
, cSetpointPoint{ 0, 24 }
{}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousNominalGrafik::DrawSetpoint(CDC* pDC)
{
	TextOut(pDC, cSetpointPoint, FORMATKGH(GetSetpoint()));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousNominalGrafik::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	DrawSetpoint(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseContinousNominalGrafik::IsTextUpdate(void) const
{
	BOOL bUpdate = CBaseClass::IsTextUpdate();
	if ( ! bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bSetpointChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousNominalGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNew.m_fSetpoint);
}



