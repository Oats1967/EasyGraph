//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonVolGrav
///
/// @file   ButtonVolGrav.h
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


class CButtonVolGrav : public CButtonECExt
{
public:
	CButtonVolGrav()  :
		CButtonECExt(
						{ {
							{ IDB_GRAVIMETRIC_FOCUS,		IDB_GRAVIMETRIC_FOCUS,	IDB_GRAVIMETRIC_DISABLE },
							{ IDB_VOLUMETRIC_FOCUS,			IDB_VOLUMETRIC_FOCUS,		IDB_VOLUMETRIC_DISABLE },
						} },
						{ {
							{ IDB_GRAVIMETRIC_FOCUS_EXT,	IDB_GRAVIMETRIC_FOCUS_EXT,	IDB_GRAVIMETRIC_DISABLE_EXT },
							{ IDB_VOLUMETRIC_FOCUS_EXT,		IDB_VOLUMETRIC_FOCUS_EXT,		IDB_VOLUMETRIC_DISABLE_EXT }
						} }
					)
	{}
};

