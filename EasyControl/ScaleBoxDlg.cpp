//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ScaleBoxDlg
///
/// @file   ScaleBoxDlg.cpp
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
#include "ScaleBoxDlg.h"
#include "ECMessageBox.h"
#include "RemoteControl.h"
#include "FormatMetric.h"
#include "Focus.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"




#define TIMERID 1
#define EDITITEM(_a, _func) 	BINDFUNC(_a, CScaleBoxDlg, _func)



//***********************************************************************************************
//***********************************************************************************************
// CScaleBoxDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CScaleBoxDlg, CBaseClass)
CScaleBoxDlg::CScaleBoxDlg()
	: CBaseClass()
	, c_EditMap({
	EDITITEM(IDC_SCALEBOX_TARAWEIGHT,	OnNotifyEditTaraWeight),
	EDITITEM(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR,	OnNotifyEditCorrectionFactor),
		})
	, c_GateFilterState{{
			// Textcolor     Backhround
			{ RGBCOLORWHITE, RGBCOLORBLACK },		// GATEFILTER_NONE
			{ RGBCOLORWHITE, RGBCOLORGREEN },		// GATEFILTER_GREEN
			{ RGBCOLORBLACK, RGBCOLORYELLOW },		// GATEFILTER_YELLOW
			{ RGBCOLORWHITE, RGBCOLORRED }			// GATEFILTER_RED
		}}
	, c_EditBrush{ EDITBKCOLOR }
	, m_CorrectionFactorInfoButton()
	, m_TareBt{ IDS_LOGINPERMISSION_TARIEREN , IDS_BT_STOP }
	, m_pParent{ nullptr }
	, m_bInit{ FALSE }
	, m_fRawWeight{ 0.0F }
	, m_fWaagenbereich{ 0.0F }
	, m_fNetWeight{ 0.0F }
	, m_fCorrectionFactor{ 0.0F }
	, m_fNoise{ 0.0F }
	, m_fTaraWeight{ 0.0F }
	, m_bAuthorized{ FALSE }
	, m_bLCClr{ FALSE }
	, m_ShowScale{ -1 }
	, m_eScaleSubType{ base::eLcSubType::eUnknown }
	, m_sItem{ INVALID_LC_LINKADDRESS }
{
	for (int32_t i = 0; i < _S32(base::eGatefilter::GATEFILTER_MAX); i++)
	{
		const auto& rItem = c_GateFilterState[i];
		m_aBrush[i].CreateSolidBrush(rItem.second);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CScaleBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SCALEBOX_ZZ_NOISE, m_fNoise);
	DDX_Text(pDX, IDC_SCALEBOX_ZZ_CORRECTIONFACTOR, m_fCorrectionFactor);
	DDX_FloatKg(pDX, IDC_SCALEBOX_SCALERANGE, m_fWaagenbereich);
	DDX_FloatKgHR(pDX, IDC_SCALEBOX_GROSSWEIGHT, m_fRawWeight);
	DDX_FloatKgHR(pDX, IDC_SCALEBOX_NETWEIGHT, m_fNetWeight);
	DDX_Float(pDX, IDC_SCALEBOX_TARAWEIGHT, m_fTaraWeight);
	DDX_Control(pDX, IDC_SCALEBOX_ZZ_CORRECTIONFACTOR_INFO, m_CorrectionFactorInfoButton);
	DDX_Control(pDX, IDC_SCALEBOX_ZZ_NOISE_INFO, m_NoiseInfoButton);
	DDX_Control(pDX, IDC_SCALEBOX_TARE, m_TareBt);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CScaleBoxDlg, CBaseClass)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_SCALEBOX_ZZ_LCLINKADDRESS_CLR, &CScaleBoxDlg::OnBnClickedScaleBoxSensoradresseClr)
	ON_BN_CLICKED(IDC_SCALEBOX_LCLINKADDRESS_SET, &CScaleBoxDlg::OnBnClickedScaleBoxSensoradresseSet)
	ON_BN_CLICKED(IDC_SCALEBOX_TARE, &CScaleBoxDlg::OnBnClickedScaleBoxTare)
	ON_BN_CLICKED(IDC_SCALEBOX_ZZ_AUTHORIZE, &CScaleBoxDlg::OnBnClickedScaleBoxAuthorize)
	ON_BN_CLICKED(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR_INFO, &CScaleBoxDlg::OnClickedScaleBoxCorrectionFactorInfo)
	ON_BN_CLICKED(IDC_SCALEBOX_ZZ_NOISE_INFO, &CScaleBoxDlg::OnClickedScaleBoxNoiseInfo)
	ON_STN_CLICKED(IDC_SCALEBOX_TARAWEIGHT, &CScaleBoxDlg::OnStnClickedScaleBoxTaragewicht)
	ON_STN_CLICKED(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR, &CScaleBoxDlg::OnStnClickedScaleBoxKorrekturfaktor)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
BOOL CScaleBoxDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	ASSERT(result);
	if (result)
	{
		CString szExpandLabel;
		VERIFY(szExpandLabel.LoadString(IDS_MORE));
		CString szCollapseLabel;
		VERIFY(szCollapseLabel.LoadString(IDS_LESS));

		EnableExpand(IDC_SCALEBOX_ZZ, szExpandLabel, szCollapseLabel); //  , _T("&mehr ..."), _T("&weniger ..."));
		Expand(FALSE);

		base::ProcessStatus status;
		REMOTEREF.getDoseProcessStatus(m_sItem, status);
		const BOOL bTareRun = status.flags.tareActive;
		m_TareBt.Init(bTareRun);
		SetValue();
	}
	return result;
}

//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::RemoveBottomControls(std::vector<int32_t>& ids)
{
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	BOOL bLCActive = status.flags.LCActive;

	base::eLcType eTypes;
	REMOTEREF.getDoseLCType(m_sItem, eTypes);
	BOOL bRS484Scale = (eTypes == base::eLcType::LC_PESA) || (eTypes == base::eLcType::LC_SOEMER);

	m_bAuthorized = (USERRIGHTSREF.IsAktSupervisor()) && (LAYOUTCONFIGREF.GetLCAuthorize()) && (bLCActive) && (bRS484Scale);
	if ( ! m_bAuthorized)
	{
		ids.push_back(IDC_SCALEBOX_ZZ_AUTHORIZE);
	}
	const BOOL bSensorLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SENSORADRESSE);
	m_bLCClr = ((bSensorLogin) && (bLCActive));
	if (! m_bLCClr)
	{
		ids.push_back(IDC_SCALEBOX_ZZ_LCLINKADDRESS_CLR);
		ids.push_back(IDC_SCALEBOX_ZZ_LCLINKADDRESS_CLR_STATIC);
	}
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnStnClickedScaleBoxTaragewicht()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_SCALEBOX_TARAWEIGHT);
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnStnClickedScaleBoxKorrekturfaktor()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_SCALEBOX_ZZ_CORRECTIONFACTOR);
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnClickedScaleBoxCorrectionFactorInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_SCALEBOX_ZZ_CORRECTIONFACTOR, IDS_CORRECTIONFACTOR, IDS_INFO_CORRECTIONFACTOR);
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnClickedScaleBoxNoiseInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_SCALEBOX_ZZ_NOISE, IDS_NOISE, IDS_INFO_NOISE);
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnBnClickedScaleBoxSensoradresseSet()
{
	LoadCellSetAddress();
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnBnClickedScaleBoxSensoradresseClr()
{
	LoadCellClrAddress();
}
//**************************************************************************************
//**************************************************************************************
BOOL CScaleBoxDlg::OnNotifyEditTaraWeight()
{
	auto bModified = CEditCtrlEx::GetFloat(m_fTaraWeight);
	if (bModified)
	{
		REMOTEREF.setDoseLCTaraWeight(m_sItem, m_fTaraWeight);
	}
	return bModified;
}
//**************************************************************************************
//**************************************************************************************
BOOL CScaleBoxDlg::OnNotifyEditCorrectionFactor()
{
	auto bModified = CEditCtrlEx::GetFloat(m_fCorrectionFactor);
	if (bModified)
	{
		REMOTEREF.setDoseLCCorrectionFactor(m_sItem, m_fCorrectionFactor);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CScaleBoxDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
		if (bModified)
		{
			SetValue();
		}
	}
	OnSetFocus();
	return 0;
}
//***********************************************************************************************
//***********************************************************************************************
void CScaleBoxDlg::Create(CWnd* pParent, const int32_t index, const CPoint& top)
{
	m_pParent		= pParent;
	m_sItem			= index;

	mfcmacros::SendMessage(pParent, WM_NOTIFYSCALEBOX);

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
void CScaleBoxDlg::OnTimer(UINT_PTR )
{
	if ( ! m_bInit )
	{
		m_bInit = TRUE;
		OnSetFocus();
	}
	if ( ! IsWindowLocked() )
	{
		SetValue();
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CScaleBoxDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_SCALEBOXCLOSED, m_sItem);
	}
	CEditCtrl::Destroy();
	delete this;
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::ShowScaleItems(BOOL bShow)
{
	if (_S32(bShow) == m_ShowScale)
	{
		return;
	}
	m_ShowScale = _S32(bShow);
	SHOWW_ID(IDC_SCALEBOX_SCALERANGE, bShow);
	SHOWW_ID(IDC_SCALEBOX_SCALERANGE_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_GROSSWEIGHT, bShow);
	SHOWW_ID(IDC_SCALEBOX_GROSSWEIGHT_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_NETWEIGHT, bShow);
	SHOWW_ID(IDC_SCALEBOX_NETWEIGHT_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_TARAWEIGHT, bShow);
	SHOWW_ID(IDC_SCALEBOX_TARAWEIGHT_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_NOISE, bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_NOISE_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR, bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR_STATIC, bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR_INFO, bShow);
	SHOWW_ID(IDC_SCALEBOX_LCLINKADDRESS_SET, ! bShow);
	SHOWW_ID(IDC_SCALEBOX_LCLINKADDRESS_SET_STATIC, ! bShow);

	SHOWW_ID(IDC_SCALEBOX_ZZ_LCLINKADDRESS_CLR,			m_bLCClr && bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_LCLINKADDRESS_CLR_STATIC,	m_bLCClr && bShow);
	SHOWW_ID(IDC_SCALEBOX_ZZ_AUTHORIZE,					m_bAuthorized && bShow);

	SHOWW_ID(IDC_SCALEBOX_ZZ, bShow);
	if ( ! bShow)
	{
		SHOWW_ID(IDC_SCALEBOX_TARE, FALSE);
		m_eScaleSubType = base::eLcSubType::eUnknown;
	}
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::SetControlStyleScale(void)
{
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	BOOL bLCAktiv = status.flags.LCActive;

	const BOOL bRunMode = status.flags.started || status.flags.running;
	const BOOL bTareRun = status.flags.tareActive;

	const BOOL bSensorLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SENSORADRESSE);
	const BOOL bTareLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TARIEREN);
	const BOOL bEnable = !bRunMode;

	ENABLE_ID(IDC_SCALEBOX_SCALERANGE, bLCAktiv);
	ENABLE_ID(IDC_SCALEBOX_GROSSWEIGHT, bLCAktiv);
	ENABLE_ID(IDC_SCALEBOX_NETWEIGHT, bLCAktiv);
	ENABLE_ID(IDC_SCALEBOX_ZZ_NOISE, bEnable && bLCAktiv);
	ENABLE_ID(IDC_SCALEBOX_ZZ_CORRECTIONFACTOR, bEnable && bSensorLogin && bLCAktiv);
	ENABLE_ID(IDC_SCALEBOX_TARAWEIGHT, bEnable && bTareLogin && bLCAktiv);

	BOOL bExtendedEnable = bSensorLogin && bEnable;
	ENABLE_ID(IDC_SCALEBOX_LCLINKADDRESS_SET, bExtendedEnable);
	ENABLE_ID(IDC_SCALEBOX_LCLINKADDRESS_SET_STATIC, bExtendedEnable);

	if (bTareLogin && bEnable && bLCAktiv && bTareRun)
	{
		m_TareBt.SetState(CButtonStartStop::eState::eStarted);
	}
	else
	{
		m_TareBt.SetState(CButtonStartStop::eState::eStopped);
	}
	ENABLE_SHOW_ID(IDC_SCALEBOX_TARE, bTareLogin && bEnable && bLCAktiv, (!bRunMode));
	REMOTEREF.getDoseLCTaraWeight(m_sItem, m_fTaraWeight);
	REMOTEREF.getDoseLCCorrectionFactor(m_sItem, m_fCorrectionFactor);

	REMOTEREF.getDoseLCRawWeight(m_sItem, m_fRawWeight);
	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fNetWeight);

	ENABLE_ID(IDC_SCALEBOX_ZZ_AUTHORIZE, m_bAuthorized && bEnable);

	// Waagenbereich
	REMOTEREF.getDoseLCWaagenbereich(m_sItem, m_fWaagenbereich);
	REMOTEREF.getDoseLCActualNoise(m_sItem, m_fNoise);
	m_fNoise = ROUND2(100.0F*m_fNoise);
	REMOTEREF.getDosePidGatefilterState(m_sItem, m_GateFilterState);

	// SubType
	base::eLcSubType eSubType = base::eLcSubType::eUnknown;
	REMOTEREF.getDoseLCSubType(m_sItem, eSubType);
	if (eSubType != m_eScaleSubType)
	{
		m_eScaleSubType = eSubType;
		CString szSubType{ toCString(c_LCSubTypeMap.get(eSubType)) };
		CString szScaleSubType;
		szScaleSubType.Format(L"*** %s ***", LPCTSTR(szSubType));
		ShowHeader(szScaleSubType);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CScaleBoxDlg::GetValue()
{
	UpdateData(TRUE);
}
//******************************************************************************************************
//******************************************************************************************************
void CScaleBoxDlg::SetValue()
{
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	BOOL bScaleEnable = status.flags.LCEnable;
	ShowScaleItems(bScaleEnable);
	if (bScaleEnable)
	{
		SetControlStyleScale();
	}
	UpdateData(FALSE);
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnAfterExpand()
{
	m_ShowScale = -1;
	SetValue();
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnBnClickedScaleBoxAuthorize()
{
	LoadCellAuthorize();
}
//**************************************************************************************
//**************************************************************************************
void CScaleBoxDlg::OnBnClickedScaleBoxTare()
{
	CFocus rFocus(this, m_bWindowLocked);

	const BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TARIEREN);
	if (!bLogin)
	{
		ECMessageBox(IDS_ERROR_LOGINPERMISSION, MB_OK | MB_ICONSTOP);
		return;
	}
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	const BOOL bLCAktiv = status.flags.LCActive;
	if (!bLCAktiv)
	{
		m_TareBt.SetState(CButtonStartStop::eState::eStopped);
		ECMessageBox(IDS_ERROR_MISSING_LOADCELL, MB_ICONSTOP | MB_OK);
		return;
	}
	const BOOL bTareRun = status.flags.tareActive;
	if (!bTareRun)
	{
		if (ECMessageBox(IDS_QUERY_TARASTART, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			REMOTEREF.setDosePBTareStart(m_sItem);
			m_TareBt.SetState(CButtonStartStop::eState::eStarted);
		}
	}
	else
	{
		if (ECMessageBox(IDS_QUERY_TARASTOP, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			REMOTEREF.setDosePBTareStop(m_sItem);
			m_TareBt.SetState(CButtonStartStop::eState::eStopped);
		}
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CScaleBoxDlg::LoadCellAuthorize()
{
	CFocus rFocus(this, m_bWindowLocked);

	REMOTEREF.setDoseCmdLCSuspend();
	BeginWaitCursor();
	auto result = REMOTEREF.setDoseCmdLCPrepareWriteEEprom(m_sItem); // 
	EndWaitCursor();

	if (result != base::LC_ERRORCODE::LC_OK)
	{
		ECMessageBox(IDS_ERROR_REGISTERRESET, MB_ICONSTOP | MB_OK);
		REMOTEREF.setDoseCmdLCResume();
		return;
	}
	ECMessageBox(IDS_ERROR_REGISTERWRITEPROTECTIONOPEN, MB_OK);
	BeginWaitCursor();

	result = REMOTEREF.setDoseCmdLCEnableEEpromWriteProtection(m_sItem, FALSE); // 
	if (result != base::LC_ERRORCODE::LC_OK)
	{
		EndWaitCursor();
		ECMessageBox(IDS_ERROR_REGISTERWRITEPROTECTIONOFF, MB_ICONSTOP | MB_OK);
		REMOTEREF.setDoseCmdLCResume();
		return;
	}
	result = REMOTEREF.setDoseCmdLCRegisterSCL(m_sItem); // 
	if (result != base::LC_ERRORCODE::LC_OK)
	{
		EndWaitCursor();
		ECMessageBox(IDS_ERROR_REGISTERERROR, MB_ICONSTOP | MB_OK);
		REMOTEREF.setDoseCmdLCResume();
		return;
	}
	result = REMOTEREF.setDoseCmdLCEnableEEpromWriteProtection(m_sItem, TRUE); // 
	if (result != base::LC_ERRORCODE::LC_OK)
	{
		EndWaitCursor();
		ECMessageBox(IDS_ERROR_REGISTERWRITEPROTECTIONON, MB_ICONSTOP | MB_OK);
		REMOTEREF.setDoseCmdLCResume();
		return;
	}
	EndWaitCursor();
	REMOTEREF.setDoseCmdLCResume();
	ECMessageBox(IDS_ERROR_REGISTERWRITEPROTECTIONCLOSE, MB_OK);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CScaleBoxDlg::LoadCellClrAddress()
{
	CFocus rFocus(this, m_bWindowLocked);

	BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SENSORADRESSE);
	if (!bLogin)
	{
		ECMessageBox(IDS_ERROR_LOGINPERMISSION, MB_OK | MB_ICONSTOP);
		return;
	}
	GetValue();

	BeginWaitCursor();
	auto result = REMOTEREF.setDoseCmdLCClrAddress(m_sItem); // 
	EndWaitCursor();
	switch (result)
	{
	case base::LC_ERRORCODE::LC_INCOMPATIBLE_SCALES:
		ECMessageBox(IDS_ERROR_LC_INCOMPATIBLE_TYPES, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_NO_DEFINITION:
		ECMessageBox(IDS_ERROR_LC_NO_DEFINITION, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_SRC_BUSY:
		ECMessageBox(IDS_ERROR_LC_SRC_BUSY, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_NO_SOURCE_SCALE:
		ECMessageBox(IDS_ERROR_LC_NO_SCALE, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_OK:
		ECMessageBox(IDS_ERROR_LC_NEWBUSADDRESS, MB_ICONINFORMATION | MB_OK);
		break;
	default:
		ECMessageBox(IDS_ERROR_MPMSETSENORTADRESSE, MB_ICONERROR | MB_OK);
		break;
	}
	if (m_pParent)
	{
		m_pParent->PostMessage(WM_DESTROYSCALEBOX);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CScaleBoxDlg::LoadCellSetAddress()
{
	CFocus rFocus(this, m_bWindowLocked);

	BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SENSORADRESSE);
	if (!bLogin)
	{
		ECMessageBox(IDS_ERROR_LOGINPERMISSION, MB_OK | MB_ICONSTOP);
		return;
	}
	base::ProcessStatus status;
	REMOTEREF.getDoseProcessStatus(m_sItem, status);
	BOOL bSLCActive = status.flags.LCActive;
	if (bSLCActive)
	{
		CString szWaegezelle, szExist, szMessage;
		VERIFY(szWaegezelle.LoadString(IDS_LOADCELL));
		VERIFY(szExist.LoadString(IDS_LOADCELL_ALREADY_EXIST));
		szMessage.Format(_T("%s %d %s"), LPCTSTR(szWaegezelle), int(m_sItem + 1), LPCTSTR(szExist));
		if (ECMessageBox(szMessage, MB_ICONQUESTION | MB_YESNO) != IDYES)
			return;
	}
	GetValue();
	BeginWaitCursor();
	auto result = REMOTEREF.setDoseCmdLCSetAddress(m_sItem); // 
	EndWaitCursor();

	switch (result)
	{
	case base::LC_ERRORCODE::LC_INCOMPATIBLE_SCALES:
		ECMessageBox(IDS_ERROR_LC_INCOMPATIBLE_TYPES, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_NO_DEFINITION:
		ECMessageBox(IDS_ERROR_LC_NO_DEFINITION, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_SRC_BUSY:
		ECMessageBox(IDS_ERROR_LC_SRC_BUSY, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_NO_SOURCE_SCALE:
		ECMessageBox(IDS_ERROR_LC_SRC_BUSY, MB_ICONERROR | MB_OK);
		break;

	case base::LC_ERRORCODE::LC_OK:
		ECMessageBox(IDS_ERROR_LC_NEWBUSADDRESS, MB_ICONINFORMATION | MB_OK);
		Sleep(1000);
		REMOTEREF.setDosePBGravimetric(m_sItem);
		break;
	default:
		ECMessageBox(IDS_ERROR_MPMSETSENORTADRESSE, MB_ICONERROR | MB_OK);
		break;
	}
	SetValue();
}

//*************************************************************************************
//*************************************************************************************
HBRUSH CScaleBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseClass::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_SCALEBOX_ZZ_CORRECTIONFACTOR:
	case IDC_SCALEBOX_TARAWEIGHT:
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

	case IDC_SCALEBOX_ZZ_NOISE:
	{
		ASSERT( (_S32(m_GateFilterState) >= 0) && (_S32(m_GateFilterState) < _S32(base::eGatefilter::GATEFILTER_MAX)));
		const auto& rItem = c_GateFilterState[_S32(m_GateFilterState)];
		// Set the text color to red
		pDC->SetTextColor(rItem.first);

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkColor(rItem.second);
		pDC->SetBkMode(OPAQUE);
		hbr = m_aBrush[_S32(m_GateFilterState)];
	}
	break;


	default:
		break;
	}
	return hbr;
}





