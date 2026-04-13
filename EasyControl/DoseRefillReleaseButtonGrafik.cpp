//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseRefillReleaseButtonGrafik
///
/// @file   DoseRefillReleaseButtonGrafik.cpp
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
#include "DoseRefillReleaseButtonGrafik.h"
#include "DoseRefillReleaseButtonGrafikLws.h"
#include "DoseRefillReleaseButtonGrafikLwp.h"
#include "DoseRefillReleaseButtonGrafikLwv.h"
#include "DoseRefillReleaseButtonGrafikLwb.h"
#include "DoseRefillReleaseButtonGrafikGwb.h"
#include "DoseRefillReleaseButtonGrafikVol.h"
#include "DoseRefillReleaseButtonGrafikPid.h"
#include "DoseRefillReleaseButtonGrafikIcp.h"
#include "DoseRefillReleaseButtonGrafikIfs.h"
#include "DoseRefillReleaseButtonGrafikIfl.h"
#include "DoseRefillReleaseButtonGrafikLcl.h"
#include "DoseRefillReleaseButtonGrafikIcb.h"
#include "DoseRefillReleaseButtonGrafikWbf.h"
#include "DoseRefillReleaseButtonGrafikWbm.h"
#include "EasyControl_DEU/resource.h"
#include "MFCMacros.h"


/////////////////////////////////////////////////////////////////////////////
// CDoseRefillReleaseButtonGrafik command target
// 
//***********************************************************************************************************
//***********************************************************************************************************
CDoseRefillReleaseButtonGrafik* CDoseRefillReleaseButtonGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	CDoseRefillReleaseButtonGrafik* pBase = nullptr;

	switch (lTyp)
	{
	case base::eDoseType::DOSETYPE_LWS:
		pBase = new CDoseRefillReleaseButtonGrafikLws(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWP:
		pBase = new CDoseRefillReleaseButtonGrafikLwp(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWV:
		pBase = new CDoseRefillReleaseButtonGrafikLwv(rRect);
		break;

	case base::eDoseType::DOSETYPE_LWB:
		pBase = new CDoseRefillReleaseButtonGrafikLwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_GWB:
		pBase = new CDoseRefillReleaseButtonGrafikGwb(rRect);
		break;

	case base::eDoseType::DOSETYPE_VOL:
		pBase = new CDoseRefillReleaseButtonGrafikVol(rRect);
		break;

	case base::eDoseType::DOSETYPE_PID:
		pBase = new CDoseRefillReleaseButtonGrafikPid(rRect);
		break;

	case base::eDoseType::DOSETYPE_ICP:
		pBase = new CDoseRefillReleaseButtonGrafikIcp(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFSO:
	case base::eDoseType::DOSETYPE_IFSU:
		pBase = new CDoseRefillReleaseButtonGrafikIfs(rRect);
		break;

	case base::eDoseType::DOSETYPE_IFL:
		pBase = new CDoseRefillReleaseButtonGrafikIfl(rRect);
		break;

	case base::eDoseType::DOSETYPE_LCL:
		pBase = new CDoseRefillReleaseButtonGrafikLcl(rRect);
		break;

	case base::eDoseType::DOSETYPE_ICB:
		pBase = new CDoseRefillReleaseButtonGrafikIcb(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBF:
		pBase = new CDoseRefillReleaseButtonGrafikWbf(rRect);
		break;

	case base::eDoseType::DOSETYPE_WBM:
		pBase = new CDoseRefillReleaseButtonGrafikWbm(rRect);
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
CDoseRefillReleaseButtonGrafik::CDoseRefillReleaseButtonGrafik(const CRect& rRect) : CBaseClass(rRect)
, cDoseRefillReleaseButtonPoint{ 0,0 }
{
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillReleaseButtonGrafik::IsVisible() const
{
	return GetRefillControlEnable();
}

//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillReleaseButtonGrafik::DrawStatus(CDC* pDC)
{
	uint32_t nRessourceID = 0;

	ASSERT(IsVisible());

	{
		if (GetRefillEnable())
		{
			nRessourceID = IDB_BTREFILLRELEASE;
		}
		else
		{
			nRessourceID = IDB_BTREFILLNORELEASE;
		}
		ShowBitmap(pDC, nRessourceID, cDoseRefillReleaseButtonPoint);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillReleaseButtonGrafik::OnRedraw(CDC* pDC)
{
	DrawStatus(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillReleaseButtonGrafik::OnUpdate(CDC* )
{
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillReleaseButtonGrafik::IsTextUpdate(void) const
{
	return 	FALSE;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseRefillReleaseButtonGrafik::IsBitmapUpdate(void) const
{
	auto result = m_StatusFlags.allflags != 0;
	return 	result;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseRefillReleaseButtonGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CDoseBase::OnSetNewData(rNew);

	if (IsVisible())
	{
		m_StatusFlags.flags.bRefillReleaseChanged = IsRefillEnableChanged();
		m_StatusFlags.flags.bRefillActiveChanged = IsRefillActiveChanged();
	}
	else
	{
		m_StatusFlags.allflags = 0;
	}
}




