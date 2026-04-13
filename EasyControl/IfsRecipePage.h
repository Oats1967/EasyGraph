//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfsRecipePage
///
/// @file   IfsRecipePage.h
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
#include "ButtonOneScrewSpeed.h"
#include "ButtonProductList.h"
#include "ButtonLocalLine.h"
#include "DosePage.h"



class CIfsRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CIfsRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_IFS_RECIPE };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CInfoButton				m_GainFactorInfoButton;
	CInfoButton				m_ReduceFactorInfoButton;
	CInfoButton				m_StepTimeGainInfoButton;
	CInfoButton				m_StepTimeReduceInfoButton;
	CInfoButton				m_SetpointOverflowInfoButton;
	CButtonProductList		m_ProductListName;


	float32_t  m_GainFactor;
	float32_t  m_ReduceFactor;
	uint32_t  m_StepTimeGain;
	uint32_t  m_StepTimeReduce;
	float32_t  m_SetpointOverflow;


private:
	void SetControlStyle (void);
	BOOL OnNotifyEditGainFactor();
	BOOL OnNotifyEditReduceFactor();
	BOOL OnNotifyStepTimeGain();
	BOOL OnNotifyStepTimeReduce();
	BOOL OnNotifySetpointOverflowInfo();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CIfsRecipePage();
	~CIfsRecipePage() override = default;

	afx_msg void OnStnClickedGainFactor();
	afx_msg void OnStnClickedReduceFactor();
	afx_msg void OnStnClickedIfsName();
	afx_msg void OnStnClickedIfsStepTimeGain();
	afx_msg void OnStnClickedIfsStepTimeReduce();
	afx_msg void OnStnClickedIfsSetpointOverflow();
	afx_msg void OnStnClickedIfsSetting();

	afx_msg void OnBnClickedGainFactorInfo();
	afx_msg void OnBnClickedReduceFactorInfo();
	afx_msg void OnBnClickedIfsNameBt();
	afx_msg void OnBnClickedIfsStepTimeGainInfo();
	afx_msg void OnBnClickedIfsStepTimeReduceInfo();
	afx_msg void OnBnClickedIfsSetpointOverflowInfo();

	LRESULT OnNotifyEdit	(WPARAM , LPARAM);
	LRESULT OnIfsSettingClosed		(WPARAM, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

