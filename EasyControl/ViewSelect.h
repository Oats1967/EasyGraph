//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ViewSelect
///
/// @file   ViewSelect.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once
#include "BASE/types.h"

enum class eViewSelect : uint16_t
{
	eDefaultView = 0,
	eRecipeView,
	eAlarmArchiveView,
	eDiagramView,
	eViewMax,
};

static const uint32_t cViewSelectMax = _U32(eViewSelect::eViewMax);


