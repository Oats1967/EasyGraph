  //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwbMotorPage.h
///
/// @file   LwbMotorPage.h
///
///
/// @coypright(c)  Ing.b�ro Hafer
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
#include "ButtonGrossStream.h"
#include "ButtonFineStream.h"
#include "DosePage.h"



class CLwbMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CLwbMotorPage)

	enum eParamterOption {
		eNone = 0,
		eStandard,
		eExtended,
		eExtendedEx
	};

	enum { IDD = IDD_LWB_MOTOR };
private:
	CMoreLessButton		m_MoreLessButton;
	CInfoButton			m_StartupdelayInfoButton;
	CInfoButton			m_MaxrotspeedInfoButton;
	CInfoButton			m_MaxsetpointInfoButton;
	CInfoButton			m_GrossStreamInfoButton;
	CInfoButton			m_FineStreamInfoButton;
	CInfoButton			m_SwitchpointInfoButton;
	CInfoButton			m_EncodermonitorInfoButton;
	CInfoButton			m_ScaleExtInfoButton;
	CInfoButton			m_BatchTimeInfoButton;
	CInfoButton			m_OverrunCompensationInfoButton;
	CButtonGrossStream	m_GrossSpeedCtrl;
	CButtonFineStream	m_FineSpeedCtrl;


	uint32_t	    m_lStartupDelay;
	float32_t	    m_fMaxRotationalSpeed;
	float32_t	    m_fMaxSetpoint;
	uint32_t	    m_lEncoderMonitor;
	float32_t		m_fSwitchpoint;
	float32_t		m_fGrossStream;
	float32_t		m_fFineStream;
	int32_t			m_lScaleExt;
	uint32_t		m_lBatchTime;
	float32_t		m_fOverrunCompensation;

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
	///  @class        CLwbMotorPage
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetControlStyle(void);
	void ShowExtendedStyle(void);

	void ShowStandardStyle(void);

	BOOL OnNotifyMaxRotationalSpeed();
	BOOL OnNotifyMaxMaxSetpoint();
	BOOL OnNotifyEncoderMonitor();
	BOOL OnNotifyStartupDelay();
	BOOL OnNotifyBatchGrossStream();
	BOOL OnNotifyBatchFineStream();
	BOOL OnNotifyBatchSwitchpoint();
	BOOL OnNotifyBatchScale();
	BOOL OnNotifyBatchTime();
	BOOL OnNotifyBatchOverrunCompensation();

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
	CLwbMotorPage();
	~CLwbMotorPage() override = default;

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
	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

	afx_msg void OnBnClickedControlParamMoreLess();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorMaxSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorMaxSetpoint();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorRotationalSpeed
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorMaxRotSpeed();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorEncoderMonitor
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorEncoderMonitor();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorStartupDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorStartupDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorGrossStream
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorGrossStream();
	afx_msg void OnStnClickedMotorFineStream();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedMotorSwitchpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedMotorSwitchpoint();
	afx_msg void OnStnClickedMotorScale();
	afx_msg void OnStnClickedMotorBatchTime();
	afx_msg void OnStnClickedMotorOverrunCompensation();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedMotorCalibrateExtended
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnClickedDoseMotorMaxsetpointInfo();
	afx_msg void OnClickedDoseMotorEncodermonitorInfo();
	afx_msg void OnClickedDoseMotorMaxrotspeedInfo();
	afx_msg void OnClickedDoseMotorStartupdelayInfo();
	afx_msg void OnClickedMotorGrossStreamInfo();
	afx_msg void OnClickedMotorFineStreamInfo();
	afx_msg void OnClickedMotorSwitchpointInfo();
	afx_msg void OnClickedDoseMotorScaleInfo();
	afx_msg void OnClickedDoseMotorOverrunCompensationInfo();
	afx_msg void OnClickedDoseMotorBatchtimeInfo();

	afx_msg void OnClickGrossStreamCtrl();
	afx_msg void OnClickFineStreamCtrl();

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

