//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseContinousActualGrafikIfs
///
/// @file   DoseContinousActualGrafikIfs.cpp
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
#include "DoseContinousActualGrafikIfs.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafikIfs::DrawActualSetpoint(CDC* pDC)
{
	TextOut(pDC, cSetpointPoint, FORMATKGH(GetSetpoint()));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafikIfs::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	DrawActualSetpoint(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseContinousActualGrafikIfs::IsTextUpdate(void) const
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
void CDoseContinousActualGrafikIfs::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bSetpointChanged = IsSetpointChanged(rNew.m_fMassflow);

}







