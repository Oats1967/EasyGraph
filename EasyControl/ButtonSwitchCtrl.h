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


#include "BASE/types.h"


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
	CButtonSwitchCtrl(const CButtonSwitchCtrl& rButton) : CBCGPSwitchCtrl()
		, m_bInteractiveMode { rButton.m_bInteractiveMode  }
		, m_id{ rButton.m_id }
	{}

	virtual ~CButtonSwitchCtrl()
	{
		// ExternalRelease is mandatory otherwise there will be memory leaks
		ExternalRelease();
	}

	CBCGPSwitchImpl* GetSwitch() override
	{
		BOOL bInit = (m_pSwitch == NULL);
		auto pSwitch = CBCGPSwitchCtrl::GetSwitch();
		if ( bInit)
		{
			pSwitch->EnableOnOffLabels();
			pSwitch->SetStyle(CBCGPSwitchImpl::BCGP_SWITCH_CIRCLE);
			pSwitch->SetVisualManagerTheme();
			pSwitch->SetInteractiveMode(m_bInteractiveMode);
			pSwitch->SetID(m_id);
			pSwitch->SetAnimationTime(0);

			CBCGPSwitchColors colorsHorz;
			colorsHorz.m_brFillOn = CBCGPBrush(CBCGPColor::LightSeaGreen, CBCGPColor::DarkGreen, CBCGPBrush::BCGP_GRADIENT_HORIZONTAL);
			colorsHorz.m_brFillOff = CBCGPBrush(CBCGPColor::Gainsboro, CBCGPColor::Silver, CBCGPBrush::BCGP_GRADIENT_HORIZONTAL);
			colorsHorz.m_brFillThumb = CBCGPBrush(CBCGPColor::LightGray, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_RADIAL_TOP_LEFT);
			colorsHorz.m_brLabelOff = CBCGPBrush(CBCGPColor::DarkRed);


			SetColors(colorsHorz);
		}
		return pSwitch;
	}

	BOOL IsOn(void)
	{
		auto pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		return pSwitch->IsOn();
	}


	void SetOn(BOOL b)
	{
		auto pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		if (pSwitch->IsOn() != b)
		{
			pSwitch->SetOn(b);
			pSwitch->Invalidate();
		}
	}
	void SetColors(CBCGPSwitchColors& colors)
	{
		auto pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		pSwitch->SetColors(colors);
	}

	void SetInteractiveMode(BOOL bInteractiveMode)
	{
		auto pSwitch = GetSwitch();
		ASSERT_VALID(pSwitch);
		pSwitch->SetInteractiveMode(bInteractiveMode);
	}
};
