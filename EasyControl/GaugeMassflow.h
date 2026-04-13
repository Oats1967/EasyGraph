//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GaugeMassflow
///
/// @file   GaugeMassflow.h
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


class CGaugeMassflow : public IGauge
{
	float32_t m_fSetpoint;
	float32_t m_fMassflow;
	float32_t m_fMaxSetpoint;

private:
	CGaugeMassflow() = delete;
	void Init();

public:
	CGaugeMassflow(CWnd* pParent) : IGauge { pParent }
		, m_fSetpoint{ 0.0F }
		, m_fMassflow{ 0.0F }
		, m_fMaxSetpoint{ 0.0F }
	{
		Init();
	}

	void SetSetpoint(const float32_t value);

	void SetMaxSetpoint(const float32_t value);

	void SetMassflow(const float32_t value);

	void Update() override;;
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeMassflow::SetMaxSetpoint(const float32_t value)
{
	if (value != m_fMaxSetpoint)
	{
		m_fMaxSetpoint = value;
		OnSetTicks(m_fMaxSetpoint);
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeMassflow::SetSetpoint(const float32_t value)
{
	if (value != m_fSetpoint)
	{
		m_fSetpoint = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeMassflow::SetMassflow(const float32_t value)
{
	if (value != m_fMassflow)
	{
		m_fMassflow = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CGaugeMassflow::Update()
{
	if (m_bRedraw)
	{
		m_bRedraw = FALSE;
		if (m_pNumeric)
		{
			m_pNumeric->SetValue(m_fMassflow);
		}
		auto* pGauge = m_wndGauge.GetGauge();
		ASSERT_VALID(pGauge);
		pGauge->SetValue(m_fMassflow, 0,0, TRUE);
		pGauge->SetLevelBarValue(m_fSetpoint, 0, 0, TRUE);
	}
}
//***************************************************************************************
//***************************************************************************************
inline void CGaugeMassflow::Init()
{
	IGauge::Init();

	CreateLevelBar();
	CreateNumericField(3);

	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_KGH, NULL, TRUE), CBCGPGaugeImpl::BCGP_SUB_GAUGE_CENTER, CBCGPSize(), CBCGPPoint(0, 24.));

	m_fSetpoint		= 0.0F;
	m_fMassflow		= 0.0F;
	m_fMaxSetpoint	= 0.0F;
	m_bRedraw = FALSE;
}




