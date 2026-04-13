//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillVolBoxDlg
///
/// @file   RefillVolBoxDlg.cpp
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
#include "RefillVolBoxDlg.h"
#include "RemoteControl.h"
#include "RefillFeederSpeedBoxDlg.h"
#include "EmptyFeederSpeedBoxDlg.h"
#include "FormatMetric.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"




#define TIMERID 1

#define EDITITEM(_a, _func) 	BINDFUNC(_a, CRefillVolBoxDlg, _func)

CRefillVolBoxDlg* CRefillVolBoxDlg:: g_RefillVolBoxDlg = NULL;

//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CRefillVolBoxDlg::CreateRefillBox(pWnd, index, top, szCaption);
}

//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const CString& szCaption)
{
	if (!g_RefillVolBoxDlg)
	{
		g_RefillVolBoxDlg = new CRefillVolBoxDlg;
		g_RefillVolBoxDlg->Create(pWnd, index, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::DestroyRefillBox(void)
{
	if (g_RefillVolBoxDlg && g_RefillVolBoxDlg->GetSafeHwnd())
	{
		g_RefillVolBoxDlg->PostMessageW(WM_DESTROY);
	}
}

//***********************************************************************************************
//***********************************************************************************************
// CRefillVolBoxDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CRefillVolBoxDlg, CBaseClass)
CRefillVolBoxDlg::CRefillVolBoxDlg()
	: CBaseClass()
	, c_EditMap({
		EDITITEM(IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT,	OnNotifyEditFeederEmptyStart),
		EDITITEM(IDC_REFILLVOL_TIME_EDIT,	OnNotifyEditRefillTime),
		EDITITEM(IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT,	OnNotifyEditDebounceMax),
		EDITITEM(IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT,	OnNotifyEditDebounceMin),
		})
	, c_EditBrush{ EDITBKCOLOR }
	, m_pParent{ nullptr }
	, m_szCaption { "" }
	, m_bInit{ FALSE }
	, m_EmptyFeederInfoButton()
	, m_MaxDebounceInfoButton()
	, m_MinDebounceInfoButton()
	, m_RefillTimeInfoButton()
	, m_RefillStartInfoButton()
	, m_RefillFeederSpeedBt()
	, m_RefillStartBt(IDS_BT_REFILL_START, IDS_BT_REFILL_STOP)
	, m_EmptyFeederStartBt(IDS_BT_EMPTYFEEDER_START, IDS_BT_EMPTYFEEDER_STOP)
	, m_EmptyFeederSpeedBt()
	, m_sItem{ 0 }
	, m_FeederEmptyStart{ 0 }
	, m_MaxDebounce{ 0 }
	, m_MinDebounce{ 0 }
	, m_lRefillTime{ 0 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT, m_FeederEmptyStart);
	DDX_Text(pDX, IDC_REFILLVOL_TIME_EDIT, m_lRefillTime);
	DDX_Text(pDX, IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT, m_MaxDebounce);
	DDX_Text(pDX, IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT, m_MinDebounce);

	DDX_Control(pDX, IDC_REFILLVOL_FEEDEREMTPYSTART_INFO, m_FeederEmptyStartInfoButton);
	DDX_Control(pDX, IDC_REFILLVOL_TIME_INFO, m_RefillTimeInfoButton);

	DDX_Control(pDX, IDC_REFILLVOL_ZZ_DEBOUNCEMAX_INFO, m_MaxDebounceInfoButton);
	DDX_Control(pDX, IDC_REFILLVOL_ZZ_DEBOUNCEMIN_INFO, m_MinDebounceInfoButton);
	DDX_Control(pDX, IDC_REFILLVOL_START_INFO, m_RefillStartInfoButton);

	DDX_Control(pDX, IDC_REFILLVOL_REFILLFEEDER_SPEED, m_RefillFeederSpeedBt);
	DDX_Control(pDX, IDC_REFILLVOL_START, m_RefillStartBt);

	DDX_Control(pDX, IDC_REFILLVOL_EMPTYFEEDER_SPEED, m_EmptyFeederSpeedBt);
	DDX_Control(pDX, IDC_REFILLVOL_EMPTYFEEDER_INFO, m_EmptyFeederInfoButton);
	DDX_Control(pDX, IDC_REFILLVOL_EMPTYFEEDER, m_EmptyFeederStartBt);

}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CRefillVolBoxDlg, CBaseClass)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SPEEDBOXCLOSED, OnSpeedBoxClosed)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT, &CRefillVolBoxDlg::OnStnClickedFeederEmptyStart)
	ON_STN_CLICKED(IDC_REFILLVOL_TIME_EDIT, &CRefillVolBoxDlg::OnStnClickedRefillTime)
	ON_STN_CLICKED(IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT, &CRefillVolBoxDlg::OnStnDebounceMax)
	ON_STN_CLICKED(IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT, &CRefillVolBoxDlg::OnStnDebounceMin)
	ON_BN_CLICKED(IDC_REFILLVOL_FEEDEREMTPYSTART_INFO, &CRefillVolBoxDlg::OnBnClickedFeederEmptyStartInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_TIME_INFO, &CRefillVolBoxDlg::OnBnClickedRefillTimeInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_ZZ_DEBOUNCEMAX_INFO, &CRefillVolBoxDlg::OnBnClickedDebounceMaxInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_ZZ_DEBOUNCEMIN_INFO, &CRefillVolBoxDlg::OnBnClickedDebounceMinInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_START, &CRefillVolBoxDlg::OnBnClickedRefillStart)
	ON_BN_CLICKED(IDC_REFILLVOL_REFILLFEEDER_SPEED, &CRefillVolBoxDlg::OnBnClickRefillFeederSpeed)
	ON_BN_CLICKED(IDC_REFILLVOL_START_INFO, &CRefillVolBoxDlg::OnBnClickedRefillStartInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_EMPTYFEEDER_INFO, &CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStartedInfo)
	ON_BN_CLICKED(IDC_REFILLVOL_EMPTYFEEDER_SPEED, &CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStartedSpeed)
	ON_BN_CLICKED(IDC_REFILLVOL_EMPTYFEEDER, &CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStarted)


	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::Create(CWnd* pParent, const int32_t id, const CPoint& top, const CString& szCaption)
{
	m_pParent = pParent;
	m_sItem = id;
	m_szCaption = szCaption;

	auto result = CBaseClass::Create(IDD, pParent);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 1000, NULL);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::PostNcDestroy()
{
	CSpeedBoxDlg::DestroySpeedBox();

	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_REFILLBOXCLOSED, m_sItem);
	}
	CEditCtrl::Destroy();
	delete this;
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::OnTimer(UINT_PTR)
{
	if (! m_bInit)
	{
		m_bInit = TRUE;
		OnSetFocus();
	}
	if ( ! IsWindowLocked())
	{
		SetValue();
	}
}
//***************************************************************************************
//***************************************************************************************
LRESULT CRefillVolBoxDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			LOGERROR("Error");
		}
		if ( bModified )
		{
			SetValue();
		}
	}
	OnSetFocus();
	return 0;
}
//**************************************************************************************
//**************************************************************************************
LRESULT CRefillVolBoxDlg::OnSpeedBoxClosed(WPARAM, LPARAM)
{
	OnSetFocus();
	SetValue();
	return 0L;
}

//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillVolBoxDlg::OnNotifyEditFeederEmptyStart()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_FeederEmptyStart);
	if (bModified)
	{
		REMOTEREF.setDoseRefillFeederEmptyStart(m_sItem, m_FeederEmptyStart);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillVolBoxDlg::OnNotifyEditDebounceMax()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_MaxDebounce);
	if (bModified)
	{
		REMOTEREF.setDoseRefillDebounceMax(m_sItem, m_MaxDebounce);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillVolBoxDlg::OnNotifyEditRefillTime()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lRefillTime);
	if (bModified)
	{
		REMOTEREF.setDoseRefillTime(m_sItem, m_lRefillTime);
	}
	return bModified;
}

//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillVolBoxDlg::OnNotifyEditDebounceMin()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_MinDebounce);
	if (bModified)
	{
		REMOTEREF.setDoseRefillDebounceMin(m_sItem, m_MinDebounce);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
void CRefillVolBoxDlg::SetControlStyle(void)
{
	if ( m_EmptyFeederSpeedBt.IsWindowVisible())
	{
		float32_t value = 0.0F;
		REMOTEREF.getDoseEmptyFeederSpeed(m_sItem, value);
		m_EmptyFeederSpeedBt.SetSpeed(value);
	}

	BOOL bLoginRefillgrenzen = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_REFILL_LIMITS);

	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bRefillFeederEnable = deviceconfig.flags.refillScrewSpeedEnable;
	ENABLE_SHOW_ID(IDC_REFILLVOL_REFILLFEEDER_SPEED, bRefillFeederEnable, bRefillFeederEnable);

	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL bRefillState = status.flags.refillactive;
	const BOOL bRefillRelease = status.flags.refillrelease;

	if (m_RefillFeederSpeedBt.IsWindowVisible())
	{
		float32_t value = 0.0F;
		REMOTEREF.getDoseRefillFeederSetpoint(m_sItem, value);
		m_RefillFeederSpeedBt.SetSpeed(value);
	}

	BOOL bShowRefill = bRefillRelease;;
	m_RefillStartBt.SetState(((bShowRefill) && (bRefillState)) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);
	ENABLE_SHOW_ID(IDC_REFILLVOL_START, bShowRefill, bShowRefill);
	ENABLE_SHOW_ID(IDC_REFILLVOL_START_INFO, bShowRefill, bShowRefill);


	REMOTEREF.getDoseRefillDebounceMax(m_sItem, m_MaxDebounce);
	REMOTEREF.getDoseRefillDebounceMin(m_sItem, m_MinDebounce);
	REMOTEREF.getDoseRefillFeederEmptyStart(m_sItem, m_FeederEmptyStart);
	REMOTEREF.getDoseRefillTime(m_sItem, m_lRefillTime);

	ENABLE_ID(IDC_REFILLVOL_TIME_EDIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT, bLoginRefillgrenzen);

	{
		base::ProcessStatus processstatus;
		REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
		const BOOL bAlarmActive = processstatus.flags.alarmactive;
		const BOOL bRunning = processstatus.flags.started || processstatus.flags.running;
		const BOOL bFeederEmpty = processstatus.flags.emptyfeederActive;
		const BOOL bIOReleaseExt = processstatus.flags.releaseext;
		const BOOL bServiceMode = processstatus.flags.serviceMode;

		const BOOL bServiceModecfg = (SYSTEMCONFIGREF.GetItem(m_sItem).GetEmptyFeederConfig().emptyFeederType == base::eEmptyFeederType::eService);
		const BOOL bShowFeederEmpty = (bServiceMode || ((!bServiceModecfg) && (bIOReleaseExt)));
		const BOOL bEnableFeederEmpty = (!bRunning) && (!bRefillState);

		m_EmptyFeederStartBt.SetState((bFeederEmpty) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);

		ENABLE_SHOW_ID(IDC_REFILLVOL_EMPTYFEEDER, bEnableFeederEmpty, bShowFeederEmpty);
		ENABLE_SHOW_ID(IDC_REFILLVOL_EMPTYFEEDER_INFO, bShowFeederEmpty, bShowFeederEmpty);
		ENABLE_SHOW_ID(IDC_REFILLVOL_EMPTYFEEDER_SPEED, bEnableFeederEmpty, bShowFeederEmpty);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillVolBoxDlg::OnActivateWnd()
{
	m_EmptyFeederSpeedBt.Redraw();
	m_RefillFeederSpeedBt.Redraw();
}

//*****************************************************************************************************
//*****************************************************************************************************
void CRefillVolBoxDlg::SetValue(void)
{
	SetControlStyle();
	UpdateData(FALSE);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillVolBoxDlg::GetValue(void)
{
	UpdateData(TRUE);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillVolBoxDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		ASSERT(! m_szCaption.IsEmpty());
		ShowHeader(m_szCaption);

		CString szExpandLabel;
		VERIFY(szExpandLabel.LoadString(IDS_MORE));
		CString szCollapseLabel;
		VERIFY(szCollapseLabel.LoadString(IDS_LESS));
		EnableExpand(IDC_REFILLVOL_ZZ, szExpandLabel, szCollapseLabel);
		Expand(FALSE);

		INITINFOBUTTON(m_FeederEmptyStartInfoButton);
		INITINFOBUTTON(m_MaxDebounceInfoButton);
		INITINFOBUTTON(m_MinDebounceInfoButton);
		INITINFOBUTTON(m_RefillTimeInfoButton);
		INITINFOBUTTON(m_RefillStartInfoButton);
		INITINFOBUTTON(m_EmptyFeederInfoButton);


		INITINFOBUTTON(m_FeederEmptyStartInfoButton);

		base::RefillStatus status;
		REMOTEREF.getDoseRefillState(m_sItem, status);
		const BOOL bRefillState = status.flags.refillactive;
		m_RefillStartBt.Init(bRefillState);

		base::ProcessStatus processstatus;
		REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
		m_EmptyFeederStartBt.Init(BOOL(processstatus.flags.emptyfeederActive));

		SetValue();
	}
	return result;
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnStnClickedFeederEmptyStart()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnStnDebounceMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnStnDebounceMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnStnClickedRefillTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLVOL_TIME_EDIT);
}
//*************************************************************************************
//*************************************************************************************
HBRUSH CRefillVolBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseClass::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT:
		case IDC_REFILLVOL_TIME_EDIT:
		case IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT:
		case IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT:
		{
			if (pWnd->IsWindowEnabled())
			{
				pDC->SetTextColor(EDITTEXTCOLOR);
				pDC->SetBkColor(EDITBKCOLOR);
				pDC->SetBkMode(OPAQUE);
				hbr = (HBRUSH)c_EditBrush;
			}
		}
		break;

	default:
		break;
	}
	return hbr;
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedFeederEmptyStartInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_FEEDEREMTPYSTART_EDIT, IDS_VOLEMPTYFEEDERSTART_S, IDS_INFO_VOLEMPTYFEEDERSTART);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedDebounceMaxInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_ZZ_DEBOUNCEMAX_EDIT, IDS_DEBOUNCEMAX_S, IDS_INFO_DEBOUNCE);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedDebounceMinInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_ZZ_DEBOUNCEMIN_EDIT, IDS_DEBOUNCEMIN_S, IDS_INFO_DEBOUNCE);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillTimeInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_TIME_EDIT, IDS_REFILLTIME_S, IDS_INFO_REFILLTIME);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillStartInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_START, IDS_REFILLSTART, IDS_INFO_REFILLSTART);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStartedInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLVOL_EMPTYFEEDER, IDS_FEEDEREMPTYSTART, IDS_INFO_FEEDEREMPTY);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStarted()
{
	GetValue();
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	const BOOL bEmptyFeeder = status.flags.emptyfeederActive;
	if (bEmptyFeeder)
	{
		REMOTEREF.setDosePBEmptyFeederStop(m_sItem);
		m_EmptyFeederStartBt.SetState(CButtonStartStop::eState::eStopped);
	}
	else
	{
		LockWindow();
		if (ECMessageBox(IDS_QUERYREALLYEMPTYFEEDER, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			REMOTEREF.setDosePBEmptyFeederStart(m_sItem);
			m_EmptyFeederStartBt.SetState(CButtonStartStop::eState::eStarted);
		}
		UnlockWindow();
	}
}

//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillStart()
{
	GetValue();
	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL RefillActive = status.flags.refillactive;
	if (RefillActive)
	{
		REMOTEREF.setDosePBRefillStop(m_sItem);
		m_RefillStartBt.SetState(CButtonStartStop::eState::eStopped);
	}
	else
	{
		REMOTEREF.setDosePBRefillStart(m_sItem);
		m_RefillStartBt.SetState(CButtonStartStop::eState::eStarted);
	}
}
//***************************************************************************************
//***************************************************************************************
void CRefillVolBoxDlg::OnBnClickRefillFeederSpeed()
{
	auto pWnd = GetDlgItem(IDC_REFILLVOL_REFILLFEEDER_SPEED);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CRefillFeederSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 140, cp.y - 80 }, IDS_REFILL_REFILLFEEDER);
}
//*************************************************************************************
//*************************************************************************************
void CRefillVolBoxDlg::OnBnClickedRefillFeederEmptyStartedSpeed()
{
	auto pWnd = GetDlgItem(IDC_REFILLVOL_EMPTYFEEDER_SPEED);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CEmptyFeederSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 200, cp.y - 80 }, IDS_EMPTYFEEDER_SPEED_P);
}





