//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfMotorPage
///
/// @file   LwfMotorPage.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "BASE/include/DoseType.h"
#include "BASE/Base.def"
#include "RemoteControl.h"
#include "LwfMotorPage.h"
#include "BASE/Utils/public/UserRightsList.h"
#include "GlobalConst.h"
#include "CalibrationDlg.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLwfMotorPage, _func)

BOOL CLwfMotorPage::g_ShowLess = TRUE;

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CLwfMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_LWF_MOTOR_CONTROLPARAMETER_MORELESS, &CLwfMotorPage::OnBnClickedControlParamMoreLess)
	ON_BN_CLICKED(IDC_LWF_MOTOR_CALIBRATE, &CLwfMotorPage::OnBnClickedMotorCalibrate)
	ON_STN_CLICKED(IDC_LWF_MOTOR_GATEFILTER, &CLwfMotorPage::OnStnClickedMotorGatefilter)
	ON_STN_CLICKED(IDC_LWF_MOTOR_PIDSAMPLEINTERVAL, &CLwfMotorPage::OnStnClickedMotorPidSampleInterval)
	ON_STN_CLICKED(IDC_LWF_MOTOR_GAIN, &CLwfMotorPage::OnStnClickedMotorGain)
	ON_STN_CLICKED(IDC_LWF_MOTOR_DOSEPERFORMANCE, &CLwfMotorPage::OnStnClickedMotorDosePerformance)
	ON_STN_CLICKED(IDC_LWF_MOTOR_MAXSETPOINT, &CLwfMotorPage::OnStnClickedMotorMaxSetpoint)
	ON_STN_CLICKED(IDC_LWF_MOTOR_MAXROTSPEED, &CLwfMotorPage::OnStnClickedMotorMaxRotSpeed)
	ON_STN_CLICKED(IDC_LWF_MOTOR_ENCODERMONITOR, &CLwfMotorPage::OnStnClickedMotorEncoderMonitor)
	ON_STN_CLICKED(IDC_LWF_MOTOR_STARTUPDELAY, &CLwfMotorPage::OnStnClickedMotorStartupDelay)
	ON_STN_CLICKED(IDC_LWF_MOTOR_MASSFLOWFILTER, &CLwfMotorPage::OnStnClickedMotorMassflowfilter)
	ON_STN_CLICKED(IDC_LWF_MOTOR_MAXDRIVECOMMAND, &CLwfMotorPage::OnStnClickedMotorMaxDriveCommand)
	ON_STN_CLICKED(IDC_LWF_MOTOR_MINSETPOINTCHANGE, &CLwfMotorPage::OnStnClickedMotorMinSetpointChange)

	ON_BN_CLICKED(IDC_LWF_MOTOR_PIDSAMPLEINTERVAL_INFO, OnBnClickedMotorPidSampleIntervalInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_DOSEPERFORMANCE_INFO, &CLwfMotorPage::OnClickedDoseMotorDoseperformanceInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_MAXSETPOINT_INFO, &CLwfMotorPage::OnClickedDoseMotorMaxsetpointInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_ENCODERMONITOR_INFO, &CLwfMotorPage::OnClickedDoseMotorEncodermonitorInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_MINSETPOINTCHANGE_INFO, &CLwfMotorPage::OnClickedDoseMotorMinSetpointChangeInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_GAIN_INFO, &CLwfMotorPage::OnClickedDoseMotorGainInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_GATEFILTER_INFO, &CLwfMotorPage::OnClickedDoseMotorGatefilterInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_MASSFLOWFILTER_INFO, &CLwfMotorPage::OnClickedDoseMotorMassflowfilterInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_MAXDRIVECOMMAND_INFO, &CLwfMotorPage::OnClickedDoseMotorMaxdrivecommandInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_MAXROTSPEED_INFO, &CLwfMotorPage::OnClickedDoseMotorMaxrotspeedInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_STARTUPDELAY_INFO, &CLwfMotorPage::OnClickedDoseMotorStartupdelayInfo)
	ON_BN_CLICKED(IDC_LWF_MOTOR_PARAMETER_EXTENDED, &CLwfMotorPage::OnBnClickedDoseMotorParameterExtended)
	ON_BN_CLICKED(IDC_LWF_MOTOR_PARAMETER_STANDARD, &CLwfMotorPage::OnBnClickedDoseMotorParameterStandard)
	ON_BN_CLICKED(IDC_LWF_MOTOR_PARAMETER_EXTENDEDEX, &CLwfMotorPage::OnBnClickedDoseMotorParameterExtendedEx)
	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CLwfMotorPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CLwfMotorPage::CLwfMotorPage() : CDosePage(CLwfMotorPage::IDD)
			, m_MoreLessButton()
			, m_SampleIntervalInfoButton()
			, m_DosePerformanceInfoButton()
			, m_StartupdelayInfoButton()
			, m_MaxrotspeedInfoButton()
			, m_MaxdrivecommandInfoButton()
			, m_MassflowfilterInfoButton()
			, m_GatefilterInfoButton()
			, m_GainInfoButton()
			, m_EncodermonitorInfoButton()
			, m_MaxsetpointInfoButton()
			, m_MinSetpointChangeInfoButton()
			, m_lStartupDelay(0)
			, m_fPIDGrobVerstaerkung(0.0f)
			, m_fMaxRotationalSpeed(0)
			, m_SampleInterval(0)
			, m_fMaxSetpoint(0)
			, m_fMaxDriveCommandChange(0.0f)
			, m_lEncoderMonitor(0)
			, m_fGatefilter(0)
			, m_fMassflowfilter(0)
			, m_fDosePerformance(0.0f)
			, m_fMinSetpointChange{ 0.0F }
			, m_ParameterOption{ eParamterOption::eNone }
			, m_ParameterOptionLast{ eParamterOption::eNone }
			, m_bRunMode{ FALSE }
			, m_bInit{ FALSE }
			, m_bLogin{ FALSE }
			, m_bMaxRotationalSpeedAllowed{ FALSE }
			, m_EditMap({
						EDITITEM(IDC_LWF_MOTOR_GAIN,	OnNotifyPidPropGainGross),
						EDITITEM(IDC_LWF_MOTOR_PIDSAMPLEINTERVAL, OnNotifySampleInterval),
						EDITITEM(IDC_LWF_MOTOR_DOSEPERFORMANCE,	OnNotifyDosePerformance),
						EDITITEM(IDC_LWF_MOTOR_MAXROTSPEED,	OnNotifyMaxRotationalSpeed),
						EDITITEM(IDC_LWF_MOTOR_MAXSETPOINT,	OnNotifyMaxMaxSetpoint),
						EDITITEM(IDC_LWF_MOTOR_ENCODERMONITOR,	OnNotifyEncoderMonitor),
						EDITITEM(IDC_LWF_MOTOR_STARTUPDELAY,	OnNotifyStartupDelay),
						EDITITEM(IDC_LWF_MOTOR_GATEFILTER,	OnNotifyGatefilter),
						EDITITEM(IDC_LWF_MOTOR_MASSFLOWFILTER,	OnNotifyMassflowfilter),
						EDITITEM(IDC_LWF_MOTOR_MAXDRIVECOMMAND,	OnNotifyMaxDriveCommandChange),
						EDITITEM(IDC_LWF_MOTOR_MINSETPOINTCHANGE,	OnNotifyMinSetpointChange)
				})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CLwfMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_LWF_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_LWF_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_LWF_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_LWF_MOTOR_GAIN, m_fPIDGrobVerstaerkung);
	DDX_Text(pDX, IDC_LWF_MOTOR_PIDSAMPLEINTERVAL, m_SampleInterval);
	DDX_Float(pDX, IDC_LWF_MOTOR_DOSEPERFORMANCE, m_fDosePerformance);
	DDX_Float(pDX, IDC_LWF_MOTOR_MAXROTSPEED, m_fMaxRotationalSpeed);
	DDX_Float(pDX, IDC_LWF_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_LWF_MOTOR_ENCODERMONITOR, m_lEncoderMonitor);
	DDX_Text(pDX, IDC_LWF_MOTOR_STARTUPDELAY, m_lStartupDelay);
	DDX_Text(pDX, IDC_LWF_MOTOR_GATEFILTER, m_fGatefilter);
	DDX_Text(pDX, IDC_LWF_MOTOR_MASSFLOWFILTER, m_fMassflowfilter);
	DDX_Text(pDX, IDC_LWF_MOTOR_MAXDRIVECOMMAND, m_fMaxDriveCommandChange);
	DDX_Text(pDX, IDC_LWF_MOTOR_MINSETPOINTCHANGE, m_fMinSetpointChange);
	DDX_Control(pDX, IDC_LWF_MOTOR_IMAGE, m_aGrafikContainer);
	DDX_Control(pDX, IDC_LWF_MOTOR_CONTROLPARAMETER_MORELESS, m_MoreLessButton);

	DDX_Control(pDX, IDC_LWF_MOTOR_PIDSAMPLEINTERVAL_INFO, m_SampleIntervalInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_DOSEPERFORMANCE_INFO, m_DosePerformanceInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_MAXSETPOINT_INFO, m_MaxsetpointInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_ENCODERMONITOR_INFO, m_EncodermonitorInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_GAIN_INFO, m_GainInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_GATEFILTER_INFO, m_GatefilterInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_MASSFLOWFILTER_INFO, m_MassflowfilterInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_MAXDRIVECOMMAND_INFO, m_MaxdrivecommandInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_MAXROTSPEED_INFO, m_MaxrotspeedInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_STARTUPDELAY_INFO, m_StartupdelayInfoButton);
	DDX_Control(pDX, IDC_LWF_MOTOR_MINSETPOINTCHANGE_INFO, m_MinSetpointChangeInfoButton);
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyPidPropGainGross()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fPIDGrobVerstaerkung);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainGross(m_sItem, m_fPIDGrobVerstaerkung);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifySampleInterval()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_SampleInterval, MIN_LWF_SAMPLETIME, MAX_LWF_SAMPLETIME);
	if (bModified)
	{
		REMOTEREF.setPidSampleInterval(m_sItem, m_SampleInterval);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyDosePerformance()
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	if (value < 0.01F)
	{
		ECMessageBox(IDS_ERROR_DOSEPERFORMANCE_LOW, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	BOOL bModified = (value != m_fDosePerformance);
	if (bModified)
	{
		m_fDosePerformance = value;
		REMOTEREF.setDoseDosePerformance(m_sItem, 0, m_fDosePerformance);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyMaxRotationalSpeed()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMaxRotationalSpeed);
	if (bModified)
	{
		REMOTEREF.setDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyMaxMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMaxSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyEncoderMonitor()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lEncoderMonitor);
	if (bModified)
	{
		REMOTEREF.setDoseEncoderMonitor(m_sItem, m_lEncoderMonitor);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyStartupDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lStartupDelay);
	if (bModified)
	{
		REMOTEREF.setDoseStartupDelay(m_sItem, m_lStartupDelay);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyGatefilter()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fGatefilter, 0.0F, 1000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseGatefilter(m_sItem, m_fGatefilter);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyMassflowfilter()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMassflowfilter, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowFilter(m_sItem, m_fMassflowfilter);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyMaxDriveCommandChange()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxDriveCommandChange, 0.0F, 50.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMaxDriveCommandChange(m_sItem, m_fMaxDriveCommandChange);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnNotifyMinSetpointChange()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMinSetpointChange, 0.0F, 5.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMinSetpointChange(m_sItem, m_fMinSetpointChange);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
LRESULT CLwfMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = m_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			PostMessage(WM_TIMER_REFRESH);
		}
	}
	return 0;
}
//******************************************************************************************************
//******************************************************************************************************
void CLwfMotorPage::ShowExtendedExStyle(void)
{
	CHECK_ID(IDC_LWF_MOTOR_PARAMETER_EXTENDEDEX, m_ParameterOption == eParamterOption::eExtendedEx);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bShowControlParam = !g_ShowLess;
	const BOOL bExtendedEx = (m_ParameterOption == eParamterOption::eExtendedEx) && bShowControlParam;

	SHOWW_ID(IDC_LWF_MOTOR_PARAMETER_EXTENDEDEX, bShowControlParam);

	SHOWW_ID(IDC_LWF_MOTOR_MINSETPOINTCHANGE_STATIC, bExtendedEx);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MINSETPOINTCHANGE, m_bLogin && bExtendedEx, bExtendedEx);
	SHOWW_ID(IDC_LWF_MOTOR_MINSETPOINTCHANGE_INFO, bExtendedEx);
}

//******************************************************************************************************
//******************************************************************************************************
void CLwfMotorPage::ShowExtendedStyle(void)
{
	CHECK_ID(IDC_LWF_MOTOR_PARAMETER_EXTENDED, m_ParameterOption == eParamterOption::eExtended);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bShowControlParam = ! g_ShowLess;
	const BOOL bExtended = (m_ParameterOption == eParamterOption::eExtended) && bShowControlParam;

	SHOWW_ID(IDC_LWF_MOTOR_PARAMETER_EXTENDED, bShowControlParam);

	SHOWW_ID(IDC_LWF_MOTOR_MAXSETPOINT_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MAXSETPOINT, bEnable && bExtended, bExtended);
	SHOWW_ID(IDC_LWF_MOTOR_MAXSETPOINT_INFO, bExtended);

	SHOWW_ID(IDC_LWF_MOTOR_STARTUPDELAY_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_STARTUPDELAY, bEnable, bExtended);
	SHOWW_ID(IDC_LWF_MOTOR_STARTUPDELAY_INFO, bExtended);

	SHOWW_ID(IDC_LWF_MOTOR_MAXROTSPEED_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MAXROTSPEED, (!m_bRunMode) && m_bMaxRotationalSpeedAllowed, bExtended);
	SHOWW_ID(IDC_LWF_MOTOR_MAXROTSPEED_INFO, bExtended);

	SHOWW_ID(IDC_LWF_MOTOR_ENCODERMONITOR_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_ENCODERMONITOR, bExtended && m_bLogin, bExtended);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_ENCODERMONITOR_INFO, m_bLogin, bExtended);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::ShowStandardStyle(void)
{
	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bScaleEnable = deviceconfig.flags.LCEnable;

	CHECK_ID(IDC_LWF_MOTOR_PARAMETER_STANDARD, m_ParameterOption == eParamterOption::eStandard);

	const BOOL bShowControlParam = ! g_ShowLess;
	const BOOL bStandard = (m_ParameterOption == eParamterOption::eStandard) && bShowControlParam && bScaleEnable;

	SHOWW_ID(IDC_LWF_MOTOR_PARAMETER_STANDARD, bShowControlParam && bScaleEnable);
	// Regelparameter
	SHOWW_ID(IDC_LWF_MOTOR_GATEFILTER_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_GATEFILTER, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_GATEFILTER_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_LWF_MOTOR_MASSFLOWFILTER_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MASSFLOWFILTER, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MASSFLOWFILTER_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_LWF_MOTOR_GAIN_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_GAIN, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_GAIN_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_LWF_MOTOR_MAXDRIVECOMMAND_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWF_MOTOR_MAXDRIVECOMMAND, bStandard && m_bLogin, bStandard);
	SHOWW_ID(IDC_LWF_MOTOR_MAXDRIVECOMMAND_INFO, bStandard);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::SetControlStyle (void)
{
	const BOOL bShowControlParam = !g_ShowLess;
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bRunMode = processstatus.flags.started;
	const BOOL bLCAktiv = processstatus.flags.LCActive;
	const BOOL bLCEnable = processstatus.flags.LCEnable;

	const BOOL bQMaxfaktorAllowed				 = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSEPERFORMANCECHANGE);
	const BOOL bMaxRotationalSpeedAllowed		 = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN);
	const BOOL bCalibrateAllowed				 = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CALIBRATE);
	const BOOL bMeasureTimeAllowed				 = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MEASURETIME);

	ENABLE_SHOW_ID(IDC_LWF_MOTOR_CALIBRATE, bCalibrateAllowed && bLCAktiv, bLCEnable);
	ENABLE_ID(IDC_LWF_MOTOR_DOSEPERFORMANCE, bQMaxfaktorAllowed /*(!bRunMode) || (bVolumetric)*/);
	ENABLE_ID(IDC_LWF_MOTOR_PIDSAMPLEINTERVAL, (bLogin || bMeasureTimeAllowed));

	if (m_bInit || (m_ParameterOption != m_ParameterOptionLast) || (bRunMode != m_bRunMode) || (bLogin != m_bLogin) || (bMaxRotationalSpeedAllowed != m_bMaxRotationalSpeedAllowed))
	{
		m_bRunMode = bRunMode;
		m_ParameterOptionLast = m_ParameterOption;
		m_bLogin					 = bLogin;
		m_bMaxRotationalSpeedAllowed = bMaxRotationalSpeedAllowed;
		m_bInit = FALSE;

		ShowStandardStyle();
		ShowExtendedStyle();
		ShowExtendedExStyle();
	}
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISLWFTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDosePidPropGainGross(m_sItem, m_fPIDGrobVerstaerkung);
	REMOTEREF.getDoseGatefilter(m_sItem, m_fGatefilter);
	REMOTEREF.getDoseMassflowFilter(m_sItem, m_fMassflowfilter);
	REMOTEREF.getPidSampleInterval(m_sItem, m_SampleInterval);
	REMOTEREF.getDoseDosePerformance(m_sItem, 0, m_fDosePerformance);
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseEncoderMonitor(m_sItem, m_lEncoderMonitor);
	REMOTEREF.getDoseStartupDelay(m_sItem, m_lStartupDelay);
	REMOTEREF.getDoseMaxDriveCommandChange(m_sItem, m_fMaxDriveCommandChange);
	REMOTEREF.getDoseMinSetpointChange(m_sItem, m_fMinSetpointChange);
	ASSERT(__ISLWFTYPE(m_lDoseType));
	SetControlStyle ();
	m_MoreLessButton.ShowLess(g_ShowLess);
	m_MoreLessButton.Show();
	return CDosePage::OnUpdateControls();
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwfMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		m_ParameterOption = eParamterOption::eStandard;
		m_ParameterOptionLast = eParamterOption::eNone;
		m_bInit = TRUE;

		INITINFOBUTTON(m_SampleIntervalInfoButton)
		INITINFOBUTTON(m_DosePerformanceInfoButton);
		INITINFOBUTTON(m_StartupdelayInfoButton);
		INITINFOBUTTON(m_MaxrotspeedInfoButton);
		INITINFOBUTTON(m_MaxdrivecommandInfoButton);
		INITINFOBUTTON(m_MassflowfilterInfoButton);
		INITINFOBUTTON(m_GatefilterInfoButton);
		INITINFOBUTTON(m_GainInfoButton);
		INITINFOBUTTON(m_EncodermonitorInfoButton);
		INITINFOBUTTON(m_MaxsetpointInfoButton);
		INITINFOBUTTON(m_MinSetpointChangeInfoButton)
		INITINFOBUTTON(m_MoreLessButton)

		SetNumberFont(IDC_LWF_MOTOR_NR);
		SetValue();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnBnClickedMotorCalibrate()
{
	CCalibrationDlg aDlg(this, m_sItem);

	aDlg.DoModal();
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorGatefilter()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_GATEFILTER);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorPidSampleInterval()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_MOTOR_PIDSAMPLEINTERVAL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorGain()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_GAIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorDosePerformance()
{
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bRunMode = processstatus.flags.started || processstatus.flags.running;
	BOOL bEdit     = ! bRunMode;
	if ( bRunMode )
	{
		if ( ECMessageBox(IDS_QUERY_DOSEPERFORMANCE_CHANGE, MB_ICONQUESTION | MB_YESNO) == IDYES )
		{
			bEdit = TRUE;
		}
	}
	if ( bEdit ) 
	{
		CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_DOSEPERFORMANCE);
	}
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorMaxSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorMassflowfilter()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_MASSFLOWFILTER);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorMaxDriveCommand()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_MAXDRIVECOMMAND);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorMinSetpointChange()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_MINSETPOINTCHANGE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorMaxRotSpeed()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_MOTOR_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorEncoderMonitor()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_MOTOR_ENCODERMONITOR);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnStnClickedMotorStartupDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_MOTOR_STARTUPDELAY);
}
//**************************************************************************************************************
//**************************************************************************************************************
HBRUSH CLwfMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LWF_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_LWF_MOTOR_IMAGE:
                        {
							m_aGrafikContainer.Update(pDC);
                        }
                        break;

	case IDC_LWF_MOTOR_GAIN:
	case IDC_LWF_MOTOR_PIDSAMPLEINTERVAL:
	case IDC_LWF_MOTOR_DOSEPERFORMANCE:
	case IDC_LWF_MOTOR_MAXROTSPEED:
	case IDC_LWF_MOTOR_MAXSETPOINT:
	case IDC_LWF_MOTOR_ENCODERMONITOR:
	case IDC_LWF_MOTOR_STARTUPDELAY:
	case IDC_LWF_MOTOR_GATEFILTER:
	case IDC_LWF_MOTOR_MASSFLOWFILTER:
	case IDC_LWF_MOTOR_MAXDRIVECOMMAND:
	case IDC_LWF_MOTOR_MINSETPOINTCHANGE:
	{
                                if ( pWnd->IsWindowEnabled() )
                                {
                                    pDC->SetBkColor(EDITBKCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = (HBRUSH) c_EditBrush;
                                }
                                pDC->SetTextColor(EDITTEXTCOLOR);
                            }
                            break;

        default:    
                            break;
    }
    return hbr;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnBnClickedMotorPidSampleIntervalInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_PIDSAMPLEINTERVAL, IDS_MEASURETIME_S, IDS_INFO_PIDSAMPLEINTERVAL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorGatefilterInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_GATEFILTER, IDS_GATEFILTER_P, IDS_INFO_GATEFILTER);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorMassflowfilterInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_MASSFLOWFILTER, IDS_MASSFLOWFILTER_P, IDS_INFO_MASSFLOWFILTER);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorGainInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_GAIN, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorMaxdrivecommandInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_MAXDRIVECOMMAND, IDS_MAXDRIVECOMMAND_P, IDS_INFO_MAXDRIVECOMMAND);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorDoseperformanceInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_DOSEPERFORMANCE, IDS_DOSEPERFORMANCE_KGH, IDS_INFO_DOSEPERFORMANCE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorMaxrotspeedInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_MAXROTSPEED, IDS_MAXROTSPEED_UPM, IDS_INFO_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorEncodermonitorInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_ENCODERMONITOR, IDS_ENCODERMONITOR_PP, IDS_INFO_ENCODERMONITOR);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorStartupdelayInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_STARTUPDELAY, IDS_STARTUPDELAY_S, IDS_INFO_STARTUPDELAY);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorMaxsetpointInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfMotorPage::OnClickedDoseMotorMinSetpointChangeInfo()
{
	CreateHelpInfoBox(IDC_LWF_MOTOR_MINSETPOINTCHANGE, IDS_MINSETPOINT_P, IDS_INFO_MINSETPOINTCHANGE);
}
//************************************************************************************
//************************************************************************************
void CLwfMotorPage::OnBnClickedDoseMotorParameterExtendedEx()
{
	if (m_ParameterOption != eParamterOption::eExtendedEx)
	{
		m_ParameterOption = eParamterOption::eExtendedEx;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CLwfMotorPage::OnBnClickedDoseMotorParameterExtended()
{
	if (m_ParameterOption != eParamterOption::eExtended)
	{
		m_ParameterOption = eParamterOption::eExtended;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CLwfMotorPage::OnBnClickedDoseMotorParameterStandard()
{
	if (m_ParameterOption != eParamterOption::eStandard)
	{
		m_ParameterOption = eParamterOption::eStandard;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CLwfMotorPage::OnBnClickedControlParamMoreLess()
{
	g_ShowLess = ! g_ShowLess;
	m_bInit = TRUE;
	SetValue();
}
//************************************************************************************
//************************************************************************************
BOOL CLwfMotorPage::OnSetActive()
{
	m_MoreLessButton.Redraw();
	m_bInit = TRUE;
	return CDosePage::OnSetActive();
}


