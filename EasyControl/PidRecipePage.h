//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module PidRecipePage.h
///
/// @file   PidRecipePage.h
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
#include "DosePage.h"
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "ButtonProductList.h"


class CPidRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CPidRecipePage)

	enum { IDD = IDD_PID_RECIPE };

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	const CBrush			c_aRegeneratBrush;
	CGaugeMassflow			m_GaugeMassflow;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CButtonVolGrav			m_aVolGrav;
	CButtonLocalLine		m_aLocalMode;
	CButtonProductList		m_ProductListName;
	CButtonProductList		m_ProductListKey;

    BOOL            m_bRegenerat;
	float32_t		m_fSetpoint;
	float32_t		m_fMassflow;
	float32_t		m_fDriveCommand;
	float32_t		m_fMischung;
	float32_t		m_fActTotalizer;
	float32_t		m_fActDoseperformance;
	CString         m_szANNumber;


private:
	void CheckDosePerformance(void);
	void SetControlStyle (void);
	BOOL OnNotifyEditSetpoint(void);
	BOOL OnNotifyEditANNumber(void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;

public:
	CPidRecipePage();
	~CPidRecipePage() override = default;

	afx_msg void OnBnClickedPidVolumetric();
	afx_msg void OnBnClickedPidTotalizerReset();
	afx_msg void OnStnClickedPidName();
	afx_msg void OnStnClickedPidQmNumber();
	afx_msg void OnStnClickedANnummer();
	afx_msg void OnStnClickedPidSetpoint();
	afx_msg void OnBnClickedPidLinie();

	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

	afx_msg void OnClickNameBt();
	afx_msg void OnClickKeyBt();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

