//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HelpInfoDlgEx
///
/// @file   HelpInfoDlgEx.h
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

#include "EasyControl.h"
#include "HelpInfoDlg.h"
#include "BASE/Base.h"


class CHelpInfoDlgEx : public CHelpInfoDlg
{
public:
	static void CreateHelpInfoBox(CWnd* pWnd, const int32_t InfoId, const int32_t captionId, const int32_t textId);

public:
	CHelpInfoDlgEx() = default;
	virtual ~CHelpInfoDlgEx() = default;
};


