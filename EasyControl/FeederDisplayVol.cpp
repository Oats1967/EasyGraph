//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayVol
///
/// @file   FeederDisplayVol.cpp
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
#include "FeederDisplayVol.h"
#include "FormatMetric.h"
#include "EasyControl_DEU/resource.h"



/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayVol message handlers
CFeederDisplayVol::CFeederDisplayVol(const int32_t sID, const CRect& rRect) :	CBaseClass(sID, rRect)
	, cCalibratePoint{ 76, 0 }
	, cTarePoint{ 0, 0 }				// Not used
	, cEmptyFeederPoint{ 0, 0 }			// Not used
	, cReleasePoint{ 23, 60 }
	, cRefillPoint{ 50, 0 }
	, cDriveCommandPoint{ 0, 40 }
	, cWeightAboveMaxPoint{ 85, 22 }
	, cWeightBelowMinPoint{ 85, 52 }
	, m_Hopper{}
	, m_brushBlueSolid(COLORBLUE)
	, m_brushRedSolid(COLORRED)
	, m_brushWhiteDashed(HS_DIAGCROSS, COLORWHITE)
	, m_PenBlue(PS_NULL, 0, COLORBLUE)

{
	SetDoseType(base::eDoseType::DOSETYPE_VOL);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::DrawFilling(CDC* pDC)
{
	ASSERT(pDC);

	if (GetRefillControlEnable()) // Ist Refill controlwaagen aktiv
	{
		if (IsEmptyFeederSensor())
		{
			GetGrafikHopper().DrawFilling(pDC, &m_brushRedSolid, &m_PenBlue, 0.15F);
		}
		else if (IsWeightBelowMinSensor())
		{
			// do nothing
		}
		else if (IsWeightAboveMaxSensor())
		{
			GetGrafikHopper().DrawFilling(pDC, &m_brushBlueSolid, &m_PenBlue, 0.9F);
		}
		else
		{
			GetGrafikHopper().DrawFilling(pDC, &m_brushWhiteDashed, &m_PenBlue, 0.4F);
		}
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::DrawDriveCommand(CDC* pDC)
{
	TextOut(pDC, GetDriveCommandPoint(), FORMATPERCENT(GetDriveCommand()), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::DrawRefillState(CDC* pDC)
{
	BOOL bLocked = TRUE;
	if (GetRefillEnable()) // Ist Freigabe
	{
		if ( GetRefillControlEnable() ) // Ist Refill controlwaagen aktiv
		{
			CBaseClass::DrawRefillState(pDC);
			bLocked = FALSE;
		}
	}
	if (bLocked)
	{
		ShowBitmap(pDC, IDB_DOSIERERVERRIEGELUNG, GetRefillPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::DrawMinMaxSensor(CDC* pDC)
{
	if (GetRefillControlEnable()) // Ist Refill controlwaagen aktiv
	{
		ShowBitmap(pDC, (IsWeightAboveMaxSensor()) ? IDB_BTGREEN: IDB_BTNONE, GetWeightAboveMaxPoint());

		uint32_t nRessourceID = 0;
		if (IsEmptyFeederSensor())
		{
			nRessourceID = IDB_BTRED;
		}
		else if ( IsWeightBelowMinSensor() )
		{
			nRessourceID = IDB_BTNONE;
		}
		else
		{
			nRessourceID = IDB_BTGREEN;
		}
		ShowBitmap(pDC, nRessourceID, GetWeightBelowMinPoint());
	}
}

//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	TextOut(pDC, GetDriveCommandPoint(), FORMATPERCENT(GetDriveCommand()), COLORBLACK, TRANSPARENT);
	DrawDriveCommand(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::DrawBitmap(CDC* pDC)
{
	UINT nRessourceId = 0;
	if (GetAlarm())
	{
		nRessourceId = IDB_LWS_CONTAINER_ALARM;
	}
	else if (GetRunning())
	{
		nRessourceId = IDB_LWS_CONTAINER_RUN;
	}
	else
	{
		nRessourceId = IDB_LWS_CONTAINER;
	}
	ShowBitmap(pDC, nRessourceId);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::OnRedraw(CDC* pDC)
{
	CBaseClass::OnRedraw(pDC);
	DrawFilling(pDC);
	DrawMinMaxSensor(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayVol::IsBitmapUpdate() const
{
	auto bUpdate = CBaseClass::IsBitmapUpdate();
	if ( ! bUpdate)
	{
		bUpdate =   (m_StatusFlags.flags.bRefillEnableChanged == true) ||
					(m_StatusFlags.flags.bMinMaxSensorChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayVol::IsTextUpdate(void) const
{
	BOOL bUpdate = m_StatusFlags.flags.bDriveCommandChanged;
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayVol::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bRefillEnableChanged = IsRefillEnableChanged();
	m_StatusFlags.flags.bDriveCommandChanged = IsDriveCommandChanged(rNewData.m_fActualDriveCommand);
	m_StatusFlags.flags.bMinMaxSensorChanged = IsWeightAboveMaxSensorChanged() || 
											   IsWeightBelowMinSensorChanged() || 
											   IsEmptyFeederSensorChanged();
}




