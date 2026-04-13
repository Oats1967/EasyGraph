//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayLwp.cpp
///
/// @file   FeederDisplayLwp.cpp
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
#include "FeederDisplayLwp.h"
#include "MFCMacros.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayLwp::CFeederDisplayLwp(const int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
    , m_Hopper{}
    , cCalibratePoint{ 40, 0 }
    , cTarePoint{ 40, 0 }
    , cEmptyFeederPoint{ 74, 0 }
    , cReleasePoint{ 70, 60 }
    , cFillPoint{ 50, 34 }
    , cGravVolPoint{ 44, 88 }
    , cWeightPoint{ 45, 16 }
    , cRefillPoint{ 50, 0 }
    , cDriveCommandPoint{ 0, 40 }
    , cRefillStatePoint{ 35, 9 }
{
   
    SetDoseType(base::eDoseType::DOSETYPE_LWP);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwp::DrawBitmap(CDC* pDC)
{
    UINT nRessourceId = 0;
    if (GetAlarm())
    {
        nRessourceId = IDB_LWP_CONTAINER_ALARM;
    }
    else if (GetRunning())
    {
        nRessourceId = IDB_LWP_CONTAINER_RUN;
    }
    else
    {
        nRessourceId = IDB_LWP_CONTAINER;
    }
    ShowBitmap(pDC, nRessourceId);
}







