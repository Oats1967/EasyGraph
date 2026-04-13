//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CalibrateDlg
///
/// @file   CalibrateDlg.cpp
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
#include "ECMessageBox.h"
#include "CalibrationVolDlg.h"
#include "CalibrateProgressDlg.h"
#include "BASE/Utils/public/Memory.h"
#include "MFCMacros.h"
#include "FormatMetric.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"




#define MINCALIBTIME 10U
#define MINCALIBWEIGHT 0.1F

#define EDITITEM(_a, _func) 	BINDFUNC(_a, CCalibrationVolDlg, _func)


//*********************************************************************************************************
//*********************************************************************************************************
BEGIN_MESSAGE_MAP(CCalibrationVolDlg, CDialog)
	ON_MESSAGE(WM_NOTIFYEDIT, &CCalibrationVolDlg::OnNotifyEdit)
	ON_BN_CLICKED(IDC_CALIBRATIONVOL_CALIBRATE, &CCalibrationVolDlg::OnClickedCalibrationCalibrate)
	ON_STN_CLICKED(IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT, &CCalibrationVolDlg::OnClickedCalibrationDrivecommand)
	ON_STN_CLICKED(IDC_CALIBRATIONVOL_MEASURETIME_EDIT, &CCalibrationVolDlg::OnClickedCalibrationMeasuretime)
	ON_STN_CLICKED(IDC_CALIBRATIONVOL_WEIGHT_EDIT, &CCalibrationVolDlg::OnClickedCalibrationWeight)
	ON_BN_CLICKED(IDC_CALIBRATIONVOL_WEIGHT_INFO, &CCalibrationVolDlg::OnClickedCalibrationWeightInfo)
	ON_BN_CLICKED(IDC_CALIBRATIONVOL_MEASURETIME_INFO, &CCalibrationVolDlg::OnClickedCalibrationMeasuretimeInfo)
	ON_BN_CLICKED(IDC_CALIBRATIONVOL_DRIVECOMMAND_INFO, &CCalibrationVolDlg::OnClickedCalibrationDrivecommandInfo)
	ON_BN_CLICKED(IDC_CALIBRATIONVOL_DOSEPERFORMANCE_INFO, &CCalibrationVolDlg::OnClickedCalibrationDoseperformanceInfo)
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//*********************************************************************************************************
//*********************************************************************************************************
CCalibrationVolDlg::CCalibrationVolDlg(CWnd* pParent, int32_t id) : CDialog(IDD, pParent)
	, m_DoseperformanceInfoButton()
	, m_MeasuretimeInfoButton()
	, m_WeightInfoButton()
	, m_DriveCommandInfoButton()
	, m_StartBt{ IDS_BT_CALIBVOL_START, IDS_BT_CALIBVOL_STOP }
	, m_fCalibDriveCommand{ 0.0F }
	, m_fDosePerformance{ 0.0F }
	, m_fWeight{ 0.0F }
	, m_lCalibDuration{ 0L }
	, m_bTerminated{ FALSE }
	, m_sItem(id)
	, m_EditMap({
			EDITITEM(IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT,	OnNotifyCalibrationDrivecommand),
			EDITITEM(IDC_CALIBRATIONVOL_MEASURETIME_EDIT,	OnNotifyCalibrationMeasuretime),
			EDITITEM(IDC_CALIBRATIONVOL_WEIGHT_EDIT, OnNotifyCalibrationWeight)
		})
	, c_EditBrush(EDITBKCOLOR)
{
}

//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Float(pDX, IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT, m_fCalibDriveCommand);
	DDX_Text(pDX, IDC_CALIBRATIONVOL_MEASURETIME_EDIT, m_lCalibDuration);
	DDX_Text(pDX, IDC_CALIBRATIONVOL_WEIGHT_EDIT, m_fWeight);
	DDX_Float(pDX, IDC_CALIBRATIONVOL_DOSEPERFORMANCE, m_fDosePerformance);
	DDX_Control(pDX, IDC_CALIBRATIONVOL_DRIVECOMMAND_INFO, m_DriveCommandInfoButton);
	DDX_Control(pDX, IDC_CALIBRATIONVOL_MEASURETIME_INFO, m_MeasuretimeInfoButton);
	DDX_Control(pDX, IDC_CALIBRATIONVOL_WEIGHT_INFO, m_WeightInfoButton);
	DDX_Control(pDX, IDC_CALIBRATIONVOL_DOSEPERFORMANCE_INFO, m_DoseperformanceInfoButton);
	DDX_Control(pDX, IDC_CALIBRATIONVOL_CALIBRATE, m_StartBt);

}
//**************************************************************************************************************
//**************************************************************************************************************
void CCalibrationVolDlg::SetControlStyle(void)
{
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bRun = processstatus.flags.running || processstatus.flags.started;
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_CALIBRATE, (!bRun), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT, !bRun, !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_MEASURETIME_EDIT, (!bRun), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_WEIGHT_EDIT, (!bRun), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_DRIVECOMMAND_STATIC, (!bRun), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_MEASURETIME_STATIC, (!bRun), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATIONVOL_WEIGHT_STATIC, (!bRun), !bRun);

	ENABLE_SHOW_ID(IDOK, (!bRun), !bRun);

	BOOL bCalibrateActive = processstatus.flags.calibrateActive;
	m_StartBt.SetState((bCalibrateActive) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);

}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CCalibrationVolDlg::OnUpdateControls(void)
{
	base::calib::CalibType cfg;
	REMOTEREF.getDoseCalibCfg(m_sItem, cfg);
	ASSERT(cfg.count > 0);

	m_fCalibDriveCommand = cfg.fDriveCommand[0];
	m_fDosePerformance	 = cfg.fDosePerformance[0];
	m_lCalibDuration = __max(cfg.lMeasureTime[0], MINCALIBTIME);
	SetControlStyle();
	return TRUE;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::GetValue()
{
	UpdateData(TRUE);

	base::calib::CalibType cfg;
	cfg.fDriveCommand[0]	= m_fCalibDriveCommand;
	cfg.fDosePerformance[0] = m_fDosePerformance;
	cfg.lMeasureTime[0]		= m_lCalibDuration;
	cfg.count				= 1U;
	REMOTEREF.setDoseCalibCfg(m_sItem, cfg);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::SetValue()
{
	OnUpdateControls();
	UpdateData(FALSE);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationVolDlg::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_DriveCommandInfoButton);
		INITINFOBUTTON(m_MeasuretimeInfoButton);
		INITINFOBUTTON(m_WeightInfoButton);
		INITINFOBUTTON(m_DoseperformanceInfoButton);
		SetValue();
	}
	return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationCalibrate()
{
	BOOL success = FALSE;

	GetValue();
	if (fabs(m_fCalibDriveCommand) < 2.0F)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_INSUFFICIENT_DRIVECOMMAND, MB_ICONSTOP | MB_OK);
		success = FALSE;
	}
	else if (m_lCalibDuration < MINCALIBTIME)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_TIMETOOSHORT, MB_ICONSTOP | MB_OK);
		success = FALSE;
	}
	else
	{
		base::ProcessStatus processstatus;
		REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
		const BOOL bIOReleaseInput = processstatus.flags.releaseext || processstatus.flags.serviceMode;
		if (!bIOReleaseInput)
		{
			ECMessageBox(IDS_ERROR_CALIBRATE_NO_RELEASEEXT, MB_ICONSTOP | MB_OK);
			success = FALSE;
		}
		else
		{
			success = TRUE;
		}
	}
	if (success)
	{
		m_StartBt.SetState(CButtonStartStop::eState::eStarted);

		CCalibrateProgressDlg dlg(this, m_sItem);
		base::calib::CalibType cfg;
		REMOTEREF.getDoseCalibCfg(m_sItem, cfg);
		ASSERT(cfg.count ==1U);

		dlg.SetCalibCfg(cfg);
		dlg.SetCalibIndex(0);
		dlg.DoModal();
		m_StartBt.SetState(CButtonStartStop::eState::eStopped);

		base::eAlarmErrorBits alarmbits;
		REMOTEREF.getDoseAlarmStatus(m_sItem, base::eAlarmLevel::eTotal, alarmbits);
		if (alarmbits.flags.calibReleaseFailed)
		{
			ECMessageBox(IDS_ERROR_CALIBRATE_NO_RELEASEEXT, MB_ICONSTOP | MB_OK);
		}
		else if (alarmbits.flags.calibInvalidSetup)
		{
			ECMessageBox(IDS_ERROR_CALIBRATE_EXECUTE, MB_ICONSTOP | MB_OK);
		}
		else if ( dlg.IsFinished())
		{
			m_bTerminated = TRUE;
			ECMessageBox(IDS_QUERY_EDITWEIGHT, MB_OK);
			CWnd* pBtnB = GetDlgItem(_S32(IDC_CALIBRATIONVOL_WEIGHT_EDIT));
			ASSERT(pBtnB);
			PostMessage(WM_COMMAND,
				MAKEWPARAM(_S32(IDC_CALIBRATIONVOL_WEIGHT_EDIT), STN_CLICKED),
				reinterpret_cast<LPARAM>(pBtnB->GetSafeHwnd()));
		}
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::OnOK()
{
	REMOTEREF.setDoseDosePerformance(m_sItem, 0, m_fDosePerformance);
	CDialog::OnOK();
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationVolDlg::OnNotifyCalibrationDrivecommand()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fCalibDriveCommand, 0, 100.0F);
	if (bModified)
	{
		base::calib::CalibType cfg;
		REMOTEREF.getDoseCalibCfg(m_sItem, cfg);
		cfg.fDriveCommand[0] = m_fCalibDriveCommand;
		REMOTEREF.setDoseCalibCfg(m_sItem, cfg);
	}
	return bModified;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationDrivecommand()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationVolDlg::OnNotifyCalibrationMeasuretime()
{
	auto value = _U32(CEditCtrl::GetLongAbs());
	if (value < MINCALIBTIME)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_INSUFFICIENT_MEASURETIME, MB_ICONSTOP | MB_OK);
		value = MINCALIBTIME;
	}
	BOOL bModified = (value != m_lCalibDuration);
	if (bModified)
	{
		base::calib::CalibType cfg;
		REMOTEREF.getDoseCalibCfg(m_sItem, cfg);
		m_lCalibDuration = cfg.lMeasureTime[0] = value;
		REMOTEREF.setDoseCalibCfg(m_sItem, cfg);
	}
	return bModified;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationMeasuretime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_CALIBRATIONVOL_MEASURETIME_EDIT);
}

//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationVolDlg::OnNotifyCalibrationWeight()
{
	auto value = _F32(fabs(_F32(CEditCtrl::GetFloat())));
	if (value < MINCALIBWEIGHT)
	{
		ECMessageBox(IDS_ERROR_DOSEPERFORMANCE_LOW, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	BOOL bModified = (value != m_fWeight);
	if (bModified)
	{
		m_fWeight = value;

		m_fDosePerformance = (m_fWeight * 360000.0F) / ( _F32(m_lCalibDuration)  * m_fCalibDriveCommand);
		base::calib::CalibType cfg;
		cfg.fDriveCommand[0]		= m_fCalibDriveCommand;
		cfg.fDosePerformance[0]		= m_fDosePerformance;
		cfg.lMeasureTime[0]			= m_lCalibDuration;
		cfg.count = 1U;
		REMOTEREF.setDoseCalibCfg(m_sItem, cfg);
		if (m_bTerminated)
		{
			OnOK();
		}
	}
	return bModified;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationWeight()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_CALIBRATIONVOL_WEIGHT_EDIT);
}
//*********************************************************************************************************
//*********************************************************************************************************
LRESULT CCalibrationVolDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			SetValue();
		}
	}
	return 0;
}
//**************************************************************************************************************
//**************************************************************************************************************
HBRUSH CCalibrationVolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT:
	case IDC_CALIBRATIONVOL_MEASURETIME_EDIT:
	case IDC_CALIBRATIONVOL_WEIGHT_EDIT:
	{
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetBkColor(EDITBKCOLOR);
			pDC->SetBkMode(OPAQUE);
			hbr = (HBRUSH)c_EditBrush;
		}
		pDC->SetTextColor(EDITTEXTCOLOR);
	}
	break;

	default:
		break;
	}
	return hbr;
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationWeightInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATIONVOL_WEIGHT_EDIT, IDS_WEIGHT_KG, IDS_INFO_CALIB_WEIGHT);
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationMeasuretimeInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATIONVOL_MEASURETIME_EDIT, IDS_MEASURETIME_S, IDS_INFO_CALIB_MEASURETIME);
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationDrivecommandInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATIONVOL_DRIVECOMMAND_EDIT, IDS_DRIVECOMMAND_PERCENTAGE, IDS_INFO_CALIB_DRIVECOMMAND);
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationVolDlg::OnClickedCalibrationDoseperformanceInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATIONVOL_DOSEPERFORMANCE, IDS_DOSEPERFORMANCE_KGH, IDS_INFO_DOSEPERFORMANCE );
}


