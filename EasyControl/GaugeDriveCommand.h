//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GaugeDriveCommand
///
/// @file   GaugeDriveCommand.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "IGauge.h"


class CGaugeDriveCommand : public IGauge
{
	const float32_t c_ColorRanges[6];
	float32_t m_fDriveCommand;

private:
	CGaugeDriveCommand() = delete;
	void Init();

public:
	explicit CGaugeDriveCommand(CWnd* pWnd) :IGauge{ pWnd }
		, c_ColorRanges{ 0, 3.5F / 100.0F, 5.0F / 100.0F, 75.0F / 100.0F, 90.0F / 100.0F, 1.0F }
		, m_fDriveCommand{ 0.0F }
	{
		Init();
	}

	void SetDriveCommand(const float32_t value);

	void Update() override;
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeDriveCommand::SetDriveCommand(const float32_t value)
{
	const float32_t epsilon = _F32(1e-3);

	if (_F32(fabs(value - m_fDriveCommand)) > epsilon)
	{
		m_fDriveCommand = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeDriveCommand::Update()
{
	if (m_bRedraw)
	{
		m_bRedraw = FALSE;
		if (m_pNumeric)
		{
			m_pNumeric->SetValue(m_fDriveCommand);
		}
		auto* pGauge = m_wndGauge.GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetValue(m_fDriveCommand, 0, 0, TRUE);
	}
}
//***************************************************************************************
//***************************************************************************************
inline void CGaugeDriveCommand::Init()
{
	IGauge::Init();

	const float32_t cMaxDriveCommand = 100.0F;

	SetColorRanges(cMaxDriveCommand, c_ColorRanges);
	OnSetTicks(cMaxDriveCommand);

	CreateNumericField(3);
	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_PERCENT, NULL, TRUE), CBCGPGaugeImpl::BCGP_SUB_GAUGE_CENTER, CBCGPSize(), CBCGPPoint(0, 24.));

	m_fDriveCommand = 0.0F;
	m_bRedraw		= TRUE;
}




