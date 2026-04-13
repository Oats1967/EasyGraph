//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonStartStop
///
/// @file   ButtonStartStop.h
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


class CButtonStartStop : public CBCGPButton
{
public:
	enum class eState : uint32_t
	{
		eInit = 0,
		eStarted,
		eStopped
	};

private:
	eState   m_eState;
	CString  m_startText;
	CString  m_stopText;

public:
	CButtonStartStop(const uint32_t startText = IDS_BT_START, const uint32_t stopText = IDS_BT_STOP) : CBCGPButton()
		, m_eState(eState::eInit)
		, m_startText("")
		, m_stopText("")
	{
		VERIFY(m_startText.LoadString(startText));
		VERIFY(m_stopText.LoadString(stopText));
	}
	void Init(BOOL bStart)
	{
		m_bIsLeftText = FALSE;
		m_bTopImage = FALSE;
		m_bVisualManagerStyle = TRUE;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_3D;


		SetState((bStart) ? eState::eStarted : eState::eStopped);
	}

	void SetState(const eState newstate)
	{
		if (m_eState != newstate)
		{
			m_eState = newstate;
			switch (m_eState)
			{
				case eState::eStarted:
					SetImage(IDB_LINESTOP);
					SetWindowText(m_stopText);
					RedrawWindow();
					break;
				case eState::eStopped:
					SetImage(IDB_LINESTART);
					SetWindowText(m_startText);
					RedrawWindow();
					break;
				default:
					break;
			}
		}
	}
	BOOL isStarted() const
	{ return m_eState==eState::eStarted; }
};
