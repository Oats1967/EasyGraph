//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonSwitchCtrl
///
/// @file   ButtonSwitchCtrl.h
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


#include "types.h"


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CRadioButtonCtrl : public CBCGPButton
{
public:
	CRadioButtonCtrl(void) : CBCGPButton()
	{}

	~CRadioButtonCtrl() = default;

	void Init()
	{
		m_nFlatStyle = CBCGPButton::BUTTONSTYLE_SEMIFLAT;
		m_bResponseOnButtonDown = FALSE;
		m_bDrawFocus = FALSE;
		m_bHighlightChecked = FALSE;
		SetImage(IDB_RADIO_OFF32);
		SetCheckedImage(IDB_RADIO_ON32);
		SizeToContent();
		EnableWindow(FALSE);
	}
};
