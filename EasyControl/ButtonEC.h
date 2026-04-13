//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonEC
///
/// @file   ButtonEC.h
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

class CButtonEC : public CBCGPButton
{
public:
	using tBitmaps = std::array<std::array<int32_t, 3>, 2>;

protected:
	BOOL	 m_bEnable;
	BOOL	 m_bRedraw;

public:
	CButtonEC(const tBitmaps& rBitmaps) :
		  m_bEnable{ FALSE }
		, m_bRedraw{ TRUE }
	{
		SetImage(rBitmaps[0][0], rBitmaps[0][1], rBitmaps[0][2]);
		SetCheckedImage(rBitmaps[1][0], rBitmaps[1][1], rBitmaps[1][2]);
	}


	void Enable(const BOOL bEnable)
	{ m_bEnable = bEnable;	}

	BOOL IsEnable(void) const
	{ return m_bEnable;	}

	void Redraw()
	{ m_bRedraw = TRUE;	}

	void Show();
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CButtonEC::Show()
{
	if (m_bRedraw)
	{
		m_bRedraw = FALSE;
		SizeToContent();
	}
}


