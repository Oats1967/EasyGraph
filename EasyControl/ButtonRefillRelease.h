  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonRefillRelease.h
///
/// @file   ButtonRefillRelease.h
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

#include "ButtonECExt.h"

class CButtonRefillRelease : public CButtonECExt
{
public:
	CButtonRefillRelease() :
		CButtonECExt (
			{ {  
				{ IDB_REFILL_RELEASE_DISABLE_UP,		IDB_REFILL_RELEASE_DISABLE_UP,		IDB_REFILL_RELEASE_DISABLE_DISABLE },
				{ IDB_REFILL_RELEASE_ENABLE_DOWN,		IDB_REFILL_RELEASE_ENABLE_DOWN,		IDB_REFILL_RELEASE_ENABLE_DISABLE }
			} },
			{ {
				{ IDB_REFILL_RELEASE_DISABLE_UP_EXT,	IDB_REFILL_RELEASE_DISABLE_UP_EXT,	IDB_REFILL_RELEASE_DISABLE_DISABLE_EXT },
				{ IDB_REFILL_RELEASE_ENABLE_DOWN_EXT,	IDB_REFILL_RELEASE_ENABLE_DOWN_EXT,	IDB_REFILL_RELEASE_ENABLE_DISABLE_EXT }
			} } )
	{}
};

