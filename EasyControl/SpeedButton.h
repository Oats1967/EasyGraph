//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SpeedButton
///
/// @file   SpeedButton.h
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

class CSpeedButton : public CBCGPButton
{
private:
	int32_t m_Speed;

public:
	CSpeedButton(uint32_t up, uint32_t down, uint32_t disable) : CBCGPButton(), m_Speed{ 0 }
	{
		m_bIsLeftText = FALSE;
		m_bTopImage = TRUE;
		m_bDrawText = TRUE;
		m_bVisualManagerStyle = TRUE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_3D;

		SetImage(up, down, disable);
	}

	void Redraw()
	{
		auto save = _F32(m_Speed);
		m_Speed = -1;
		SetSpeed(save);
		SizeToContent();
		RedrawWindow();
	}

	void SetSpeed(float32_t fSpeed)
	{
		auto iSpeed = NEXTLONG(fSpeed);
		if (iSpeed != m_Speed)
		{
			m_Speed = iSpeed;
			CString szTemp;
			szTemp.Format(_T("%d %%"), m_Speed);
			SetWindowText(szTemp);
		}
	}
};
