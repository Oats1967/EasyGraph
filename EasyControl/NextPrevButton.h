//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module NextPrev.h
///
/// @file   SettingsLayoutPage.h
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

class CNextButton : public CBCGPButton
{
public:
	CNextButton()
	{
		m_bRighImage = FALSE;
		m_bTopImage = TRUE;
		//m_bTransparent = TRUE;
		m_bDrawText = FALSE;
		m_nAlignStyle = CBCGPButton::ALIGN_CENTER;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
		m_bVisualManagerStyle = FALSE;
		SetImage(IDB_BTNEXTUP, IDB_BTNEXTUP, IDB_BTNEXTDISABLE, IDB_BTNEXTDOWN);
	}
};

class CPreviousButton : public CBCGPButton
{
public:
	CPreviousButton()
	{
		m_bRighImage = FALSE;
		m_bTopImage = TRUE;
		//m_bTransparent = TRUE;
		m_bDrawText = FALSE;
		m_nAlignStyle = CBCGPButton::ALIGN_CENTER;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
		m_bVisualManagerStyle = FALSE;
		SetImage(IDB_BTPREVIOUSUP, IDB_BTPREVIOUSUP, IDB_BTPREVIOUSDISABLE, IDB_BTPREVIOUSDOWN);
	}
};
