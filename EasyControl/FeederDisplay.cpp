//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplay
///
/// @file   FeederDisplay.cpp
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
#include <cassert>
#include "BASE/include/DoseType.h"
#include "FeederDisplay.h"
#include "FeederDisplayLws.h"
#include "FeederDisplayLcl.h"
#include "FeederDisplayLwp.h"
#include "FeederDisplayLwv.h"
#include "FeederDisplayLwb.h"
#include "FeederDisplayIcb.h"
#include "FeederDisplayGwb.h"
#include "FeederDisplayWbf.h"
#include "FeederDisplayPid.h"
#include "FeederDisplayIcp.h"
#include "FeederDisplayWbm.h"
#include "FeederDisplayVol.h"
#include "FeederDisplayIfl.h"
#include "FeederDisplayIfso.h"
#include "FeederDisplayIfsu.h"
#include "EasyControl_DEU/resource.h"



//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplay::CFeederDisplay(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
, c_AlarmBrush { c_alarmfillcolor }
, c_WarningBrush{ c_warningfillcolor }
, c_RunBrush { c_runfillcolor }
, c_AlarmPen{ PS_SOLID, 1, c_alarmfillcolor }
, c_WarningPen{ PS_SOLID, 1, c_warningfillcolor }
, c_RunPen{ PS_SOLID, 1, c_runfillcolor }
{
}
//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplay* CFeederDisplay :: Create (int32_t sID, base::eDoseType lTyp, const CRect& rRect)
{
	CFeederDisplay* pDisplay = nullptr;

	switch(lTyp)
	{
		case base::eDoseType::DOSETYPE_LCL:
			pDisplay = new CFeederDisplayLcl(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_LWS:
			pDisplay = new CFeederDisplayLws(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_GWB:
			pDisplay = new CFeederDisplayGwb(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_LWB:
			pDisplay = new CFeederDisplayLwb(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_ICB:
			pDisplay = new CFeederDisplayIcb(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_LWV:
			pDisplay = new CFeederDisplayLwv(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_LWP:
			pDisplay = new CFeederDisplayLwp(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_PID:
			pDisplay = new CFeederDisplayPid(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_WBF:
			pDisplay = new CFeederDisplayWbf(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_ICP:
			pDisplay = new CFeederDisplayIcp(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_WBM:
			pDisplay = new CFeederDisplayWbm(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_VOL:
			pDisplay = new CFeederDisplayVol(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_IFL:
			pDisplay = new CFeederDisplayIfl(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_IFSO:
			pDisplay = new CFeederDisplayIfso(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_IFSU:
			pDisplay = new CFeederDisplayIfsu(sID, rRect);
			break;
		case base::eDoseType::DOSETYPE_NONE:
			pDisplay = nullptr;
			break;

		default:
			assert(FALSE);
			break;
	}
	if (pDisplay)
	{
		pDisplay->SetDoseType(lTyp);
	}
	return pDisplay;
}
//***********************************************************************************************************
//***********************************************************************************************************
void	CFeederDisplay::Update(CDC* pDC, CStatic& aWnd)
{
	CRect aRect;
	aWnd.GetClientRect(&aRect);
	CGrafikContext::ShowGrafik(pDC, aRect);
	if (m_dcImage.IsBitmapDC())
	{
		aWnd.SetBitmap(m_dcImage.GetBitmap());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplay::DrawRelease(CDC* pDC)
{
	uint32_t nIDResource = IDB_MOTORREADY;

	if (GetRunning())
	{
		nIDResource = IDB_MOTORRUN;
	}
	else
	{
		nIDResource = IDB_MOTORREADY;
		if ( ! GetIOReleaseInput())
		{
			nIDResource = IDB_MOTOROFF;
		}
	}
	ShowBitmap(pDC, nIDResource, GetReleasePoint());
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplay::DrawTaring(CDC* pDC)
{
	if (GetTaring())
	{
		uint32_t nRessourceID = IDB_TARAWARNING;
		ShowBitmap(pDC, nRessourceID, GetTarePoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplay::DrawCalibration(CDC* pDC)
{
	if (GetCalibration())
	{
		uint32_t nRessourceID = IDB_KALIBWARNING;
		ShowBitmap(pDC, nRessourceID, GetCalibratePoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplay::DrawEmptyFeeder(CDC* pDC)
{
	if (GetEmptyFeeder())
	{
		ShowBitmap(pDC, IDB_EMPTYFEEDERWARNING, GetEmptyFeederPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void	CFeederDisplay::DrawRefillState(CDC* pDC)
{
	uint32_t nRessourceID = 0;

	if (GetRefillAlarm())
	{
		nRessourceID = IDB_REFILL_RED;
	}
	else
	{
		if (GetRefillActive())
		{
			nRessourceID = IDB_REFILL_BLUE;
		}
	}
	if (nRessourceID)
	{
		ShowBitmap(pDC, nRessourceID, GetRefillPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void	CFeederDisplay::FillRoundRect(CDC* pDC, const CRect& aRect, CBrush* pBrush, CPen* pPen)
{
	auto oldBrush	= (pBrush == nullptr) ? pDC->SelectStockObject(NULL_BRUSH) : pDC->SelectObject(pBrush);
	auto oldPen		= (pPen == nullptr) ? pDC->SelectStockObject(NULL_PEN) : pDC->SelectObject(pPen);
	pDC->Rectangle(&aRect);
	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}
//***********************************************************************************************************
//***********************************************************************************************************
void	CFeederDisplay::DrawBackground(CDC* pDC)
{
	const auto bAlarm   = GetAlarm();
	const auto bWarning = GetWarning();
	const auto bRun     = GetRunning();

	if (bRun || bAlarm || bWarning)
	{
		auto& aRect = GetBackgroundRect();

		if (bAlarm)
		{
			FillRoundRect(pDC, aRect, &c_AlarmBrush, &c_AlarmPen);
		}
		else if (bWarning)
		{
			FillRoundRect(pDC, aRect, &c_WarningBrush, &c_WarningPen);
		}
		else 
		{
			FillRoundRect(pDC, aRect, &c_RunBrush, &c_RunPen);
		}
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void	CFeederDisplay::OnRedraw(CDC* pDC)
{
	DrawBackground(pDC);
	DrawBitmap(pDC);
	DrawRelease(pDC);
	DrawTaring(pDC);
	DrawCalibration(pDC);
	DrawEmptyFeeder(pDC);
	DrawRefillState(pDC);
}

//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplay::IsBitmapUpdate(void) const
{
	return (m_StatusFlags.flags.bCalibrationChanged == true) ||
			(m_StatusFlags.flags.bTaringChanged == true) ||
			(m_StatusFlags.flags.bEmptyFeederChanged == true) ||
			(m_StatusFlags.flags.bReleaseChanged == true) ||
			(m_StatusFlags.flags.bIOReleaseInputChanged == true) ||
			(m_StatusFlags.flags.bDoseTypeChanged == true) ||
			(m_StatusFlags.flags.bRunningChanged == true) ||
			(m_StatusFlags.flags.bAlarmChanged == true) ||
			(m_StatusFlags.flags.bWarningChanged == true) ||
			(m_StatusFlags.flags.bIdChanged == true) ||
			(m_StatusFlags.flags.bRefillActiveChanged == true) ||
			(m_StatusFlags.flags.bRefillAlarmChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplay::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CDoseBase::OnSetNewData(rNew);

	m_StatusFlags.flags.bIdChanged					= IsIdChanged(rNew.m_id);
	m_StatusFlags.flags.bSetpointChanged			= IsSetpointChanged(rNew.m_fSetpoint);
	m_StatusFlags.flags.bDoseTypeChanged			= IsDoseTypeChanged(rNew.m_DoseType);
	m_StatusFlags.flags.bAlarmChanged				= IsAlarmChanged(rNew.m_ulDoseAlarm);
	m_StatusFlags.flags.bWarningChanged				= IsWarningChanged(rNew.m_ulDoseWarning);
	m_StatusFlags.flags.bLineModeChanged			= IsLocalModeChanged();
	m_StatusFlags.flags.bReleaseChanged				= IsReleaseChanged();
	m_StatusFlags.flags.bIOStartInputChanged		= IsIOStartInputChanged();
	m_StatusFlags.flags.bIOLineStartInputChanged	= IsIOLineStartInputChanged();
	m_StatusFlags.flags.bIOLogoStartChanged			= IsIOLogoStartChanged();
	m_StatusFlags.flags.bRunningChanged				= IsRunningChanged();
	m_StatusFlags.flags.bCalibrationChanged			= IsCalibrationChanged();
	m_StatusFlags.flags.bEmptyFeederChanged			= IsEmptyFeederChanged();
	m_StatusFlags.flags.bTaringChanged				= IsTaringChanged();
	m_StatusFlags.flags.bIOReleaseInputChanged		= IsIOReleaseInputChanged();
	m_StatusFlags.flags.bRefillAlarmChanged			= IsRefillAlarmChanged();
	m_StatusFlags.flags.bRefillActiveChanged		= IsRefillActiveChanged();
}




