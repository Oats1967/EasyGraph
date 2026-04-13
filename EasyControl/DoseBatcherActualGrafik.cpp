//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseBatcherActualGrafik.cpp
///
/// @file   DoseBatcherActualGrafik.cpp
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
#include "DoseBatcherActualGrafik.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
CDoseBatcherActualGrafik::CDoseBatcherActualGrafik(const CRect& rRect) : CDoseBaseActualGrafik(rRect)
, cFeederTotalizerPoint{ 0, 46 }
{}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBatcherActualGrafik::OnUpdate(CDC* pDC)
{
	CDoseBaseActualGrafik::OnUpdate(pDC);

	TextOut(pDC, cFeederTotalizerPoint, FORMATKG(GetTotalizer()));
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBatcherActualGrafik::IsTextUpdate(void) const
{
	BOOL bUpdate = CBaseClass::IsTextUpdate();
	if (!bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bTotalizerChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBatcherActualGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bTotalizerChanged = IsTotalizerChanged(rNew.m_fTotalizer);
}

