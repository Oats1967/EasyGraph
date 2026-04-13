//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbfMotorPage
///
/// @file   WbfMotorPage.cpp
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
#include "WbfMotorPage.h"
#include "GlobalConst.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"

#define EDITITEM(_a, _func) 	BINDFUNC(_a, CWbfMotorPage, _func)

BOOL CWbfMotorPage::g_ShowLess = TRUE;

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbfMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CWbfMotorPage::OnNotifyEdit)
	ON_MESSAGE(WM_VERWIEGUNGCHANGED, &CWbfMotorPage::OnWbfWeighingChanged)
	ON_BN_CLICKED(IDC_WBF_MOTOR_CONTROLPARAMETER_MORELESS, &CWbfMotorPage::OnBnClickedControlParamMoreLess)
	ON_STN_CLICKED(IDC_WBF_MOTOR_REDUCTION, &CWbfMotorPage::OnStnClickedWbfMotorReduction)
	ON_STN_CLICKED(IDC_WBF_MOTOR_BELTLOADSETPOINT, &CWbfMotorPage::OnStnClickedWbfMotorBeltLoadSetpoint)
	ON_STN_CLICKED(IDC_WBF_MOTOR_WHEELSIZE, &CWbfMotorPage::OnStnClickedWbfMotorWheelSize)
	ON_STN_CLICKED(IDC_WBF_MOTOR_WAEGESTRECKE, &CWbfMotorPage::OnStnClickedWbfMotorWeighingLine)
	ON_STN_CLICKED(IDC_WBF_MOTOR_MASSFLOWDAMPING, &CWbfMotorPage::OnStnClickedWbfMotorMassflowDamping)
	ON_STN_CLICKED(IDC_WBF_MOTOR_PROPGAIN, &CWbfMotorPage::OnStnClickedWbfMotorPropGain)
	ON_STN_CLICKED(IDC_WBF_MOTOR_PIDSAMPLEINTERVAL, &CWbfMotorPage::OnStnClickedWbfMotorPidSampleInterval)
	ON_STN_CLICKED(IDC_WBF_MOTOR_SWITCHVOL, &CWbfMotorPage::OnStnClickedWbfMotorSwitchVol)
	ON_STN_CLICKED(IDC_WBF_MOTOR_DREHZAHL, &CWbfMotorPage::OnStnClickedWbfMotorRotationalSpeed)
	ON_STN_CLICKED(IDC_WBF_MOTOR_MAXSETPOINT, &CWbfMotorPage::OnStnClickedWbfMotorBezugsgroesse)
	ON_STN_CLICKED(IDC_WBF_MOTOR_STARTUPDELAY, &CWbfMotorPage::OnStnClickedWbfMotorStartupDelay)
	ON_STN_CLICKED(IDC_WBF_MOTOR_MINDRIVECOMMAND, &CWbfMotorPage::OnStnClickedWbfMotorMinDriveCommand)

	ON_BN_CLICKED(IDC_WBF_MOTOR_MASSFLOWDAMPING_INFO, &CWbfMotorPage::OnBnClickedWbfMotorMassflowFilterInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_PIDSAMPLEINTERVAL_INFO, &CWbfMotorPage::OnBnClickedWbfMotorPidSampleIntervalInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_PROPGAIN_INFO, &CWbfMotorPage::OnBnClickedWbfMotorPidPropGainInfo)

	ON_BN_CLICKED(IDC_WBF_MOTOR_REDUCTION_INFO, &CWbfMotorPage::OnBnClickedWbfMotorReductionInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_BELTLOADSETPOINT_INFO, &CWbfMotorPage::OnBnClickedWbfMotorBeltLoadSetpointInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_WHEELSIZE_INFO, &CWbfMotorPage::OnBnClickedWbfMotorWheelSizeInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_WAEGESTRECKE_INFO, &CWbfMotorPage::OnBnClickedWbfMotorWeighingLineInfo)

	ON_BN_CLICKED(IDC_WBF_MOTOR_SWITCHVOL_INFO, &CWbfMotorPage::OnBnClickedWbfMotorBeltLoadVolInf)
	ON_BN_CLICKED(IDC_WBF_MOTOR_DREHZAHL_INFO, &CWbfMotorPage::OnBnClickedWbfMotorMaxRotationalSpeedInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_MAXSETPOINT_INFO, &CWbfMotorPage::OnBnClickedWbfMotorMaxSetpointInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_STARTUPDELAY_INFO, &CWbfMotorPage::OnBnClickedWbfMotorStartupDelayInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_MINDRIVECOMMAND_INFO, &CWbfMotorPage::OnBnClickedWbfMotorMinDriveCommandInfo)
	ON_BN_CLICKED(IDC_WBF_MOTOR_PARAMETER_EXTENDED, &CWbfMotorPage::OnBnClickedWbfMotorParameterExtended)
	ON_BN_CLICKED(IDC_WBF_MOTOR_PARAMETER_STANDARD, &CWbfMotorPage::OnBnClickedWbfMotorParameterStandard)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbfMotorPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CWbfMotorPage::CWbfMotorPage(): CDosePage(CWbfMotorPage::IDD)
			, m_MoreLessButton()
			, m_PidIntegralGainInfo()
			, m_PidMassflowFilterInfo()
			, m_PidSampleIntervalInfoButton()
			, m_ReductionInfoButton()
			, m_BeltLoadSetpointInfoButton()
			, m_WheelSizeInfoButton()
			, m_WeighingLineInfoButton()
			, m_WbfBeltLoadVolInfoButton()
			, m_MaxRotationalSpeedInfoButton()
			, m_MaxSetpointInfoButton()
			, m_StartupDelayInfoButton()
			, m_MinDriveCommandInfoButton()
			, m_szWeighingLineName("")
			, m_lStartupDelay(0)
			, m_fPropVerstaerkung(0)
			, m_fInputFilter(0)
			, m_fMaxRotationalSpeed(0)
			, m_fMaxSetpoint(0)
			, m_fReduction(0)
			, m_fBeltLoadSetpoint(0)
			, m_lWheelSize(0)
			, m_fSampleInterval(0.0f)
			, m_lWeighingLine(0)
			, m_fWbfBeltLoadVol(0.0F)
			, m_fRollenumfang(0.0f)
			, m_fMinDriveCommand(0.0f)
			, m_ParameterOption{ eParamterOption::eNone }
			, m_ParameterOptionLast{ eParamterOption::eNone }
			, m_bVerwiegungFull(-1)
			, m_bRunMode{ FALSE }
			, m_bInit{ FALSE }
			, m_bLogin{ FALSE }
			, m_EditMap( {
							EDITITEM(IDC_WBF_MOTOR_MASSFLOWDAMPING, OnNotifyBeltLoadMinDriveCommand),
							EDITITEM(IDC_WBF_MOTOR_PROPGAIN,  OnNotifyPidPropGainGross),
							EDITITEM(IDC_WBF_MOTOR_DREHZAHL,	OnNotifyMaxRotationalSpeed),
							EDITITEM(IDC_WBF_MOTOR_STARTUPDELAY, OnNotifyStartupDelay),
							EDITITEM(IDC_WBF_MOTOR_REDUCTION,	OnNotifyReduction),
							EDITITEM(IDC_WBF_MOTOR_BELTLOADSETPOINT,	OnNotifyBeltLoadSetpoint),
							EDITITEM(IDC_WBF_MOTOR_WHEELSIZE,	OnNotifyBeltLoadWheelSize),
							EDITITEM(IDC_WBF_MOTOR_WAEGESTRECKE,	OnNotifyWeighingLine),
							EDITITEM(IDC_WBF_MOTOR_SWITCHVOL,	OnNotifyBeltLoadVolSwitch),
							EDITITEM(IDC_WBF_MOTOR_PIDSAMPLEINTERVAL,	OnNotifySampleInterval),
							EDITITEM(IDC_WBF_MOTOR_MAXSETPOINT,	OnNotifyMaxSetpoint),
							EDITITEM(IDC_WBF_MOTOR_MINDRIVECOMMAND,	OnNotifyBeltLoadMinDriveCommand),
				})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_WBF_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_WBF_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_WBF_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_WBF_MOTOR_MASSFLOWDAMPING, m_fInputFilter);
	DDX_Text(pDX, IDC_WBF_MOTOR_PROPGAIN, m_fPropVerstaerkung);
	DDX_Text(pDX, IDC_WBF_MOTOR_DREHZAHL, m_fMaxRotationalSpeed);
	DDX_Text(pDX, IDC_WBF_MOTOR_STARTUPDELAY, m_lStartupDelay);
	DDX_Text(pDX, IDC_WBF_MOTOR_REDUCTION, m_fReduction);
	DDX_Text(pDX, IDC_WBF_MOTOR_BELTLOADSETPOINT, m_fBeltLoadSetpoint);
	DDX_Text(pDX, IDC_WBF_MOTOR_WHEELSIZE, m_lWheelSize);
	DDX_Text(pDX, IDC_WBF_MOTOR_ROLLENUMFANG, m_fRollenumfang);
	DDX_Text(pDX, IDC_WBF_MOTOR_WAEGESTRECKE, m_lWeighingLine);
	DDX_Text(pDX, IDC_WBF_MOTOR_SWITCHVOL, m_fWbfBeltLoadVol);
	DDX_Text(pDX, IDC_WBF_MOTOR_PIDSAMPLEINTERVAL, m_fSampleInterval);
	DDX_Text(pDX, IDC_WBF_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_WBF_MOTOR_MINDRIVECOMMAND, m_fMinDriveCommand);
	DDX_Text(pDX, IDC_WBF_MOTOR_WAEGESTRECKE_NAME, m_szWeighingLineName);
	DDX_Control(pDX, IDC_WBF_MOTOR_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_WBF_MOTOR_CONTROLPARAMETER_MORELESS, m_MoreLessButton);

	DDX_Control(pDX, IDC_WBF_MOTOR_MASSFLOWDAMPING_INFO, m_PidMassflowFilterInfo);
	DDX_Control(pDX, IDC_WBF_MOTOR_PROPGAIN_INFO, m_PidIntegralGainInfo);
	DDX_Control(pDX, IDC_WBF_MOTOR_PIDSAMPLEINTERVAL_INFO, m_PidSampleIntervalInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_REDUCTION_INFO, m_ReductionInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_BELTLOADSETPOINT_INFO, m_BeltLoadSetpointInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_WHEELSIZE_INFO, m_WheelSizeInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_WAEGESTRECKE_INFO, m_WeighingLineInfoButton);

	DDX_Control(pDX, IDC_WBF_MOTOR_SWITCHVOL_INFO, m_WbfBeltLoadVolInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_DREHZAHL_INFO, m_MaxRotationalSpeedInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_MAXSETPOINT_INFO, m_MaxSetpointInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_STARTUPDELAY_INFO, m_StartupDelayInfoButton);
	DDX_Control(pDX, IDC_WBF_MOTOR_MINDRIVECOMMAND_INFO, m_MinDriveCommandInfoButton);
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CWbfMotorPage::OnWbfWeighingChanged(WPARAM , LPARAM)
{
	int32_t result = ECMessageBox(IDS_BANDWAAGE_MOTOR_WAEGESTRECKE_VERWIEGUNG, MB_YESNO|MB_ICONQUESTION);
	REMOTEREF.setDoseWbfVerwiegungFull(m_sItem, BOOL(result == IDYES));
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyMassflowFilter()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fInputFilter, 0, 100.0F);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowFilter(m_sItem, m_fInputFilter);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyPidPropGainGross()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPropVerstaerkung, 0, 100.0F);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainGross(m_sItem, m_fPropVerstaerkung);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyMaxRotationalSpeed()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxRotationalSpeed, 0, 10000.0F);
	if (bModified)
	{
		REMOTEREF.setDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyStartupDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lStartupDelay, 0, 1000);
	if (bModified)
	{
		REMOTEREF.setDoseStartupDelay(m_sItem, m_lStartupDelay);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyReduction()
{
	auto bModified = CEditCtrlEx::GetFloat(m_fReduction);
	if (bModified)
	{
		REMOTEREF.setDoseWbfReduction(m_sItem, m_fReduction);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyBeltLoadSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloat(m_fBeltLoadSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseWbfBeltLoadSetpoint(m_sItem, m_fBeltLoadSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyBeltLoadWheelSize()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lWheelSize);
	if (bModified)
	{
		REMOTEREF.setDoseWbfWheelSize(m_sItem, m_lWheelSize);
		m_fRollenumfang = UMFANG(m_lWheelSize);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyWeighingLine()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lWeighingLine);
	if (bModified)
	{
		REMOTEREF.setDoseWbfWeighingLine(m_sItem, m_lWeighingLine);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMaxSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifySampleInterval()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fSampleInterval, 0.0f, 10.0f);
	if (bModified)
	{
		REMOTEREF.setPidSampleInterval(m_sItem, SEC2MS(m_fSampleInterval));
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyBeltLoadVolSwitch()
{
	auto bModified = CEditCtrlEx::GetFloat(m_fWbfBeltLoadVol);
	if (bModified)
	{
		REMOTEREF.setDoseWbfBeltLoadVolSwitch(m_sItem, m_fWbfBeltLoadVol);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnNotifyBeltLoadMinDriveCommand()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMinDriveCommand, 0.0f, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDoseWbfMinDriveCommand(m_sItem, m_fMinDriveCommand);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CWbfMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
		if (id == IDC_WBF_MOTOR_WAEGESTRECKE)
		{
			PostMessage(WM_VERWIEGUNGCHANGED);
		}
		if (bModified)
		{
			SetValue();
		}
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::ShowExtendedStyle(void)
{
	const BOOL bShowControlParam = ! g_ShowLess;
	CHECK_ID(IDC_WBF_MOTOR_PARAMETER_EXTENDED, m_ParameterOption == eParamterOption::eExtended);
	SHOWW_ID(IDC_WBF_MOTOR_PARAMETER_EXTENDED, bShowControlParam);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bExtended = (m_ParameterOption == eParamterOption::eExtended) && bShowControlParam;

	SHOWW_ID(IDC_WBF_MOTOR_REDUCTION_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_REDUCTION, bEnable && bExtended, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_REDUCTION_INFO, bExtended, bExtended);

	SHOWW_ID(IDC_WBF_MOTOR_BELTLOADSETPOINT_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_BELTLOADSETPOINT, bEnable && bExtended, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_BELTLOADSETPOINT_INFO, bExtended, bExtended);

	SHOWW_ID(IDC_WBF_MOTOR_WHEELSIZE_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_WHEELSIZE, bEnable && bExtended, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_WHEELSIZE_INFO, bExtended, bExtended);

	SHOWW_ID(IDC_WBF_MOTOR_WAEGESTRECKE_NAME, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_WAEGESTRECKE, bEnable && bExtended, bExtended);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_WAEGESTRECKE_INFO, bExtended, bExtended);

	SHOWW_ID(IDC_WBF_MOTOR_ROLLENUMFANG_STATIC, bExtended);
	SHOWW_ID(IDC_WBF_MOTOR_ROLLENUMFANG, bExtended);

}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::ShowStandardStyle(void)
{
	const BOOL bShowControlParam = ! g_ShowLess;
	CHECK_ID(IDC_WBF_MOTOR_PARAMETER_STANDARD, m_ParameterOption == eParamterOption::eStandard);
	SHOWW_ID(IDC_WBF_MOTOR_PARAMETER_STANDARD, bShowControlParam);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bStandard = (m_ParameterOption == eParamterOption::eStandard) && bShowControlParam;

	SHOWW_ID(IDC_WBF_MOTOR_SWITCHVOL_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_SWITCHVOL, bEnable && bStandard, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_SWITCHVOL_INFO, bStandard, bStandard);

	const BOOL bMaxRotationalSpeedalSpeedAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN);
	SHOWW_ID(IDC_WBF_MOTOR_DREHZAHL_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_DREHZAHL, bMaxRotationalSpeedalSpeedAllowed && bEnable && bStandard, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_DREHZAHL_INFO, bStandard, bStandard);

	SHOWW_ID(IDC_WBF_MOTOR_MAXSETPOINT_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_MAXSETPOINT, bEnable && bStandard, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_MAXSETPOINT_INFO, bStandard, bStandard);

	SHOWW_ID(IDC_WBF_MOTOR_STARTUPDELAY_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_STARTUPDELAY, bEnable && bStandard, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_STARTUPDELAY_INFO, bStandard, bStandard);

	SHOWW_ID(IDC_WBF_MOTOR_MINDRIVECOMMAND_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_MINDRIVECOMMAND, bEnable && bStandard, bStandard);
	ENABLE_SHOW_ID(IDC_WBF_MOTOR_MINDRIVECOMMAND_INFO, bStandard, bStandard);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bRunMode = ulStatus.flags.started;

	ENABLE_ID(IDC_WBF_MOTOR_MASSFLOWDAMPING, bLogin);
	ENABLE_ID(IDC_WBF_MOTOR_PROPGAIN, bLogin);
	ENABLE_ID(IDC_WBF_MOTOR_PIDSAMPLEINTERVAL, bLogin);

	if (m_bInit || (m_ParameterOption != m_ParameterOptionLast) || ( bRunMode != m_bRunMode) || (bLogin != m_bLogin))
	{
		m_bRunMode = bRunMode;
		m_ParameterOptionLast = m_ParameterOption;
		m_bLogin = bLogin;
		m_bInit = FALSE;

		ShowStandardStyle();
		ShowExtendedStyle();
	}
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDosePidPropGainGross(m_sItem, m_fPropVerstaerkung);
	REMOTEREF.getDoseMassflowFilter(m_sItem, m_fInputFilter);
	uint32_t temp = 0;
	REMOTEREF.getPidSampleInterval(m_sItem, temp);
	m_fSampleInterval = MS2SEC(temp);
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	REMOTEREF.getDoseStartupDelay(m_sItem, m_lStartupDelay);
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseWbfReduction(m_sItem, m_fReduction);
	REMOTEREF.getDoseWbfBeltLoadSetpoint(m_sItem, m_fBeltLoadSetpoint);
	REMOTEREF.getDoseWbfWheelSize(m_sItem, m_lWheelSize);
	m_fRollenumfang = UMFANG(m_lWheelSize);
	m_fRollenumfang = ROUND1(m_fRollenumfang);
	REMOTEREF.getDoseWbfWeighingLine(m_sItem, m_lWeighingLine);
	REMOTEREF.getDoseWbfBeltLoadVolSwitch(m_sItem, m_fWbfBeltLoadVol);
	REMOTEREF.getDoseWbfMinDriveCommand(m_sItem, m_fMinDriveCommand);
	REMOTEREF.getDoseWbfVerwiegungFull(m_sItem, m_bVerwiegungFull);

	VERIFY(m_szWeighingLineName.LoadString(IDS_BANDWAAGE_MOTOR_WAEGESTRECKE_NAME));
	if ( m_bVerwiegungFull)
	{
		m_szWeighingLineName +=" 1/1";
	}
	else
	{
		m_szWeighingLineName += " 1/2";
	}
	SetControlStyle ();
	m_MoreLessButton.ShowLess(g_ShowLess);
	m_MoreLessButton.Show();
	return CDosePage::OnUpdateControls();
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	m_ParameterOption = eParamterOption::eStandard;
	m_ParameterOptionLast = eParamterOption::eNone;
	m_bInit						= TRUE;

	INITINFOBUTTON(m_PidMassflowFilterInfo)
	INITINFOBUTTON(m_PidIntegralGainInfo)
	INITINFOBUTTON(m_PidSampleIntervalInfoButton)
	INITINFOBUTTON(m_MoreLessButton)
	INITINFOBUTTON(m_ReductionInfoButton)
	INITINFOBUTTON(m_BeltLoadSetpointInfoButton)
	INITINFOBUTTON(m_WheelSizeInfoButton)
	INITINFOBUTTON(m_WeighingLineInfoButton)
	INITINFOBUTTON(m_WbfBeltLoadVolInfoButton)
	INITINFOBUTTON(m_MaxRotationalSpeedInfoButton)
	INITINFOBUTTON(m_MaxSetpointInfoButton)
	INITINFOBUTTON(m_StartupDelayInfoButton)
	INITINFOBUTTON(m_MinDriveCommandInfoButton)

	SetNumberFont(IDC_WBF_MOTOR_NR);
	SetValue();
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorReduction()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_REDUCTION);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorBeltLoadSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_BELTLOADSETPOINT);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorWheelSize()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_MOTOR_WHEELSIZE);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorWeighingLine()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_MOTOR_WAEGESTRECKE);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorMassflowDamping()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_MASSFLOWDAMPING);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorPropGain()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_PROPGAIN);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorPidSampleInterval()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_PIDSAMPLEINTERVAL);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorSwitchVol()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_SWITCHVOL);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorRotationalSpeed()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_DREHZAHL);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorBezugsgroesse()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_MAXSETPOINT);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorStartupDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_MOTOR_STARTUPDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnStnClickedWbfMotorMinDriveCommand()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_MOTOR_MINDRIVECOMMAND);
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CWbfMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBF_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

	case IDC_WBF_MOTOR_MASSFLOWDAMPING:
	case IDC_WBF_MOTOR_PROPGAIN:
	case IDC_WBF_MOTOR_DREHZAHL:
	case IDC_WBF_MOTOR_STARTUPDELAY:
	case IDC_WBF_MOTOR_REDUCTION:
	case IDC_WBF_MOTOR_BELTLOADSETPOINT:
	case IDC_WBF_MOTOR_WHEELSIZE:
	case IDC_WBF_MOTOR_WAEGESTRECKE:
	case IDC_WBF_MOTOR_SWITCHVOL:
	case IDC_WBF_MOTOR_PIDSAMPLEINTERVAL:
	case IDC_WBF_MOTOR_MAXSETPOINT:
	case IDC_WBF_MOTOR_MINDRIVECOMMAND:
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

	case IDC_WBF_MOTOR_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;


        default:    
                            break;
    }
    return hbr;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorPidPropGainInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_PROPGAIN, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorMassflowFilterInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_MASSFLOWDAMPING, IDS_MASSFLOWFILTER_P, IDS_INFO_MASSFLOWFILTER);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorPidSampleIntervalInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_PIDSAMPLEINTERVAL, IDS_MEASURETIME_S, IDS_INFO_PIDSAMPLEINTERVAL);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedControlParamMoreLess()
{
	g_ShowLess = !g_ShowLess;
	m_bInit = TRUE;;
	SetValue();
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorParameterExtended()
{
	if (m_ParameterOption != eParamterOption::eExtended)
	{
		m_ParameterOption = eParamterOption::eExtended;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorParameterStandard()
{
	if (m_ParameterOption != eParamterOption::eStandard)
	{
		m_ParameterOption = eParamterOption::eStandard;
		SetValue();
	}
}

//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorReductionInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_REDUCTION, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorBeltLoadSetpointInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_BELTLOADSETPOINT, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorWheelSizeInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_WHEELSIZE, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorWeighingLineInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_WAEGESTRECKE, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorBeltLoadVolInf()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_SWITCHVOL, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorMaxRotationalSpeedInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_DREHZAHL, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorMaxSetpointInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorStartupDelayInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_STARTUPDELAY, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CWbfMotorPage::OnBnClickedWbfMotorMinDriveCommandInfo()
{
	CreateHelpInfoBox(IDC_WBF_MOTOR_MINDRIVECOMMAND, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
BOOL CWbfMotorPage::OnSetActive()
{
	m_MoreLessButton.Redraw();
	return CDosePage::OnSetActive();
}
