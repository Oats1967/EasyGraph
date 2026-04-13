//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseContinousActualGrafik.cpp
///
/// @file   DoseContinousActualGrafik.cpp
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
#include "DoseContinousActualGrafik.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
CDoseContinousActualGrafik::CDoseContinousActualGrafik(const CRect& rRect) : CBaseClass(rRect)
, cFeederMassflowPoint{ 0, 46 }
{}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafik::DrawMassflow(CDC* pDC)
{
	TextOut(pDC, cFeederMassflowPoint, FORMATKGH(GetMassflow()));
}

//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafik::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	DrawMassflow(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseContinousActualGrafik::IsTextUpdate(void) const
{
	BOOL bUpdate = CBaseClass::IsTextUpdate();
	if (! bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bMassflowChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseContinousActualGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bMassflowChanged = IsMassflowChanged(rNew.m_fMassflow);
}
