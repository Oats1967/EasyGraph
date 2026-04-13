//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VolRecipePage.h
///
/// @file   VolRecipePage.h
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
#include "BASE/types.h"
#include "DosePage.h"
#include "BASE/include/EncoderState.h"
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonAgitator.h"
#include "ButtonRefillRelease.h"
#include "ButtonProductList.h"


class CVolRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CVolRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_VOL_RECIPE };

	DECLARE_MESSAGE_MAP()
private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CGaugeMassflow			m_GaugeMassflow;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CGaugeRotSpeed			m_GaugeRotSpeed;
	CButtonRefillRelease	m_RefillRelease;
	CButtonLocalLine		m_aLocalMode;
	CButtonAgitator			m_Agitator;
	CButtonProductList		m_ProductListName;
	CButtonProductList		m_ProductListKey;


	const CBrush			c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;
	CString			m_szActRotSpeed;		// kann auch aus/run sein
	CString         m_szANNumber;

	float32_t			m_fPercentage;
	float32_t			m_fSetpoint;
	float32_t			m_fMassflow;
	float32_t			m_fDriveCommand;
	float32_t			m_fActRotSpeed;
	base::eEncoderState	m_sEncoderState;
	float32_t			m_fActTotalizer;
    BOOL				m_bLocalMode;
	BOOL				m_bRegenerat;


private:
	void SetControlStyle (void);
	void CheckQMaxfaktor (void);

	//void LoadLocalModeButtons(BOOL bLocalNode);

	BOOL OnNotifyEditSetpoint(void);
	BOOL OnNotifyEditANNumber(void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;
	BOOL OnKillActive() override;

public:
	CVolRecipePage();
	~CVolRecipePage() override = default;

	afx_msg void OnBnClickedVolTotalizerReset();
	afx_msg void OnBnClickedVolName();
	afx_msg void OnBnClickedRefillRelease();
	afx_msg void OnBnClickedVolQMNumber();
	afx_msg void OnStnClickedVolANnummer();
	afx_msg void OnStnClickedVolSetpoint();
	afx_msg void OnStnClickedVolLine();
	afx_msg void OnStnClickedVolRefill();
	afx_msg void OnClickAgitator();
	afx_msg void OnClickNameBt();
	afx_msg void OnClickKeyBt();
	afx_msg void OnClickToolTipMassflow();
	afx_msg void OnClickToolTipDriveCommand();
	afx_msg void OnClickToolTipRotation();
	afx_msg void OnClickedFeederTypeInfo();


	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

