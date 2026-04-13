//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcbRecipePage
///
/// @file   IcbRecipePage.h
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

#include <map>
#include <functional>
#include <array>
#include "DosePage.h"
#include "GaugeTotalizer.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "ButtonAgitator.h"
#include "BASE/include/EncoderState.h"



class CIcbRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CIcbRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_ICB_RECIPE };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	CGaugeTotalizer			m_GaugeTotalizer;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CGaugeRotSpeed			m_GaugeRotSpeed;

	CButtonLocalLine		m_aLocalMode;
	CButtonAgitator			m_Agitator;

	const CBrush          c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;
	CString			m_szActRotSpeed;		// kann auch aus/run sein

	float32_t		m_fPercentage;
	float32_t		m_fSetpoint;
	float32_t		m_fDriveCommand;
	float32_t		m_fActRotSpeed;
	base::eEncoderState m_sEncoderState;

	float32_t		m_fActTotalizer;
	BOOL            m_bRegenerat;

private:
	void SetControlStyle (void);
	BOOL OnNotifyEditSetpoint(void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CIcbRecipePage();
	~CIcbRecipePage() override = default;


	afx_msg void OnBnClickedDosiererTotalizerReset();
	afx_msg void OnStnClickedDosiererName();
	afx_msg void OnStnClickedDosiererQmnummer();
	afx_msg void OnStnClickedDosiererSetpoint();
	afx_msg void OnBnClickedDosiererLinie();

	afx_msg void OnClickToolTipTotalizer();
	afx_msg void OnClickToolTipDriveCommand();
	afx_msg void OnClickToolTipRotation();
	afx_msg void OnClickAgitator();

	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

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
};

