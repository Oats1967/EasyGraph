//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module CalbrateProgressDlg
///
/// @file   CalbrateProgressDlg.cpp
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
#include "BASE/Base.h"
#include "EasyControl.h"
#include "CalibrateProgressDlg.h"
#include "RemoteControl.h"
#include "ECMessageBox.h"
#include "BASE/EasyTime/public/EasyTime.h"


#define ID_PROGRESS	1


IMPLEMENT_DYNAMIC(CCalibrateProgressDlg, CDialog)
CCalibrateProgressDlg::CCalibrateProgressDlg(CWnd* pParent, int32_t _s)
	: CDialog(CCalibrateProgressDlg::IDD, pParent)
		, m_Cfg{ 0 }
		, m_saveCfg { 0 }
		, m_szDoseFaktor("")
		, m_szDriveCommand("")
        , m_nTimer(NULL)
		, m_lTime{ 0 }
		, m_ProgressStart{ 0 }
		, m_ProgressFinish{ 0 }
		, m_nCurrProgress { 0 }
		, m_sID { _s }
		, m_calibindex { 0 }
{
}
//****************************************************************************************************
//****************************************************************************************************
BEGIN_MESSAGE_MAP(CCalibrateProgressDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()
//****************************************************************************************************
//****************************************************************************************************
BOOL CCalibrateProgressDlg::GetCalibState() const
{
	base::ProcessStatus pulStatus;
	REMOTEREF.getDoseProcessStatus(m_sID, pulStatus);
	return  pulStatus.flags.calibrateActive;
}
//****************************************************************************************************
//****************************************************************************************************
void CCalibrateProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALIBRATE_PROGRESS, m_wndProgress);
	DDX_Text(pDX, IDC_CALIBRATE_PROGRESS_DOSEPERFORMANCE, m_szDoseFaktor);
	DDX_Text(pDX, IDC_CALIBRATE_PROGRESS_DRIVECOMMAND, m_szDriveCommand);
}
//****************************************************************************************************
//****************************************************************************************************
void CCalibrateProgressDlg::OnUpdateProgress()
{
	UpdateData(TRUE);

	CBCGPCircularProgressIndicatorImpl* pProgress = m_wndProgress.GetCircularProgressIndicator();
	ASSERT_VALID(pProgress);

	pProgress->SetColors(CBCGPCircularGaugeColors::BCGP_CIRCULAR_GAUGE_VISUAL_MANAGER);

	CBCGPCircularProgressIndicatorOptions options = pProgress->GetOptions();
	CBCGPCircularProgressIndicatorColors colors;

	options.m_strPercentageFormat.Empty();
	options.m_strProgressLabelTrailingSign.Empty();

	if (TRUE)
	{
		if (TRUE)
		{
			options.m_strPercentageFormat = _T("%1.0f");
			options.m_strProgressLabelTrailingSign = _T("%");
		}
		else
		{
			options.m_strPercentageFormat = _T("%1.0f%%");
		}
	}

	options.m_Shape = CBCGPCircularProgressIndicatorOptions::Shape::BCGPCircularProgressIndicator_Pie;
	options.m_bClockwiseRotation = TRUE;
	options.m_bFadeSize = FALSE;
	options.m_bFadeColor = FALSE;

	if (TRUE)
	{
		colors.m_brFill = CBCGPBrush(CBCGPColor::CornflowerBlue, 0.1);
		colors.m_brFrameOutline = CBCGPBrush();
		colors.m_brProgressLabel = CBCGPColor::Coral;
		colors.m_brProgressLabelTrailingSign = CBCGPColor::PaleVioletRed;
		colors.m_brTextLabel = CBCGPColor::SteelBlue;
		colors.m_brProgressFill = CBCGPColor::CornflowerBlue;
	}

	pProgress->SetOptions(options);
	pProgress->SetColors(colors);
	pProgress->SetRange(m_ProgressStart, m_ProgressFinish);
	pProgress->SetPos(m_nCurrProgress);
}
//****************************************************************************************************
//****************************************************************************************************
uint32_t CCalibrateProgressDlg::GetRunTime() const
{
	uint32_t lTime = 0L;
	if (m_calibindex < 0)
	{
		for (uint32_t i = 0; i < m_Cfg.count; i++)
		{
			lTime += m_Cfg.lMeasureTime[i];
		}
	}
	else if (m_calibindex < _S32(m_Cfg.count))
	{
		lTime += m_Cfg.lMeasureTime[m_calibindex];
	}
	else
	{
	}
	return lTime;
}

//****************************************************************************************************
//****************************************************************************************************
BOOL CCalibrateProgressDlg::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		REMOTEREF.getDoseCalibCfg(m_sID, m_saveCfg);
		m_lTime = GetRunTime();
		ASSERT(m_lTime >= base::calib::MINCALIBTIME);
		if (m_lTime < base::calib::MINCALIBTIME)
		{
			//m_Error = base::calib::eCalibrationError::eTimeout;
			ECMessageBox(IDS_ERROR_CALIBRATE_TIMETOOSHORT, MB_ICONSTOP | MB_OK);
			EndDialog(IDOK);
			return TRUE;
		}

		REMOTEREF.setDoseCalibCfg(m_sID, m_Cfg);
		REMOTEREF.setDoseCalibActiveIndex(m_sID, m_calibindex);
		REMOTEREF.setDosePBCalibStart(m_sID);

		BeginWaitCursor();
		BOOL bCalibrate = FALSE;
		auto dEnd = base::time::GetTimeU64() + 10000U;
		do
		{
			Sleep(1000);
			bCalibrate = GetCalibState();
		} while ((!bCalibrate) && (base::time::GetTimeU64() < dEnd));
		EndWaitCursor();

		if (!bCalibrate)
		{
			ECMessageBox(IDS_ERROR_CALIBRATE_NOT_STARTED, MB_ICONSTOP | MB_OK);
			Stop();
		}
		else
		{
			m_nCurrProgress = 0;
			m_ProgressStart = 0;
			m_ProgressFinish = m_lTime - 1;
			OnUpdateProgress();
			m_nTimer = SetTimer(ID_PROGRESS, 1000, NULL);
		}
	}
	return result;
}
//****************************************************************************************************
//****************************************************************************************************
void CCalibrateProgressDlg::Stop()
{
	if ( m_nTimer )
	{
		KillTimer(ID_PROGRESS);
		m_nTimer = NULL;
	}
	REMOTEREF.getDoseCalibCfg(m_sID, m_Cfg);
	REMOTEREF.setDoseCalibCfg(m_sID, m_saveCfg);
	EndDialog(IDOK);
}
//****************************************************************************************************
//****************************************************************************************************
void CCalibrateProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);

	if (nIDEvent == ID_PROGRESS)
	{
		base::calib::CalibType cfg;
		int32_t aktindex = -1;
		REMOTEREF.getDoseCalibCfg(m_sID, cfg);
		REMOTEREF.getDoseCalibActiveIndex(m_sID, aktindex);

		assert(aktindex >= -1 && aktindex < _S32(cfg.count));
		if (aktindex >= 0)
		{
			m_szDoseFaktor.Format(_T("%3.1f [kg/h]"), cfg.fDosePerformance[aktindex]);
			m_szDriveCommand.Format(_T("%d %%"), _S32(cfg.fDriveCommand[aktindex]));
		}
		if (m_ProgressStart < m_ProgressFinish)
		{
			if (m_nCurrProgress < m_ProgressFinish)
			{
				m_nCurrProgress++;
			}
		}
		else if (m_ProgressStart > m_ProgressFinish)
		{
			if (m_nCurrProgress)
			{
				m_nCurrProgress--;
			}
		}

		m_wndProgress.SetPos(m_nCurrProgress);

		if (m_nCurrProgress == m_ProgressFinish)
		{
			m_nCurrProgress++;
			CBCGPCircularProgressIndicatorImpl* pProgress = m_wndProgress.GetCircularProgressIndicator();
			ASSERT_VALID(pProgress);

			pProgress->FlashProgressLabel();
		}
		BOOL bCalibrate = GetCalibState();
		if ( ! bCalibrate)
		{
			Stop();
		}
	}
	UpdateData(FALSE);
}
//****************************************************************************************************
//****************************************************************************************************
void CCalibrateProgressDlg::OnBnClickedCancel()
{
	if ( ECMessageBox(IDS_QUERY_CALIBRATION_STOP, MB_ICONQUESTION | MB_YESNO) == IDYES)	
	{
		REMOTEREF.setDosePBCalibStop(m_sID);
	}
}

