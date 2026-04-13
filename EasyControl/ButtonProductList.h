//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonProductList
///
/// @file   ButtonProductList.h
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

class CButtonProductList : public CBCGPButton
{
public:
	CButtonProductList() : CBCGPButton()
	{
		m_bIsLeftText = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_3D;

		SetImage(IDB_ARROW_RIGHT, IDB_ARROW_RIGHT, IDB_ARROW_RIGHT);
	}

	void Redraw()
	{
		SizeToContent();
		RedrawWindow();
	}
};
