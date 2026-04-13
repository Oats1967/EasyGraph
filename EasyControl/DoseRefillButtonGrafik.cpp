//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseRefillButtonGrafik
///
/// @file   DoseRefillButtonGrafik.cpp
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
#include "DoseRefillButtonGrafik.h"
#include "DoseRefillButtonGrafikLws.h"
#include "DoseRefillButtonGrafikLwp.h"
#include "DoseRefillButtonGrafikLwv.h"
#include "DoseRefillButtonGrafikLwb.h"
#include "DoseRefillButtonGrafikGwb.h"
#include "DoseRefillButtonGrafikVol.h"
#include "DoseRefillButtonGrafikPid.h"
#include "DoseRefillButtonGrafikIcp.h"
#include "DoseRefillButtonGrafikIfs.h"
#include "DoseRefillButtonGrafikIfl.h"
#include "DoseRefillButtonGrafikLcl.h"
#include "DoseRefillButtonGrafikIcb.h"
#include "DoseRefillButtonGrafikWbf.h"
#include "DoseRefillButtonGrafikWbm.h"
#include "EasyControl_DEU/resource.h"
#include "MFCMacros.h"


/////////////////////////////////////////////////////////////////////////////
// CDoseRefillButtonGrafik command target
// 
//***********************************************************************************************************
//***********************************************************************************************************
CDoseRefillButtonGrafik* CDoseRefillButtonGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	CDoseRefillButtonGrafik* pBase = nullptr;

	switch (lTyp)
	{
	case base::eDoseType::DOSETYPE_LWS:
		pBase = new CDoseRefillButtonGrafikLws(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWP:
		pBase = new CDoseRefillButtonGrafikLwp(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWV:
		pBase = new CDoseRefillButtonGrafikLwv(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWB:
		pBase = new CDoseRefillButtonGrafikLwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_GWB:
		pBase = new CDoseRefillButtonGrafikGwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_VOL:
		pBase = new CDoseRefillButtonGrafikVol(rRect);
		break;

	case base::eDoseType::DOSETYPE_PID:
		pBase = new CDoseRefillButtonGrafikPid(rRect);
		break;

	case base::eDoseType::DOSETYPE_ICP:
		pBase = new CDoseRefillButtonGrafikIcp(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFSU:
	case base::eDoseType::DOSETYPE_IFSO:
		pBase = new CDoseRefillButtonGrafikIfs(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFL:
		pBase = new CDoseRefillButtonGrafikIfl(rRect);
		break;

	case base::eDoseType::DOSETYPE_LCL:
		pBase = new CDoseRefillButtonGrafikLcl(rRect);
		break;

	case base::eDoseType::DOSETYPE_ICB:
		pBase = new CDoseRefillButtonGrafikIcb(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBF:
		pBase = new CDoseRefillButtonGrafikWbf(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBM:
		pBase = new CDoseRefillButtonGrafikWbm(rRect);
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
CDoseRefillButtonGrafik::CDoseRefillButtonGrafik(const CRect& rRect) : CBaseClass(rRect)
, cDoseRefillButtonPoint{ 0,0 }
{
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillButtonGrafik::IsVisible() const
{
	auto result = GetRefillControlEnable();
	result     &= GetRefillEnable();
	return result;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillButtonGrafik::DrawStatus(CDC* pDC)
{
	ASSERT(IsVisible());

	UINT nRessourceID = 0;
	if (! GetRefillActive())
	{
		nRessourceID = IDB_BTREFILL;
	}
	else
	{
		nRessourceID = IDB_BTNOREFILL;
	}
	ShowBitmap(pDC, nRessourceID, cDoseRefillButtonPoint);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillButtonGrafik::OnRedraw(CDC* pDC)
{
	DrawStatus(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillButtonGrafik::OnUpdate(CDC* )
{
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillButtonGrafik::IsTextUpdate(void) const
{
	return 	FALSE;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillButtonGrafik::IsBitmapUpdate(void) const
{
	return BOOL(m_StatusFlags.allflags != 0);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillButtonGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CDoseBase::OnSetNewData(rNew);

	m_StatusFlags.flags.bRefillReleaseChanged = IsRefillEnableChanged();
	m_StatusFlags.flags.bRefillActiveChanged   = IsRefillActiveChanged();
}



