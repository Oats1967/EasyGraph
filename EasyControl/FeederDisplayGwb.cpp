//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayGwb
///
/// @file   FeederDisplayGwb.h
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
#include "FeederDisplayGwb.h"



#define FDX  38
#define FDY  32

#define FX0 43
#define FX1 ( FX0 + FDX) 
#define FX2  FX1
#define FX3 FX0
#define FX4 FX0

#define FY0 30
#define FY1 FY0 
#define FY2  ( FY1 + FDY )
#define FY3  FY2
#define FY4 FY0

//							  { FX0/FX4, FY0/FY4 } |            | { FX1, FY1 }
//                                                 |            |
//                                                 |            |
//                                                 |            |
//                                  { FX3, FY3 }   |            | { FX2, FY2 }
//                                                         

#define FPOLY_0 { FX0, FY0 }
#define FPOLY_1 { FX1, FY1 }
#define FPOLY_2 { FX2, FY2 }
#define FPOLY_3 { FX3, FY3 }
#define FPOLY_4 { FX4, FY4 }



//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayGwb::CFeederDisplayGwb(int32_t sID, const CRect& rRect) : CFeederDisplayLwf(sID, rRect)
, m_Hopper{ base::C3DHopper::HopperPoints { base::IPoint(FPOLY_0), base::IPoint(FPOLY_1), base::IPoint(FPOLY_2), base::IPoint(FPOLY_3), base::IPoint(FPOLY_4)  } }
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
    SetDoseType(base::eDoseType::DOSETYPE_GWB);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayGwb::DrawBitmap(CDC* pDC)
{
    UINT nRessourceId = 0;
    if (GetAlarm())
    {
        nRessourceId = IDB_GWB_CONTAINER_ALARM;
    }
    else if (GetRunning())
    {
        nRessourceId = IDB_GWB_CONTAINER_RUN;
    }
    else
    {
        nRessourceId = IDB_GWB_CONTAINER;
    }
    ShowBitmap(pDC, nRessourceId);
}


