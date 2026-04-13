//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module PidMotorPage.cpp
///
/// @file   PidMotorPage.cpp
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
#include "RemoteControl.h"
#include "PidMotorPage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CPidMotorPage, _func)

BOOL CPidMotorPage::g_ShowLess = TRUE;

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CPidMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_PID_MOTOR_CONTROLPARAMETER_MORELESS, &CPidMotorPage::OnBnClickedPidControlParamMoreLess)
	ON_BN_CLICKED(IDC_PID_MOTOR_DRIVECOMMANDAUSGABE_CHECK, &CPidMotorPage::OnBnClickedPidMotorDriveCommandausgabeCheck)
	ON_BN_CLICKED(IDC_PID_MOTOR_DRIVECOMMANDAUSGABEINV_CHECK, &CPidMotorPage::OnBnClickedPidMotorDriveCommandausgabeinvCheck)
	ON_STN_CLICKED(IDC_PID_MOTOR_PIDPROPGAIN_FINE, &CPidMotorPage::OnStnClickedPidMotorPidPropGainFine)
	ON_STN_CLICKED(IDC_PID_MOTOR_PIDPROPGAIN_GROSS, &CPidMotorPage::OnStnClickedPidMotorPidPropGainGross)
	ON_STN_CLICKED(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE, &CPidMotorPage::OnStnClickedPidMotorPidPropGainSwitchGrossFine)
	ON_STN_CLICKED(IDC_PID_MOTOR_INTEGRALGAIN, &CPidMotorPage::OnStnClickedPidMotorPidIntegralGain)
	ON_STN_CLICKED(IDC_PID_MOTOR_MASSFLOWFILTER, &CPidMotorPage::OnStnClickedPidMotorMassflowFilter)
	ON_STN_CLICKED(IDC_PID_MOTOR_PIDSAMPLEINTERVAL, &CPidMotorPage::OnStnClickedPidMotorPidSampleInterval)
	ON_STN_CLICKED(IDC_PID_MOTOR_DOSEPERFORMANCE, &CPidMotorPage::OnStnClickedPidMotorDosePerformance)
	ON_STN_CLICKED(IDC_PID_MOTOR_MAXSETPOINT, &CPidMotorPage::OnStnClickedPidMotorMaxSetpoint)
	ON_STN_CLICKED(IDC_PID_MOTOR_STEEPNESSMASSFLOW, &CPidMotorPage::OnStnClickedPidMotorSteepnessMassflow)
	ON_STN_CLICKED(IDC_PID_MOTOR_STARTUPDELAY, &CPidMotorPage::OnStnClickedPidMotorStartupDelay)
	ON_STN_CLICKED(IDC_PID_MOTOR_STARTUPRAMP, &CPidMotorPage::OnStnClickedPidMotorStartupRamp)
	ON_BN_CLICKED(IDC_PID_MOTOR_PIDPROPGAIN_FINE_INFO, &CPidMotorPage::OnBnClickedPidMotorPidPropGainFineInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_PIDPROPGAIN_GROSS_INFO, &CPidMotorPage::OnBnClickedPidMotorPidPropGainGrossInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE_INFO, &CPidMotorPage::OnBnClickedPidMotorPidPropGainSwitchGrossFineInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_INTEGRALGAIN_INFO, &CPidMotorPage::OnBnClickedPidMotorPidIntegralGainInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_MASSFLOWFILTER_INFO, &CPidMotorPage::OnBnClickedPidMotorMassflowFilterInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_PIDSAMPLEINTERVAL_INFO, &CPidMotorPage::OnBnClickedPidMotorPidSampleIntervalInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_DOSEPERFORMANCE_INFO, &CPidMotorPage::OnBnClickedPidMotorDosePerformanceInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_MAXSETPOINT_INFO, &CPidMotorPage::OnBnClickedPidMotorMaxSetpointInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_STEEPNESSMASSFLOW_INFO, &CPidMotorPage::OnBnClickedPidMotorSteepnessMassflowInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_STARTUPRAMP_INFO, &CPidMotorPage::OnBnClickedPidMotorStartupRampInfo)
	ON_BN_CLICKED(IDC_PID_MOTOR_STARTUPDELAY_INFO, &CPidMotorPage::OnBnClickedPidMotorStartupDelayInfo)

	ON_BN_CLICKED(IDC_PID_MOTOR_PARAMETER_EXTENDED, &CPidMotorPage::OnBnClickedDoseMotorParameterExtended)
	ON_BN_CLICKED(IDC_PID_MOTOR_PARAMETER_EXTENDEDEX, &CPidMotorPage::OnBnClickedDoseMotorParameterExtendedEx)
	ON_BN_CLICKED(IDC_PID_MOTOR_PARAMETER_STANDARD, &CPidMotorPage::OnBnClickedDoseMotorParameterStandard)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CPidMotorPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CPidMotorPage::CPidMotorPage() : CDosePage(CPidMotorPage::IDD)
	, m_MoreLessButton()
	, m_PidPropGainFineInfo()
	, m_PidPropGainGrossInfo()
	, m_PidPropGainSwitchGrossFineInfo()
	, m_PidIntegralGainInfo()
	, m_PidMassflowFilterInfo()
	, m_PidDosePerformanceInfo()
	, m_PidSteepnessMassflowInfo()
	, m_MaxsetpointInfoButton()
	, m_PidSampleIntervalInfoButton()
	, m_StartupRampInfoButton()
	, m_StartupDelayInfoButton()
	, m_fPidPropGainFine(0.0F)
	, m_fPidPropGainGross(0.0F)
	, m_fPidPropGainSwitchGrossFine(0.0F)
	, m_fPidIntegralGain(0.0F)
	, m_fMassflowFilter(0.0F)
	, m_fMaxSetpoint(0.0F)
	, m_fSteepnessMassflow(0.0F)
	, m_lStartupDelay(0)
	, m_bDriveCommandInv(FALSE)
	, m_bDriveCommand(FALSE)
	, m_lSampleInterval(0)
	, m_lStartupRamp(0)
	, m_fDosePerformance(0)
	, m_ParameterOption{ eParamterOption::eNone }
	, m_ParameterOptionLast{ eParamterOption::eNone }
	, m_bLogin{ FALSE }
	, m_bRunMode{ FALSE }
	, m_bInit{ FALSE }
	, m_EditMap({
				EDITITEM(IDC_PID_MOTOR_PIDPROPGAIN_FINE, OnNotifyPidPropGainFine),
				EDITITEM(IDC_PID_MOTOR_PIDPROPGAIN_GROSS,  OnNotifyPidPropGainGross),
				EDITITEM(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE,	OnNotifyPidPropGainSwitchGrossFine),
				EDITITEM(IDC_PID_MOTOR_INTEGRALGAIN, OnNotifyPidIntegralGain),
				EDITITEM(IDC_PID_MOTOR_MASSFLOWFILTER,	OnNotifyMassflowFilter),
				EDITITEM(IDC_PID_MOTOR_MAXSETPOINT,	OnNotifyMaxSetpoint),
				EDITITEM(IDC_PID_MOTOR_STEEPNESSMASSFLOW,	OnNotifyMassflowSteepness),
				EDITITEM(IDC_PID_MOTOR_STARTUPDELAY,	OnNotifyStartupDelay),
				EDITITEM(IDC_PID_MOTOR_PIDSAMPLEINTERVAL,	OnNotifySampleInterval),
				EDITITEM(IDC_PID_MOTOR_STARTUPRAMP,	OnNotifyStartupRamp),
				EDITITEM(IDC_PID_MOTOR_DOSEPERFORMANCE,	OnNotifyDosePerformance)
				})
{
}

void CPidMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_PID_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_PID_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_PID_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_PID_MOTOR_PIDPROPGAIN_FINE, m_fPidPropGainFine);
	DDX_Text(pDX, IDC_PID_MOTOR_PIDPROPGAIN_GROSS, m_fPidPropGainGross);
	DDX_Text(pDX, IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE, m_fPidPropGainSwitchGrossFine);
	DDX_Text(pDX, IDC_PID_MOTOR_INTEGRALGAIN, m_fPidIntegralGain);
	DDX_Text(pDX, IDC_PID_MOTOR_MASSFLOWFILTER, m_fMassflowFilter);
	DDX_Text(pDX, IDC_PID_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_PID_MOTOR_STEEPNESSMASSFLOW, m_fSteepnessMassflow);
	DDX_Text(pDX, IDC_PID_MOTOR_STARTUPDELAY, m_lStartupDelay);
	DDX_Check(pDX, IDC_PID_MOTOR_DRIVECOMMANDAUSGABEINV_CHECK, m_bDriveCommandInv);
	DDX_Check(pDX, IDC_PID_MOTOR_DRIVECOMMANDAUSGABE_CHECK, m_bDriveCommand);
	DDX_Text(pDX, IDC_PID_MOTOR_PIDSAMPLEINTERVAL, m_lSampleInterval);
	DDX_Text(pDX, IDC_PID_MOTOR_STARTUPRAMP, m_lStartupRamp);
	DDX_Text(pDX, IDC_PID_MOTOR_DOSEPERFORMANCE, m_fDosePerformance);
	DDX_Control(pDX, IDC_PID_MOTOR_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_PID_MOTOR_CONTROLPARAMETER_MORELESS, m_MoreLessButton);


	DDX_Control(pDX, IDC_PID_MOTOR_PIDPROPGAIN_FINE_INFO, m_PidPropGainFineInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_PIDPROPGAIN_GROSS_INFO, m_PidPropGainGrossInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE_INFO, m_PidPropGainSwitchGrossFineInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_INTEGRALGAIN_INFO, m_PidIntegralGainInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_MASSFLOWFILTER_INFO, m_PidMassflowFilterInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_DOSEPERFORMANCE_INFO, m_PidDosePerformanceInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_STEEPNESSMASSFLOW_INFO, m_PidSteepnessMassflowInfo);
	DDX_Control(pDX, IDC_PID_MOTOR_MAXSETPOINT_INFO, m_MaxsetpointInfoButton);
	DDX_Control(pDX, IDC_PID_MOTOR_PIDSAMPLEINTERVAL_INFO, m_PidSampleIntervalInfoButton);
	DDX_Control(pDX, IDC_PID_MOTOR_STARTUPRAMP_INFO, m_StartupRampInfoButton);
	DDX_Control(pDX, IDC_PID_MOTOR_STARTUPDELAY_INFO, m_StartupDelayInfoButton);

}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyPidPropGainFine()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPidPropGainFine, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainFine(m_sItem, m_fPidPropGainFine);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyPidPropGainGross()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPidPropGainGross, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainGross(m_sItem, m_fPidPropGainGross);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyPidIntegralGain()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPidIntegralGain, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainGross(m_sItem, m_fPidIntegralGain);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyPidPropGainSwitchGrossFine()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPidPropGainSwitchGrossFine, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainSwitchGrossFine(m_sItem, m_fPidPropGainSwitchGrossFine);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyMassflowFilter()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMassflowFilter, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowFilter(m_sItem, m_fMassflowFilter);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxSetpoint, 0.0F, 10000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	}
	return bModified;
}

//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyMassflowSteepness()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fSteepnessMassflow, 0.0F, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowSteepness(m_sItem, m_fSteepnessMassflow);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyStartupDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lStartupDelay, 0U, 1000U);
	if (bModified)
	{
		REMOTEREF.setDoseStartupDelay(m_sItem, m_lStartupDelay);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifySampleInterval()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lSampleInterval, 0U, 1000U);
	if (bModified)
	{
		REMOTEREF.setPidSampleInterval(m_sItem, m_lSampleInterval);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CPidMotorPage::OnNotifyStartupRamp()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lStartupRamp);
	if (bModified)
	{
		REMOTEREF.setDoseStartupRamp(m_sItem, m_lStartupRamp);
	}
	return bModified;
}
//******************************************************************************************************************
//******************************************************************************************************************
BOOL CPidMotorPage::OnNotifyDosePerformance()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fDosePerformance, 0.01f, 10000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseDosePerformance(m_sItem, 0, m_fDosePerformance);
	}
	return bModified;
}
//******************************************************************************************************************
//******************************************************************************************************************
LRESULT CPidMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
	return 0L;
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::SetControlStyle (void)
{
	const BOOL bShowControlParam = !g_ShowLess;
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}

	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);

	const BOOL bRunMode			= ulStatus.flags.started || ulStatus.flags.running;
	const BOOL bQMaxfaktorAllowed  = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSEPERFORMANCECHANGE);

	ENABLE_ID(IDC_PID_MOTOR_MASSFLOWFILTER, bLogin);
	ENABLE_ID(IDC_PID_MOTOR_DOSEPERFORMANCE, bLogin && bQMaxfaktorAllowed);
	ENABLE_ID(IDC_PID_MOTOR_PIDSAMPLEINTERVAL, bLogin);

	if (m_bInit || (m_ParameterOption != m_ParameterOptionLast) || (bRunMode != m_bRunMode) || (bLogin != m_bLogin))
	{
		m_bRunMode = bRunMode;
		m_ParameterOptionLast = m_ParameterOption;
		m_bLogin = bLogin;
		m_bInit = FALSE;

		ShowStandardStyle();
		ShowExtendedStyle();
		ShowExtendedExStyle();
	}
}
//******************************************************************************************************************
//******************************************************************************************************************
BOOL CPidMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISPIDTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDosePidPropGainFine(m_sItem, m_fPidPropGainFine);
	REMOTEREF.getDosePidPropGainGross(m_sItem, m_fPidPropGainGross);
	REMOTEREF.getDosePidPropGainSwitchGrossFine(m_sItem, m_fPidPropGainSwitchGrossFine);
	REMOTEREF.getDosePidIntegralGain(m_sItem, m_fPidIntegralGain);
	REMOTEREF.getDoseMassflowFilter(m_sItem, m_fMassflowFilter);
	REMOTEREF.getDoseMassflowSteepness(m_sItem, m_fSteepnessMassflow);
	REMOTEREF.getPidSampleInterval(m_sItem, m_lSampleInterval);
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseStartupDelay(m_sItem, m_lStartupDelay);
	REMOTEREF.getDoseStartupRamp(m_sItem, m_lStartupRamp);
	REMOTEREF.getDoseDriveCommandInverse(m_sItem, m_bDriveCommandInv);
	REMOTEREF.getDoseDosePerformance(m_sItem, 0, m_fDosePerformance);
	m_bDriveCommand				= ! m_bDriveCommandInv;
	SetControlStyle ();
	m_MoreLessButton.ShowLess(g_ShowLess);
	m_MoreLessButton.Show();
	return CDosePage::OnUpdateControls();
}
//******************************************************************************************************************
//******************************************************************************************************************
BOOL CPidMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		m_ParameterOption = eParamterOption::eStandard;
		m_ParameterOptionLast = eParamterOption::eNone;
		m_bInit = TRUE;

		INITINFOBUTTON(m_PidPropGainFineInfo)
		INITINFOBUTTON(m_PidPropGainGrossInfo);
		INITINFOBUTTON(m_PidPropGainSwitchGrossFineInfo);
		INITINFOBUTTON(m_PidIntegralGainInfo);
		INITINFOBUTTON(m_PidMassflowFilterInfo);
		INITINFOBUTTON(m_PidDosePerformanceInfo);
		INITINFOBUTTON(m_PidSteepnessMassflowInfo);
		INITINFOBUTTON(m_MaxsetpointInfoButton);
		INITINFOBUTTON(m_PidSampleIntervalInfoButton);
		INITINFOBUTTON(m_StartupRampInfoButton);
		INITINFOBUTTON(m_StartupDelayInfoButton);
		INITINFOBUTTON(m_MoreLessButton)

		SetNumberFont(IDC_PID_MOTOR_NR);
		SetValue();
	}
	return result;
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorDriveCommandausgabeCheck()
{
	m_bDriveCommandInv = ! m_bDriveCommand;
	REMOTEREF.setDoseDriveCommandInverse(m_sItem, m_bDriveCommandInv);
	SetValue();
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorDriveCommandausgabeinvCheck()
{
	m_bDriveCommand = ! m_bDriveCommandInv;
	REMOTEREF.setDoseDriveCommandInverse(m_sItem, m_bDriveCommandInv);
	SetValue();
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorPidPropGainFine()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_PIDPROPGAIN_FINE);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorPidPropGainGross()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_PIDPROPGAIN_GROSS);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorPidPropGainSwitchGrossFine()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorPidIntegralGain()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_INTEGRALGAIN);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorMassflowFilter()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_MASSFLOWFILTER);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorPidSampleInterval()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_MOTOR_PIDSAMPLEINTERVAL);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorDosePerformance()
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bRunMode = ulStatus.flags.started || ulStatus.flags.running;

	BOOL bEdit     = !bRunMode;
	if ( bRunMode )
	{
		if ( ECMessageBox(IDS_QUERY_DOSEPERFORMANCE_CHANGE, MB_ICONQUESTION | MB_YESNO) == IDYES )
		{
			bEdit = TRUE;
		}
	}
	if ( bEdit )
	{
		CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_DOSEPERFORMANCE);
	}
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorMaxSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_MAXSETPOINT);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorSteepnessMassflow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_MOTOR_STEEPNESSMASSFLOW);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorStartupDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_MOTOR_STARTUPDELAY);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnStnClickedPidMotorStartupRamp()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_MOTOR_STARTUPRAMP);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorPidPropGainFineInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_PIDPROPGAIN_FINE, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorPidPropGainGrossInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_PIDPROPGAIN_GROSS, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorPidPropGainSwitchGrossFineInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorPidIntegralGainInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_INTEGRALGAIN, IDS_INFO_TODO, IDS_INFO_TODO);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorMassflowFilterInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_MASSFLOWFILTER, IDS_MASSFLOWFILTER_P, IDS_INFO_MASSFLOWFILTER);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorPidSampleIntervalInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_PIDSAMPLEINTERVAL, IDS_MEASURETIME_S, IDS_INFO_PIDSAMPLEINTERVAL);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorDosePerformanceInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_DOSEPERFORMANCE, IDS_DOSEPERFORMANCE_KGH, IDS_INFO_DOSEPERFORMANCE);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorMaxSetpointInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorSteepnessMassflowInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_STEEPNESSMASSFLOW, IDS_INFO_TODO, IDS_INFO_TODO);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorStartupRampInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_STARTUPRAMP, IDS_INFO_TODO, IDS_INFO_TODO);
}
//******************************************************************************************************************
//******************************************************************************************************************
void CPidMotorPage::OnBnClickedPidMotorStartupDelayInfo()
{
	CreateHelpInfoBox(IDC_PID_MOTOR_STARTUPDELAY, IDS_INFO_TODO, IDS_INFO_TODO);
}
//******************************************************************************************************************
//******************************************************************************************************************
HBRUSH CPidMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_PID_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

		case IDC_PID_MOTOR_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

		case IDC_PID_MOTOR_PIDPROPGAIN_FINE:
		case IDC_PID_MOTOR_PIDPROPGAIN_GROSS:
		case IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE:
		case IDC_PID_MOTOR_INTEGRALGAIN:
		case IDC_PID_MOTOR_MASSFLOWFILTER:
		case IDC_PID_MOTOR_MAXSETPOINT:
		case IDC_PID_MOTOR_STEEPNESSMASSFLOW:
		case IDC_PID_MOTOR_STARTUPDELAY:
		case IDC_PID_MOTOR_PIDSAMPLEINTERVAL:
		case IDC_PID_MOTOR_STARTUPRAMP:
		case IDC_PID_MOTOR_DOSEPERFORMANCE:
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
//******************************************************************************************************
//******************************************************************************************************
void CPidMotorPage::ShowExtendedExStyle(void)
{
	const BOOL bShowControlParam = ! g_ShowLess;
	CHECK_ID(IDC_PID_MOTOR_PARAMETER_EXTENDEDEX, m_ParameterOption == eParamterOption::eExtendedEx);
	SHOWW_ID(IDC_PID_MOTOR_PARAMETER_EXTENDEDEX, bShowControlParam);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bExtendedEx = (m_ParameterOption == eParamterOption::eExtendedEx) && bShowControlParam;

	ENABLE_SHOW_ID(IDC_PID_MOTOR_DRIVECOMMANDAUSGABE_CHECK, bEnable, bExtendedEx);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_DRIVECOMMANDAUSGABEINV_CHECK, bEnable, bExtendedEx);
}

//******************************************************************************************************
//******************************************************************************************************
void CPidMotorPage::ShowExtendedStyle(void)
{
	const BOOL bShowControlParam = ! g_ShowLess;
	CHECK_ID(IDC_PID_MOTOR_PARAMETER_EXTENDED, m_ParameterOption == eParamterOption::eExtended);
	SHOWW_ID(IDC_PID_MOTOR_PARAMETER_EXTENDED, bShowControlParam);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bExtended = (m_ParameterOption == eParamterOption::eExtended) && bShowControlParam;

	SHOWW_ID(IDC_PID_MOTOR_STEEPNESSMASSFLOW_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_STEEPNESSMASSFLOW, bEnable && bExtended, bExtended);
	SHOWW_ID(IDC_PID_MOTOR_STEEPNESSMASSFLOW_INFO, bExtended);

	SHOWW_ID(IDC_PID_MOTOR_MAXSETPOINT_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_MAXSETPOINT, bEnable && bExtended, bExtended);
	SHOWW_ID(IDC_PID_MOTOR_MAXSETPOINT_INFO, bExtended);

	SHOWW_ID(IDC_PID_MOTOR_STARTUPDELAY_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_STARTUPDELAY, bEnable && bExtended, bExtended);
	SHOWW_ID(IDC_PID_MOTOR_STARTUPDELAY_INFO, bExtended);

	SHOWW_ID(IDC_PID_MOTOR_STARTUPRAMP_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_STARTUPRAMP, bEnable && bExtended, bExtended);
	SHOWW_ID(IDC_PID_MOTOR_STARTUPRAMP_INFO, bExtended);

}
//**************************************************************************************************************
//**************************************************************************************************************
void CPidMotorPage::ShowStandardStyle(void)
{
	const BOOL bShowControlParam = ! g_ShowLess;

	CHECK_ID(IDC_PID_MOTOR_PARAMETER_STANDARD, m_ParameterOption == eParamterOption::eStandard);

	SHOWW_ID(IDC_PID_MOTOR_PARAMETER_STANDARD, bShowControlParam);

	const BOOL bStandard = (m_ParameterOption == eParamterOption::eStandard) && bShowControlParam;

	// Regelparameter
	SHOWW_ID(IDC_PID_MOTOR_PIDPROPGAIN_FINE_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAIN_FINE, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAIN_FINE_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_PID_MOTOR_PIDPROPGAIN_GROSS_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAIN_GROSS, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAIN_GROSS_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_PIDPROPGAINSWITCHGROSSFINE_INFO, m_bLogin, bStandard);

	SHOWW_ID(IDC_PID_MOTOR_INTEGRALGAIN_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_INTEGRALGAIN, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_PID_MOTOR_INTEGRALGAIN_INFO, m_bLogin, bStandard);
}
//************************************************************************************
//************************************************************************************
void CPidMotorPage::OnBnClickedDoseMotorParameterExtended()
{
	if (m_ParameterOption != eParamterOption::eExtended)
	{
		m_ParameterOption = eParamterOption::eExtended;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CPidMotorPage::OnBnClickedDoseMotorParameterExtendedEx()
{
	if (m_ParameterOption != eParamterOption::eExtendedEx)
	{
		m_ParameterOption = eParamterOption::eExtendedEx;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CPidMotorPage::OnBnClickedDoseMotorParameterStandard()
{
	if (m_ParameterOption != eParamterOption::eStandard)
	{
		m_ParameterOption = eParamterOption::eStandard;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CPidMotorPage::OnBnClickedPidControlParamMoreLess()
{
	g_ShowLess = !g_ShowLess;
	m_bInit = TRUE;
	SetValue();
}
//************************************************************************************
//************************************************************************************
BOOL CPidMotorPage::OnSetActive()
{
	m_MoreLessButton.Redraw();
	return CDosePage::OnSetActive();
}




