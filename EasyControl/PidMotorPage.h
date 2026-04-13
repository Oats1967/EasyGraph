//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module PidMotorPage.h
///
/// @file   PidMotorPage.h
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


// CPidMotorPage-Dialogfeld

class CPidMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CPidMotorPage)

	enum eParamterOption {
		eNone = 0,
		eStandard,
		eExtended,
		eExtendedEx,
	};

	// Dialogfelddaten
	enum { IDD = IDD_PID_MOTOR };

private:
	CMoreLessButton m_MoreLessButton;
	CInfoButton		m_PidPropGainFineInfo;
	CInfoButton		m_PidPropGainGrossInfo;
	CInfoButton		m_PidPropGainSwitchGrossFineInfo;
	CInfoButton		m_PidIntegralGainInfo;
	CInfoButton		m_PidMassflowFilterInfo;
	CInfoButton		m_PidDosePerformanceInfo;
	CInfoButton		m_PidSteepnessMassflowInfo;
	CInfoButton		m_MaxsetpointInfoButton;
	CInfoButton		m_PidSampleIntervalInfoButton;
	CInfoButton		m_StartupRampInfoButton;
	CInfoButton		m_StartupDelayInfoButton;


	float32_t	m_fPidPropGainFine;
	float32_t	m_fPidPropGainGross;
	float32_t	m_fPidPropGainSwitchGrossFine;
	float32_t	m_fPidIntegralGain;
	float32_t	m_fMassflowFilter;
	float32_t	m_fMaxSetpoint;
	float32_t	m_fSteepnessMassflow;
	uint32_t	m_lStartupDelay;
	uint32_t	m_lSampleInterval;
	uint32_t	m_lStartupRamp;
	float32_t	m_fDosePerformance;
	BOOL		m_bDriveCommandInv;
	BOOL		m_bDriveCommand;
	eParamterOption	m_ParameterOption;
	eParamterOption	m_ParameterOptionLast;
	BOOL		m_bLogin;
	BOOL		m_bRunMode;
	BOOL		m_bInit;

	const std::map <int32_t, std::function<BOOL()> > m_EditMap;
	static BOOL	g_ShowLess;
private:
	//------------------------------------------------------------------------------------
	///  @brief   SetControlStyle
	///
	///           This method updates the state of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetControlStyle	  ( void);
	BOOL OnNotifyPidPropGainFine(void);
	BOOL OnNotifyPidPropGainGross(void);
	BOOL OnNotifyPidIntegralGain(void);
	BOOL OnNotifyPidPropGainSwitchGrossFine(void);
	BOOL OnNotifyMassflowFilter(void);
	BOOL OnNotifyMaxSetpoint(void);
	BOOL OnNotifyMassflowSteepness(void);
	BOOL OnNotifyStartupDelay(void);
	BOOL OnNotifySampleInterval(void);
	BOOL OnNotifyStartupRamp(void);
	BOOL OnNotifyDosePerformance(void);
	void ShowExtendedStyle(void);
	void ShowExtendedExStyle(void);
	void ShowStandardStyle(void);


protected:
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

public:
	CPidMotorPage();
	~CPidMotorPage() override = default;

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
	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedPidMotorDriveCommandausgabeCheck
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedPidMotorDriveCommandausgabeCheck();
	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedPidMotorDriveCommandausgabeinvCheck
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedPidMotorDriveCommandausgabeinvCheck();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorPidPropGainFine
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorPidPropGainFine();


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorPidPropGainGross
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorPidPropGainGross();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorPidPropGainSwitchGrossFine
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorPidPropGainSwitchGrossFine();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorPidIntegralGain
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorPidIntegralGain();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorMassflowFilter
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorMassflowFilter();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorPidSampleInterval
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorPidSampleInterval();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorDosePerformance
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorDosePerformance();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorMaxSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorMaxSetpoint();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorSteepnessMassflow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorSteepnessMassflow();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorStartupDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorStartupDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidMotorStartupRamp
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidMotorStartupRamp();

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


	afx_msg  void OnBnClickedPidMotorPidPropGainFineInfo();
	afx_msg  void OnBnClickedPidMotorPidPropGainGrossInfo();
	afx_msg  void OnBnClickedPidMotorPidPropGainSwitchGrossFineInfo();
	afx_msg  void OnBnClickedPidMotorPidIntegralGainInfo();
	afx_msg  void OnBnClickedPidMotorMassflowFilterInfo();
	afx_msg  void OnBnClickedPidMotorPidSampleIntervalInfo();
	afx_msg  void OnBnClickedPidMotorDosePerformanceInfo();
	afx_msg  void OnBnClickedPidMotorMaxSetpointInfo();
	afx_msg  void OnBnClickedPidMotorSteepnessMassflowInfo();
	afx_msg  void OnBnClickedDoseMotorParameterExtended();
	afx_msg  void OnBnClickedDoseMotorParameterExtendedEx();
	afx_msg  void OnBnClickedDoseMotorParameterStandard();
	afx_msg  void OnBnClickedPidControlParamMoreLess();
	afx_msg  void OnBnClickedPidMotorStartupRampInfo();
	afx_msg  void OnBnClickedPidMotorStartupDelayInfo();

	DECLARE_MESSAGE_MAP()
};

