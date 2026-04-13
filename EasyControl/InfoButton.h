  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module InfoButton.h
///
/// @file   InfoButton.h
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


class CInfoButton : public CBCGPButton
{
public:
	CInfoButton()
	{
		m_bRighImage = FALSE;
		m_bTopImage = FALSE;
		//m_bTransparent = TRUE;
		m_bDrawText = FALSE;
		m_pToolTip = nullptr;
		m_nAlignStyle = CBCGPButton::ALIGN_CENTER;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
		m_bVisualManagerStyle = FALSE;
		SetImage(IDB_HELPINFO, IDB_HELPINFO, IDB_HELPINFO, IDB_HELPINFO);
	}
};

