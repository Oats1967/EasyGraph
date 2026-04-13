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
#include "CalibrationDlg.h"
#include "CalibrateProgressDlg.h"
#include "BASE/Utils/public/Memory.h"
#include "MFCMacros.h"
#include "FormatMetric.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"




#define EDITITEM(_a, _func) 	BINDFUNC(_a, CCalibrationDlg, _func)
#define ICON_SIZE 32
#define SWAP(a,b) { auto temp = a; a = b; b = temp; }


//*********************************************************************************************************
//*********************************************************************************************************
BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialog)
	ON_MESSAGE(WM_NOTIFYEDIT, &CCalibrationDlg::OnNotifyEdit)
	ON_BN_CLICKED(IDC_CALIBRATION_ADD, &CCalibrationDlg::OnClickedCalibrationAdd)
	ON_BN_CLICKED(IDC_CALIBRATION_CALIBRATE, &CCalibrationDlg::OnClickedCalibrationCalibrate)
	ON_BN_CLICKED(IDC_CALIBRATION_CALIBRATE_ALL, &CCalibrationDlg::OnClickedCalibrationCalibrateAll)
	ON_BN_CLICKED(IDC_CALIBRATION_DELETE, &CCalibrationDlg::OnClickedCalibrationDelete)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CALIBRATION_LIST, &CCalibrationDlg::OnItemactivateCalibrationList)
	ON_STN_CLICKED(IDC_CALIBRATION_DRIVECOMMAND_EDIT, &CCalibrationDlg::OnClickedCalibrationDrivecommand)
	ON_STN_CLICKED(IDC_CALIBRATION_MEASURETIME_EDIT, &CCalibrationDlg::OnClickedCalibrationMeasuretime)
	ON_STN_CLICKED(IDC_CALIBRATION_DOSEPERFORMANCE_EDIT, &CCalibrationDlg::OnClickedCalibrationDosePerformance)
	ON_BN_CLICKED(IDC_CALIBRATION_MEASURETIME_INFO, &CCalibrationDlg::OnClickedCalibrationMeasuretimeInfo)
	ON_BN_CLICKED(IDC_CALIBRATION_DRIVECOMMAND_INFO, &CCalibrationDlg::OnClickedCalibrationDrivecommandInfo)
	ON_BN_CLICKED(IDC_CALIBRATION_DOSEPERFORMANCE_INFO, &CCalibrationDlg::OnClickedCalibrationDoseperformanceInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//*********************************************************************************************************
//*********************************************************************************************************
CCalibrationDlg::CCalibrationDlg(CWnd* pParent, int32_t id) : CDialog(IDD, pParent)
	, m_DoseperformanceInfoButton()
	, m_MeasuretimeInfoButton()
	, m_DriveCommandInfoButton()
	, m_cfg{ 0 }
	, m_fCalibDriveCommand(0.0F)
	, m_fDosePerformance(0.0F)
	, m_lCalibDuration(0)
	, m_CalibIndex(0)
	, m_sItem(id)
	, c_EditMap({
			EDITITEM(IDC_CALIBRATION_DRIVECOMMAND_EDIT,	OnNotifyCalibrationDrivecommand),
			EDITITEM(IDC_CALIBRATION_MEASURETIME_EDIT,	OnNotifyCalibrationMeasuretime),
			EDITITEM(IDC_CALIBRATION_DOSEPERFORMANCE_EDIT, OnNotifyCalibrationDosePerformance),
		})
	, c_EditBrush(EDITBKCOLOR)
{
	m_aImageList.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR, 0, 1); // 32, 32 for large icons
	CBitmap aBitmap;
	VERIFY(aBitmap.LoadBitmap(IDB_STOPPUHR));
	m_aImageList.Add(&aBitmap, RGB(0,0,0));
}

//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALIBRATION_LIST, m_aCalibrationList);
	DDX_Float(pDX, IDC_CALIBRATION_DRIVECOMMAND_EDIT, m_fCalibDriveCommand);
	DDX_Text(pDX, IDC_CALIBRATION_MEASURETIME_EDIT, m_lCalibDuration);
	DDX_Float(pDX, IDC_CALIBRATION_DOSEPERFORMANCE_EDIT, m_fDosePerformance);
	DDX_Control(pDX, IDC_CALIBRATION_DRIVECOMMAND_INFO, m_DriveCommandInfoButton);
	DDX_Control(pDX, IDC_CALIBRATION_MEASURETIME_INFO, m_MeasuretimeInfoButton);
	DDX_Control(pDX, IDC_CALIBRATION_DOSEPERFORMANCE_INFO, m_DoseperformanceInfoButton);

}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::SetValue()
{
	ASSERT(m_cfg.count > 0);
	m_CalibIndex = __min(m_CalibIndex, m_cfg.count - 1U);
	if (m_CalibIndex >= 0)
	{
		m_fCalibDriveCommand = m_cfg.fDriveCommand[m_CalibIndex];
		m_fDosePerformance = m_cfg.fDosePerformance[m_CalibIndex];
		m_lCalibDuration = m_cfg.lMeasureTime[m_CalibIndex];
		UpdateData(FALSE);
	}
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bRun = processstatus.flags.running || processstatus.flags.started;
	const BOOL bEnable = (!bRun) && (! processstatus.flags.calibrateActive);
	ENABLE_SHOW_ID(IDC_CALIBRATION_CALIBRATE, bEnable && BOOL(m_CalibIndex >= 0), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_DELETE, bEnable && BOOL(m_CalibIndex >= 0) && (m_cfg.count > 1), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_ADD, bEnable && BOOL(m_cfg.count < base::calib::MAXCALIBITEMS), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_CALIBRATE_ALL, bEnable && BOOL(m_cfg.count > 0U), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_DRIVECOMMAND_EDIT, bEnable && BOOL(m_CalibIndex >= 0), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_MEASURETIME_EDIT, bEnable && BOOL(m_CalibIndex >= 0), !bRun);
	ENABLE_SHOW_ID(IDC_CALIBRATION_DOSEPERFORMANCE_EDIT, bEnable && BOOL(m_CalibIndex >= 0), !bRun);

	SHOWW_ID(IDC_CALIBRATION_DOSEPERFORMANCE_STATIC, !bRun);
	SHOWW_ID(IDC_CALIBRATION_DRIVECOMMAND_STATIC, !bRun);
	SHOWW_ID(IDC_CALIBRATION_MEASURETIME_STATIC, !bRun);
	SHOWW_ID(IDC_CALIBRATION_MEASURETIME_INFO, !bRun);
	SHOWW_ID(IDC_CALIBRATION_DRIVECOMMAND_INFO, !bRun);
	SHOWW_ID(IDC_CALIBRATION_DOSEPERFORMANCE_INFO, !bRun);

	ENABLE_SHOW_ID(IDOK, (!bRun), !bRun);

	FillListCtrl(m_aCalibrationList);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationDlg::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_DriveCommandInfoButton);
		INITINFOBUTTON(m_MeasuretimeInfoButton);
		INITINFOBUTTON(m_DoseperformanceInfoButton);

		REMOTEREF.getDoseCalibCfg(m_sItem, m_cfg);
		ASSERT(m_cfg.count > 0);
		m_CalibIndex = 0;
		m_aCalibrationList.SetExtendedStyle(m_aCalibrationList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
		m_aCalibrationList.SetImageList(&m_aImageList, LVSIL_SMALL);

		//CListCtrl& aListCtrl = GetDlgItem();
		BuildHeadLine(m_aCalibrationList);
		SetValue();
	}

	return result;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationAdd()
{
	if (m_cfg.count < base::calib::MAXCALIBITEMS)
	{
		m_cfg.fDosePerformance[m_cfg.count] = 0.0F;
		m_cfg.fDriveCommand[m_cfg.count] = 0.0F;
		m_cfg.lMeasureTime[m_cfg.count] = 30U;
		m_CalibIndex = m_cfg.count;
		m_cfg.count++;
		SetValue();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::Calibrate(const int32_t index)
{
	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL bRefill = status.flags.refillactive;
	if (bRefill)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_REFILL_ACTIVE, MB_ICONSTOP | MB_OK);
		return;
	}
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bIOReleaseInput = processstatus.flags.releaseext || processstatus.flags.serviceMode;
	if (!bIOReleaseInput)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_NO_RELEASEEXT, MB_ICONSTOP | MB_OK);
		return;
	}
	const BOOL bLCAktiv = processstatus.flags.LCActive;
	if (!bLCAktiv)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_NO_LOADCELL, MB_ICONSTOP | MB_OK);
		return;
	}

	CCalibrateProgressDlg dlg(this, m_sItem);
	dlg.SetCalibCfg(m_cfg);
	dlg.SetCalibIndex(index);
	dlg.DoModal();

	base::eAlarmErrorBits alarmbits;
	REMOTEREF.getDoseAlarmStatus(m_sItem, base::eAlarmLevel::eTotal, alarmbits);
	if (alarmbits.flags.calibRefillActive)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_REFILL_ACTIVE, MB_ICONSTOP | MB_OK);
	}
	else if (alarmbits.flags.calibReleaseFailed)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_NO_RELEASEEXT, MB_ICONSTOP | MB_OK);
	} 
	else if ( alarmbits.flags.calibInvalidSetup || alarmbits.flags.calibLoadCell )
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_EXECUTE, MB_ICONSTOP | MB_OK);
	}
	else
	{
		m_cfg = dlg.GetCalibCfg();
		SetValue();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationCalibrate()
{
	BOOL success = TRUE;

	if (m_CalibIndex >= 0)
	{
		if (fabs(m_cfg.fDriveCommand[m_CalibIndex]) < 2.0F)
		{
			ECMessageBox(IDS_ERROR_CALIBRATE_INSUFFICIENT_DRIVECOMMAND, MB_ICONSTOP | MB_OK);
			success = FALSE;
		}
		if (m_cfg.lMeasureTime[m_CalibIndex] < base::calib::MINCALIBTIME)
		{
			CString szTemp;
			ECMessageBox(IDS_ERROR_CALIBRATE_TIMETOOSHORT, MB_ICONSTOP | MB_OK);
			success = FALSE;
		}
	}
	if (success)
	{
		Calibrate(m_CalibIndex);
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationCalibrateAll()
{
	BOOL success = TRUE;

	for ( uint32_t index = 0; index < m_cfg.count; index++)
	{
		if (fabs(m_cfg.fDriveCommand[index]) < 2.0F)
		{
			CString szTemp;
			VERIFY(szTemp.LoadString(IDS_ERROR_CALIBRATE_INSUFFICIENT_DRIVECOMMAND));
			CString szMessage;
			szMessage.Format(_T("%s, %d"), LPCTSTR(szTemp), index+1);
			ECMessageBox(szMessage, MB_ICONSTOP | MB_OK);
			success = FALSE;
			break;
		}
		if ( m_cfg.lMeasureTime[index] < base::calib::MINCALIBTIME)
		{
			CString szTemp;
			VERIFY(szTemp.LoadString(IDS_ERROR_CALIBRATE_TIMETOOSHORT));
			CString szMessage;
			szMessage.Format(_T("%s, %d"), LPCTSTR(szTemp), index + 1);
			ECMessageBox(szMessage, MB_ICONSTOP | MB_OK);
			success = FALSE;
			break;
		}
	}
	if (success)
	{
		Sort(m_cfg);
		Calibrate(-1);
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationDelete()
{
	if (m_cfg.count > 1)
	{
		for (uint32_t k = m_CalibIndex; k < m_cfg.count - 1U; k++)
		{
			m_cfg.fDosePerformance[k] = m_cfg.fDosePerformance[k + 1];
			m_cfg.fDriveCommand[k] = m_cfg.fDriveCommand[k + 1];
			m_cfg.lMeasureTime[k] = m_cfg.lMeasureTime[k + 1];
		}
		m_cfg.count--;
		m_CalibIndex = __min(m_CalibIndex, m_cfg.count - 1U);
		SetValue();
	}
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::Sort(base::calib::CalibType& cfg)
{
	base::calib::CalibType newcfg;

	base::utils::baseZeroMemory(newcfg);
	{
		uint32_t k = 0;
		for (uint32_t index = 0; index < m_cfg.count; index++)
		{
			if (fabs(m_cfg.fDriveCommand[index]) > 1.0F)
			{
				newcfg.fDosePerformance[k] = m_cfg.fDosePerformance[index];
				newcfg.fDriveCommand[k] = m_cfg.fDriveCommand[index];
				newcfg.lMeasureTime[k] = m_cfg.lMeasureTime[index];
				k++;
			}
		}
		newcfg.count = k;
	}

	if (newcfg.count > 1U)
	{
		for (uint32_t i = 0; i < newcfg.count - 1U; i++)
		{
			uint32_t k = i;
			for (uint32_t j = i + 1U; j < newcfg.count; j++)
			{
				if (newcfg.fDriveCommand[j] < newcfg.fDriveCommand[k])
				{
					k = j;
				}
			}
			if (k != i)
			{
				SWAP(newcfg.fDriveCommand[k], newcfg.fDriveCommand[i]);
				SWAP(newcfg.fDosePerformance[k], newcfg.fDosePerformance[i]);
				SWAP(newcfg.lMeasureTime[k], newcfg.lMeasureTime[i]);
			}
		}
	}
	cfg = newcfg;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnOK()
{
	base::calib::CalibType cfg{ m_cfg };
	Sort(cfg);
	if (cfg.count > 0)
	{
		REMOTEREF.setDoseCalibCfg(m_sItem, cfg);
	}
	else
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_INSUFFICIENT_ITEMS, MB_ICONSTOP | MB_OK);
	}
	CDialog::OnOK();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnItemactivateCalibrationList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: Fügen Sie hier Ihren Kontrollbehandlungscode für die Benachrichtigung ein.
	m_CalibIndex = pNMIA->iItem;
	SetValue();
	*pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationDlg::OnNotifyCalibrationDrivecommand()
{
	return CEditCtrlEx::GetFloatAbsRange(m_cfg.fDriveCommand[m_CalibIndex], 0, 100.0F);
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationDrivecommand()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_CALIBRATION_DRIVECOMMAND_EDIT);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationDlg::OnNotifyCalibrationMeasuretime()
{
	auto value = _U32(CEditCtrl::GetLongAbs());
	if (value < base::calib::MINCALIBTIME)
	{
		ECMessageBox(IDS_ERROR_CALIBRATE_INSUFFICIENT_MEASURETIME, MB_ICONSTOP | MB_OK);
		value = base::calib::MINCALIBTIME;
	}
	BOOL bModified = (value != m_cfg.lMeasureTime[m_CalibIndex]);
	if (bModified)
	{
		m_cfg.lMeasureTime[m_CalibIndex] = value;
	}
	return bModified;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationMeasuretime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_CALIBRATION_MEASURETIME_EDIT);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CCalibrationDlg::OnNotifyCalibrationDosePerformance()
{
	auto value = _F32(CEditCtrl::GetFloatAbs());
	if (value < 0.01F)
	{
		ECMessageBox(IDS_ERROR_DOSEPERFORMANCE_LOW, MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	BOOL bModified = (value != m_cfg.fDosePerformance[m_CalibIndex]);
	if (bModified)
	{
		m_cfg.fDosePerformance[m_CalibIndex] = value;
	}
	return bModified;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationDosePerformance()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_CALIBRATION_DOSEPERFORMANCE_EDIT);
}
//*********************************************************************************************************
//*********************************************************************************************************
LRESULT CCalibrationDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = c_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error not found index" << _S32(id));
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
HBRUSH CCalibrationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_CALIBRATION_DRIVECOMMAND_EDIT:
	case IDC_CALIBRATION_MEASURETIME_EDIT:
	case IDC_CALIBRATION_DOSEPERFORMANCE_EDIT:
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
//*****************************************************************************************************
//*****************************************************************************************************
void CCalibrationDlg::BuildHeadLine(CListCtrl& iListCtrl)
{
	LV_COLUMN lvc;
	CString szTemp;


	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.cx = 70;
	lvc.fmt = LVCFMT_RIGHT;
	VERIFY(szTemp.LoadString(IDS_ALARMNR));
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = (int32_t)szTemp.GetLength();
	iListCtrl.InsertColumn(0, &lvc);

	lvc.iSubItem = 1;
	lvc.cx = 100;
	lvc.fmt = LVCFMT_CENTER;
	VERIFY(szTemp.LoadString(IDS_DRIVECOMMAND));
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = (int32_t)szTemp.GetLength();
	iListCtrl.InsertColumn(1, &lvc);

	VERIFY(szTemp.LoadString(IDS_TIME));
	lvc.cx = 40;
	lvc.iSubItem = 2;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = szTemp.GetLength();
	iListCtrl.InsertColumn(2, &lvc);

	lvc.iSubItem = 3;
	VERIFY(szTemp.LoadString(IDS_DOSEPERFORMANCE));
	lvc.cx = 110;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = szTemp.GetLength();
	iListCtrl.InsertColumn(3, &lvc);

}
//*****************************************************************************************************
//*****************************************************************************************************
void CCalibrationDlg::FillListCtrl(CListCtrl& iListCtrl)
{
	LVITEM lvi;

	iListCtrl.DeleteAllItems();
	for (uint32_t i = 0; i < m_cfg.count; i++)
	{
		CString szMessage, szFormat;
		szFormat.Format(_T("%d"), int32_t(i + 1));

		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = LPTSTR(LPCTSTR(szFormat));
		lvi.iImage = 0;
		lvi.stateMask = LVIS_STATEIMAGEMASK;
		lvi.state = INDEXTOSTATEIMAGEMASK(1);
		if (i == m_CalibIndex)
		{
			lvi.stateMask |= LVIS_SELECTED | LVIS_FOCUSED;
			lvi.state	  |= LVIS_SELECTED | LVIS_FOCUSED;
		}
		lvi.cchTextMax = szFormat.GetLength();
		int32_t k = iListCtrl.InsertItem(&lvi);

		CString szDriveCommand;
		szDriveCommand.Format(_T("%3.1f"), m_cfg.fDriveCommand[i]);
		CString szDosePerformance;
		szDosePerformance.Format(_T("%3.1f"), m_cfg.fDosePerformance[i]);
		CString szMeasureTime;
		szMeasureTime.Format(_T("%3d"), m_cfg.lMeasureTime[i]);

		iListCtrl.SetItemText(k, 1, szDriveCommand);
		iListCtrl.SetItemText(k, 2, szMeasureTime);
		iListCtrl.SetItemText(k, 3, szDosePerformance);
	}
	iListCtrl.SetFocus();
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationMeasuretimeInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATION_MEASURETIME_EDIT, IDS_MEASURETIME_S, IDS_INFO_CALIB_MEASURETIME);
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationDrivecommandInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATION_DRIVECOMMAND_EDIT, IDS_DRIVECOMMAND_PERCENTAGE, IDS_INFO_CALIB_DRIVECOMMAND);
}
//********************************************************************************************************
//********************************************************************************************************
void CCalibrationDlg::OnClickedCalibrationDoseperformanceInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_CALIBRATION_DOSEPERFORMANCE_EDIT, IDS_DOSEPERFORMANCE_KGH, IDS_INFO_DOSEPERFORMANCE);
}


