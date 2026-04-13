//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbmRecipePage.h
///
/// @file   WbmRecipePage.h
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

#include <array>
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "DosePage.h"

class CWbmRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CWbmRecipePage)

	enum { IDD = IDD_WBM_RECIPE };


private:
	CGaugeMassflow		m_GaugeMassflow;
	CGaugeRotSpeed		m_GaugeRotSpeed;
	CButtonLocalLine	m_aLocalMode;
	const CBrush				c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush,   _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;
	CString				m_szIstRotationalSpeed;

    BOOL				m_bRegenerat;
	float32_t			m_fMassflow;
	float32_t			m_fActRotSpeed;
	float32_t			m_fActTotalizer;
	float32_t			m_fAktVelocity;
	float32_t			m_fBandlastMassflow;
	float32_t			m_fMaxSetpoint;
	float32_t			m_fMaxRotationalSpeed;
	base::eEncoderState	m_sEncoderState;
    BOOL				m_bLocalMode;
	float32_t			m_fSetpoint;

private:
	void SetControlStyle (void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateControls
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnUpdateControls() override;

public:
	CWbmRecipePage();
	~CWbmRecipePage() override = default;

	LRESULT OnNotifyEdit		(WPARAM , LPARAM);
	afx_msg void OnStnClickedWbfRecipeName();
	afx_msg void OnStnClickedWbfRecipeQmnummer();
	afx_msg void OnBnClickedWbfRecipeLinie();
	afx_msg void OnBnClickedWbfRecipeDoseTotalizerReset();

	//------------------------------------------------------------------------------------
	///  @brief   OnCtlColor
	///
	///           This method is called by the framework when a child control is about to
	///           be drawn
	///
	///  @param[in] nCtlColor , UINT
	///  @param[in] pWnd , CWnd*
	///  @param[in] pDC , CDC*
	///  @return HBRUSH
	///
	//------------------------------------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

