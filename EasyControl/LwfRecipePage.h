//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfRecipePage
///
/// @file   LwfRecipePage.h
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
#include <map>
#include <functional>
#include "DosePage.h"
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "ButtonAgitator.h"
#include "ButtonRefillRelease.h"
#include "ButtonProductList.h"
#include "BASE/include/EncoderState.h"



class CLwfRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CLwfRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_LWF_RECIPE };


	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CGaugeMassflow			m_GaugeMassflow;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CGaugeRotSpeed			m_GaugeRotSpeed;

	CButtonRefillRelease	m_RefillRelease;
	CButtonVolGrav			m_aVolGrav;
	CButtonLocalLine		m_aLocalMode;
	CButtonAgitator			m_Agitator;
	CButtonProductList		m_ProductListName;
	CButtonProductList		m_ProductListKey;

	const CBrush          c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;
	CString			m_szActRotSpeed;		// kann auch aus/run sein
	CString          m_szANNumber;

	float32_t		m_fPercentage;
	float32_t		m_fSetpoint;
	float32_t		m_fMassflow;
	float32_t		m_fDriveCommand;
	float32_t		m_fActRotSpeed;
	float32_t		m_fActWeight;
	base::eEncoderState m_sEncoderState;

	float32_t		m_fActTotalizer;
	float32_t		m_fActDoseperformance;
	BOOL            m_bRegenerat;

private:
	void SetControlStyle (void);
	void CheckQMaxfaktor (void);

	BOOL OnNotifyEditSetpoint(void);
	BOOL OnNotifyEditANNumber(void);



protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;

	//------------------------------------------------------------------------------------
	///  @brief   OnInitDialog
	///
	///           This method is called by the framework in response to WM_INITDIALOG message
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;
	BOOL OnKillActive() override;

public:
	CLwfRecipePage();
	~CLwfRecipePage() override = default;


	afx_msg void OnBnClickedDosiererVolumetric();
	afx_msg void OnBnClickedDosiererTotalizerReset();
	afx_msg void OnStnClickedDosiererName();
	afx_msg void OnStnClickedDosiererQmnummer();
	afx_msg void OnStnClickedDosiererANnummer();
	afx_msg void OnStnClickedDosiererSetpoint();
	afx_msg void OnStnClickedDosiererScale();
	afx_msg void OnStnClickedDosiererRefill();
	afx_msg void OnBnClickedDosiererLinie();
	afx_msg void OnBnClickedRefillRelease();


	LRESULT OnNotifyEdit		(WPARAM , LPARAM);
	LRESULT OnDestroyScaleBox   (WPARAM, LPARAM);

	afx_msg void OnClickToolTipMassflow();
	afx_msg void OnClickToolTipDriveCommand();
	afx_msg void OnClickToolTipRotation();
	afx_msg void OnClickAgitator();
	afx_msg void OnClickNameBt();
	afx_msg void OnClickKeyBt();
	afx_msg void OnClickedFeederTypeInfo();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

