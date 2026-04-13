//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbmMotorPage.cpp
///
/// @file   WbmMotorPage.cpp
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
#include "WbmMotorPage.h"
#include "GlobalConst.h"
#include "BASE/Base.def"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbmMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CWbmMotorPage::OnNotifyEdit)
	ON_MESSAGE(WM_VERWIEGUNGCHANGED, &CWbmMotorPage::OnWeighingChanged)
	ON_STN_CLICKED(IDC_WBM_MOTOR_REDUCTION, &CWbmMotorPage::OnStnClickedWBMMotorReduction)
	ON_STN_CLICKED(IDC_WBM_MOTOR_BELTLOADSETPOINT, &CWbmMotorPage::OnStnClickedWBMMotorBeltLoadSetpoint)
	ON_STN_CLICKED(IDC_WBM_MOTOR_WHEELSIZE, &CWbmMotorPage::OnStnClickedWBMMotorWheelSize)
	ON_STN_CLICKED(IDC_WBM_MOTOR_WAEGESTRECKE, &CWbmMotorPage::OnStnClickedWBMMotorWeighingLine)
	ON_STN_CLICKED(IDC_WBM_MOTOR_MAXROTSPEED, &CWbmMotorPage::OnStnClickedWBMMotorRotationalSpeed)
	ON_STN_CLICKED(IDC_WBM_MOTOR_MAXSETPOINT, &CWbmMotorPage::OnStnClickedWBMMotorBezugsgroesse)
	ON_STN_CLICKED(IDC_WBM_MOTOR_STARTUPDELAY, &CWbmMotorPage::OnStnClickedWBMMotorStartupDelay)
	ON_STN_CLICKED(IDC_WBM_MOTOR_MASSFLOWDAMPING, &CWbmMotorPage::OnStnClickedWBMMotorMassflowDamping)
	ON_STN_CLICKED(IDC_WBM_MOTOR_PIDSAMPLEINTERVAL, &CWbmMotorPage::OnStnClickedWBMMotorPidSampleInterval)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_WBM_MOTOR_PIDSAMPLEINTERVAL_INFO, &CWbmMotorPage::OnBnClickedWBMMotorPidSampleIntervalInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_MASSFLOWDAMPING_INFO, &CWbmMotorPage::OnBnClickedWBMMotorMassflowFilterInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_MAXROTSPEED_INFO, &CWbmMotorPage::OnClickedWBMMotorMaxrotspeedInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_MAXSETPOINT_INFO, &CWbmMotorPage::OnClickedWBMMotorMaxsetpointInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_STARTUPDELAY_INFO, &CWbmMotorPage::OnClickedWBMMotorStartupdelayInfo)

	ON_BN_CLICKED(IDC_WBM_MOTOR_REDUCTION_INFO, &CWbmMotorPage::OnClickedWBMMotorReductionInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_BELTLOADSETPOINT_INFO, &CWbmMotorPage::OnClickedWBMMotorBeltLoadSetpointInfo)
	ON_BN_CLICKED(IDC_WBM_MOTOR_WHEELSIZE_INFO, &CWbmMotorPage::OnClickedWBMMotorWheelSizeInfo)

END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbmMotorPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CWbmMotorPage::CWbmMotorPage() : CDosePage(CWbmMotorPage::IDD)
			, m_SampleIntervalInfoButton()
			, m_StartupdelayInfoButton()
			, m_MaxrotspeedInfoButton()
			, m_MassflowfilterInfoButton()
			, m_MaxsetpointInfoButton()
			, m_ReductionInfoButton()
			, m_BeltLoadSetpointInfoButton()
			, m_WheelSizeInfoButton()
			, m_szWeighingLineName("")
			, m_lStartupDelay(0)
			, m_fMaxRotationalSpeed(0)
            , m_fInputFilter(0)
			, m_fMaxSetpoint(0)
			, m_fReduction(0)
			, m_fBeltLoadSetpoint(0)
			, m_lWheelSize(0)
			, m_fSampleInterval(0.0f)
			, m_lWeighingLine(0)
			, m_fRollenumfang(0.0f)
			, m_bVerwiegungFull(-1)

{}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_WBM_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_WBM_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_WBM_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_WBM_MOTOR_MAXROTSPEED, m_fMaxRotationalSpeed);
	DDX_Text(pDX, IDC_WBM_MOTOR_STARTUPDELAY, m_lStartupDelay);
	DDX_Text(pDX, IDC_WBM_MOTOR_REDUCTION, m_fReduction);
	DDX_Text(pDX, IDC_WBM_MOTOR_BELTLOADSETPOINT, m_fBeltLoadSetpoint);
	DDX_Text(pDX, IDC_WBM_MOTOR_WHEELSIZE, m_lWheelSize);
	DDX_Text(pDX, IDC_WBM_MOTOR_ROLLENUMFANG, m_fRollenumfang);
	DDX_Text(pDX, IDC_WBM_MOTOR_WAEGESTRECKE, m_lWeighingLine);
	DDX_Text(pDX, IDC_WBM_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_WBM_MOTOR_MASSFLOWDAMPING, m_fInputFilter);
	DDX_Text(pDX, IDC_WBM_MOTOR_PIDSAMPLEINTERVAL, m_fSampleInterval);
	DDX_Text(pDX, IDC_WBM_MOTOR_WAEGESTRECKE_NAME, m_szWeighingLineName);
	DDX_Control(pDX, IDC_WBM_MOTOR_BITMAP, m_aGrafikContainer);

	DDX_Control(pDX, IDC_WBM_MOTOR_PIDSAMPLEINTERVAL_INFO, m_SampleIntervalInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_MAXSETPOINT_INFO, m_MaxsetpointInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_MASSFLOWDAMPING_INFO, m_MassflowfilterInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_MAXROTSPEED_INFO, m_MaxrotspeedInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_STARTUPDELAY_INFO, m_StartupdelayInfoButton);

	DDX_Control(pDX, IDC_WBM_MOTOR_REDUCTION_INFO, m_ReductionInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_BELTLOADSETPOINT_INFO, m_BeltLoadSetpointInfoButton);
	DDX_Control(pDX, IDC_WBM_MOTOR_WHEELSIZE_INFO, m_WheelSizeInfoButton);
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CWbmMotorPage::OnWeighingChanged(WPARAM, LPARAM)
{
	int iErg = ECMessageBox(IDS_BANDWAAGE_MOTOR_WAEGESTRECKE_VERWIEGUNG, MB_YESNO|MB_ICONQUESTION);
	REMOTEREF.setDoseWbfVerwiegungFull(m_sItem, BOOL(iErg == IDYES));
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyMassflowFilter()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fInputFilter, 0.0F, 100.0F);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowFilter(m_sItem, m_fInputFilter);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyMaxRotationalSpeed()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxRotationalSpeed, 0.0F, 10000.0F);
	if (bModified)
	{
		REMOTEREF.setDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyStartupDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lStartupDelay, 0, 1000U);
	if (bModified)
	{
		REMOTEREF.setDoseStartupDelay(m_sItem, m_lStartupDelay);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyReduction()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fReduction, 0.0F, 1000.0F);
	if (bModified)
	{
		REMOTEREF.setDoseWbfReduction(m_sItem, m_fReduction);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyBeltLoadSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fBeltLoadSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseWbfBeltLoadSetpoint(m_sItem, m_fBeltLoadSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnNotifyBeltLoadWheelSize()
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
BOOL CWbmMotorPage::OnNotifyWeighingLine()
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
BOOL CWbmMotorPage::OnNotifyMaxSetpoint()
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
BOOL CWbmMotorPage::OnNotifySampleInterval()
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
LRESULT CWbmMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;

		switch (id)
		{
		case IDC_WBM_MOTOR_MASSFLOWDAMPING:	bModified = OnNotifyMassflowFilter();
			break;
		case IDC_WBM_MOTOR_MAXROTSPEED:			bModified = OnNotifyMaxRotationalSpeed();
			break;
		case IDC_WBM_MOTOR_STARTUPDELAY:	bModified = OnNotifyStartupDelay();
			break;
		case IDC_WBM_MOTOR_REDUCTION:		bModified = OnNotifyReduction();
			break;
		case IDC_WBM_MOTOR_BELTLOADSETPOINT:	bModified = OnNotifyBeltLoadSetpoint();
			break;
		case IDC_WBM_MOTOR_WHEELSIZE:	bModified = OnNotifyBeltLoadWheelSize();
			break;
		case IDC_WBM_MOTOR_WAEGESTRECKE:		bModified = OnNotifyWeighingLine();
			break;
		case IDC_WBM_MOTOR_MAXSETPOINT:		bModified = OnNotifyMaxSetpoint();
			break;
		case IDC_WBM_MOTOR_PIDSAMPLEINTERVAL:	bModified = OnNotifySampleInterval();
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		if (bModified)
		{
			SetValue();
			PostMessage(WM_VERWIEGUNGCHANGED);
		}
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::SetControlStyle (void)
{
    const BOOL bSuperVisor		   = USERRIGHTSREF.IsAktSupervisor();
	const BOOL bLogin			   = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS);
	const BOOL bMaxRotationalSpeedalSpeedAllowed = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN);

	base::ProcessStatus pulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, pulStatus);
	const BOOL bRunMode = pulStatus.flags.started;
	const BOOL bEnable = bLogin && (!bRunMode);
 
    // Control
	ENABLE_ID(IDC_WBM_MOTOR_MASSFLOWDAMPING,   bLogin);
	ENABLE_ID(IDC_WBM_MOTOR_MAXROTSPEED,           (!bRunMode) && bMaxRotationalSpeedalSpeedAllowed);
	ENABLE_ID(IDC_WBM_MOTOR_STARTUPDELAY, bEnable);
	ENABLE_ID(IDC_WBM_MOTOR_REDUCTION,       (!bRunMode) && bSuperVisor);
	ENABLE_ID(IDC_WBM_MOTOR_BELTLOADSETPOINT,   bLogin);
	ENABLE_ID(IDC_WBM_MOTOR_WHEELSIZE,   (!bRunMode) && bSuperVisor);
	ENABLE_ID(IDC_WBM_MOTOR_WAEGESTRECKE,       (!bRunMode) && bSuperVisor);
	ENABLE_ID(IDC_WBM_MOTOR_MAXSETPOINT,      bEnable);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	REMOTEREF.getDoseStartupDelay(m_sItem, m_lStartupDelay);
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseWbfReduction(m_sItem, m_fReduction);
	REMOTEREF.getDoseWbfBeltLoadSetpoint(m_sItem, m_fBeltLoadSetpoint);
	REMOTEREF.getDoseWbfWheelSize(m_sItem, m_lWheelSize);
	m_fRollenumfang = UMFANG(m_lWheelSize);
	m_fRollenumfang = ROUND1(m_fRollenumfang);
	REMOTEREF.getDoseWbfWeighingLine(m_sItem, m_lWeighingLine);
	REMOTEREF.getDoseMassflowFilter(m_sItem, m_fInputFilter);
	uint32_t lTemp = 0;
	REMOTEREF.getPidSampleInterval(m_sItem, lTemp);
	m_fSampleInterval			= MS2SEC(lTemp);
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
	return CDosePage::OnUpdateControls();
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	INITINFOBUTTON(m_SampleIntervalInfoButton);
	INITINFOBUTTON(m_MaxsetpointInfoButton);
	INITINFOBUTTON(m_MassflowfilterInfoButton);
	INITINFOBUTTON(m_MaxrotspeedInfoButton);
	INITINFOBUTTON(m_StartupdelayInfoButton);
	INITINFOBUTTON(m_ReductionInfoButton);
	INITINFOBUTTON(m_BeltLoadSetpointInfoButton);
	INITINFOBUTTON(m_WheelSizeInfoButton);

	SetNumberFont(IDC_WBM_MOTOR_NR);
	SetValue();
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorMassflowDamping()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_MASSFLOWDAMPING);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorPidSampleInterval()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_PIDSAMPLEINTERVAL);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorReduction()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_REDUCTION);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorBeltLoadSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_BELTLOADSETPOINT);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorWheelSize()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBM_MOTOR_WHEELSIZE);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorWeighingLine()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBM_MOTOR_WAEGESTRECKE);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorRotationalSpeed()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_MAXROTSPEED);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorBezugsgroesse()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_MOTOR_MAXSETPOINT);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmMotorPage::OnStnClickedWBMMotorStartupDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBM_MOTOR_STARTUPDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CWbmMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBM_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

	case IDC_WBM_MOTOR_MASSFLOWDAMPING:
	case IDC_WBM_MOTOR_MAXROTSPEED:
	case IDC_WBM_MOTOR_STARTUPDELAY:
	case IDC_WBM_MOTOR_REDUCTION:
	case IDC_WBM_MOTOR_BELTLOADSETPOINT:
	case IDC_WBM_MOTOR_WHEELSIZE:
	case IDC_WBM_MOTOR_WAEGESTRECKE:
	case IDC_WBM_MOTOR_PIDSAMPLEINTERVAL:
	case IDC_WBM_MOTOR_MAXSETPOINT:
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

	case IDC_WBM_MOTOR_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

        default:    
                            break;
    }
    return hbr;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorMaxsetpointInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnBnClickedWBMMotorPidSampleIntervalInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_PIDSAMPLEINTERVAL, IDS_MEASURETIME_S, IDS_INFO_PIDSAMPLEINTERVAL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnBnClickedWBMMotorMassflowFilterInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_MASSFLOWDAMPING, IDS_MASSFLOWFILTER_P, IDS_INFO_MASSFLOWFILTER);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorStartupdelayInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_STARTUPDELAY, IDS_STARTUPDELAY_S, IDS_INFO_STARTUPDELAY);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorMaxrotspeedInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_MAXROTSPEED, IDS_MAXROTSPEED_UPM, IDS_INFO_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorReductionInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_REDUCTION, IDS_INFO_TODO, IDS_INFO_TODO);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorBeltLoadSetpointInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_BELTLOADSETPOINT, IDS_INFO_TODO, IDS_INFO_TODO);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CWbmMotorPage::OnClickedWBMMotorWheelSizeInfo()
{
	CreateHelpInfoBox(IDC_WBM_MOTOR_WHEELSIZE, IDS_INFO_TODO, IDS_INFO_TODO);
}


