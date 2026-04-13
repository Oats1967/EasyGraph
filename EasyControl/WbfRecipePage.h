//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbfRecipePage.h
///
/// @file   WbfRecipePage.h
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

#include "BASE/include/EncoderState.h"
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "DosePage.h"

class CWbfRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CWbfRecipePage)

	enum { IDD = IDD_WBF_RECIPE };

private:
	CGaugeMassflow			m_GaugeMassflow;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CGaugeRotSpeed			m_GaugeRotSpeed;

	CButtonVolGrav			m_aVolGrav;
	CButtonLocalLine		m_aLocalMode;

	const CBrush			c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;

	float32_t		m_fMischung;
	float32_t		m_fSetpoint;
	float32_t		m_fMassflow;
	float32_t		m_fDriveCommand;
	float32_t		m_fActRotSpeed;
	float32_t		m_fActTotalizer;
	float32_t		m_fAktVelocity;
	float32_t		m_fBandlastMassflow;
	base::eEncoderState	m_sEncoderState;
	CString			m_szIstRotationalSpeed;
	BOOL            m_bRegenerat;

private:
	void CheckQMaxfaktor (void);
	BOOL OnNotifySetpoint();
	void SetControlStyle (void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CWbfRecipePage();
	~CWbfRecipePage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnNotifyEdit		(WPARAM , LPARAM);

	afx_msg void OnStnClickedWbfRecipeName();
	afx_msg void OnStnClickedWbfRecipeQmnumber();
	afx_msg void OnStnClickedWbfRecipeSetpoint();
	afx_msg void OnBnClickedWbfRecipeLine();
	afx_msg void OnBnClickedWbfVolumetric();
	afx_msg void OnBnClickedWbfTotalizerReset();

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

