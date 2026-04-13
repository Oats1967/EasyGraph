//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayIcp.cpp
///
/// @file   FeederDisplayIcp.cpp
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
#include "FeederDisplayIcp.h"
#include "EasyControl_DEU/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayVol
#define BCX 4
#define BCY 0
#define BCDX 80
#define BCDY 74


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayIcp::CFeederDisplayIcp(int32_t sID, const CRect& rRect) :	CFeederDisplay(sID, rRect)
, cCalibratePoint { 30, 4 }
, cTarePoint  { 10, 4 }
, cRefillPoint  { 50, 0 }
, cReleasePoint{ 35, 86 }
, cEmptyFeederPoint{ 30, 4 }
, cRefillStatePoint{ 35, 9 }
, cBackgroundRect{ BCX, BCY, BCX + BCDX, BCY + BCDY }
{
	SetDoseType(base::eDoseType::DOSETYPE_ICP);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIcp::DrawBitmap(CDC* pDC)
{
	ShowBitmap(pDC, IDB_ICP_CONTAINER);
}




