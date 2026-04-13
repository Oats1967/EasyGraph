//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederScaleBoxDlg
///
/// @file   FeederScaleBoxDlg.h
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

#include "ScaleBoxDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CFeederScaleBoxDlg : public CScaleBoxDlg
{
private:
	static CScaleBoxDlg* g_ScaleBoxDlg;

public:
	CFeederScaleBoxDlg() : CScaleBoxDlg()
	{}
	~CFeederScaleBoxDlg()
	{ g_ScaleBoxDlg = nullptr;	}

	static void CreateScaleBox(CWnd*pWnd, const int32_t id, const CPoint& top);
	static void DestroyScaleBox(void);

};

