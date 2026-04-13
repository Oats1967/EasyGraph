//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GaugeRotSpeed
///
/// @file   GaugeRotSpeed.h
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


class CGaugeRotSpeed : public IGauge
{
	const float32_t c_ColorRanges[6];
	const float32_t divisior = 100.0F;

	float32_t		m_fRotSpeed;
	float32_t		m_fMaxRotSpeed;

private:
	float32_t DisplaySpeed(const float32_t speed);

	CGaugeRotSpeed() = delete;
	void Init();

public:
	CGaugeRotSpeed(CWnd* pWnd) : IGauge{ pWnd }
		, m_fRotSpeed(0.0F)
		, m_fMaxRotSpeed(0.0F)
		, c_ColorRanges{ 0, 3.5F / 100.0F, 5.0F / 100.0F, 75.0F / 100.0F, 90.0F / 100.0F, 1.0F }
	{
		Init();
	}

	void SetRotSpeed(const float32_t value);

	void SetMaxRotSpeed(const float32_t value);

	void Update() override;

};
//******************************************************************************************************
//******************************************************************************************************
inline  float32_t CGaugeRotSpeed::DisplaySpeed(const float32_t speed)
{
	return _F32(_S32((speed + (divisior / 2.0F)) / divisior));
}
//******************************************************************************************************
//******************************************************************************************************
inline void CGaugeRotSpeed::SetRotSpeed(const float32_t fRotSpeed)
{
	if (_S32(fRotSpeed) != _S32(m_fRotSpeed))
	{
		m_fRotSpeed = fRotSpeed;
		m_bRedraw = TRUE;
	}
}
//******************************************************************************************************
//******************************************************************************************************
inline void CGaugeRotSpeed::SetMaxRotSpeed(const float32_t value)
{
	auto fMaxRotSpeed = value; 
	if (_S32(fMaxRotSpeed) != _S32(m_fMaxRotSpeed))
	{
		m_fMaxRotSpeed = fMaxRotSpeed;
		float32_t display = DisplaySpeed(m_fMaxRotSpeed);
		OnSetTicks(display);
		SetColorRanges(display, c_ColorRanges);
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeRotSpeed::Update()
{
	if (m_bRedraw)
	{
		assert(m_fRotSpeed >= 0.0F);
		assert(m_fMaxRotSpeed >= 0.0F);

		m_bRedraw = FALSE;
		if (m_pNumeric)
		{
			m_pNumeric->SetValue(m_fRotSpeed);
		}
		auto* pGauge = m_wndGauge.GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetValue(DisplaySpeed(m_fRotSpeed),0,400, TRUE);
	}
}
//***************************************************************************************
//***************************************************************************************
inline void CGaugeRotSpeed::Init()
{
	IGauge::Init();

	CreateNumericField(4);
	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_UPM, NULL, TRUE), CBCGPGaugeImpl::BCGP_SUB_GAUGE_CENTER, CBCGPSize(), CBCGPPoint(0, 24.));

	m_fRotSpeed    = -1.0F;
	m_fMaxRotSpeed = -1.0F;
	m_bRedraw	   = FALSE;
}




