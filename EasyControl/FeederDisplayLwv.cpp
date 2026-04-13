//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLwf.h
///
/// @file   FeederDisplayLwf.h
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
#include "FeederDisplayLwv.h"


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayLwv::CFeederDisplayLwv(const int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
    , m_Hopper{ }
    , cCalibratePoint{ 68, 0 }
    , cReleasePoint{ 30, 70 }
    , cTarePoint{ 68, 0 }
    , cEmptyFeederPoint{ 74, 0 }
    , cFillPoint{ 50, 44 }
    , cGravVolPoint{ 44, 88 }
    , cWeightPoint{ 45, 26 }
    , cRefillPoint{ 50, 0 }
    , cDriveCommandPoint{ 0, 40 }
    , cRefillStatePoint{ 35, 9 }
{ 	
    SetDoseType(base::eDoseType::DOSETYPE_LWV); 
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwv::DrawBitmap(CDC* pDC)
{
    UINT nRessourceId = 0;
    if (GetAlarm())
    {
        nRessourceId = IDB_LWV_CONTAINER_ALARM;
    }
    else if (GetRunning())
    {
        nRessourceId = IDB_LWV_CONTAINER_RUN;
    }
    else
    {
        nRessourceId = IDB_LWV_CONTAINER;
    }

    ShowBitmap(pDC, nRessourceId);
}


