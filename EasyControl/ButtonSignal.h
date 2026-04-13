//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonSignal
///
/// @file   ButtonSignal.h
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

class CButtonSignal : public CBCGPButton
{
public:
	CButtonSignal() : CBCGPButton()
	{
		m_bIsLeftText = TRUE;
		//m_bTopImage = TRUE;
		m_bDrawText = TRUE;
		//m_bVisualManagerStyle = TRUE;
		m_nFlatStyle = CBCGPButton::BUTTONSTYLE_FLAT;
		//m_nAlignStyle = AlignStyle::ALIGN_CENTER;
		//rImageList.Set
		SetImage(IDB_RADIOOFF);
		SetCheckedImage(IDB_RADIOON);
	}

	void Redraw()
	{
		SizeToContent();
		RedrawWindow();
	}
};
