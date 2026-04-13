//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VolAlarmPage
///
/// @file   VolAlarmPage.cpp
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
#include "VolAlarmPage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CVolAlarmPage, _func)


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CVolAlarmPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_VOL_ALARM_DRIVECOMMAND_HIGH, &CVolAlarmPage::OnStnClickedVolAlarmDriveCommandHigh)
	ON_STN_CLICKED(IDC_VOL_ALARM_DRIVECOMMAND_LOW, &CVolAlarmPage::OnStnClickedVolAlarmDriveCommandLow)
	ON_STN_CLICKED(IDC_VOL_ALARM_REACTIONDELAY, &CVolAlarmPage::OnStnClickedVolAlarmReactionDelay)
	ON_STN_CLICKED(IDC_VOL_ALARM_STARTREACTIONDELAY, &CVolAlarmPage::OnStnClickedVolAlarmStartReactionDelay)

	ON_BN_CLICKED(IDC_VOL_ALARM_DRIVECOMMAND_HIGH_INFO, &CVolAlarmPage::OnClickedVolAlarmDriveCommandHighInfo)
	ON_BN_CLICKED(IDC_VOL_ALARM_DRIVECOMMAND_LOW_INFO, &CVolAlarmPage::OnClickedVolAlarmDriveCommandLowInfo)
	ON_BN_CLICKED(IDC_VOL_ALARM_REACTIONDELAY_INFO, &CVolAlarmPage::OnClickedVolAlarmReactionDelayInfo)
	ON_BN_CLICKED(IDC_VOL_ALARM_STARTREACTIONDELAY_INFO, &CVolAlarmPage::OnClickedVolAlarmStartReactionDelayInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CVolAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CVolAlarmPage::CVolAlarmPage()	: CDosePage(CVolAlarmPage::IDD)
	, c_EditMap({
		EDITITEM(IDC_VOL_ALARM_DRIVECOMMAND_HIGH,OnNotifyAlarmDriveCommandHigh),
		EDITITEM(IDC_VOL_ALARM_DRIVECOMMAND_LOW,OnNotifyAlarmDriveCommandLow),
		EDITITEM(IDC_VOL_ALARM_REACTIONDELAY,OnNotifyAlarmReactionDelay),
		EDITITEM(IDC_VOL_ALARM_STARTREACTIONDELAY,OnNotifyAlarmStartReactionDelay)
		})
	, m_AlarmDriveCommandHighInfoButton()
	, m_AlarmDriveCommandLowInfoButton()
	, m_AlarmReactionDelayInfoButton()
	, m_AlarmStartReactionDelayInfoButton()
	, m_DriveCommandHigh(0)
	, m_DriveCommandLow(0)
	, m_AlarmReactionDelay(0)
	, m_lAlarmStartReactionDelay(0)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CVolAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VOL_ALARM_NR, m_lNr);
	DDX_Text(pDX, IDC_VOL_ALARM_NAME, m_szName);
	DDX_Text(pDX, IDC_VOL_ALARM_DRIVECOMMAND_HIGH, m_DriveCommandHigh);
	DDX_Text(pDX, IDC_VOL_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
	DDX_Text(pDX, IDC_VOL_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);
	DDX_Text(pDX, IDC_VOL_ALARM_DRIVECOMMAND_LOW, m_DriveCommandLow);
	DDX_Control(pDX, IDC_VOL_ALARM_BITMAP, m_aGrafikContainer);

	DDX_Control(pDX, IDC_VOL_ALARM_DRIVECOMMAND_HIGH_INFO, m_AlarmDriveCommandHighInfoButton);
	DDX_Control(pDX, IDC_VOL_ALARM_DRIVECOMMAND_LOW_INFO, m_AlarmDriveCommandLowInfoButton);
	DDX_Control(pDX, IDC_VOL_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
	DDX_Control(pDX, IDC_VOL_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CVolAlarmPage::OnNotifyAlarmDriveCommandHigh()
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
BOOL CVolAlarmPage::OnNotifyAlarmDriveCommandLow()
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
BOOL CVolAlarmPage::OnNotifyAlarmReactionDelay()
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
BOOL CVolAlarmPage::OnNotifyAlarmStartReactionDelay()
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
LRESULT CVolAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
	return 0;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CVolAlarmPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISVOLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

	REMOTEREF.getDoseAlarmDriveCommandHigh(m_sItem, m_DriveCommandHigh);
	REMOTEREF.getDoseAlarmDriveCommandLow(m_sItem, m_DriveCommandLow);
	REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);

	ENABLE_ID(IDC_VOL_ALARM_DRIVECOMMAND_HIGH, bPermitted);
	ENABLE_ID(IDC_VOL_ALARM_DRIVECOMMAND_LOW, bPermitted);
	ENABLE_ID(IDC_VOL_ALARM_REACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_VOL_ALARM_STARTREACTIONDELAY, bPermitted);

	return CDosePage::OnUpdateControls();
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CVolAlarmPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	INITINFOBUTTON(m_AlarmDriveCommandHighInfoButton);
	INITINFOBUTTON(m_AlarmDriveCommandLowInfoButton);
	INITINFOBUTTON(m_AlarmReactionDelayInfoButton);
	INITINFOBUTTON(m_AlarmStartReactionDelayInfoButton);

	SetNumberFont(IDC_VOL_ALARM_NR);
	SetValue();
	return result;
}
//***************************************************************************************************************
//***************************************************************************************************************
void CVolAlarmPage::OnStnClickedVolAlarmDriveCommandHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_VOL_ALARM_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CVolAlarmPage::OnStnClickedVolAlarmDriveCommandLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_VOL_ALARM_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CVolAlarmPage::OnStnClickedVolAlarmReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_VOL_ALARM_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CVolAlarmPage::OnStnClickedVolAlarmStartReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_VOL_ALARM_STARTREACTIONDELAY);

}
//***************************************************************************************************************
//***************************************************************************************************************
HBRUSH CVolAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_VOL_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

	case IDC_VOL_ALARM_DRIVECOMMAND_HIGH:
	case IDC_VOL_ALARM_REACTIONDELAY:
	case IDC_VOL_ALARM_STARTREACTIONDELAY:
	case IDC_VOL_ALARM_DRIVECOMMAND_LOW:
                            {
                                if ( pWnd->IsWindowEnabled() )
                                {
                                    pDC->SetTextColor(EDITTEXTCOLOR);
                                    pDC->SetBkColor(EDITBKCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = (HBRUSH) c_EditBrush;
                                }
                            }
                            break;

	case IDC_VOL_ALARM_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;



        default:    
                            break;
    }
    return hbr;
}

//********************************************************************************************************
//********************************************************************************************************
void CVolAlarmPage::OnClickedVolAlarmDriveCommandHighInfo()
{
	CreateHelpInfoBox(IDC_VOL_ALARM_DRIVECOMMAND_HIGH, IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CVolAlarmPage::OnClickedVolAlarmDriveCommandLowInfo()
{
	CreateHelpInfoBox(IDC_VOL_ALARM_DRIVECOMMAND_LOW, IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CVolAlarmPage::OnClickedVolAlarmReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_VOL_ALARM_REACTIONDELAY, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
void CVolAlarmPage::OnClickedVolAlarmStartReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_VOL_ALARM_STARTREACTIONDELAY, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}

