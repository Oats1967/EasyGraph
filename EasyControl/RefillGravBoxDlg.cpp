//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillGravBoxDlg
///
/// @file   RefillGravBoxDlg.cpp
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
#include "RefillGravBoxDlg.h"
#include "ECMessageBox.h"
#include "RemoteControl.h"
#include "EmptyFeederSpeedBoxDlg.h"
#include "RefillFeederSpeedBoxDlg.h"
#include "FeederScaleBoxDlg.h"
#include "FormatMetric.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"
#include "FormatMetric.h"



#define TIMERID 1
#define EDITITEM(_a, _func) 	BINDFUNC(_a, CRefillGravBoxDlg, _func)

CRefillGravBoxDlg* CRefillGravBoxDlg:: g_RefillGravBoxDlg = NULL;

//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CRefillGravBoxDlg::CreateRefillBox(pWnd, index, top, szCaption);
}

//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const CString& szCaption)
{
	if (!g_RefillGravBoxDlg)
	{
		g_RefillGravBoxDlg = new CRefillGravBoxDlg;
		g_RefillGravBoxDlg->Create(pWnd, index, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::DestroyRefillBox(void)
{
	if (g_RefillGravBoxDlg && g_RefillGravBoxDlg->GetSafeHwnd())
	{
		g_RefillGravBoxDlg->PostMessageW(WM_DESTROY);
	}
}

//***********************************************************************************************
//***********************************************************************************************
// CRefillGravBoxDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CRefillGravBoxDlg, CBaseClass)
CRefillGravBoxDlg::CRefillGravBoxDlg()
	: CBaseClass()
	, c_EditMap({
		EDITITEM(IDC_REFILLGRAV_MINLIMIT, OnNotifyEditRefillMin),
		EDITITEM(IDC_REFILLGRAV_MAXLIMIT, OnNotifyEditRefillMax),
		EDITITEM(IDC_REFILLGRAV_ALARMLIMITLIMIT, OnNotifyEditRefillMinMin),
		EDITITEM(IDC_REFILLGRAV_TIME, OnNotifyEditRefillTime),
		EDITITEM(IDC_REFILLGRAV_ZZ_SWITCHDELAY,	OnNotifyEditRefillSwitchDelay),
		})
	, c_EditBrush{ EDITBKCOLOR }
	, m_pParent{ nullptr }
	, m_szCaption { "" }
	, m_bInit{ FALSE }
	, m_MinInfoButton()
	, m_MaxInfoButton()
	, m_MinMinInfoButton()
	, m_RefillTimeInfoButton()
	, m_RefillSwitchDelayInfoButton()
	, m_RefillStartInfoButton()
	, m_EmptyFeederInfoButton()
	, m_RefillStartBt(IDS_BT_REFILL_START, IDS_BT_REFILL_STOP)
	, m_EmptyFeederStartBt(IDS_BT_EMPTYFEEDER_START, IDS_BT_EMPTYFEEDER_STOP)
	, m_EmptyFeederSpeedBt()
	, m_RefillFeederSpeedBt()
	, m_fMax(0.0f)
	, m_fMin(0.0f)
	, m_fMinMin(0.0f)
	, m_lRefillTime(0L)
	, m_lRefillSwitchDelay(0)
	, m_sItem{ 0 }
	, m_fActWeight(0.0f)
	, m_ShowScale{ -1 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_REFILLGRAV_MINLIMIT, m_fMin);
	DDX_Text(pDX, IDC_REFILLGRAV_MAXLIMIT, m_fMax);
	DDX_Text(pDX, IDC_REFILLGRAV_ALARMLIMITLIMIT, m_fMinMin);
	DDX_Text(pDX, IDC_REFILLGRAV_TIME, m_lRefillTime);
	DDX_Text(pDX, IDC_REFILLGRAV_ZZ_SWITCHDELAY, m_lRefillSwitchDelay);

	DDX_Control(pDX, IDC_REFILLGRAV_MINLIMIT_INFO, m_MinInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_MAXLIMIT_INFO, m_MaxInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_ALARMLIMITLIMIT_INFO, m_MinMinInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_TIME_INFO, m_RefillTimeInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_ZZ_SWITCHDELAY_INFO, m_RefillSwitchDelayInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_START_INFO, m_RefillStartInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_START, m_RefillStartBt);
	DDX_Control(pDX, IDC_REFILLGRAV_REFILLFEEDER_SPEED, m_RefillFeederSpeedBt);

	DDX_Control(pDX, IDC_REFILLGRAV_ZZ_EMPTYFEEDER, m_EmptyFeederStartBt);
	DDX_Control(pDX, IDC_REFILLGRAV_ZZ_EMPTYFEEDER_INFO, m_EmptyFeederInfoButton);
	DDX_Control(pDX, IDC_REFILLGRAV_ZZ_EMPTYFEEDER_SPEED, m_EmptyFeederSpeedBt);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CRefillGravBoxDlg, CBaseClass)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SCALEBOXCLOSED, OnScaleBoxClosed)
	ON_MESSAGE(WM_SPEEDBOXCLOSED, OnSpeedBoxClosed)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_REFILLGRAV_MINLIMIT, &CRefillGravBoxDlg::OnStnClickedRefillMin)
	ON_STN_CLICKED(IDC_REFILLGRAV_MAXLIMIT, &CRefillGravBoxDlg::OnStnClickedRefillMax)
	ON_STN_CLICKED(IDC_REFILLGRAV_ALARMLIMITLIMIT, &CRefillGravBoxDlg::OnStnClickedRefillMinMin)
	ON_STN_CLICKED(IDC_REFILLGRAV_TIME, &CRefillGravBoxDlg::OnStnClickedRefillTime)
	ON_STN_CLICKED(IDC_REFILLGRAV_ZZ_SWITCHDELAY, &CRefillGravBoxDlg::OnStnClickedRefillSwitchDelay)
	ON_BN_CLICKED(IDC_REFILLGRAV_START, &CRefillGravBoxDlg::OnBnClickedRefillStart)
	ON_BN_CLICKED(IDC_REFILLGRAV_ZZ_EMPTYFEEDER, &CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStarted)
	ON_BN_CLICKED(IDC_REFILLGRAV_MINLIMIT_INFO, &CRefillGravBoxDlg::OnClickedRefillMinInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_MAXLIMIT_INFO, &CRefillGravBoxDlg::OnClickedRefillMaxInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_ALARMLIMITLIMIT_INFO, &CRefillGravBoxDlg::OnClickedRefillMinMinInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_TIME_INFO, &CRefillGravBoxDlg::OnClickedRefillTimeInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_ZZ_SWITCHDELAY_INFO, &CRefillGravBoxDlg::OnClickedRefillSwitchDelayInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_INFO, &CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStartedInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_START_INFO, &CRefillGravBoxDlg::OnBnClickedRefillInfo)
	ON_BN_CLICKED(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_SPEED, &CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStartedSpeed)
	ON_BN_CLICKED(IDC_REFILLGRAV_REFILLFEEDER_SPEED, &CRefillGravBoxDlg::OnBnClickRefillFeederSpeed)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::Create(CWnd* pParent, const int32_t id, const CPoint& top, const CString& szCaption)
{
	m_pParent = pParent;
	m_sItem = id;
	m_szCaption = szCaption;

	auto result = CBaseClass::Create(IDD, pParent);
	ASSERT(result);
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
void CRefillGravBoxDlg::PostNcDestroy()
{
	CFeederScaleBoxDlg::DestroyScaleBox();
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
void CRefillGravBoxDlg::OnTimer(UINT_PTR)
{
	if ( ! m_bInit)
	{
		m_bInit = TRUE;
		OnSetFocus();
	}
	if ( ! IsWindowLocked())
	{
		SetValue();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillGravBoxDlg::OnNotifyEditRefillMin(void)
{
	BOOL bModified = FALSE;
	auto fMin = CEditCtrl::GetFloatAbs();
	if (fMin > m_fMax)
	{
		ECMessageBox(IDS_INPUTERROR_MINGROESSERMAX, MB_OK | MB_ICONSTOP);
	}
	else if (m_fMinMin > fMin)
	{
		ECMessageBox(IDS_INPUTERROR_MINMINGROESSERMIN, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fMin != fMin);
		if (bModified)
		{
			m_fMin = fMin;
			REMOTEREF.setDoseRefillLimitMin(m_sItem, m_fMin);
		}
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillGravBoxDlg::OnNotifyEditRefillMax()
{
	BOOL bModified = FALSE;
	auto fMax = CEditCtrl::GetFloatAbs();
	if (m_fMin > fMax)
	{
		ECMessageBox(IDS_INPUTERROR_MINGROESSERMAX, MB_OK | MB_ICONSTOP);
	}
	else if (m_fMinMin > fMax)
	{
		ECMessageBox(IDS_INPUTERROR_MINMINGROESSERMAX, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fMax != fMax);
		if (bModified)
		{
			m_fMax = fMax;
			REMOTEREF.setDoseRefillLimitMax(m_sItem, m_fMax);
		}
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillGravBoxDlg::OnNotifyEditRefillMinMin()
{
	BOOL bModified = FALSE;
	auto fMinMin = CEditCtrl::GetFloatAbs();
	if (fMinMin > m_fMax)
	{
		ECMessageBox(IDS_INPUTERROR_MINMINGROESSERMAX, MB_OK | MB_ICONSTOP);
	}
	else  if (fMinMin > m_fMin)
	{
		ECMessageBox(IDS_INPUTERROR_MINMINGROESSERMIN, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fMinMin != fMinMin);
		if (bModified)
		{
			m_fMinMin = fMinMin;
			REMOTEREF.setDoseRefillLimitAlarm(m_sItem, m_fMinMin);
		}
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillGravBoxDlg::OnNotifyEditRefillTime()
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
BOOL CRefillGravBoxDlg::OnNotifyEditRefillSwitchDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_lRefillSwitchDelay);
	if (bModified)
	{
		REMOTEREF.setDoseRefillSwitchDelay(m_sItem, m_lRefillSwitchDelay);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CRefillGravBoxDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
	return 0L;
}
//**************************************************************************************
//**************************************************************************************
LRESULT CRefillGravBoxDlg::OnSpeedBoxClosed(WPARAM , LPARAM )
{
	OnSetFocus();
	SetValue();
	return 0L;
}
//**************************************************************************************
//**************************************************************************************
LRESULT CRefillGravBoxDlg::OnScaleBoxClosed(WPARAM, LPARAM)
{
	OnSetFocus();
	SetValue();
	return 0L;
}
//**************************************************************************************
//**************************************************************************************
void CRefillGravBoxDlg::RemoveBottomControls(std::vector<int32_t>& ids)
{
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bLCAktiv = processstatus.flags.LCActive;
	const BOOL bIOReleaseExt = processstatus.flags.releaseext;
	const BOOL bServiceMode = processstatus.flags.serviceMode;
	const BOOL bServiceModecfg = (SYSTEMCONFIGREF.GetItem(m_sItem).GetEmptyFeederConfig().emptyFeederType == base::eEmptyFeederType::eService);
	m_EmptyFeederSpeedBtEnable = bLCAktiv;
	m_EmptyFeederSpeedBtEnable &= (bServiceMode || ((!bServiceModecfg) && (bIOReleaseExt)));

	if (! m_EmptyFeederSpeedBtEnable)
	{
		ids.push_back(IDC_REFILLGRAV_ZZ_EMPTYFEEDER);
		ids.push_back(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_INFO);
		ids.push_back(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_SPEED);
	}
}
//***************************************************************************************
//***************************************************************************************
void CRefillGravBoxDlg::SetControlStyleScale(void)
{
	float32_t value = 0.0F;
	REMOTEREF.getDoseEmptyFeederSpeed(m_sItem, value);
	m_EmptyFeederSpeedBt.SetSpeed(value);

	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fActWeight);
	REMOTEREF.getDoseRefillLimit(m_sItem, m_fMin, m_fMax, m_fMinMin);
	REMOTEREF.getDoseRefillTime(m_sItem, m_lRefillTime);
	REMOTEREF.getDoseRefillSwitchDelay(m_sItem, m_lRefillSwitchDelay);

	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL bRefillState = status.flags.refillactive;
	//const BOOL bRefillIOReleaseInputern = status.flags.refillreleaseextI;
	const BOOL bRefillRelease = status.flags.refillrelease;

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bAlarmActive = processstatus.flags.alarmactive;
	const BOOL bRefillAlarm = processstatus.flags.refillTimeOut;
	const BOOL bRunning = processstatus.flags.started || processstatus.flags.running;
	const BOOL bTaring = processstatus.flags.tareActive;
	const BOOL bLCAktiv = processstatus.flags.LCActive;
	//const BOOL bRefillControlEnable = processstatus.flags.refillControlEnable;
	const BOOL bIOReleaseExt = processstatus.flags.releaseext;
	const BOOL bServiceMode = processstatus.flags.serviceMode;

	BOOL bLoginRefillgrenzen = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_REFILL_LIMITS);

	ENABLE_ID(IDC_REFILLGRAV_MINLIMIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLGRAV_ALARMLIMITLIMIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLGRAV_MAXLIMIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLGRAV_TIME, bLoginRefillgrenzen);
	ENABLE_ID(IDC_REFILLGRAV_ZZ_SWITCHDELAY, bLoginRefillgrenzen);


	if ( ! bLCAktiv)
	{
		m_RefillStartBt.SetState(CButtonStartStop::eState::eStopped);
		ENABLE_SHOW_ID(IDC_REFILLGRAV_START, FALSE, FALSE);
		ENABLE_SHOW_ID(IDC_REFILLGRAV_START_INFO, FALSE, FALSE);
		ASSERT(!m_EmptyFeederSpeedBtEnable);
	}
	else
	{
		const BOOL bFeederEmpty = processstatus.flags.emptyfeederActive;
		{
			BOOL bShowRefill = (bRefillRelease && (!bFeederEmpty));
			m_RefillStartBt.SetState(((bShowRefill) && (bRefillState)) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);
			ENABLE_SHOW_ID(IDC_REFILLGRAV_START, bShowRefill, bShowRefill);
			ENABLE_SHOW_ID(IDC_REFILLGRAV_START_INFO, bShowRefill, bShowRefill);
		}

		//if ( m_EmptyFeederSpeedBtEnable )
		{
			const BOOL bEnableFeederEmpty = m_EmptyFeederSpeedBtEnable && (!bRunning) && (!bRefillState);
			m_EmptyFeederStartBt.SetState((bFeederEmpty) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);

			ENABLE_ID(IDC_REFILLGRAV_ZZ_EMPTYFEEDER, bEnableFeederEmpty);
			ENABLE_ID(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_INFO, bEnableFeederEmpty);
			ENABLE_ID(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_SPEED, bEnableFeederEmpty);
		}
	}
}
//***************************************************************************************
//***************************************************************************************
void CRefillGravBoxDlg::ShowScaleItems(BOOL bShow)
{
	if (_S32(bShow) == m_ShowScale)
	{
		return;
	}
	m_ShowScale = _S32(bShow);

	m_EmptyFeederStartBt.SetState(CButtonStartStop::eState::eStopped);
	SHOWW_ID(IDC_REFILLGRAV_START, bShow);
	SHOWW_ID(IDC_REFILLGRAV_START_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MINLIMIT, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MINLIMIT_STATIC, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MAXLIMIT, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MAXLIMIT_STATIC, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ALARMLIMITLIMIT, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ALARMLIMITLIMIT_STATIC, bShow);
	SHOWW_ID(IDC_REFILLGRAV_TIME, bShow);
	SHOWW_ID(IDC_REFILLGRAV_TIME_STATIC, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ZZ_SWITCHDELAY, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ZZ_SWITCHDELAY_STATIC, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MINLIMIT_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_MAXLIMIT_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ALARMLIMITLIMIT_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_TIME_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_ZZ_SWITCHDELAY_INFO, bShow);
	SHOWW_ID(IDC_REFILLGRAV_START_INFO, bShow);
}
//***************************************************************************************
//***************************************************************************************
void CRefillGravBoxDlg::SetControlStyle(void)
{
	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bRefillFeederEnable = deviceconfig.flags.refillScrewSpeedEnable;
	ENABLE_SHOW_ID(IDC_REFILLGRAV_REFILLFEEDER_SPEED, bRefillFeederEnable, bRefillFeederEnable);

	if (m_RefillFeederSpeedBt.IsWindowVisible())
	{
		float32_t value = 0.0F;
		REMOTEREF.getDoseRefillFeederSetpoint(m_sItem, value);
		m_RefillFeederSpeedBt.SetSpeed(value);
	}

	if (deviceconfig.flags.refillControlEnable)
	{
		ShowScaleItems(TRUE);
		SetControlStyleScale();
	}
	else
	{
		ShowScaleItems(FALSE);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillGravBoxDlg::SetValue(void)
{
	SetControlStyle();
	UpdateData(FALSE);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CRefillGravBoxDlg::GetValue(void)
{
	UpdateData(TRUE);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CRefillGravBoxDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		ShowHeader(m_szCaption);

		CString szExpandLabel;
		VERIFY(szExpandLabel.LoadString(IDS_MORE));
		CString szCollapseLabel;
		VERIFY(szCollapseLabel.LoadString(IDS_LESS));
		EnableExpand(IDC_REFILLGRAV_ZZ, szExpandLabel, szCollapseLabel);
		Expand(FALSE);

		base::RefillStatus status;
		REMOTEREF.getDoseRefillState(m_sItem, status);
		const BOOL bRefillState = status.flags.refillactive;

		INITINFOBUTTON(m_MinInfoButton);
		INITINFOBUTTON(m_MaxInfoButton);
		INITINFOBUTTON(m_MinMinInfoButton);
		INITINFOBUTTON(m_RefillTimeInfoButton);
		INITINFOBUTTON(m_RefillSwitchDelayInfoButton);
		INITINFOBUTTON(m_RefillStartInfoButton);

		INITINFOBUTTON(m_EmptyFeederInfoButton);

		m_RefillStartBt.Init(bRefillState);
		base::ProcessStatus processstatus;
		REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
		m_EmptyFeederStartBt.Init(BOOL(processstatus.flags.emptyfeederActive));

		SetValue();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillGravBoxDlg::OnActivateWnd()
{
	m_EmptyFeederSpeedBt.Redraw();
	m_RefillFeederSpeedBt.Redraw();
	m_ShowScale = -1;
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnBnClickedRefillStart()
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
		REMOTEREF.setDosePBAlarmClear(m_sItem);
		REMOTEREF.setDosePBRefillStart(m_sItem);
		m_RefillStartBt.SetState(CButtonStartStop::eState::eStarted);
	}
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStarted()
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
void CRefillGravBoxDlg::OnStnClickedRefillMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_REFILLGRAV_MINLIMIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnStnClickedRefillMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_REFILLGRAV_MAXLIMIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnStnClickedRefillMinMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_REFILLGRAV_ALARMLIMITLIMIT);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnStnClickedRefillTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLGRAV_TIME);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnStnClickedRefillSwitchDelay()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_REFILLGRAV_ZZ_SWITCHDELAY);
}
//*************************************************************************************
//*************************************************************************************
HBRUSH CRefillGravBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseClass::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_REFILLGRAV_MINLIMIT:
		case IDC_REFILLGRAV_MAXLIMIT:
		case IDC_REFILLGRAV_ALARMLIMITLIMIT:
		case IDC_REFILLGRAV_TIME:
		case IDC_REFILLGRAV_ZZ_SWITCHDELAY:
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
void CRefillGravBoxDlg::OnClickedRefillMinInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_MINLIMIT, IDS_REFILLLIMITMIN_KG, IDS_INFO_REFILLLIMITMIN);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnClickedRefillMaxInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_MAXLIMIT, IDS_REFILLLIMITMAX_KG, IDS_INFO_REFILLLIMITMAX);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnClickedRefillMinMinInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_ALARMLIMITLIMIT, IDS_REFILLLIMITALARM_KG, IDS_INFO_REFILLLIMITALARM);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnClickedRefillTimeInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_TIME, IDS_REFILLTIME_S, IDS_INFO_REFILLTIME);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnClickedRefillSwitchDelayInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_ZZ_SWITCHDELAY, IDS_REFILLSWITCHDELAY_S, IDS_INFO_REFILLSWITCHDELAY);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStartedInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_ZZ_EMPTYFEEDER, IDS_FEEDEREMPTYSTART, IDS_INFO_FEEDEREMPTY);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnBnClickedRefillInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_REFILLGRAV_START, IDS_REFILLSTART, IDS_INFO_REFILLSTART);
}
//*************************************************************************************
//*************************************************************************************
void CRefillGravBoxDlg::OnBnClickedRefillFeederEmptyStartedSpeed()
{
	auto pWnd = GetDlgItem(IDC_REFILLGRAV_ZZ_EMPTYFEEDER_SPEED);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CEmptyFeederSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 200, cp.y - 80 }, IDS_EMPTYFEEDER_SPEED_P);
}
//***************************************************************************************
//***************************************************************************************
void CRefillGravBoxDlg::OnBnClickRefillFeederSpeed()
{
	auto pWnd = GetDlgItem(IDC_REFILLGRAV_REFILLFEEDER_SPEED);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	auto cp = aRect.TopLeft();
	CRefillFeederSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ cp.x - 200, cp.y - 80 }, IDS_REFILL_REFILLFEEDER);
}
//***************************************************************************************
//***************************************************************************************
void CRefillGravBoxDlg::OnStnClickedRefillScale()
{
	CRect aRect;
	GetWindowRect(aRect);
	CFeederScaleBoxDlg::CreateScaleBox(this, m_sItem, aRect.TopLeft());
}



