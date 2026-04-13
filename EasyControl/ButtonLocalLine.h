//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonLocaLine
///
/// @file   ButtonLocaLine.h
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

#include "ButtonEC.h"

class CButtonLocalLine : public CButtonEC
{
public:
	CButtonLocalLine() :
		CButtonEC(
						{ {
							{ IDB_LOCALMODE_FOCUS,	IDB_LOCALMODE_FOCUS,IDB_LOCALMODE_DISABLE },
							{ IDB_LINE_FOCUS,		IDB_LINE_FOCUS,		IDB_LINE_DISABLE }
						} }
					)
	{}
};
