//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbfMotorPage.h
///
/// @file   WbfMotorPage.h
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
#include "InfoButton.h"
#include "MoreLessButton.h"


class CWbfMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CWbfMotorPage)

	enum eParamterOption {
		eNone = 0,
		eStandard,
		eExtended,
	};


	enum { IDD = IDD_WBF_MOTOR };

private:
	CMoreLessButton m_MoreLessButton;
	CInfoButton		m_PidIntegralGainInfo;
	CInfoButton		m_PidMassflowFilterInfo;
	CInfoButton		m_PidSampleIntervalInfoButton;
	CInfoButton		m_ReductionInfoButton;
	CInfoButton		m_BeltLoadSetpointInfoButton;
	CInfoButton		m_WheelSizeInfoButton;
	CInfoButton		m_WeighingLineInfoButton;
	CInfoButton		m_WbfBeltLoadVolInfoButton;
	CInfoButton		m_MaxRotationalSpeedInfoButton;
	CInfoButton		m_MaxSetpointInfoButton;
	CInfoButton		m_StartupDelayInfoButton;
	CInfoButton		m_MinDriveCommandInfoButton;

	CString		m_szWeighingLineName;
	uint32_t	m_lStartupDelay;
	float32_t	m_fPropVerstaerkung;
	float32_t	m_fInputFilter;
	float32_t	m_fMaxRotationalSpeed;
	float32_t	m_fMaxSetpoint;
	float32_t	m_fSampleInterval;
	float32_t	m_fReduction;
	float32_t	m_fBeltLoadSetpoint;
	uint32_t	m_lWheelSize;
	uint32_t	m_lWeighingLine;
	float32_t	m_fRollenumfang;
	float32_t	m_fWbfBeltLoadVol;
	float32_t	m_fMinDriveCommand;
	eParamterOption	m_ParameterOption;
	eParamterOption	m_ParameterOptionLast;
	BOOL		m_bVerwiegungFull;
	BOOL		m_bRunMode;
	BOOL		m_bInit;
	BOOL		m_bLogin;

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
	void SetControlStyle(void);
	void ShowExtendedStyle(void);
	void ShowStandardStyle(void);
	BOOL OnNotifyMassflowFilter();
	BOOL OnNotifyPidPropGainGross();
	BOOL OnNotifyMaxRotationalSpeed();
	BOOL OnNotifyStartupDelay();
	BOOL OnNotifyReduction();
	BOOL OnNotifyBeltLoadSetpoint();
	BOOL OnNotifyBeltLoadWheelSize();
	BOOL OnNotifyWeighingLine();
	BOOL OnNotifyMaxSetpoint();
	BOOL OnNotifySampleInterval();
	BOOL OnNotifyBeltLoadVolSwitch();
	BOOL OnNotifyBeltLoadMinDriveCommand();


	//void InitMap();


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

	BOOL OnSetActive() override;

public:
	CWbfMotorPage();
	~CWbfMotorPage() override = default;
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
	LRESULT OnNotifyEdit(WPARAM , LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnWeighingChanged
	///
	///           This method ....
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnWbfWeighingChanged(WPARAM w, LPARAM);


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorReduction
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorReduction();



	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorBeltLoadSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorBeltLoadSetpoint();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorWheelSize
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorWheelSize();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorWeighingLine
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorWeighingLine();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorMassflowDamping
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorMassflowDamping();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorPropGain
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorPropGain();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorPidSampleInterval
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorPidSampleInterval();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorSwitchVol
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorSwitchVol();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorRotationalSpeed
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorRotationalSpeed();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorBezugsgroesse
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorBezugsgroesse();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorStartupDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorStartupDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfMotorMinDriveCommand
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfMotorMinDriveCommand();
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

	afx_msg void OnBnClickedWbfMotorMassflowFilterInfo();
	afx_msg void OnBnClickedWbfMotorPidSampleIntervalInfo();
	afx_msg void OnBnClickedWbfMotorPidPropGainInfo();
	afx_msg void OnBnClickedControlParamMoreLess();
	afx_msg void OnBnClickedWbfMotorReductionInfo();
	afx_msg void OnBnClickedWbfMotorBeltLoadSetpointInfo();
	afx_msg void OnBnClickedWbfMotorWheelSizeInfo();
	afx_msg void OnBnClickedWbfMotorWeighingLineInfo();

	afx_msg void OnBnClickedWbfMotorBeltLoadVolInf();
	afx_msg void OnBnClickedWbfMotorMaxRotationalSpeedInfo();
	afx_msg void OnBnClickedWbfMotorMaxSetpointInfo();
	afx_msg void OnBnClickedWbfMotorStartupDelayInfo();
	afx_msg void OnBnClickedWbfMotorMinDriveCommandInfo();
	afx_msg void OnBnClickedWbfMotorParameterExtended();
	afx_msg void OnBnClickedWbfMotorParameterStandard();
	DECLARE_MESSAGE_MAP()
};

