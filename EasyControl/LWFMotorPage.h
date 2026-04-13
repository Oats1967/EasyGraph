  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfMotorPage.h
///
/// @file   LwfMotorPage.h
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
#include "MoreLessButton.h"
#include "InfoButton.h"
#include "DosePage.h"



class CLwfMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CLwfMotorPage)

	enum eParamterOption {
		eNone = 0,
		eStandard,
		eExtended,
		eExtendedEx
	};


	enum { IDD = IDD_LWF_MOTOR };
private:
	CMoreLessButton m_MoreLessButton;
	CInfoButton		m_SampleIntervalInfoButton;
	CInfoButton		m_DosePerformanceInfoButton;
	CInfoButton		m_StartupdelayInfoButton;
	CInfoButton		m_MaxrotspeedInfoButton;
	CInfoButton		m_MaxdrivecommandInfoButton;
	CInfoButton		m_MassflowfilterInfoButton;
	CInfoButton		m_GatefilterInfoButton;
	CInfoButton		m_GainInfoButton;
	CInfoButton		m_EncodermonitorInfoButton;
	CInfoButton		m_MaxsetpointInfoButton;
	CInfoButton		m_MinSetpointChangeInfoButton;

	uint32_t	    m_lStartupDelay;
	float32_t	    m_fPIDGrobVerstaerkung;
	float32_t	    m_fMaxRotationalSpeed;
	uint32_t	    m_SampleInterval;
	float32_t	    m_fMaxSetpoint;
    float32_t       m_fMaxDriveCommandChange;
	uint32_t	    m_lEncoderMonitor;
	float32_t	    m_fGatefilter;
    float32_t       m_fMassflowfilter;
	float32_t		m_fDosePerformance;
	float32_t		m_fMinSetpointChange;

	eParamterOption	m_ParameterOption;
	eParamterOption	m_ParameterOptionLast;
	BOOL			m_bRunMode;
	BOOL			m_bInit;
	BOOL			m_bLogin;
	BOOL			m_bMaxRotationalSpeedAllowed;


	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	static BOOL	g_ShowLess;

private:
	//------------------------------------------------------------------------------------
	///  @brief   SetControlStyle
	///
	///           This method updates the state of the controls
	///
	///  @author  Detlef Hafer
	///
	///  @class        CLwfMotorPage
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetControlStyle(void);
	void ShowExtendedStyle(void);
	void ShowExtendedExStyle(void);

	void ShowStandardStyle(void);

	BOOL OnNotifyPidPropGainGross();
	BOOL OnNotifySampleInterval();
	BOOL OnNotifyDosePerformance();
	BOOL OnNotifyMaxRotationalSpeed();
	BOOL OnNotifyMaxMaxSetpoint();
	BOOL OnNotifyEncoderMonitor();
	BOOL OnNotifyStartupDelay();
	BOOL OnNotifyGatefilter();
	BOOL OnNotifyMassflowfilter();
	BOOL OnNotifyMinSetpointChange();
	BOOL OnNotifyMaxDriveCommandChange();


protected:
	//------------------------------------------------------------------------------------
	///  @brief   DoDataExchange
	///
	///           Called by the framework to exchange data
	///
	///  @param[in] pDX , CDataExchange*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoDataExchange(CDataExchange* pDX) override;

	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateControls
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnUpdateControls(void) override;


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
	BOOL OnSetActive()override;


public:
	CLwfMotorPage();
	~CLwfMotorPage() override = default;

	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

	afx_msg void OnBnClickedMotorCalibrate();
	afx_msg void OnBnClickedControlParamMoreLess();
	afx_msg void OnStnClickedMotorGatefilter();
	afx_msg void OnStnClickedMotorPidSampleInterval();
	afx_msg void OnStnClickedMotorGain();
	afx_msg void OnStnClickedMotorDosePerformance();
	afx_msg void OnStnClickedMotorMaxSetpoint();
	afx_msg void OnStnClickedMotorMaxRotSpeed();
	afx_msg void OnStnClickedMotorEncoderMonitor();
	afx_msg void OnStnClickedMotorStartupDelay();
	afx_msg void OnStnClickedMotorMassflowfilter();
	afx_msg void OnStnClickedMotorMaxDriveCommand();
	afx_msg void OnStnClickedMotorMinSetpointChange();
	afx_msg void OnBnClickedMotorPidSampleIntervalInfo();
	afx_msg void OnClickedDoseMotorDoseperformanceInfo();
	afx_msg void OnClickedDoseMotorMaxsetpointInfo();
	afx_msg void OnClickedDoseMotorEncodermonitorInfo();
	afx_msg void OnClickedDoseMotorGainInfo();
	afx_msg void OnClickedDoseMotorGatefilterInfo();
	afx_msg void OnClickedDoseMotorMassflowfilterInfo();
	afx_msg void OnClickedDoseMotorMaxdrivecommandInfo();
	afx_msg void OnClickedDoseMotorMaxrotspeedInfo();
	afx_msg void OnClickedDoseMotorStartupdelayInfo();
	afx_msg void OnClickedDoseMotorMinSetpointChangeInfo();
	afx_msg void OnBnClickedDoseMotorParameterExtendedEx();
	afx_msg void OnBnClickedDoseMotorParameterExtended();
	afx_msg void OnBnClickedDoseMotorParameterStandard();

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

