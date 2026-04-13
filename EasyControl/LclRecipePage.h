//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LclRecipePage
///
/// @file   LclRecipePage.h
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
#include "ButtonProductList.h"
#include "DosePage.h"



class CLclRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CLclRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_LCL_RECIPE };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CInfoButton				m_MinLevelInfoButton;
	CInfoButton				m_MaxLevelInfoButton;
	CButtonProductList		m_ProductListName;


	float32_t		m_fActWeight;
	float32_t		m_fMinLevel;
	float32_t		m_fMaxLevel;

private:
	void SetControlStyle (void);
	//void ShowScaleItems(BOOL bShow);
	BOOL OnNotifyEditMinLevel(void);
	BOOL OnNotifyEditMaxLevel(void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CLclRecipePage();
	~CLclRecipePage() override = default;


	afx_msg void OnBnClickedLclName();
	afx_msg void OnBnClickedLclNameBt();
	afx_msg void OnStnClickedScale();
	afx_msg void OnBnClickedLclMinLevelInfo();
	afx_msg void OnBnClickedLclMaxLevelInfo();
	afx_msg void OnStnClickedMinLevel();
	afx_msg void OnStnClickedMaxLevel();


	LRESULT OnNotifyEdit		(WPARAM , LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

