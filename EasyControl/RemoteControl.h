//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteInterface
///
/// @file   RemoteInterface.h
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

#include "WrapperBase.h"
#include "LCStatus.h"
#include "StringConvert.h"

extern int32_t getActiveItem();
extern void    setActiveItem(int32_t);
extern CLCStatus& GetLCStatus();



