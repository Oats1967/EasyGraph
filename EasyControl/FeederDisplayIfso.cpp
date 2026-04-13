//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayIfso
///
/// @file   FeederDisplayIfso.cpp
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
#include "FeederDisplayIfso.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayIfso::CFeederDisplayIfso(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
{
    SetDoseType(base::eDoseType::DOSETYPE_IFSO);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfso::DrawBitmap(CDC* pDC)
{
    ShowBitmap(pDC, IDB_IFOS_CONTAINER);
}

