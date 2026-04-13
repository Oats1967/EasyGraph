//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseBatcherNominalGrafik
///
/// @file   DoseBatcherNominalGrafik.cpp
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
#include "DoseBatcherNominalGrafik.h"
#include "FormatMetric.h"



//***********************************************************************************************************
//***********************************************************************************************************
CDoseBatcherNominalGrafik::CDoseBatcherNominalGrafik(const CRect& rRect) : CDoseBaseNominalGrafik(rRect)
, cSetpointPoint{ 0, 24 }
{}

//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBatcherNominalGrafik::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	TextOut(pDC, cSetpointPoint, FORMATKG(GetSetpoint()));
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBatcherNominalGrafik::IsTextUpdate(void) const
{
	auto bUpdate = CBaseClass::IsTextUpdate();
	if (!bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bSetpointChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBatcherNominalGrafik::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);
	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNewData.m_fSetpoint);
}





