//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfAlarmPage
///
/// @file   LwfAlarmPage.cpp
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
#include "LwfAlarmPage.h"
#include "RemoteControl.h"
#include "GlobalConst.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLwfAlarmPage, _func)



//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CLwfAlarmPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_LWF_ALARM_MASSFLOW_HIGH, &CLwfAlarmPage::OnStnClickedDosiererAlarmMassflowHigh)
	ON_STN_CLICKED(IDC_LWF_ALARM_MASSFLOW_LOW, &CLwfAlarmPage::OnStnClickedDosiererAlarmMassflowLow)
	ON_STN_CLICKED(IDC_LWF_ALARM_DRIVECOMMAND_HIGH, &CLwfAlarmPage::OnStnClickedDosiererAlarmDriveCommandHigh)
	ON_STN_CLICKED(IDC_LWF_ALARM_DRIVECOMMAND_LOW, &CLwfAlarmPage::OnStnClickedDosiererAlarmDriveCommandLow)
	ON_STN_CLICKED(IDC_LWF_ALARM_REACTIONDELAY, &CLwfAlarmPage::OnStnClickedDosiererAlarmReactionDelay)
	ON_STN_CLICKED(IDC_LWF_ALARM_STARTREACTIONDELAY, &CLwfAlarmPage::OnStnClickedDosiererAlarmStartReactionDelay)
	ON_STN_CLICKED(IDC_LWF_ALARM_DOSEPERFORMANCE, &CLwfAlarmPage::OnStnClickedDosiererAlarmDosePerformance)
	ON_STN_CLICKED(IDC_LWF_ALARM_MAXBATCHTIME, &CLwfAlarmPage::OnStnClickedDosiererAlarmMaxBatchTime)
	ON_STN_CLICKED(IDC_LWF_ALARM_NOISELIMIT, &CLwfAlarmPage::OnStnClickedDosiererAlarmNoiseLimit)

	ON_BN_CLICKED(IDC_LWF_ALARM_MASSFLOW_HIGH_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmMassflowHighInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_MASSFLOW_LOW_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmMassflowLowInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_DRIVECOMMAND_HIGH_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmDriveCommandHighInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_DRIVECOMMAND_LOW_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmDriveCommandLowInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_REACTIONDELAY_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmReactionDelayInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_STARTREACTIONDELAY_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmStartReactionDelayInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_DOSEPERFORMANCE_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmDosePerformanceInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_MAXBATCHTIME_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmMaxBatchTimeInfo)
	ON_BN_CLICKED(IDC_LWF_ALARM_NOISELIMIT_INFO, &CLwfAlarmPage::OnClickedDosiererAlarmNoiseLimitInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CLwfAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CLwfAlarmPage::CLwfAlarmPage(): CDosePage(CLwfAlarmPage::IDD)
	, m_AlarmMassflowHighInfoButton()
	, m_AlarmMassflowLowInfoButton()
	, m_AlarmDriveCommandHighInfoButton()
	, m_AlarmDriveCommandLowInfoButton()
	, m_AlarmReactionDelayInfoButton()
	, m_AlarmStartReactionDelayInfoButton()
	, m_AlarmDosePerformanceInfoButton()
	, m_AlarmDoseMaxBatchTimeInfoButton()
	, m_AlarmNoiseLimitInfoButton()
	, m_AlarmMassflowHigh(0)
	, m_AlarmMassflowLow(0)
	, m_AlarmDriveCommandHigh(0)
	, m_AlarmDriveCommandLow(0)
	, m_AlarmReactionDelay(0)
	, m_lAlarmStartReactionDelay(0)
	, m_lAlarmDosePerformance(0)
	, m_lAlarmMaxBatchTime { 0 }
	, m_AlarmNoiseLimit(0.0f)
	, c_EditMap({
		EDITITEM(IDC_LWF_ALARM_DRIVECOMMAND_HIGH,OnNotifyAlarmDriveCommandHigh),
		EDITITEM(IDC_LWF_ALARM_DRIVECOMMAND_LOW,OnNotifyAlarmDriveCommandLow),
		EDITITEM(IDC_LWF_ALARM_MASSFLOW_HIGH,OnNotifyAlarmMassflowHigh),
		EDITITEM(IDC_LWF_ALARM_MASSFLOW_LOW,OnNotifyAlarmMassflowLow),
		EDITITEM(IDC_LWF_ALARM_DOSEPERFORMANCE,OnNotifyAlarmDosePerformance),
		EDITITEM(IDC_LWF_ALARM_REACTIONDELAY,OnNotifyAlarmReactionDelay),
		EDITITEM(IDC_LWF_ALARM_STARTREACTIONDELAY,OnNotifyAlarmStartReactionDelay),
		EDITITEM(IDC_LWF_ALARM_NOISELIMIT,OnNotifyAlarmNoiseLimit),
		EDITITEM(IDC_LWF_ALARM_MAXBATCHTIME,OnNotifyAlarmMaxBatchTime)
		})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CLwfAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LWF_ALARM_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_LWF_ALARM_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_LWF_ALARM_NR, m_lNr);

	DDX_Text(pDX, IDC_LWF_ALARM_MASSFLOW_HIGH, m_AlarmMassflowHigh);
	DDX_Text(pDX, IDC_LWF_ALARM_MASSFLOW_LOW, m_AlarmMassflowLow);
	DDX_Text(pDX, IDC_LWF_ALARM_DRIVECOMMAND_HIGH, m_AlarmDriveCommandHigh);
	DDX_Text(pDX, IDC_LWF_ALARM_DRIVECOMMAND_LOW, m_AlarmDriveCommandLow);
	DDX_Text(pDX, IDC_LWF_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
	DDX_Text(pDX, IDC_LWF_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);
	DDX_Text(pDX, IDC_LWF_ALARM_DOSEPERFORMANCE, m_lAlarmDosePerformance);
	DDX_Text(pDX, IDC_LWF_ALARM_MAXBATCHTIME, m_lAlarmMaxBatchTime);
	DDX_Text(pDX, IDC_LWF_ALARM_NOISELIMIT, m_AlarmNoiseLimit);

	DDX_Control(pDX, IDC_LWF_ALARM_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_LWF_ALARM_MASSFLOW_HIGH_INFO, m_AlarmMassflowHighInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_MASSFLOW_LOW_INFO, m_AlarmMassflowLowInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_DRIVECOMMAND_HIGH_INFO, m_AlarmDriveCommandHighInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_DRIVECOMMAND_LOW_INFO, m_AlarmDriveCommandLowInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_DOSEPERFORMANCE_INFO, m_AlarmDosePerformanceInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_MAXBATCHTIME_INFO, m_AlarmDoseMaxBatchTimeInfoButton);
	DDX_Control(pDX, IDC_LWF_ALARM_NOISELIMIT_INFO, m_AlarmNoiseLimitInfoButton);
}

//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmMassflowHigh()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmMassflowHigh, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmMassflowHigh(m_sItem, m_AlarmMassflowHigh);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmMassflowLow()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmMassflowLow, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmMassflowLow(m_sItem, m_AlarmMassflowLow);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmDriveCommandHigh()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmDriveCommandHigh, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmDriveCommandHigh(m_sItem, m_AlarmDriveCommandHigh);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmDriveCommandLow()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmDriveCommandLow, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmDriveCommandLow(m_sItem, m_AlarmDriveCommandLow);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmDosePerformance()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmDosePerformance, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmDosePerformance(m_sItem, m_lAlarmDosePerformance);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmReactionDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmReactionDelay, 0, 1000U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmStartReactionDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmStartReactionDelay, 0, 1000U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmNoiseLimit()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_AlarmNoiseLimit, 0.0f, 1000.0f);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmNoiseLimit(m_sItem, m_AlarmNoiseLimit);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnNotifyAlarmMaxBatchTime()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmMaxBatchTime, 0, 10000U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmMaxBatchTime(m_sItem, m_lAlarmMaxBatchTime);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
LRESULT CLwfAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
BOOL CLwfAlarmPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (! (__ISLWFTYPE(m_lDoseType) || __ISBATCHERTYPE(m_lDoseType))))
	{
		return FALSE;
	}
	auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

	REMOTEREF.getDoseAlarmMassflowHigh(m_sItem, m_AlarmMassflowHigh);
	REMOTEREF.getDoseAlarmMassflowLow(m_sItem, m_AlarmMassflowLow);
	REMOTEREF.getDoseAlarmDriveCommandHigh(m_sItem, m_AlarmDriveCommandHigh);
	REMOTEREF.getDoseAlarmDriveCommandLow(m_sItem, m_AlarmDriveCommandLow);
	REMOTEREF.getDoseAlarmNoiseLimit(m_sItem, m_AlarmNoiseLimit);
	REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	REMOTEREF.getDoseAlarmMaxBatchTime(m_sItem, m_lAlarmMaxBatchTime);
	REMOTEREF.getDoseAlarmDosePerformance(m_sItem, m_lAlarmDosePerformance);

	ENABLE_ID(IDC_LWF_ALARM_MASSFLOW_HIGH, bPermitted);
	ENABLE_ID(IDC_LWF_ALARM_MASSFLOW_LOW, bPermitted);
	ENABLE_ID(IDC_LWF_ALARM_DRIVECOMMAND_HIGH, bPermitted);
	ENABLE_ID(IDC_LWF_ALARM_REACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_LWF_ALARM_STARTREACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_LWF_ALARM_DRIVECOMMAND_LOW, bPermitted);

	// Nur LWF
	BOOL bShow = __ISLWFTYPE(m_lDoseType);
	ENABLE_SHOW_ID(IDC_LWF_ALARM_DOSEPERFORMANCE, bPermitted, bShow);
	SHOWW_ID(IDC_LWF_ALARM_DOSEPERFORMANCE_STATIC, bShow);
	SHOWW_ID(IDC_LWF_ALARM_DOSEPERFORMANCE_INFO, bShow);

	bShow = __ISBATCHERTYPE(m_lDoseType);
	ENABLE_SHOW_ID(IDC_LWF_ALARM_MAXBATCHTIME, bPermitted, bShow);
	SHOWW_ID(IDC_LWF_ALARM_MAXBATCHTIME_STATIC, bShow);
	SHOWW_ID(IDC_LWF_ALARM_MAXBATCHTIME_INFO, bShow);

	bShow = __ISLWXTYPE(m_lDoseType);
	ENABLE_SHOW_ID(IDC_LWF_ALARM_NOISELIMIT, bPermitted, bShow);
	SHOWW_ID(IDC_LWF_ALARM_NOISELIMIT_STATIC, bShow);
	SHOWW_ID(IDC_LWF_ALARM_NOISELIMIT_INFO, bShow);


	return CDosePage::OnUpdateControls();
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CLwfAlarmPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_AlarmMassflowHighInfoButton)
			INITINFOBUTTON(m_AlarmMassflowLowInfoButton);
		INITINFOBUTTON(m_AlarmDriveCommandHighInfoButton);
		INITINFOBUTTON(m_AlarmDriveCommandLowInfoButton);
		INITINFOBUTTON(m_AlarmReactionDelayInfoButton);
		INITINFOBUTTON(m_AlarmStartReactionDelayInfoButton);
		INITINFOBUTTON(m_AlarmDosePerformanceInfoButton);
		INITINFOBUTTON(m_AlarmDoseMaxBatchTimeInfoButton);
		INITINFOBUTTON(m_AlarmNoiseLimitInfoButton);

		SetNumberFont(IDC_LWF_ALARM_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmMassflowHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_MASSFLOW_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmMassflowLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_MASSFLOW_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmDriveCommandHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmDriveCommandLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmStartReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_STARTREACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmDosePerformance()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_DOSEPERFORMANCE);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmMaxBatchTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LWF_ALARM_MAXBATCHTIME);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnStnClickedDosiererAlarmNoiseLimit()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_ALARM_NOISELIMIT);
}
//***************************************************************************************************************
//***************************************************************************************************************
HBRUSH CLwfAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
     HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LWF_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_LWF_ALARM_BITMAP:
                            {
								m_aGrafikContainer.Update(pDC);
							}
                            break;

	case IDC_LWF_ALARM_MASSFLOW_HIGH:
	case IDC_LWF_ALARM_DRIVECOMMAND_LOW:
	case IDC_LWF_ALARM_MASSFLOW_LOW:
	case IDC_LWF_ALARM_REACTIONDELAY:
	case IDC_LWF_ALARM_STARTREACTIONDELAY:
	case IDC_LWF_ALARM_DOSEPERFORMANCE:
	case IDC_LWF_ALARM_DRIVECOMMAND_HIGH:
	case IDC_LWF_ALARM_NOISELIMIT:
	case IDC_LWF_ALARM_MAXBATCHTIME:
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
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmMassflowHighInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_MASSFLOW_HIGH, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmMassflowLowInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_MASSFLOW_LOW, IDS_ALARMERROR_DOSE_MASSFLOW_LOW, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmDriveCommandHighInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_DRIVECOMMAND_HIGH, IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmDriveCommandLowInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_DRIVECOMMAND_LOW, IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_REACTIONDELAY, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmStartReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_STARTREACTIONDELAY, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmDosePerformanceInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_DOSEPERFORMANCE, IDS_ALARMERROR_DOSE_DOSEPERFORMANCE, IDS_INFO_ALARMERROR_DOSE_DOSEPERFORMANCE);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmMaxBatchTimeInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_MAXBATCHTIME, IDS_ALARMERROR_DOSE_DOSEPERFORMANCE, IDS_INFO_ALARMERROR_DOSE_DOSEPERFORMANCE);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CLwfAlarmPage::OnClickedDosiererAlarmNoiseLimitInfo()
{
	CreateHelpInfoBox(IDC_LWF_ALARM_NOISELIMIT, IDS_ALARMERROR_DOSE_NOISELIMIT_P, IDS_INFO_ALARMERROR_DOSE_NOISELIMIT);
}



