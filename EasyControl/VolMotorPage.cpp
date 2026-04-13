//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VolMotorPage.cpp
///
/// @file   VolMotorPage.cpp
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
#include "RemoteControl.h"
#include "VolMotorPage.h"
#include "CalibrationVolDlg.h"
#include "ECMessageBox.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC( _a,CVolMotorPage, _func)

//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CVolMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CVolMotorPage::OnNotifyEdit)
	ON_STN_CLICKED(IDC_VOL_MOTOR_DOSEPERFORMANCE_STATIC, &CVolMotorPage::OnStnClickedMotorCalibrate)
	ON_STN_CLICKED(IDC_VOL_MOTOR_MAXSETPOINT_EDIT, &CVolMotorPage::OnStnClickedMaxSetpoint)
	ON_STN_CLICKED(IDC_VOL_MOTOR_DREHZAHL_EDIT, &CVolMotorPage::OnStnClickedMotorRotationalSpeed)
	ON_STN_CLICKED(IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT, &CVolMotorPage::OnStnClickedDoseperformance)
	ON_BN_CLICKED(IDC_VOL_MOTOR_MAXSETPOINT_INFO, &CVolMotorPage::OnClickedMaxSetpointInfo)
	ON_BN_CLICKED(IDC_VOL_MOTOR_DREHZAHL_INFO, &CVolMotorPage::OnClickedMotorRotationalSpeedInfo)
	ON_BN_CLICKED(IDC_VOL_MOTOR_DOSEPERFORMANCE_INFO, &CVolMotorPage::OnClickedMotorDoseperformanceInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CVolMotorPage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CVolMotorPage::CVolMotorPage(): CDosePage(CVolMotorPage::IDD)
	, m_MaxSetpointInfoButton()
	, m_MaxRotationalSpeedInfoButton()
	, m_DoseperformanceInfoButton()
	, m_fMaxSetpoint{ 0.0F }
	, m_fMaxRotationalSpeed{ 0.0F }
	, m_fDoseperformance{ 0.0F }
	, m_EditMap({
				 EDITITEM(IDC_VOL_MOTOR_MAXSETPOINT_EDIT, OnNotifyMaxSetpoint),
				 EDITITEM(IDC_VOL_MOTOR_DREHZAHL_EDIT,	OnNotifyMaxRotationalSpeed),
				 EDITITEM(IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT,	OnNotifyDoseperformance)
		})
{
}
//***************************************************************************************
//***************************************************************************************
void CVolMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_VOL_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_VOL_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_VOL_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_VOL_MOTOR_MAXSETPOINT_EDIT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_VOL_MOTOR_DREHZAHL_EDIT, m_fMaxRotationalSpeed);
	DDX_Text(pDX, IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT, m_fDoseperformance);
	DDX_Control(pDX, IDC_VOL_MOTOR_MAXSETPOINT_INFO, m_MaxSetpointInfoButton);
	DDX_Control(pDX, IDC_VOL_MOTOR_DREHZAHL_INFO, m_MaxRotationalSpeedInfoButton);
	DDX_Control(pDX, IDC_VOL_MOTOR_DOSEPERFORMANCE_INFO, m_DoseperformanceInfoButton);
	DDX_Control(pDX, IDC_VOL_MOTOR_BITMAP, m_aGrafikContainer);
}
//***************************************************************************************
//***************************************************************************************
BOOL CVolMotorPage::OnNotifyMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxSetpoint, 0.0f, 10000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CVolMotorPage::OnNotifyMaxRotationalSpeed()
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
BOOL CVolMotorPage::OnNotifyDoseperformance()
{
	const float32_t c_epsilon = 1e-4F;

	auto bModified = CEditCtrlEx::GetFloatAbs(m_fDoseperformance);
	if (bModified)
	{
		REMOTEREF.setDoseDosePerformance(m_sItem, 0, m_fDoseperformance);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
LRESULT CVolMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::SetControlStyle (void)
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bRunMode = (ulStatus.flags.started == true) || (ulStatus.flags.running == true);
	const BOOL bSuperVisor			= USERRIGHTSREF.IsAktSupervisor();
	const BOOL bLogin				= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS);
	const BOOL bMaxRotationalSpeedalSpeedAllowed	= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN);
	const BOOL bEnable				= bLogin && (!bRunMode);

	ENABLE_ID(IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT, bLogin);
	ENABLE_ID(IDC_VOL_MOTOR_MAXSETPOINT_EDIT, bEnable);
    ENABLE_ID(IDC_VOL_MOTOR_DREHZAHL_EDIT, bEnable && bMaxRotationalSpeedalSpeedAllowed );
	ENABLE_ID(IDC_VOL_MOTOR_DOSEPERFORMANCE_STATIC, bEnable);
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CVolMotorPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISVOLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, m_fMaxRotationalSpeed);
	REMOTEREF.getDoseDosePerformance(m_sItem, 0, m_fDoseperformance);
	SetControlStyle ();
	return CDosePage::OnUpdateControls();
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CVolMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_MaxRotationalSpeedInfoButton);
		INITINFOBUTTON(m_MaxSetpointInfoButton);
		INITINFOBUTTON(m_DoseperformanceInfoButton);

		SetNumberFont(IDC_VOL_MOTOR_NR);
		SetValue();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnStnClickedMotorRotationalSpeed()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_VOL_MOTOR_DREHZAHL_EDIT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnStnClickedMaxSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_VOL_MOTOR_MAXSETPOINT_EDIT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnStnClickedDoseperformance()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT);
}
//**************************************************************************************************************
//**************************************************************************************************************
HBRUSH CVolMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_VOL_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

	case IDC_VOL_MOTOR_MAXSETPOINT_EDIT:
	case IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT:
	case IDC_VOL_MOTOR_DOSEPERFORMANCE_STATIC:
	case IDC_VOL_MOTOR_DREHZAHL_EDIT:
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

	case IDC_VOL_MOTOR_BITMAP:
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
void CVolMotorPage::OnClickedMaxSetpointInfo()
{
	CreateHelpInfoBox(IDC_VOL_MOTOR_MAXSETPOINT_EDIT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnClickedMotorRotationalSpeedInfo()
{
	CreateHelpInfoBox(IDC_VOL_MOTOR_DREHZAHL_EDIT, IDS_MAXROTSPEED_UPM, IDS_INFO_MAXROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnClickedMotorDoseperformanceInfo()
{
	CreateHelpInfoBox(IDC_VOL_MOTOR_DOSEPERFORMANCE_EDIT, IDS_DOSEPERFORMANCE_KGH, IDS_INFO_DOSEPERFORMANCE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolMotorPage::OnStnClickedMotorCalibrate()
{
	CCalibrationVolDlg aDlg(this, m_sItem);

	aDlg.DoModal();
}



