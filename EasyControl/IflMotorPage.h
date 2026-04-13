//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IflMotorPage
///
/// @file   IflMotorPage.h
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
#include "DosePage.h"



class CIflMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CIflMotorPage)


// Dialogfelddaten
	enum { IDD = IDD_IFL_MOTOR };

	DECLARE_MESSAGE_MAP()

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	CInfoButton		m_SampleIntervalInfoButton;
	CInfoButton		m_PidGainInfoButton;

	uint32_t	    m_SampleInterval;
	float32_t	    m_fPidGain;

private:
	void SetControlStyle (void);
	BOOL OnNotifyEditPidGain();
	BOOL OnNotifyEditSampleTime();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;

public:
	CIflMotorPage();
	~CIflMotorPage() override = default;

	afx_msg void OnBnClickedPidGainInfo();
	afx_msg void OnBnClickedSampleTimeInfo();
	afx_msg void OnStnClickedSampleTime();
	afx_msg void OnStnClickedPidGain();


	LRESULT OnNotifyEdit	(WPARAM w, LPARAM);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

