//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseContinousActualGrafikIfl
///
/// @file   DoseContinousActualGrafikIfl.cpp
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
#include "DoseContinousActualGrafikIfl.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafikIfl::DrawActualSetpoint(CDC* pDC)
{
	TextOut(pDC, cSetpointPoint, FORMATKGH(GetSetpoint()));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafikIfl::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	DrawActualSetpoint(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseContinousActualGrafikIfl::IsTextUpdate(void) const
{
	BOOL bUpdate = CBaseClass::IsTextUpdate();
	if (!bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bSetpointChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafikIfl::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNew.m_fMassflow);

}







