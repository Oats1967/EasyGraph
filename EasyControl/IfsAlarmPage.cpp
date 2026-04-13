//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfsAlarmPage
///
/// @file   IfsAlarmPage.cpp
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
#include "IfsAlarmPage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIfsAlarmPage, _func)


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CIfsAlarmPage, CDosePage)
    ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
    ON_STN_CLICKED(IDC_IFS_ALARM_REACTIONDELAY_EDIT, &CIfsAlarmPage::OnStnClickedIfsAlarmReactionDelay)
    ON_STN_CLICKED(IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT, &CIfsAlarmPage::OnStnClickedIfsAlarmStartReactionDelay)
    ON_STN_CLICKED(IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT, &CIfsAlarmPage::OnStnClickedIfsFeederEmptyTimeOut)
    ON_STN_CLICKED(IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT, &CIfsAlarmPage::OnStnClickedIfsFeederOverflowTimeOut)

    ON_BN_CLICKED(IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_INFO, &CIfsAlarmPage::OnBnClickedIfsFeederEmptyTimeOutInfo)
    ON_BN_CLICKED(IDC_IFS_ALARM_TIMEOUTOVERFLOW_INFO, &CIfsAlarmPage::OnBnClickedIfsFeederOverflowTimeOutInfo)
    ON_BN_CLICKED(IDC_IFS_ALARM_REACTIONDELAY_INFO, &CIfsAlarmPage::OnClickedIfsAlarmReactionDelayInfo)
    ON_BN_CLICKED(IDC_IFS_ALARM_STARTREACTIONDELAY_INFO, &CIfsAlarmPage::OnClickedIfsAlarmStartReactionDelayInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CIfsAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CIfsAlarmPage::CIfsAlarmPage(): CDosePage(CIfsAlarmPage::IDD)
    , c_EditMap({
            EDITITEM(IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT, OnNotifyFeederEmptyTimeOut),
            EDITITEM(IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT, OnNotifyFeederOverflowTimeOut),
            EDITITEM(IDC_IFS_ALARM_REACTIONDELAY_EDIT,	OnNotifyAlarmReactionDelay),
            EDITITEM(IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT,	OnNotifyAlarmStartReactionDelay)
        })
    , m_FeederEmptyTimeOutInfoButton()
    , m_FeederOverflowTimeoutInfoButton()
    , m_AlarmReactionDelayInfoButton()
    , m_AlarmStartReactionDelayInfoButton()
    , m_FeederEmptyTimeOut{ 0 }
    , m_FeederOverflowTimeout{ 0 }
    , m_AlarmReactionDelay{ 0 }
    , m_lAlarmStartReactionDelay{ 0 }
{}
//******************************************************************************************************
//******************************************************************************************************
void CIfsAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_IFS_ALARM_TITLE, m_szTitle);
    DDX_TextN(pDX, IDC_IFS_ALARM_NAME, m_szName, 30);
    DDX_Text(pDX, IDC_IFS_ALARM_NR, m_lNr);

    DDX_Text(pDX, IDC_IFS_ALARM_REACTIONDELAY_EDIT, m_AlarmReactionDelay);
    DDX_Text(pDX, IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT, m_lAlarmStartReactionDelay);
    DDX_Text(pDX, IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT, m_FeederEmptyTimeOut);
    DDX_Text(pDX, IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT, m_FeederOverflowTimeout);

    DDX_Control(pDX, IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_INFO, m_FeederEmptyTimeOutInfoButton);
    DDX_Control(pDX, IDC_IFS_ALARM_TIMEOUTOVERFLOW_INFO, m_FeederOverflowTimeoutInfoButton);
    DDX_Control(pDX, IDC_IFS_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
    DDX_Control(pDX, IDC_IFS_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);
    DDX_Control(pDX, IDC_IFS_ALARM_BITMAP, m_aGrafikContainer);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CIfsAlarmPage::OnNotifyFeederEmptyTimeOut()
{
    auto bModified = CEditCtrlEx::GetLongAbs(m_FeederEmptyTimeOut);
    if (bModified)
    {
        REMOTEREF.setIfsFeederEmptyTimeOut(m_sItem, m_FeederEmptyTimeOut);
    }
    return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CIfsAlarmPage::OnNotifyFeederOverflowTimeOut()
{
    auto bModified = CEditCtrlEx::GetLongAbs(m_FeederOverflowTimeout);
    if (bModified)
    {
        REMOTEREF.setIfsFeederOverflowTimeOut(m_sItem, m_FeederOverflowTimeout);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIfsAlarmPage::OnNotifyAlarmReactionDelay()
{
    auto bModified = CEditCtrlEx::CEditCtrlEx::GetLongAbsRange(m_AlarmReactionDelay, 0, 10000U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIfsAlarmPage::OnNotifyAlarmStartReactionDelay()
{
    auto bModified = CEditCtrlEx::CEditCtrlEx::GetLongAbsRange(m_lAlarmStartReactionDelay, 0, 10000U);
    if (bModified)
    {
        REMOTEREF.setDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
    }
    return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
LRESULT CIfsAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
            PostMessage(WM_TIMER_REFRESH);
        }
    }
    return 0L;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CIfsAlarmPage::OnUpdateControls(void)
{
    BOOL bValid = __ISIFSTYPE(m_lDoseType) || __ISICBTYPE(m_lDoseType);
    if ((m_sItem < 0) || (!bValid))
    {
        return FALSE;
    }
    auto bEnable = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

    REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
    REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
    REMOTEREF.getIfsFeederEmptyTimeOut(m_sItem, m_FeederEmptyTimeOut);
    REMOTEREF.getIfsFeederOverflowTimeOut(m_sItem, m_FeederOverflowTimeout);

    ENABLE_ID(IDC_IFS_ALARM_REACTIONDELAY_EDIT, bEnable);
    ENABLE_ID(IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT, bEnable);
    ENABLE_ID(IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT, bEnable);
    ENABLE_ID(IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT, bEnable);

    return CDosePage::OnUpdateControls();
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsAlarmPage::OnStnClickedIfsFeederEmptyTimeOut()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsAlarmPage::OnStnClickedIfsFeederOverflowTimeOut()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIfsAlarmPage::OnStnClickedIfsAlarmReactionDelay()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_ALARM_REACTIONDELAY_EDIT);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CIfsAlarmPage::OnStnClickedIfsAlarmStartReactionDelay()
{
    CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIfsAlarmPage::OnInitDialog()
{
    auto result = CDosePage::OnInitDialog();
    if (result)
    {
        INITINFOBUTTON(m_FeederEmptyTimeOutInfoButton);
        INITINFOBUTTON(m_FeederOverflowTimeoutInfoButton);
        INITINFOBUTTON(m_AlarmReactionDelayInfoButton);
        INITINFOBUTTON(m_AlarmStartReactionDelayInfoButton);

        SetNumberFont(IDC_IFS_ALARM_NR);
        SetValue();
    }
    return result;
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CIfsAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_IFS_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_IFS_ALARM_BITMAP:
                            {
                                m_aGrafikContainer.Update(pDC);
                            }
                            break;

        case IDC_IFS_ALARM_REACTIONDELAY_EDIT:
        case IDC_IFS_ALARM_STARTREACTIONDELAY_EDIT:
        case IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT:
        case IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT:
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
void CIfsAlarmPage::OnClickedIfsAlarmReactionDelayInfo()
{
    CreateHelpInfoBox(IDC_IFS_ALARM_REACTIONDELAY_INFO, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
void CIfsAlarmPage::OnClickedIfsAlarmStartReactionDelayInfo()
{
    CreateHelpInfoBox(IDC_IFS_ALARM_STARTREACTIONDELAY_INFO, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsAlarmPage::OnBnClickedIfsFeederEmptyTimeOutInfo()
{
    CreateHelpInfoBox(IDC_IFS_ALARM_TIMEOUTFEEDEREMPTY_EDIT, IDS_ALARMERROR_IFS_INFEEDHOPPER_EMPTY, IDS_INFO_ALARMERROR_IFS_INFEEDHOPPER_EMPTY);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsAlarmPage::OnBnClickedIfsFeederOverflowTimeOutInfo()
{
    CreateHelpInfoBox(IDC_IFS_ALARM_TIMEOUTOVERFLOW_EDIT, IDS_ALARMERROR_IFS_INFEEDHOPPER_OVERFLOW, IDS_INFO_ALARMERROR_IFS_INFEEDHOPPER_OVERFLOW);
}



