//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GaugeTotalizer
///
/// @file   GaugeTotalizer.h
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


class CGaugeTotalizer : public IGauge
{
	float32_t m_fSetpoint;
	float32_t m_fBatchTotalizer;
	float32_t m_fBatchMaxSetpoint;

private:
	CGaugeTotalizer() = delete;
	void Init();

public:
	CGaugeTotalizer(CWnd* pParent) : IGauge { pParent }
		, m_fSetpoint{ 0.0F }
		, m_fBatchTotalizer{ 0.0F }
		, m_fBatchMaxSetpoint{ 0.0F }
	{
		Init();
	}

	void SetSetpoint(const float32_t value);

	void SetBatchMaxSetpoint(const float32_t value);

	void SetBatchTotalizer(const float32_t value);

	void Update() override;;
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeTotalizer::SetBatchMaxSetpoint(const float32_t value)
{
	if (value != m_fBatchMaxSetpoint)
	{
		m_fBatchMaxSetpoint = value;
		OnSetTicks(m_fBatchMaxSetpoint);
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeTotalizer::SetSetpoint(const float32_t value)
{
	if (value != m_fSetpoint)
	{
		m_fSetpoint = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeTotalizer::SetBatchTotalizer(const float32_t value)
{
	if (value != m_fBatchTotalizer)
	{
		m_fBatchTotalizer = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeTotalizer::Update()
{
	if (m_bRedraw)
	{
		m_bRedraw = FALSE;
		if (m_pNumeric)
		{
			m_pNumeric->SetValue(m_fBatchTotalizer);
		}
		auto* pGauge = m_wndGauge.GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetValue(m_fBatchTotalizer, 0,0, TRUE);
		pGauge->SetLevelBarValue(m_fSetpoint, 0, 0, TRUE);
	}
}
//***************************************************************************************
//***************************************************************************************
inline void CGaugeTotalizer::Init()
{
	IGauge::Init();

	CreateLevelBar();
	CreateNumericField(3);

	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_KG, NULL, TRUE), CBCGPGaugeImpl::BCGP_SUB_GAUGE_CENTER, CBCGPSize(), CBCGPPoint(0, 24.));

	m_fSetpoint = 0.0F;
	m_fBatchTotalizer		= 0.0F;
	m_fBatchMaxSetpoint	= 0.0F;
	m_bRedraw = FALSE;
}




