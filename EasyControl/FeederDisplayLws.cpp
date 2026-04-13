//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLws.h
///
/// @file   FeederDisplayLws.h
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
#include "EasyControl_DEU/resource.h"
#include "FeederDisplayLws.h"




//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayLws::CFeederDisplayLws(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
, m_Hopper{}
, cCalibratePoint{ 76, 0 }
, cTarePoint{ 76, 0 }
, cEmptyFeederPoint{ 76, 0 }
, cReleasePoint{ 23, 60 }
, cFillPoint{ 50, 44 }
, cGravVolPoint{ 44, 88 }
, cWeightPoint{ 45, 26 }
, cRefillPoint{ 50, 0 }
, cDriveCommandPoint{ 0, 40 }
, cRefillStatePoint{ 35, 9 }
{
    SetDoseType(base::eDoseType::DOSETYPE_LWS);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLws::DrawBitmap(CDC* pDC)
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


