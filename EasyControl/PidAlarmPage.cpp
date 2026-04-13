//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module PidAlarmPage
///
/// @file   PidAlarmPage.cpp
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
#include "BASE/Base.h"
#include "EasyControl.h"
#include "PidAlarmPage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CPidAlarmPage, _func)


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CPidAlarmPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_PID_ALARM_MASSFLOW_HIGH, &CPidAlarmPage::OnStnClickedPidAlarmMassflowHigh)
	ON_STN_CLICKED(IDC_PID_ALARM_MASSFLOW_LOW, &CPidAlarmPage::OnStnClickedPidAlarmMassflowLow)
	ON_STN_CLICKED(IDC_PID_ALARM_DRIVECOMMAND_HIGH, &CPidAlarmPage::OnStnClickedPidAlarmDriveCommandHigh)
	ON_STN_CLICKED(IDC_PID_ALARM_DRIVECOMMAND_LOW, &CPidAlarmPage::OnStnClickedPidAlarmDriveCommandLow)
	ON_STN_CLICKED(IDC_PID_ALARM_REACTIONDELAY, &CPidAlarmPage::OnStnClickedPidAlarmReactionDelay)
	ON_STN_CLICKED(IDC_PID_ALARM_STARTREACTIONDELAY, &CPidAlarmPage::OnStnClickedPidAlarmStartReactionDelay)

	ON_BN_CLICKED(IDC_PID_ALARM_MASSFLOW_HIGH_INFO, &CPidAlarmPage::OnClickedPidAlarmMassflowHighInfo)
	ON_BN_CLICKED(IDC_PID_ALARM_MASSFLOW_LOW_INFO, &CPidAlarmPage::OnClickedPidAlarmMassflowLowInfo)
	ON_BN_CLICKED(IDC_PID_ALARM_DRIVECOMMAND_HIGH_INFO, &CPidAlarmPage::OnClickedPidAlarmDriveCommandHighInfo)
	ON_BN_CLICKED(IDC_PID_ALARM_DRIVECOMMAND_LOW_INFO, &CPidAlarmPage::OnClickedPidAlarmDriveCommandLowInfo)
	ON_BN_CLICKED(IDC_PID_ALARM_REACTIONDELAY_INFO, &CPidAlarmPage::OnClickedPidAlarmReactionDelayInfo)
	ON_BN_CLICKED(IDC_PID_ALARM_STARTREACTIONDELAY_INFO, &CPidAlarmPage::OnClickedPidAlarmStartReactionDelayInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CPidAlarmPage, CDosePage)

//******************************************************************************************************
//******************************************************************************************************
CPidAlarmPage::CPidAlarmPage(): CDosePage(CPidAlarmPage::IDD)
, c_EditMap({
	EDITITEM(IDC_PID_ALARM_MASSFLOW_HIGH,OnNotifyAlarmHigh),
	EDITITEM(IDC_PID_ALARM_MASSFLOW_LOW,OnNotifyAlarmLow),
	EDITITEM(IDC_PID_ALARM_DRIVECOMMAND_HIGH,OnNotifyAlarmDriveCommandHigh),
	EDITITEM(IDC_PID_ALARM_DRIVECOMMAND_LOW,OnNotifyAlarmDriveCommandLow),
	EDITITEM(IDC_PID_ALARM_REACTIONDELAY,OnNotifyAlarmReactionDelay),
	EDITITEM(IDC_PID_ALARM_STARTREACTIONDELAY,OnNotifyAlarmStartDelay),
	})
	, m_AlarmMassflowHighInfoButton ()
	, m_AlarmMassflowLowInfoButton()
	, m_AlarmDriveCommandHighInfoButton()
	, m_AlarmDriveCommandLowInfoButton()
	, m_AlarmReactionDelayInfoButton()
	, m_AlarmStartReactionDelayInfoButton()
	, m_MassflowHigh(0)
	, m_MassflowLow(0)
	, m_MaxDriveCommand(0)
	, m_AlarmReactionDelay(0)
	, m_lAlarmStartReactionDelay(0)
	, m_MinDriveCommand(0)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CPidAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PID_ALARM_NR, m_lNr);
	DDX_Text(pDX, IDC_PID_ALARM_NAME, m_szName);
	DDX_Text(pDX, IDC_PID_ALARM_MASSFLOW_HIGH, m_MassflowHigh);
	DDX_Text(pDX, IDC_PID_ALARM_MASSFLOW_LOW, m_MassflowLow);
	DDX_Text(pDX, IDC_PID_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
	DDX_Text(pDX, IDC_PID_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);
	DDX_Text(pDX, IDC_PID_ALARM_DRIVECOMMAND_HIGH, m_MaxDriveCommand);
	DDX_Text(pDX, IDC_PID_ALARM_DRIVECOMMAND_LOW, m_MinDriveCommand);
	DDX_Control(pDX, IDC_PID_ALARM_BITMAP, m_aGrafikContainer);

	DDX_Control(pDX, IDC_PID_ALARM_MASSFLOW_HIGH_INFO, m_AlarmMassflowHighInfoButton);
	DDX_Control(pDX, IDC_PID_ALARM_MASSFLOW_LOW_INFO, m_AlarmMassflowLowInfoButton);
	DDX_Control(pDX, IDC_PID_ALARM_DRIVECOMMAND_HIGH_INFO, m_AlarmDriveCommandHighInfoButton);
	DDX_Control(pDX, IDC_PID_ALARM_DRIVECOMMAND_LOW_INFO, m_AlarmDriveCommandLowInfoButton);
	DDX_Control(pDX, IDC_PID_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
	DDX_Control(pDX, IDC_PID_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);

}
//******************************************************************************************************
//******************************************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmHigh(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_MassflowHigh, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmMassflowHigh(m_sItem, m_MassflowHigh);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmLow(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_MassflowLow, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmMassflowLow(m_sItem, m_MassflowLow);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmDriveCommandHigh(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_MaxDriveCommand, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmDriveCommandHigh(m_sItem, m_MaxDriveCommand);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmDriveCommandLow(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_MinDriveCommand, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmDriveCommandLow(m_sItem, m_MinDriveCommand);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmReactionDelay(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmReactionDelay, 0, 10000U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
BOOL CPidAlarmPage::OnNotifyAlarmStartDelay(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmStartReactionDelay, 0, 10000U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	}
	return bModified;
}
//*******************************************************************************
//*******************************************************************************
LRESULT CPidAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
//********************************************************************************************************
//********************************************************************************************************
BOOL CPidAlarmPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISPIDTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

	REMOTEREF.getDoseAlarmMassflowHigh(m_sItem, m_MassflowHigh);
	REMOTEREF.getDoseAlarmMassflowLow(m_sItem, m_MassflowLow);
	REMOTEREF.getDoseAlarmDriveCommandHigh(m_sItem, m_MaxDriveCommand);
	REMOTEREF.getDoseAlarmDriveCommandLow(m_sItem, m_MinDriveCommand);
	REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);

	ENABLE_ID(IDC_PID_ALARM_MASSFLOW_HIGH, bPermitted);
	ENABLE_ID(IDC_PID_ALARM_MASSFLOW_LOW, bPermitted);
	ENABLE_ID(IDC_PID_ALARM_REACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_PID_ALARM_STARTREACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_PID_ALARM_DRIVECOMMAND_HIGH, bPermitted);
	ENABLE_ID(IDC_PID_ALARM_DRIVECOMMAND_LOW, bPermitted);

	return CDosePage::OnUpdateControls();
}
//********************************************************************************************************
//********************************************************************************************************
BOOL CPidAlarmPage::OnInitDialog()
{
	auto result =  CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_AlarmMassflowHighInfoButton)
		INITINFOBUTTON(m_AlarmMassflowLowInfoButton);
		INITINFOBUTTON(m_AlarmDriveCommandHighInfoButton);
		INITINFOBUTTON(m_AlarmDriveCommandLowInfoButton);
		INITINFOBUTTON(m_AlarmReactionDelayInfoButton);
		INITINFOBUTTON(m_AlarmStartReactionDelayInfoButton);

		SetNumberFont(IDC_PID_ALARM_NR);
		SetValue();
	}
	return result;
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmMassflowHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_MASSFLOW_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmMassflowLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_MASSFLOW_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmDriveCommandHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_DRIVECOMMAND_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmDriveCommandLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_DRIVECOMMAND_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_REACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnStnClickedPidAlarmStartReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_PID_ALARM_STARTREACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmMassflowHighInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_MASSFLOW_HIGH, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmMassflowLowInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_MASSFLOW_LOW, IDS_ALARMERROR_DOSE_MASSFLOW_LOW, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmDriveCommandHighInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_DRIVECOMMAND_HIGH, IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmDriveCommandLowInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_DRIVECOMMAND_LOW, IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_REACTIONDELAY, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
void CPidAlarmPage::OnClickedPidAlarmStartReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_PID_ALARM_STARTREACTIONDELAY, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}
//********************************************************************************************************
//********************************************************************************************************
HBRUSH CPidAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_PID_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

		case IDC_PID_ALARM_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

	case IDC_PID_ALARM_MASSFLOW_HIGH:
	case IDC_PID_ALARM_MASSFLOW_LOW:
	case IDC_PID_ALARM_DRIVECOMMAND_HIGH:
	case IDC_PID_ALARM_REACTIONDELAY:
	case IDC_PID_ALARM_STARTREACTIONDELAY:
	case IDC_PID_ALARM_DRIVECOMMAND_LOW:
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


        default:    
                            break;
    }
    return hbr;
}

