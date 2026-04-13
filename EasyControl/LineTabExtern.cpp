//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTabExtern
///
/// @file   LineTabExtern.cpp
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
#include "LineTabExtern.h"
#include "ECMessageBox.h"
#include "DataImage.h"
#include "MFCMacros.h"


// CLineTabExtern-Dialog

IMPLEMENT_DYNAMIC(CLineTabExtern, CDialog)

CLineTabExtern::CLineTabExtern(CWnd *pParent)
	: CDialog(CLineTabExtern::IDD, pParent)
	, m_wndCombo()
	, m_LineReleaseBt(IDC_LINETAB_EXTERN_LINERELEASE)
	, m_CurSel(-1)
{
	m_ExtName.SetSize(eExtMax);
	m_ExtName[eExtLineStart]     = _T("Ext.line start");
	m_ExtName[eExtAlarmShutdown] = _T("Ext.alarm shutdown");
	m_ExtName[eExtSetpoint]      = _T("Ext.sensorlevel mode");
	m_ExtName[eExtRampUp]        = _T("Ext.ramp up");
	m_ExtName[eExtRampDown]      = _T("Ext.ramp down");
	m_ExtName[eExtAlarmQuit0]    = _T("Ext.alarm quit 0");
	m_ExtName[eExtAlarmQuit13]   = _T("Ext.alarm quit 1-3");
	m_ExtName[eExtMainFeeder]    = _T("Ext.Mainfeeder");
	m_ExtName[eExtSideFeeder1]   = _T("Ext.Sidefeeder 1");
	m_ExtName[eExtSideFeeder2]   = _T("Ext.Sidefeeder 2");
	m_ExtName[eExtTemplateAlarm] = _T("Ext.customer alarm 1");
	m_ExtName[eExtCustomerAlarm] = _T("Ext.customer alarm 2");
	m_ExtName[eExtAutoOperate]   = _T("Ext. automatic mode");
	m_ExtName[eExtManualOperate] = _T("Ext. manual mode");
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabExtern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINETAB_EXTERN_SIGNALS, m_wndCombo);
	DDX_Control(pDX, IDC_LINETAB_EXTERN_LINERELEASE, m_LineReleaseBt);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineTabExtern, CDialog)
	ON_MESSAGE(WM_TIMER_REFRESH, &CLineTabExtern::OnTimerRefresh)
	ON_CBN_DROPDOWN(IDC_LINETAB_EXTERN_SIGNALS, &CLineTabExtern::OnCbLineDigitalInput)
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_CLICK, &CLineTabExtern::OnGaugeClick)
	ON_CBN_SELCHANGE(IDC_LINETAB_EXTERN_SIGNALS, &CLineTabExtern::OnSelchangeLinetabExternSignals)
END_MESSAGE_MAP()

//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineTabExtern::OnGaugeClick(WPARAM wp, LPARAM)
{
	int32_t id = _S32(wp);
	switch (id)
	{
		case IDC_LINETAB_EXTERN_LINERELEASE:
		{
			UpdateData(TRUE);
			OnClickedLineTabExternLineRelease();
			UpdateData(FALSE);
		}
		break;

		default:
			break;
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabExtern::OnCbLineDigitalInput()
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;
	const auto& processstatus = cache.m_ProcessStatus;
	const auto& alarmstatus	  = cache.m_Alarmstatus;

	m_wndCombo.SetItemImage(eExtLineStart, processstatus.flags.IOlinestart != FALSE);
	m_wndCombo.SetItemImage(eExtAlarmShutdown, processstatus.flags.IOalarmshutdown != FALSE);
	m_wndCombo.SetItemImage(eExtSetpoint, processstatus.flags.IOExtlineSetpoint != FALSE);
	m_wndCombo.SetItemImage(eExtRampUp, processstatus.flags.IOrampup != FALSE);
	m_wndCombo.SetItemImage(eExtRampDown, processstatus.flags.IOrampdown != FALSE);
	m_wndCombo.SetItemImage(eExtAlarmQuit0, alarmstatus.flags.IOalarm03quit != FALSE);
	m_wndCombo.SetItemImage(eExtAlarmQuit13, alarmstatus.flags.IOalarm1quit != FALSE);
	m_wndCombo.SetItemImage(eExtMainFeeder, processstatus.flags.IOmainfeeder != FALSE);
	m_wndCombo.SetItemImage(eExtSideFeeder1, processstatus.flags.IOsidefeeder1 != FALSE);
	m_wndCombo.SetItemImage(eExtSideFeeder2, processstatus.flags.IOsidefeeder2 != FALSE);
	m_wndCombo.SetItemImage(eExtTemplateAlarm, processstatus.flags.IOvorlagenalarm != FALSE);
	m_wndCombo.SetItemImage(eExtCustomerAlarm, processstatus.flags.IOcustomeralarm != FALSE);
	m_wndCombo.SetItemImage(eExtAutoOperate, processstatus.flags.IOAutoOperate != FALSE);
	m_wndCombo.SetItemImage(eExtManualOperate, processstatus.flags.IOManualOperate != FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTabExtern::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_DIGITALINPUTS));

		m_wndCombo.SetPrompt(szTemp);
		m_wndCombo.EnableAutoComplete(TRUE);

		m_wndCombo.ResetContent();
		m_wndCombo.SetImageList(IDB_RADIOOFFON, 18);
		for (int32_t i = 0; i < eExtMax; i++)
		{
			m_wndCombo.InsertString(i, m_ExtName[i]);
		}
		OnUpdateControls();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabExtern::OnSelchangeLinetabExternSignals()
{
	m_CurSel = m_wndCombo.GetCurSel();
}
//***********************************************************************************************
//*********************************************************************************************
void CLineTabExtern::UpdateImage(int32_t pos, BOOL value)
{
	if (m_CurSel == pos)
	{
		BOOL v2 = (m_wndCombo.GetItemImage(pos) != 0);
		if (v2 != value)
		{
			m_wndCombo.SetItemImage(pos, value);
			m_wndCombo.RedrawWindow();
		}
	}
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabExtern::OnUpdateControls()
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& processstatus = aV.m_Cache.m_ProcessStatus;

	const BOOL bLineReleaseEnable = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSE_START));
	const BOOL bLineReleaseShow = bLineReleaseEnable && processstatus.flags.LineReleaseEnable;
	if (bLineReleaseShow)
	{
		BOOL m_bLineReleaseValue = processstatus.flags.lineReleaseState != FALSE;
		m_LineReleaseBt.SetOn(m_bLineReleaseValue);
	}
	ENABLE_SHOW_ID(IDC_LINETAB_EXTERN_LINERELEASE, bLineReleaseEnable, bLineReleaseShow);
	ENABLE_SHOW_ID(IDC_LINETAB_EXTERN_LINERELEASE_STATIC, bLineReleaseEnable, bLineReleaseShow);

	UpdateData(FALSE);
}
//***********************************************************************************************
//*********************************************************************************************
LRESULT CLineTabExtern::OnTimerRefresh(WPARAM, LPARAM)
{
	if (m_CurSel >= 0)
	{
		const auto& aV = dataimage::getLineDataImage();
		const auto& processstatus = aV.m_Cache.m_ProcessStatus;
		const auto& alarmstatus   = aV.m_Cache.m_Alarmstatus;

		UpdateImage(eExtLineStart, processstatus.flags.IOlinestart != FALSE);
		UpdateImage(eExtAlarmShutdown, processstatus.flags.IOalarmshutdown != FALSE);
		UpdateImage(eExtSetpoint, processstatus.flags.IOExtlineSetpoint != FALSE);
		UpdateImage(eExtRampUp, processstatus.flags.IOrampup != FALSE);
		UpdateImage(eExtRampDown, processstatus.flags.IOrampdown != FALSE);
		UpdateImage(eExtAlarmQuit0, alarmstatus.flags.IOalarm03quit != FALSE);
		UpdateImage(eExtAlarmQuit13, alarmstatus.flags.IOalarm1quit != FALSE);
		UpdateImage(eExtMainFeeder, processstatus.flags.IOmainfeeder != FALSE);
		UpdateImage(eExtSideFeeder1, processstatus.flags.IOsidefeeder1 != FALSE);
		UpdateImage(eExtSideFeeder2, processstatus.flags.IOsidefeeder2 != FALSE);
		UpdateImage(eExtTemplateAlarm, processstatus.flags.IOvorlagenalarm != FALSE);
		UpdateImage(eExtCustomerAlarm, processstatus.flags.IOcustomeralarm != FALSE);
		UpdateImage(eExtAutoOperate, processstatus.flags.IOAutoOperate != FALSE);
		UpdateImage(eExtManualOperate, processstatus.flags.IOManualOperate != FALSE);
	}
	OnUpdateControls();
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabExtern::OnClickedLineTabExternLineRelease()
{
	BOOL bValue = m_LineReleaseBt.IsOn();
	REMOTEREF.setLineRelease(bValue);
	m_LineReleaseBt.SetOn(bValue);
	PostMessage(WM_TIMER_REFRESH);
}

