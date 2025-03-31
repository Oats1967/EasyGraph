//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseTypeMap
///
/// @file   DoseTypeMap.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------


#pragma once

#include <map>
#include "BASE/types.h"
#include "BASE/include/MassflowSelect.h"


using CMassflowSelectMap = std::map< base::eMassflowSelect, uint32_t>;
