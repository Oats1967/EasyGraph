//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module HelpInfoDlgEx.cpp
///
/// @file   HelpInfoDlgEx.cpp
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
#include "HelpInfoDlgEx.h"
#include "HelpInfoDlg.h"
#include "MFCMacros.h"

//**************************************************************************************************************
//**************************************************************************************************************
void CHelpInfoDlgEx::CreateHelpInfoBox(CWnd* pWnd, const int32_t InfoId, const int32_t captionId, const int32_t textId)
{
	CRect aRect;
	auto* pDlgItem = pWnd->GetDlgItem(InfoId);
	ASSERT(pDlgItem);
	pDlgItem->GetWindowRect(aRect);
	aRect.OffsetRect(30, -50);
	CHelpInfoDlg::CreateInfo(pWnd, aRect.BottomRight(), captionId, textId);
}
