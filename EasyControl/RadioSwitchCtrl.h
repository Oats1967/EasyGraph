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
class CButtonSwitchCtrl : public CBCGPSwitchCtrl
{
	BOOL	m_bInteractiveMode;
	int32_t m_id;

public:
	CButtonSwitchCtrl(int32_t id, BOOL bInteractiveMode = TRUE) : CBCGPSwitchCtrl()
		, m_bInteractiveMode(bInteractiveMode)
		, m_id(id)
	{
	}

	virtual ~CButtonSwitchCtrl()
	{
		// ExternalRelease is mandatory otherwise there will be memory leaks
		ExternalRelease();
	}

	CBCGPSwitchImpl* GetSwitch() override
	{
		if (m_pSwitch == NULL)
		{
			m_pSwitch = new CBCGPSwitchImpl;
			ASSERT_VALID(m_pSwitch);
			m_pSwitch->SetFocusRect();
			m_pSwitch->EnableOnOffLabels();
			m_pSwitch->SetStyle(CBCGPSwitchImpl::BCGP_SWITCH_CIRCLE);
			m_pSwitch->SetVisualManagerTheme();
			m_pSwitch->SetInteractiveMode(m_bInteractiveMode);
			m_pSwitch->SetID(m_id);
		}
		return m_pSwitch;
	}

	BOOL IsOn(void)
	{
		CBCGPSwitchImpl* pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		return pSwitch->IsOn();
	}


	void SetOn(BOOL b)
	{
		CBCGPSwitchImpl* pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		if (pSwitch->IsOn() != b)
		{
			pSwitch->SetOn(b);
			pSwitch->Invalidate();
		}
	}
	void SetColors(CBCGPSwitchColors& colors)
	{
		CBCGPSwitchImpl* pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		pSwitch->SetColors(colors);
	}

	void SetInteractiveMode(BOOL bInteractiveMode)
	{
		CBCGPSwitchImpl* pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		pSwitch->SetInteractiveMode(bInteractiveMode);
	}
};
