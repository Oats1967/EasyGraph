//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IGauge
///
/// @file   IGauge.h
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

#include "BASE/types.h"
#include "EasyControl.h"

class IGauge
{
protected:
	CBCGPCircularGaugeCtrl		m_wndGauge;
	CBCGPNumericIndicatorImpl*  m_pNumeric;
	CWnd*						m_pParent;
	BOOL						m_bRedraw;

private:
	int32_t GetMaxTicks(const int32_t maxvalue) const;

protected:
	void OnSetTicks(const float32_t value);
	void CreateNumericField(const int32_t cells);
	void CreateLevelBar();
	void Init();
	void SetColorRanges(float32_t value, const float32_t (&ranges)[6]);

public:
	IGauge(CWnd* pWnd = nullptr) : m_wndGauge()
		, m_pNumeric(nullptr)
		, m_pParent { pWnd }
		, m_bRedraw{ FALSE }
	{}

	virtual void Update() = 0;


	CBCGPCircularGaugeCtrl& GetCtrl()
	{ return m_wndGauge;	}

	 operator CBCGPCircularGaugeCtrl& ()
	 {	return m_wndGauge;	}

};
//******************************************************************************************************
//******************************************************************************************************
inline int32_t IGauge::GetMaxTicks(const int32_t maxvalue) const
{
	int32_t count = maxvalue / 5;
	int32_t maxticks = 0;
	if (count <= 5)
		maxticks = 5;
	else if (count <= 10)
		maxticks = 10;
	else if (count <= 20)
		maxticks = 20;
	else if (count <= 50)
		maxticks = 50;
	else if (count <= 100)
		maxticks = 100;
	else if (count <= 200)
		maxticks = 200;
	else if (count <= 500)
		maxticks = 500;
	else maxticks = 1000;
	return maxticks;
}
//***************************************************************************************
//***************************************************************************************
inline void IGauge::OnSetTicks(const float32_t value)
{
	auto* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->SetRange(0.0F, value);
	const int32_t majorstep = GetMaxTicks(_S32(value));
	const int32_t minorstep = majorstep / 5U;
	pGauge->SetMajorTickMarkStep(majorstep / minorstep);
	pGauge->SetMajorTickMarkStep((minorstep > 0) ? _F32(majorstep / minorstep) : 1.0F);
	pGauge->SetStep(minorstep);
}

//***************************************************************************************
//***************************************************************************************
inline void IGauge::CreateNumericField(const int32_t cells)
{
	ASSERT(!m_pNumeric);
	m_pNumeric = new CBCGPNumericIndicatorImpl;
	m_pNumeric->SetCells(cells);
	m_pNumeric->SetDecimals(0);
	m_pNumeric->SetValue(0.0F);
	m_pNumeric->SetVisualManagerTheme();

	const double dblWidth  =  globalUtils.ScaleByDPI(cells * 12, m_pParent);
	const double dblHeight = globalUtils.ScaleByDPI(15., m_pParent);

	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->AddSubGauge(m_pNumeric, CBCGPGaugeImpl::BCGP_SUB_GAUGE_CENTER, CBCGPSize(dblWidth, dblHeight), CBCGPPoint(0, 40.));
}
//***************************************************************************************
//***************************************************************************************
inline void IGauge::CreateLevelBar()
{
	const double width = 5.;
	const double yOffsetFromFrame = 10.0;
	const int nScale = 0;

	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	(void)pGauge->AddLevelBar(CBCGPBrush(CBCGPColor::Gainsboro, CBCGPColor::White, CBCGPBrush::BCGP_GRADIENT_HORIZONTAL, .2),
							  CBCGPBrush(CBCGPColor::Silver),
							  CBCGPBrush(RGB(0, 200, 0)),
							  nScale, 
							  width, 
							 yOffsetFromFrame);
}
//***************************************************************************************
//***************************************************************************************
inline void IGauge::Init()
{
	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->RemoveAllColoredRanges();
	pGauge->RemoveAllLevelBars();
	pGauge->RemoveAllSubGauges();
	pGauge->SetColors(CBCGPCircularGaugeColors::BCGP_CIRCULAR_GAUGE_BLUE);
	pGauge->EnableEquidistantLabels();
	pGauge->SetFrameSize(3);
	pGauge->SetCapSize(4);
	pGauge->SetTextLabelFormat(_T("%.0f"));
	pGauge->SetTickMarkSize(11, TRUE);
	pGauge->SetScaleOffsetFromFrame(0);
}
//***************************************************************************************
//***************************************************************************************
inline void IGauge::SetColorRanges(float32_t value, const float32_t(&c_ranges)[6])
{
	const double width1 = 1.;
	const double width2 = 8.0;
	const double opacity = .5;
	const int nScale = 0;


	CBCGPBrush brFrame;	// empty, no frame
	CBCGPBrush brGreen(CBCGPColor::White, CBCGPColor(CBCGPColor::Green, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
	CBCGPBrush brYellow(CBCGPColor::Gold);
	CBCGPBrush brRed(CBCGPColor(CBCGPColor::White, opacity), CBCGPColor::DarkRed, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);

	auto* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	pGauge->RemoveAllColoredRanges();
	const float32_t colorRanges[6] = { c_ranges[0] * value,
									   c_ranges[1] * value,
									   c_ranges[2] * value,
									   c_ranges[3] * value,
									   c_ranges[4] * value,
									   c_ranges[5] * value };

	pGauge->AddColoredRange(colorRanges[0], colorRanges[1], brRed, brFrame, nScale, width1, width2);
	pGauge->AddColoredRange(colorRanges[1], colorRanges[2], brYellow, brFrame, nScale, width2);
	pGauge->AddColoredRange(colorRanges[2], colorRanges[3], brGreen, brFrame, nScale, width2);
	pGauge->AddColoredRange(colorRanges[3], colorRanges[4], brYellow, brFrame, nScale, width2);
	pGauge->AddColoredRange(colorRanges[4], colorRanges[5], brRed, brFrame, nScale, width2, width1);
}



