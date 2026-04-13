//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteControl
///
/// @file   RemoteControl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "RemoteControl.h"

static int32_t g_id;;

int32_t getActiveItem()
{
    return g_id;
}

void    setActiveItem(int32_t id)
{
    g_id = id;
}





