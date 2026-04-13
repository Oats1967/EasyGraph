//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseBaseNominalGrafik.cpp
///
/// @file   DoseBaseNominalGrafik.cpp
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
#include "DoseBatcherNominalGrafikLwb.h"
#include "DoseBatcherNominalGrafikGwb.h"
#include "DoseBatcherNominalGrafikIcb.h"
#include "DoseContinousNominalGrafik.h"
#include "DoseContinousNominalGrafikIfl.h"
#include "DoseContinousNominalGrafikIfs.h"
#include "DoseContinousNominalGrafikLcl.h"
#include "DoseContinousNominalGrafikLws.h"
#include "DoseContinousNominalGrafikLwp.h"
#include "DoseContinousNominalGrafikLwv.h"
#include "DoseContinousNominalGrafikVol.h"
#include "DoseContinousNominalGrafikPid.h"
#include "DoseContinousNominalGrafikIcp.h"
#include "DoseContinousNominalGrafikWbf.h"
#include "DoseContinousNominalGrafikWbm.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
CDoseBaseNominalGrafik* CDoseBaseNominalGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	CDoseBaseNominalGrafik* pBase = nullptr;
	switch (lTyp)
	{
	case base::eDoseType::DOSETYPE_ICB:
		pBase = new CDoseBatcherNominalGrafikIcb(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFL:
		pBase = new CDoseContinousNominalGrafikIfl(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFSO:
	case base::eDoseType::DOSETYPE_IFSU:
		pBase = new CDoseContinousNominalGrafikIfs(rRect);
		break;

	case base::eDoseType::DOSETYPE_LCL:
		pBase = new CDoseContinousNominalGrafikLcl(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWS:
		pBase = new CDoseContinousNominalGrafikLws(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWP:
		pBase = new CDoseContinousNominalGrafikLwp(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWV:
		pBase = new CDoseContinousNominalGrafikLwv(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWB:
		pBase = new CDoseBatcherNominalGrafikLwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_GWB:
		pBase = new CDoseBatcherNominalGrafikGwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_VOL:
		pBase = new CDoseContinousNominalGrafikVol(rRect);
		break;

	case base::eDoseType::DOSETYPE_PID:
		pBase = new CDoseContinousNominalGrafikPid(rRect);
		break;

	case base::eDoseType::DOSETYPE_ICP:
		pBase = new CDoseContinousNominalGrafikIcp(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBF:
		pBase = new CDoseContinousNominalGrafikWbf(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBM:
		pBase = new CDoseContinousNominalGrafikWbm(rRect);
		break;

	case base::eDoseType::DOSETYPE_NONE:
		pBase = nullptr;
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	if (pBase)
	{
		pBase->SetDoseType(lTyp);
	}
	return pBase;
}
//***********************************************************************************************************
//***********************************************************************************************************
CDoseBaseNominalGrafik::CDoseBaseNominalGrafik(const CRect& rRect) : CBaseClass(rRect)
, cRecipeValuePoint{ 0, 5 }
{
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseNominalGrafik::DrawPercentage(CDC* pDC)
{
	// Feedertype
	if (!GetLocalMode())
	{
		TextOut(pDC, cRecipeValuePoint, FORMATPERCENT(GetRecipeValue()));
	}
}

//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseNominalGrafik::OnUpdate(CDC* pDC)
{
	DrawPercentage(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBaseNominalGrafik::IsTextUpdate(void) const
{
	return 	(m_StatusFlags.flags.bRecipeValueChanged == true) ||
			 (m_StatusFlags.flags.bLocalModeChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBaseNominalGrafik::IsBitmapUpdate(void) const
{ return FALSE; }
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseNominalGrafik::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);
	m_StatusFlags.flags.bLocalModeChanged	= IsLocalModeChanged();
	m_StatusFlags.flags.bRecipeValueChanged = IsRecipeValueChanged(rNewData.m_fAnteil);
}




