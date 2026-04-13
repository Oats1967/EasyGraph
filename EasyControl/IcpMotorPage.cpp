//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IcpMotorPage.cpp
///
/// @file   IcpMotorPage.cpp
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
#include "IcpMotorPage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIcpMotorPage, _func)


//****************************************************************************************
//****************************************************************************************
BEGIN_MESSAGE_MAP(CIcpMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_ICP_MOTOR_MAXSETPOINT, &CIcpMotorPage::OnStnClickedIcpMaxSetpoint)
	ON_STN_CLICKED(IDC_ICP_MOTOR_STEEPNESSMASSFLOW, &CIcpMotorPage::OnStnClickedIcpMotorSteepnessMassflow)
	ON_BN_CLICKED(IDC_ICP_MOTOR_MAXSETPOINT_INFO, &CIcpMotorPage::OnClickedIcpMaxSetpointInfo)
	ON_BN_CLICKED(IDC_ICP_MOTOR_STEEPNESSMASSFLOW_INFO, &CIcpMotorPage::OnClickedIcpMotorSteilheitistwertInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//****************************************************************************************
//****************************************************************************************
IMPLEMENT_DYNAMIC(CIcpMotorPage, CDosePage)
//****************************************************************************************
//****************************************************************************************
CIcpMotorPage::CIcpMotorPage(): CDosePage(CIcpMotorPage::IDD)
	, m_MaxsetpointInfoButton ()
	, m_SteepnessMassflowInfoButton()
	, m_fMaxSetpoint{ 0.0F }
	, m_fSteepnessMassflow{ 0.0F }
	, m_EditMap({
			EDITITEM(IDC_ICP_MOTOR_MAXSETPOINT,	OnNotifyMaxSetpoint),
			EDITITEM(IDC_ICP_MOTOR_STEEPNESSMASSFLOW, OnNotifySteepnessMassflow),
		})
{}
//****************************************************************************************
//****************************************************************************************
void CIcpMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ICP_MOTOR_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_ICP_MOTOR_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_ICP_MOTOR_NR, m_lNr);

	DDX_Text(pDX, IDC_ICP_MOTOR_MAXSETPOINT, m_fMaxSetpoint);
	DDX_Text(pDX, IDC_ICP_MOTOR_STEEPNESSMASSFLOW, m_fSteepnessMassflow);
	DDX_Control(pDX, IDC_ICP_MOTOR_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_ICP_MOTOR_MAXSETPOINT_INFO, m_MaxsetpointInfoButton);
	DDX_Control(pDX, IDC_ICP_MOTOR_STEEPNESSMASSFLOW_INFO, m_SteepnessMassflowInfoButton);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIcpMotorPage::OnNotifyMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxSetpoint, 0.1f, 10000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIcpMotorPage::OnNotifySteepnessMassflow()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fSteepnessMassflow, 0.1f, 100.0f);
	if (bModified)
	{
		REMOTEREF.setDoseMassflowSteepness(m_sItem, m_fSteepnessMassflow);
	}
	return bModified;
}
//****************************************************************************************
//****************************************************************************************
LRESULT CIcpMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
//****************************************************************************************
//****************************************************************************************
void CIcpMotorPage::SetControlStyle (void)
{
	base::ProcessStatus pulStatus;

	REMOTEREF.getDoseProcessStatus(m_sItem, pulStatus);
	//const BOOL bRunMode = pulStatus.flags.started || pulStatus.flags.running;

	const BOOL bSuperVisor    = USERRIGHTSREF.IsAktSupervisor();
	const BOOL bLogin			= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS);
	const BOOL bEnable = bLogin; //  && (!bRunMode);

	ENABLE_ID(IDC_ICP_MOTOR_MAXSETPOINT, bEnable);
	ENABLE_ID(IDC_ICP_MOTOR_STEEPNESSMASSFLOW, bEnable);
}
//****************************************************************************************
//****************************************************************************************
BOOL CIcpMotorPage::OnUpdateControls(void)
{
	BOOL bValid = __ISICPTYPE(m_lDoseType);
	if ((m_sItem < 0) || (!bValid))
	{
		return FALSE;
	}
	REMOTEREF.getDoseMaxSetpoint(m_sItem, m_fMaxSetpoint);
	REMOTEREF.getDoseMassflowSteepness(m_sItem, m_fSteepnessMassflow);
	SetControlStyle ();
	return CDosePage::OnUpdateControls();
}
//****************************************************************************************
//****************************************************************************************
BOOL CIcpMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_MaxsetpointInfoButton);
		INITINFOBUTTON(m_SteepnessMassflowInfoButton);
		SetNumberFont(IDC_ICP_MOTOR_NR);
		SetValue();
	}
	return result;
}
//****************************************************************************************
//****************************************************************************************
void CIcpMotorPage::OnStnClickedIcpMaxSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_ICP_MOTOR_MAXSETPOINT);
}
//****************************************************************************************
//****************************************************************************************
void CIcpMotorPage::OnStnClickedIcpMotorSteepnessMassflow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_ICP_MOTOR_STEEPNESSMASSFLOW);
}
//****************************************************************************************
//****************************************************************************************
HBRUSH CIcpMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_ICP_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

		case IDC_ICP_MOTOR_BITMAP:
								{
									m_aGrafikContainer.Update(pDC);
								}
								break;

		case IDC_ICP_MOTOR_MAXSETPOINT:
		case IDC_ICP_MOTOR_STEEPNESSMASSFLOW:
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
void CIcpMotorPage::OnClickedIcpMaxSetpointInfo()
{
	CreateHelpInfoBox(IDC_ICP_MOTOR_MAXSETPOINT, IDS_MAXSETPOINT_KGH, IDS_INFO_MAXSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIcpMotorPage::OnClickedIcpMotorSteilheitistwertInfo()
{
	CreateHelpInfoBox(IDC_ICP_MOTOR_STEEPNESSMASSFLOW, IDS_STEEPNESSMASSFLOW, IDS_INFO_STEEPNESSMASSFLOW);
}




