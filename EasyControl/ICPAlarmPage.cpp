//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IcpAlarmPage
///
/// @file   IcpAlarmPage.cpp
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
#include "IcpAlarmPage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIcpAlarmPage, _func)


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CIcpAlarmPage, CDosePage)
    ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
    ON_STN_CLICKED(IDC_ICP_ALARM_DRIVECOMMAND_HIGH, &CIcpAlarmPage::OnStnClickedIcpAlarmDriveCommandHigh)
    ON_STN_CLICKED(IDC_ICP_ALARM_DRIVECOMMAND_LOW, &CIcpAlarmPage::OnStnClickedIcpAlarmDriveCommandLow)
    ON_STN_CLICKED(IDC_ICP_ALARM_MASSFLOW_LOW, &CIcpAlarmPage::OnStnClickedIcpAlarmMassflowLow)
    ON_STN_CLICKED(IDC_ICP_ALARM_MASSFLOW_HIGH, &CIcpAlarmPage::OnStnClickedIcpAlarmMassflowHigh)
    ON_STN_CLICKED(IDC_ICP_ALARM_REACTIONDELAY, &CIcpAlarmPage::OnStnClickedIcpAlarmReactionDelay)
    ON_STN_CLICKED(IDC_ICP_ALARM_STARTREACTIONDELAY, &CIcpAlarmPage::OnStnClickedIcpAlarmStartReactionDelay)

    ON_BN_CLICKED(IDC_ICP_ALARM_DRIVECOMMAND_HIGH_INFO, &CIcpAlarmPage::OnClickedIcpAlarmDriveCommandHighInfo)
    ON_BN_CLICKED(IDC_ICP_ALARM_DRIVECOMMAND_LOW_INFO, &CIcpAlarmPage::OnClickedIcpAlarmDriveCommandLowInfo)
    ON_BN_CLICKED(IDC_ICP_ALARM_REACTIONDELAY_INFO, &CIcpAlarmPage::OnClickedIcpAlarmReactionDelayInfo)
    ON_BN_CLICKED(IDC_ICP_ALARM_STARTREACTIONDELAY_INFO, &CIcpAlarmPage::OnClickedIcpAlarmStartReactionDelayInfo)
    ON_BN_CLICKED(IDC_ICP_ALARM_MASSFLOW_HIGH_INFO, &CIcpAlarmPage::OnClickedIcpAlarmMassflowHighInfo)
    ON_BN_CLICKED(IDC_ICP_ALARM_MASSFLOW_LOW_INFO, &CIcpAlarmPage::OnClickedIcpAlarmMassflowLowInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CIcpAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CIcpAlarmPage::CIcpAlarmPage(): CDosePage(CIcpAlarmPage::IDD)
    , c_EditMap({
            EDITITEM(IDC_ICP_ALARM_DRIVECOMMAND_HIGH,	OnNotifyAlarmDriveCommandHigh),
            EDITITEM(IDC_ICP_ALARM_DRIVECOMMAND_LOW, OnNotifyAlarmDriveCommandLow),
            EDITITEM(IDC_ICP_ALARM_MASSFLOW_HIGH,	OnNotifyAlarmMassflowHigh),
            EDITITEM(IDC_ICP_ALARM_MASSFLOW_LOW,	OnNotifyAlarmMassflowLow),
            EDITITEM(IDC_ICP_ALARM_REACTIONDELAY,	OnNotifyAlarmReactionDelay),
            EDITITEM(IDC_ICP_ALARM_STARTREACTIONDELAY,	OnNotifyAlarmStartReactionDelay)
        })
    , m_AlarmDriveCommandHighInfoButton()
    , m_AlarmDriveCommandLowInfoButton()
    , m_AlarmMassflowHighInfoButton()
    , m_AlarmMassflowLowInfoButton()
    , m_AlarmReactionDelayInfoButton()
    , m_AlarmStartReactionDelayInfoButton()
    , m_DriveCommandHigh{ 0 }
    , m_DriveCommandLow{ 0 }
    , m_MassflowHigh{ 0 }
    , m_MassflowLow{ 0 }
    , m_AlarmReactionDelay{ 0 }
    , m_lAlarmStartReactionDelay{ 0 }
{}
//******************************************************************************************************
//******************************************************************************************************
void CIcpAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ICP_ALARM_TITLE, m_szTitle);
    DDX_TextN(pDX, IDC_ICP_ALARM_NAME, m_szName, 30);
    DDX_Text(pDX, IDC_ICP_ALARM_NR, m_lNr);

    DDX_Text(pDX, IDC_ICP_ALARM_MASSFLOW_HIGH, m_MassflowHigh);
    DDX_Text(pDX, IDC_ICP_ALARM_MASSFLOW_LOW, m_MassflowLow);
    DDX_Text(pDX, IDC_ICP_ALARM_DRIVECOMMAND_HIGH, m_DriveCommandHigh);
    DDX_Text(pDX, IDC_ICP_ALARM_DRIVECOMMAND_LOW, m_DriveCommandLow);
    DDX_Text(pDX, IDC_ICP_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
    DDX_Text(pDX, IDC_ICP_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);

    DDX_Control(pDX, IDC_ICP_ALARM_BITMAP, m_aGrafikContainer);
    DDX_Control(pDX, IDC_ICP_ALARM_MASSFLOW_HIGH_INFO, m_AlarmMassflowHighInfoButton);
    DDX_Control(pDX, IDC_ICP_ALARM_MASSFLOW_LOW_INFO, m_AlarmMassflowLowInfoButton);
    DDX_Control(pDX, IDC_ICP_ALARM_DRIVECOMMAND_HIGH_INFO, m_AlarmDriveCommandHighInfoButton);
    DDX_Control(pDX, IDC_ICP_ALARM_DRIVECOMMAND_LOW_INFO, m_AlarmDriveCommandLowInfoButton);
    DDX_Control(pDX, IDC_ICP_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
    DDX_Control(pDX, IDC_ICP_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);

}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmDriveCommandHigh()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_DriveCommandHigh, 0, 100U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmDriveCommandHigh(m_sItem, m_DriveCommandHigh);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmDriveCommandLow()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_DriveCommandLow, 0, 100U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmDriveCommandLow(m_sItem, m_DriveCommandLow);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmMassflowHigh()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_MassflowHigh, 0, 100U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmMassflowHigh(m_sItem, m_MassflowHigh);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmMassflowLow()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_MassflowLow, 0, 100U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmMassflowLow(m_sItem, m_MassflowLow);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmReactionDelay()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmReactionDelay, 0, 10000U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnNotifyAlarmStartReactionDelay()
{
    auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmStartReactionDelay, 0, 10000U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
LRESULT CIcpAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
            LOGERROR("Error");
        }
        if (bModified)
        {
            PostMessage(WM_TIMER_REFRESH);
        }
    }
    return 0L;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIcpAlarmPage::OnUpdateControls(void)
{
    BOOL bValid = __ISICPTYPE(m_lDoseType) || __ISICBTYPE(m_lDoseType);
    if ((m_sItem < 0) || (!bValid))
    {
        return FALSE;
    }
    auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

    REMOTEREF.getDoseAlarmMassflowHigh(m_sItem, m_MassflowHigh);
    REMOTEREF.getDoseAlarmMassflowLow(m_sItem, m_MassflowLow);
    REMOTEREF.getDoseAlarmDriveCommandHigh(m_sItem, m_DriveCommandHigh);
    REMOTEREF.getDoseAlarmDriveCommandLow(m_sItem, m_DriveCommandLow);
    REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
    REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);

    ENABLE_ID(IDC_ICP_ALARM_MASSFLOW_HIGH, bPermitted);
    ENABLE_ID(IDC_ICP_ALARM_MASSFLOW_LOW, bPermitted);
    ENABLE_ID(IDC_ICP_ALARM_DRIVECOMMAND_HIGH, bPermitted);
    ENABLE_ID(IDC_ICP_ALARM_DRIVECOMMAND_LOW, bPermitted);
    ENABLE_ID(IDC_ICP_ALARM_REACTIONDELAY, bPermitted);
    ENABLE_ID(IDC_ICP_ALARM_STARTREACTIONDELAY, bPermitted);

    return CDosePage::OnUpdateControls();
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmMassflowHigh()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_MASSFLOW_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmMassflowLow()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_MASSFLOW_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmDriveCommandHigh()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmDriveCommandLow()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmReactionDelay()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIcpAlarmPage::OnStnClickedIcpAlarmStartReactionDelay()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_ICP_ALARM_STARTREACTIONDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIcpAlarmPage::OnInitDialog()
{
    auto result = CDosePage::OnInitDialog();
    if (result)
    {
        SetNumberFont(IDC_ICP_ALARM_NR);
        SetValue();
    }
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CIcpAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_ICP_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_ICP_ALARM_BITMAP:
                            {
                                m_aGrafikContainer.Update(pDC);
                            }
                            break;

        case IDC_ICP_ALARM_MASSFLOW_HIGH:
        case IDC_ICP_ALARM_MASSFLOW_LOW:
        case IDC_ICP_ALARM_DRIVECOMMAND_HIGH:
        case IDC_ICP_ALARM_DRIVECOMMAND_LOW:
        case IDC_ICP_ALARM_REACTIONDELAY:
        case IDC_ICP_ALARM_STARTREACTIONDELAY:
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
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmDriveCommandHighInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_DRIVECOMMAND_HIGH, IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmDriveCommandLowInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_DRIVECOMMAND_LOW, IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmMassflowHighInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_MASSFLOW_HIGH, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmMassflowLowInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_MASSFLOW_LOW, IDS_ALARMERROR_DOSE_MASSFLOW_LOW, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmReactionDelayInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_REACTIONDELAY_INFO, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
void CIcpAlarmPage::OnClickedIcpAlarmStartReactionDelayInfo()
{
    CreateHelpInfoBox(IDC_ICP_ALARM_STARTREACTIONDELAY_INFO, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}


