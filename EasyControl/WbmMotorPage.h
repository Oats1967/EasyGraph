//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbmMotorPage.h
///
/// @file   WbmMotorPage.h
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

#include "DosePage.h"
#include "InfoButton.h"

class CWbmMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CWbmMotorPage)

// Dialogfelddaten
	enum { IDD = IDD_WBM_MOTOR };

private:
	CInfoButton		m_SampleIntervalInfoButton;
	CInfoButton		m_StartupdelayInfoButton;
	CInfoButton		m_MaxrotspeedInfoButton;
	CInfoButton		m_MassflowfilterInfoButton;
	CInfoButton		m_MaxsetpointInfoButton;
	CInfoButton		m_ReductionInfoButton;
	CInfoButton		m_BeltLoadSetpointInfoButton;
	CInfoButton		m_WheelSizeInfoButton;

	CString		m_szWeighingLineName;
	uint32_t	m_lStartupDelay;
	float32_t	m_fMaxRotationalSpeed;
	float32_t	m_fMaxSetpoint;
	float32_t	m_fInputFilter;
	float32_t	m_fSampleInterval;
	float32_t	m_fReduction;
	float32_t	m_fBeltLoadSetpoint;
	uint32_t	m_lWheelSize;
	uint32_t	m_lWeighingLine;
	float32_t	m_fRollenumfang;
	BOOL		m_bVerwiegungFull;

private:
	BOOL OnNotifyMassflowFilter(void);
	BOOL OnNotifyMaxRotationalSpeed(void);
	BOOL OnNotifyStartupDelay(void);
	BOOL OnNotifyReduction(void);
	BOOL OnNotifyBeltLoadSetpoint(void);
	BOOL OnNotifyBeltLoadWheelSize(void);
	BOOL OnNotifyWeighingLine(void);
	BOOL OnNotifyMaxSetpoint(void);
	BOOL OnNotifySampleInterval(void);

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
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateControls
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnUpdateControls() override;


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

public:
	CWbmMotorPage();
	~CWbmMotorPage() override = default;
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
	LRESULT OnWeighingChanged(WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorReduction
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorReduction();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorBeltLoadSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorBeltLoadSetpoint();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorWheelSize
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorWheelSize();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorWeighingLine
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorWeighingLine();



	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorRotationalSpeed
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorRotationalSpeed();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorBezugsgroesse
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorBezugsgroesse();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorStartupDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorStartupDelay();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorMassflowDamping
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorMassflowDamping();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMMotorPidSampleInterval
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMMotorPidSampleInterval();


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

	afx_msg void OnBnClickedWBMMotorPidSampleIntervalInfo();
	afx_msg void OnBnClickedWBMMotorMassflowFilterInfo();
	afx_msg void OnClickedWBMMotorMaxrotspeedInfo();
	afx_msg void OnClickedWBMMotorMaxsetpointInfo();
	afx_msg void OnClickedWBMMotorStartupdelayInfo();
	afx_msg void OnClickedWBMMotorReductionInfo();
	afx_msg void OnClickedWBMMotorBeltLoadSetpointInfo();
	afx_msg void OnClickedWBMMotorWheelSizeInfo();


	DECLARE_MESSAGE_MAP()
};

