//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SpeedKnob
///
/// @file   SpeedKnob.h
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


class CSpeedKnob : public CBCGPKnobCtrl
{
	float32_t m_fSpeed;
	BOOL	  m_bRedraw;

private:
	CSpeedKnob() = delete;
	void Init();

public:
	CSpeedKnob(CWnd* pParent = nullptr) : CBCGPKnobCtrl()
		, m_fSpeed{ 0.0F }
		, m_bRedraw{ FALSE }
	{
		pParent = pParent;
		Init();
	}

	void SetSpeed(const float32_t value);

	void Update();
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CSpeedKnob::SetSpeed(const float32_t value)
{
	const float32_t epsilon = _F32(1e-03);

	if (_F32(fabs(value != m_fSpeed)) > epsilon)
	{
		m_fSpeed = value;
		m_bRedraw = TRUE;
	}
}
//*************************************************************************************************************
//*************************************************************************************************************
inline void CSpeedKnob::Update()
{
	if (m_bRedraw)
	{
		m_bRedraw = FALSE;
		CBCGPKnob* pKnob = GetKnob();
		ASSERT_VALID(pKnob);
		pKnob->SetValue(m_fSpeed, 0,0);
	}
}
//***************************************************************************************
//***************************************************************************************
inline void CSpeedKnob::Init()
{
	CBCGPKnob* pKnob = GetKnob();
	ASSERT_VALID(pKnob);

	pKnob->RemoveAllColoredRanges();
	pKnob->SetColors(CBCGPCircularGaugeColors::BCGP_CIRCULAR_GAUGE_COLOR_THEME::BCGP_CIRCULAR_GAUGE_BLUE);
	pKnob->SetPointer(CBCGPKnobPointer::BCGP_KNOB_POINTER_STYLE::BCGP_KNOB_POINTER_HANDLE);

	pKnob->SetTextLabelFormat(_T("%.0f"));

	const double dblWidth = 8.0;

	pKnob->SetTickMarkSize(dblWidth / 2.0, FALSE);
	pKnob->SetTickMarkSize(dblWidth, TRUE);

	{
		double opacity = .5;

		CBCGPBrush brFrame;	// empty, no frame
		CBCGPBrush brYellow(CBCGPColor::Yellow, CBCGPColor(CBCGPColor::White, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
		CBCGPBrush brRed(CBCGPColor::Red, CBCGPColor(CBCGPColor::White, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
		CBCGPBrush brGreen(CBCGPColor::DarkGreen, CBCGPColor(CBCGPColor::White, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);

		int nScale = 0;

		pKnob->AddColoredRange(0, 5, brRed, brFrame, nScale, dblWidth);
		pKnob->AddColoredRange(5, 10, brYellow, brFrame, nScale, dblWidth);
		pKnob->AddColoredRange(10, 90, brGreen, brFrame, nScale, dblWidth);
		pKnob->AddColoredRange(90, 95, brYellow, brFrame, nScale, dblWidth);
		pKnob->AddColoredRange(95, 100, brRed, brFrame, nScale, dblWidth);
	}

	pKnob->SetImageList(0);
	pKnob->SetRange(0, 100);
	pKnob->SetMajorTickMarkStep(5);
	pKnob->SetStep(2);
	pKnob->SetDirty();
	pKnob->Redraw();

	EnableTooltip(FALSE);
	m_bRedraw = FALSE;
}




