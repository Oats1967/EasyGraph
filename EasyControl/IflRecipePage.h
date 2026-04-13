//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IflRecipePage
///
/// @file   IflRecipePage.h
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
#include "InfoButton.h"
#include "ButtonLocalLine.h"
#include "ButtonProductList.h"
#include "MoreLessButton.h"
#include "DosePage.h"



class CIflRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CIflRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_IFL_RECIPE };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CButtonLocalLine		m_aLocalMode;
	CInfoButton				m_MinLevelInfoButton;
	CInfoButton				m_MaxLevelInfoButton;
	CInfoButton				m_HysteresisInfoButton;
	CInfoButton				m_MinLineSetpointInfoButton;
	CInfoButton				m_MaxLineSetpointInfoButton;
	CInfoButton				m_LineSetpointInfoButton;
	CButtonProductList		m_ProductListName;
	CMoreLessButton			m_MoreLessButton;



	float32_t		m_fActWeight;
	float32_t		m_fMinLevel;
	float32_t		m_fMaxLevel;
	float32_t		m_fHysteresis;
	float32_t		m_fMinLineSetpoint;
	float32_t		m_fMaxLineSetpoint;
	float32_t		m_fLineSetpoint;

	static BOOL g_ShowLess;

private:
	void SetControlStyle (void);
	BOOL OnNotifyEditMinLevel();
	BOOL OnNotifyEditMaxLevel();
	BOOL OnNotifyEditHysteresis();
	BOOL OnNotifyEditMinLineSetpoint();
	BOOL OnNotifyEditMaxLineSetpoint();
	BOOL OnNotifyEditLineSetpoint();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CIflRecipePage();
	~CIflRecipePage() override = default;

	afx_msg void OnStnClickedIflName();
	afx_msg void OnBnClickedIflNameBt();
	afx_msg void OnBnClickedIflLinie();

	afx_msg void OnStnClickedScale();
	afx_msg void OnBnClickedMinLevelInfo();
	afx_msg void OnBnClickedMaxLevelInfo();
	afx_msg void OnBnClickedHysteresisInfo();
	afx_msg void OnBnClickedMinLineSetpointInfo();
	afx_msg void OnBnClickedMaxLineSetpointInfo();
	afx_msg void OnBnClickedLineSetpointInfo();

	afx_msg void OnStnClickedMinLevel();
	afx_msg void OnStnClickedMaxLevel();
	afx_msg void OnStnClickedHysteresis();
	afx_msg void OnStnClickedMinLineSetpoint();
	afx_msg void OnStnClickedMaxLineSetpoint();
	afx_msg void OnStnClickedLineSetpoint();
	afx_msg void OnBnClickedControlParamMoreLess();


	LRESULT OnNotifyEdit	(WPARAM w, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

