//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwbMotorPage
///
/// @file   LwbMotorPage.cpp
///
///
/// @coypright Ing.b�ro Hafer
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
#include "LwbMotorPage.h"
#include "BASE/Utils/public/UserRightsList.h"
#include "GlobalConst.h"
#include "CalibrationDlg.h"
#include "MaxGrossSpeedBoxDlg.h"
#include "MaxFineSpeedBoxDlg.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLwbMotorPage, _func)

BOOL CLwbMotorPage::g_ShowLess = TRUE;

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CLwbMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_LWB_MOTOR_CONTROLPARAMETER_MORELESS, &CLwbMotorPage::OnBnClickedControlParamMoreLess)
	ON_STN_CLICKED(IDC_LWB_MOTOR_MAXSETPOINT, &CLwbMotorPage::OnStnClickedMotorMaxSetpoint)
	ON_STN_CLICKED(IDC_LWB_MOTOR_MAXROTSPEED, &CLwbMotorPage::OnStnClickedMotorMaxRotSpeed)
	ON_STN_CLICKED(IDC_LWB_MOTOR_ENCODERMONITOR, &CLwbMotorPage::OnStnClickedMotorEncoderMonitor)
	ON_STN_CLICKED(IDC_LWB_MOTOR_STARTUPDELAY, &CLwbMotorPage::OnStnClickedMotorStartupDelay)
	ON_STN_CLICKED(IDC_LWB_MOTOR_GROSSSTREAM, &CLwbMotorPage::OnStnClickedMotorGrossStream)
	ON_STN_CLICKED(IDC_LWB_MOTOR_FINESTREAM, &CLwbMotorPage::OnStnClickedMotorFineStream)
	ON_STN_CLICKED(IDC_LWB_MOTOR_SWITCHPOINT, &CLwbMotorPage::OnStnClickedMotorSwitchpoint)
	ON_STN_CLICKED(IDC_LWB_MOTOR_SCALE, &CLwbMotorPage::OnStnClickedMotorScale)
	ON_STN_CLICKED(IDC_LWB_MOTOR_OVERRUNCOMPENSATION, &CLwbMotorPage::OnStnClickedMotorOverrunCompensation)
	ON_STN_CLICKED(IDC_LWB_MOTOR_BATCHTIME, &CLwbMotorPage::OnStnClickedMotorBatchTime)

	ON_BN_CLICKED(IDC_LWB_MOTOR_MAXSETPOINT_INFO, &CLwbMotorPage::OnClickedDoseMotorMaxsetpointInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_GROSSSTREAM_INFO, &CLwbMotorPage::OnClickedMotorGrossStreamInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_FINESTREAM_INFO, &CLwbMotorPage::OnClickedMotorFineStreamInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_SWITCHPOINT_INFO, &CLwbMotorPage::OnClickedMotorSwitchpointInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_ENCODERMONITOR_INFO, &CLwbMotorPage::OnClickedDoseMotorEncodermonitorInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_MAXROTSPEED_INFO, &CLwbMotorPage::OnClickedDoseMotorMaxrotspeedInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_STARTUPDELAY_INFO, &CLwbMotorPage::OnClickedDoseMotorStartupdelayInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_SCALE_INFO, &CLwbMotorPage::OnClickedDoseMotorScaleInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_BATCHTIME_INFO, &CLwbMotorPage::OnClickedDoseMotorBatchtimeInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_OVERRUNCOMPENSATION_INFO, &CLwbMotorPage::OnClickedDoseMotorOverrunCompensationInfo)
	ON_BN_CLICKED(IDC_LWB_MOTOR_PARAMETER_EXTENDED, &CLwbMotorPage::OnBnClickedDoseMotorParameterExtended)
	ON_BN_CLICKED(IDC_LWB_MOTOR_PARAMETER_STANDARD, &CLwbMotorPage::OnBnClickedDoseMotorParameterStandard)
	ON_BN_CLICKED(IDC_LWB_MOTOR_GROSSSTREAM_CTRL, &CLwbMotorPage::OnClickGrossStreamCtrl)
	ON_BN_CLICKED(IDC_LWB_MOTOR_FINESTREAM_CTRL, &CLwbMotorPage::OnClickFineStreamCtrl)
	ON_WM_CTLCOLOR()

END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CLwbMotorPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CLwbMotorPage::CLwbMotorPage() : CDosePage(CLwbMotorPage::IDD)
			, m_MoreLessButton()
			, m_StartupdelayInfoButton()
			, m_MaxrotspeedInfoButton()
			, m_MaxsetpointInfoButton()
			, m_GrossStreamInfoButton()
			, m_FineStreamInfoButton()
			, m_SwitchpointInfoButton()
			, m_EncodermonitorInfoButton()
			, m_ScaleExtInfoButton()
			, m_BatchTimeInfoButton()
			, m_OverrunCompensationInfoButton()
			, m_GrossSpeedCtrl()
			, m_FineSpeedCtrl()
			, m_lStartupDelay(0)
			, m_fMaxRotationalSpeed(0)
			, m_fMaxSetpoint(0)
			, m_lEncoderMonitor(0)
			, m_fSwitchpoint(0.0F)
			, m_fGrossStream(0.0F)
			, m_fFineStream(0.0F)
			, m_lScaleExt{ 0 }
			, m_lBatchTime{ 0 }
			, m_fOverrunCompensation{ 0.0F }
			, m_ParameterOption{ eParamterOption::eNone }
			, m_ParameterOptionLast{ eParamterOption::eNone }
			, m_bRunMode{ FALSE }
			, m_bInit{ FALSE }
			, m_bLogin{ FALSE }
			, m_bMaxRotationalSpeedAllowed{ FALSE }
			, m_EditMap({
						EDITITEM(IDC_LWB_MOTOR_MAXROTSPEED,	OnNotifyMaxRotationalSpeed),
						EDITITEM(IDC_LWB_MOTOR_MAXSETPOINT,	OnNotifyMaxMaxSetpoint),
						EDITITEM(IDC_LWB_MOTOR_ENCODERMONITOR,	OnNotifyEncoderMonitor),
						EDITITEM(IDC_LWB_MOTOR_STARTUPDELAY,	OnNotifyStartupDelay),
						EDITITEM(IDC_LWB_MOTOR_GROSSSTREAM,	OnNotifyBatchGrossStream),
						EDITITEM(IDC_LWB_MOTOR_FINESTREAM,	OnNotifyBatchFineStream),
						EDITITEM(IDC_LWB_MOTOR_SWITCHPOINT,	OnNotifyBatchSwitchpoint),
						EDITITEM(IDC_LWB_MOTOR_SCALE,		OnNotifyBatchScale),
						EDITITEM(IDC_LWB_MOTOR_BATCHTIME,	OnNotifyBatchTime),
						EDITITEM(IDC_LWB_MOTOR_OVERRUNCOMPENSATION,	OnNotifyBatchOverrunCompensation)
				})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CLwbMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_LWB_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_LWB_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_LWB_MOTOR_NR, m_lNr);

	DDX_Float(pDX, IDC_LWB_MOTOR_MAXROTSPEED, m_fMaxRotationalSpeed);
	DDX_Text(pDX, IDC_LWB_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	auto lGrossStream = NEXTLONG(m_fGrossStream);
	DDX_Text(pDX, IDC_LWB_MOTOR_GROSSSTREAM, lGrossStream);
	auto lFineStream = NEXTLONG(m_fFineStream);
	DDX_Text(pDX, IDC_LWB_MOTOR_FINESTREAM, lFineStream);
	DDX_Float(pDX, IDC_LWB_MOTOR_SWITCHPOINT, m_fSwitchpoint);
	DDX_Text(pDX, IDC_LWB_MOTOR_ENCODERMONITOR, m_lEncoderMonitor);
	DDX_Text(pDX, IDC_LWB_MOTOR_STARTUPDELAY, m_lStartupDelay);
	DDX_Text(pDX, IDC_LWB_MOTOR_SCALE, m_lScaleExt);
	DDX_Text(pDX, IDC_LWB_MOTOR_BATCHTIME, m_lBatchTime);
	DDX_Text(pDX, IDC_LWB_MOTOR_OVERRUNCOMPENSATION, m_fOverrunCompensation);
	DDX_Control(pDX, IDC_LWB_MOTOR_CONTROLPARAMETER_MORELESS, m_MoreLessButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_IMAGE, m_aGrafikContainer);

	DDX_Control(pDX, IDC_LWB_MOTOR_MAXSETPOINT_INFO, m_MaxsetpointInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_ENCODERMONITOR_INFO, m_EncodermonitorInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_MAXROTSPEED_INFO, m_MaxrotspeedInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_STARTUPDELAY_INFO, m_StartupdelayInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_GROSSSTREAM_INFO, m_GrossStreamInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_FINESTREAM_INFO, m_FineStreamInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_SWITCHPOINT_INFO, m_SwitchpointInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_SCALE_INFO, m_ScaleExtInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_BATCHTIME_INFO, m_BatchTimeInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_OVERRUNCOMPENSATION_INFO, m_OverrunCompensationInfoButton);
	DDX_Control(pDX, IDC_LWB_MOTOR_GROSSSTREAM_CTRL, m_GrossSpeedCtrl);
	DDX_Control(pDX, IDC_LWB_MOTOR_FINESTREAM_CTRL, m_FineSpeedCtrl);

}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchGrossStream()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fGrossStream);
	if (bModified)
	{
		REMOTEREF.setDoseBatchGrossStream(m_sItem, m_fGrossStream);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchFineStream()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fFineStream);
	if (bModified)
	{
		REMOTEREF.setDoseBatchFineStream(m_sItem, m_fFineStream);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchSwitchpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSwitchpoint);
	if (bModified)
	{
		REMOTEREF.setDoseBatchSwitchpoint(m_sItem, m_fSwitchpoint);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchScale()
{
	auto value = CEditCtrl::GetLong();
	BOOL bModified = (m_lScaleExt != value);
	if (bModified)
	{
		uint32_t count = 0;
		REMOTEREF.getMaxDoseCount(count);

		int32_t scaleId = value - 1;
		if ((scaleId < 0) || (scaleId >= _S32(count)))
		{
			ECMessageBox(_T("Invalid ID"), MB_OK);
		}
		else
		{
			base::ProcessStatus processstatus;
			REMOTEREF.getDoseProcessStatus(scaleId, processstatus);
			BOOL bLCEnable = processstatus.flags.LCEnable && processstatus.flags.LCActive;
			if ( ! bLCEnable)
			{
				ECMessageBox(_T("Loadcell not available"), MB_OK);
			}
			else
			{
				m_lScaleExt = value;
				REMOTEREF.setDoseGwbExtScale(m_sItem, _S32(m_lScaleExt));
			}
		}
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchTime()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lBatchTime);
	if (bModified)
	{
		REMOTEREF.setDoseBatchTime(m_sItem, m_lBatchTime);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyBatchOverrunCompensation()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fOverrunCompensation);
	if (bModified)
	{
		REMOTEREF.setDoseBatchOverrunCompensation(m_sItem, m_fOverrunCompensation);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnNotifyMaxRotationalSpeed()
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
BOOL CLwbMotorPage::OnNotifyMaxMaxSetpoint()
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
BOOL CLwbMotorPage::OnNotifyEncoderMonitor()
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
BOOL CLwbMotorPage::OnNotifyStartupDelay()
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
LRESULT CLwbMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CLwbMotorPage::ShowExtendedStyle(void)
{
	CHECK_ID(IDC_LWB_MOTOR_PARAMETER_EXTENDED, m_ParameterOption == eParamterOption::eExtended);

	const BOOL bEnable = m_bLogin && (!m_bRunMode);
	const BOOL bShowControlParam = ! g_ShowLess;
	const BOOL bExtended = (m_ParameterOption == eParamterOption::eExtended) && bShowControlParam;

	SHOWW_ID(IDC_LWB_MOTOR_PARAMETER_EXTENDED, bShowControlParam);

	SHOWW_ID(IDC_LWB_MOTOR_MAXSETPOINT_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_MAXSETPOINT, bEnable && bExtended, bExtended);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_MAXSETPOINT_INFO, bExtended, bExtended);

	SHOWW_ID(IDC_LWB_MOTOR_MAXROTSPEED_STATIC, bExtended);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_MAXROTSPEED, (!m_bRunMode) && m_bMaxRotationalSpeedAllowed, bExtended);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_MAXROTSPEED_INFO, bExtended, bExtended);

	BOOL bShowExtScale = bExtended && __ISGWBTYPE(m_lDoseType);
	SHOWW_ID(IDC_LWB_MOTOR_SCALE_STATIC, bShowExtScale);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_SCALE, bEnable && bExtended, bShowExtScale);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_SCALE_INFO, bExtended, bShowExtScale);

	BOOL bShowSteepness = bExtended && __ISICBTYPE(m_lDoseType);
	SHOWW_ID(IDC_LWB_MOTOR_STEEPNESSMASSFLOW_STATIC, bShowSteepness);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_STEEPNESSMASSFLOW, bExtended && m_bLogin, bShowSteepness);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_STEEPNESSMASSFLOW_INFO, bExtended, bShowSteepness);


}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::ShowStandardStyle(void)
{
	CHECK_ID(IDC_LWB_MOTOR_PARAMETER_STANDARD, m_ParameterOption == eParamterOption::eStandard);

	const BOOL bShowControlParam = ! g_ShowLess;
	const BOOL bStandard = (m_ParameterOption == eParamterOption::eStandard) && bShowControlParam;

	SHOWW_ID(IDC_LWB_MOTOR_PARAMETER_STANDARD, bShowControlParam);
	// Regelparameter
	SHOWW_ID(IDC_LWB_MOTOR_ENCODERMONITOR_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_ENCODERMONITOR, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_ENCODERMONITOR_INFO, bStandard, bStandard);

	BOOL bShowBatchTime = bStandard && __ISBATCHERTYPE(m_lDoseType);
	SHOWW_ID(IDC_LWB_MOTOR_BATCHTIME_STATIC, bShowBatchTime);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_BATCHTIME, bStandard && m_bLogin, bShowBatchTime);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_BATCHTIME_INFO, bStandard, bShowBatchTime);

	SHOWW_ID(IDC_LWB_MOTOR_STARTUPDELAY_STATIC, bStandard);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_STARTUPDELAY, bStandard && m_bLogin, bStandard);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_STARTUPDELAY_INFO, bStandard, bStandard);

	BOOL bShowOverrunCompensation = bStandard && __ISBATCHERTYPE(m_lDoseType);
	SHOWW_ID(IDC_LWB_MOTOR_OVERRUNCOMPENSATION_STATIC, bShowOverrunCompensation);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_OVERRUNCOMPENSATION, bStandard && m_bLogin, bShowOverrunCompensation);
	ENABLE_SHOW_ID(IDC_LWB_MOTOR_OVERRUNCOMPENSATION_INFO, bStandard, bShowOverrunCompensation);

}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::SetControlStyle (void)
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
	const BOOL bMaxRotationalSpeedAllowed		 = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN);

	ENABLE_ID(IDC_LWB_MOTOR_FINESTREAM, bLogin);
	ENABLE_ID(IDC_LWB_MOTOR_GROSSSTREAM, bLogin);
	ENABLE_ID(IDC_LWB_MOTOR_SWITCHPOINT, m_bLogin);


	if (m_bInit || (m_ParameterOption != m_ParameterOptionLast) || (bRunMode != m_bRunMode) || (bLogin != m_bLogin) || (bMaxRotationalSpeedAllowed != m_bMaxRotationalSpeedAllowed))
	{
		m_bRunMode = bRunMode;
		m_ParameterOptionLast = m_ParameterOption;
		m_bLogin					 = bLogin;
		m_bMaxRotationalSpeedAllowed = bMaxRotationalSpeedAllowed;
		m_bInit = FALSE;

		ShowStandardStyle();
		ShowExtendedStyle();
	}
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBATCHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseBatchGrossStream(m_sItem, m_fGrossStream);
	if (m_GrossSpeedCtrl.IsWindowVisible())
	{
		m_GrossSpeedCtrl.SetSpeed(m_fGrossStream);
	}
	REMOTEREF.getDoseBatchFineStream(m_sItem, m_fFineStream);
	if (m_FineSpeedCtrl.IsWindowVisible())
	{
		m_FineSpeedCtrl.SetSpeed(m_fFineStream);
	}
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseEncoderMonitor(m_sItem, m_lEncoderMonitor);
	REMOTEREF.getDoseStartupDelay(m_sItem, m_lStartupDelay);
	REMOTEREF.getDoseBatchSwitchpoint(m_sItem, m_fSwitchpoint);
	REMOTEREF.getDoseBatchTime(m_sItem, m_lBatchTime);
	REMOTEREF.getDoseGwbExtScale(m_sItem, m_lScaleExt);
	m_lScaleExt++;
	REMOTEREF.getDoseBatchOverrunCompensation(m_sItem, m_fOverrunCompensation);
	SetControlStyle ();
	m_MoreLessButton.ShowLess(g_ShowLess);
	m_MoreLessButton.Show();
	return CDosePage::OnUpdateControls();
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CLwbMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		m_ParameterOption = eParamterOption::eStandard;
		m_ParameterOptionLast = eParamterOption::eNone;
		m_bInit = TRUE;

		INITINFOBUTTON(m_StartupdelayInfoButton);
		INITINFOBUTTON(m_MaxrotspeedInfoButton);
		INITINFOBUTTON(m_GrossStreamInfoButton);
		INITINFOBUTTON(m_FineStreamInfoButton);
		INITINFOBUTTON(m_SwitchpointInfoButton);

		INITINFOBUTTON(m_EncodermonitorInfoButton);
		INITINFOBUTTON(m_MaxsetpointInfoButton);
		INITINFOBUTTON(m_ScaleExtInfoButton);
		INITINFOBUTTON(m_BatchTimeInfoButton);
		INITINFOBUTTON(m_OverrunCompensationInfoButton);

		INITINFOBUTTON(m_MoreLessButton)

			SetNumberFont(IDC_LWB_MOTOR_NR);
		SetValue();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorMaxSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorMaxRotSpeed()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorEncoderMonitor()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWB_MOTOR_ENCODERMONITOR);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorStartupDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWB_MOTOR_STARTUPDELAY);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorGrossStream()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_GROSSSTREAM);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorFineStream()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_FINESTREAM);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorSwitchpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_SWITCHPOINT);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorScale()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWB_MOTOR_SCALE);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorBatchTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWB_MOTOR_BATCHTIME);
}
//**************************************************************************************************************
void CLwbMotorPage::OnStnClickedMotorOverrunCompensation()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_MOTOR_OVERRUNCOMPENSATION);
}
//**************************************************************************************************************
//**************************************************************************************************************
HBRUSH CLwbMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LWB_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_LWB_MOTOR_IMAGE:
                        {
							m_aGrafikContainer.Update(pDC);
                        }
                        break;

	case IDC_LWB_MOTOR_MAXROTSPEED:
	case IDC_LWB_MOTOR_MAXSETPOINT:
	case IDC_LWB_MOTOR_ENCODERMONITOR:
	case IDC_LWB_MOTOR_STARTUPDELAY:
	case IDC_LWB_MOTOR_SWITCHPOINT:
	case IDC_LWB_MOTOR_GROSSSTREAM:
	case IDC_LWB_MOTOR_FINESTREAM:
	case IDC_LWB_MOTOR_SCALE:
	case IDC_LWB_MOTOR_BATCHTIME:
	case IDC_LWB_MOTOR_OVERRUNCOMPENSATION:
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
void CLwbMotorPage::OnClickedDoseMotorMaxrotspeedInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_MAXROTSPEED, IDS_MAXROTSPEED_UPM, IDS_INFO_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorEncodermonitorInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_ENCODERMONITOR, IDS_ENCODERMONITOR_PP, IDS_INFO_ENCODERMONITOR);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorStartupdelayInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_STARTUPDELAY, IDS_STARTUPDELAY_S, IDS_INFO_STARTUPDELAY);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorMaxsetpointInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedMotorGrossStreamInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_GROSSSTREAM, IDS_GROSSSTREAM_P, IDS_INFO_GROSSSTREAM);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedMotorFineStreamInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_FINESTREAM, IDS_FINESTREAM_P, IDS_INFO_FINESTREAM);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedMotorSwitchpointInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_SWITCHPOINT, IDS_SWITCHPOINT_KG, IDS_INFO_SWITCHPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorScaleInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_SCALE, IDS_INFO_TODO, IDS_INFO_TODO);
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorBatchtimeInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_BATCHTIME, IDS_BATCHTIME_S, IDS_INFO_BATCHTIME);
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnClickedDoseMotorOverrunCompensationInfo()
{
	CreateHelpInfoBox(IDC_LWB_MOTOR_OVERRUNCOMPENSATION, IDS_OVERRUNCOMPENSATION_KG, IDS_INFO_OVERRUNCOMPENSATION);
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnBnClickedDoseMotorParameterExtended()
{
	if (m_ParameterOption != eParamterOption::eExtended)
	{
		m_ParameterOption = eParamterOption::eExtended;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnBnClickedDoseMotorParameterStandard()
{
	if (m_ParameterOption != eParamterOption::eStandard)
	{
		m_ParameterOption = eParamterOption::eStandard;
		SetValue();
	}
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnBnClickedControlParamMoreLess()
{
	g_ShowLess = ! g_ShowLess;
	m_bInit = TRUE;
	SetValue();
}
//************************************************************************************
//************************************************************************************
BOOL CLwbMotorPage::OnSetActive()
{
	m_MoreLessButton.Redraw();
	m_GrossSpeedCtrl.Redraw();
	m_FineSpeedCtrl.Redraw();
	m_bInit = TRUE;
	return CDosePage::OnSetActive();
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnClickGrossStreamCtrl()
{
	auto pWnd = GetDlgItem(IDC_LWB_MOTOR_GROSSSTREAM);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CMaxGrossSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 140, cp.y - 80 }, IDS_GROSSSTREAM_SPEED);
}
//************************************************************************************
//************************************************************************************
void CLwbMotorPage::OnClickFineStreamCtrl()
{
	auto pWnd = GetDlgItem(IDC_LWB_MOTOR_FINESTREAM);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CMaxFineSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 140, cp.y - 80 }, IDS_FINESTREAM_SPEED);
}


