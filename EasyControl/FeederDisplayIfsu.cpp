//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayIfsu
///
/// @file   FeederDisplayIfsu.h
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
#include "FeederDisplayIfsu.h"


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayIfsu::CFeederDisplayIfsu(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
{
    SetDoseType(base::eDoseType::DOSETYPE_IFSU);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfsu::DrawBitmap(CDC* pDC)
{
    ShowBitmap(pDC, IDB_IFUS_CONTAINER);
}

