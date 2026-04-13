//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayIfl
///
/// @file   FeederDisplayIfl.cpp
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
#include "FeederDisplayIfl.h"
#include "FormatMetric.h"

//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayIfl::CFeederDisplayIfl(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
{
    SetDoseType(base::eDoseType::DOSETYPE_IFL);
}

//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfl::DrawBitmap(CDC* pDC)
{
    ShowBitmap(pDC, IDB_IFL_CONTAINER);
}
