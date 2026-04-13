//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayPid.cpp
///
/// @file   FeederDisplayPid.cpp
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
#include "FeederDisplayPid.h"
#include "EasyControl_DEU/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayVol
#define BCX 3
#define BCY 0
#define BCDX 80
#define BCDY 74



//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayPid::CFeederDisplayPid(const int32_t sID, const CRect& rRect) :CFeederDisplay(sID, rRect)
, cCalibratePoint { 76, 4 }
, cTarePoint { 76, 4 }
, cEmptyFeederPoint{ 76, 4 }
, cReleasePoint{ 16, 89 }
, cRefillPoint{ 0, 0 }
, cBackgroundRect{ BCX, BCY, BCX + BCDX, BCY + BCDY }
{
	SetDoseType(base::eDoseType::DOSETYPE_PID);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayPid::DrawBitmap(CDC* pDC)
{
	ShowBitmap(pDC, IDB_PID_CONTAINER);
}
