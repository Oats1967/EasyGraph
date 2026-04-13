//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonRefillFeeder
///
/// @file   ButtonRefillFeeder.h
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

#include "SpeedButton.h"

class CButtonRefillFeeder : public CSpeedButton
{
public:
	CButtonRefillFeeder() : CSpeedButton(IDB_BTREFILLSCREWUP, IDB_BTREFILLSCREWUP, IDB_BTREFILLSCREWDISABLE)
	{}
};

