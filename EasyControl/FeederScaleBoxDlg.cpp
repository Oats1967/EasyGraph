//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederScaleBoxDlg
///
/// @file   FeederScaleBoxDlg.cpp
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
#include "FeederScaleBoxDlg.h"


CScaleBoxDlg* CFeederScaleBoxDlg::g_ScaleBoxDlg = nullptr;

//***********************************************************************************************
//***********************************************************************************************
void CFeederScaleBoxDlg::CreateScaleBox(CWnd* pWnd, const int32_t index, const CPoint& top)
{
	if (!g_ScaleBoxDlg)
	{
		g_ScaleBoxDlg = new CFeederScaleBoxDlg;
		g_ScaleBoxDlg->Create(pWnd, index, top);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CFeederScaleBoxDlg::DestroyScaleBox(void)
{
	if (g_ScaleBoxDlg && g_ScaleBoxDlg->GetSafeHwnd())
	{
		g_ScaleBoxDlg->PostMessageW(WM_DESTROY);
	}
}


