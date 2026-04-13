#include "WbfAlarmPage.h"
//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbfAlarmPage
///
/// @file   WbfAlarmPage.cpp
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
#include "WbfAlarmPage.h"
#include "RemoteControl.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CWbfAlarmPage, _func)

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbfAlarmPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_WBF_ALARM_MASSFLOW_HIGH, &CWbfAlarmPage::OnStnClickedWbfAlarmMassflowHigh)
	ON_STN_CLICKED(IDC_WBF_ALARM_MASSFLOW_LOW, &CWbfAlarmPage::OnStnClickedWbfAlarmMassflowLow)
	ON_STN_CLICKED(IDC_WBF_ALARM_DRIVECOMMAND_HIGH, &CWbfAlarmPage::OnStnClickedWbfAlarmDriveCommandHigh)
	ON_STN_CLICKED(IDC_WBF_ALARM_DRIVECOMMAND_LOW, &CWbfAlarmPage::OnStnClickedWbfAlarmDriveCommandLow)
	ON_STN_CLICKED(IDC_WBF_ALARM_MINBELTLOAD, &CWbfAlarmPage::OnStnClickedWbfAlarmMinBeltLoad)
	ON_STN_CLICKED(IDC_WBF_ALARM_REACTIONDELAY, &CWbfAlarmPage::OnStnClickedWbfAlarmReactionDelay)
	ON_STN_CLICKED(IDC_WBF_ALARM_STARTREACTIONDELAY, &CWbfAlarmPage::OnStnClickedWbfAlarmStartReactionDelay)
	
	ON_BN_CLICKED(IDC_WBF_ALARM_MASSFLOW_HIGH_INFO, &CWbfAlarmPage::OnClickedWbfAlarmMassflowHighInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_MASSFLOW_LOW_INFO, &CWbfAlarmPage::OnClickedWbfAlarmMassflowLowInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_DRIVECOMMAND_HIGH_INFO, &CWbfAlarmPage::OnClickedWbfAlarmDriveCommandHighInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_DRIVECOMMAND_LOW_INFO, &CWbfAlarmPage::OnClickedWbfAlarmDriveCommandLowInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_REACTIONDELAY_INFO, &CWbfAlarmPage::OnClickedWbfAlarmReactionDelayInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_STARTREACTIONDELAY_INFO, &CWbfAlarmPage::OnClickedWbfAlarmStartReactionDelayInfo)
	ON_BN_CLICKED(IDC_WBF_ALARM_MINBELTLOAD_INFO, &CWbfAlarmPage::OnClickedWbfAlarmMinBeltLoadInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbfAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************

CWbfAlarmPage::CWbfAlarmPage(): CDosePage(CWbfAlarmPage::IDD)
	, m_MassflowHigh(0)
	, m_MassflowLow(0)
	, m_AlarmDriveCommandHigh(0)
	, m_AlarmDriveCommandLow(0)
	, m_fMinBeltLoad(0)
	, m_AlarmReactionDelay(0)
	, m_lAlarmStartReactionDelay(0)
	, m_EditMap({
				EDITITEM(IDC_WBF_ALARM_MASSFLOW_HIGH, OnNotifyAlarmMassflowHigh),
				EDITITEM(IDC_WBF_ALARM_MASSFLOW_LOW,  OnNotifyAlarmMassflowLow),
				EDITITEM(IDC_WBF_ALARM_DRIVECOMMAND_HIGH,	OnNotifyAlarmDriveCommandHigh),
				EDITITEM(IDC_WBF_ALARM_DRIVECOMMAND_LOW, OnNotifyAlarmDriveCommandLow),
				EDITITEM(IDC_WBF_ALARM_MINBELTLOAD,	OnNotifyAlarmMinBeltLoad),
				EDITITEM(IDC_WBF_ALARM_REACTIONDELAY,	OnNotifyAlarmReactionDelay),
				EDITITEM(IDC_WBF_ALARM_STARTREACTIONDELAY,	OnNotifyAlarmStartReactionDelay),
		})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WBF_ALARM_NR, m_lNr);
	DDX_Text(pDX, IDC_WBF_ALARM_NAME, m_szName);
	DDX_Text(pDX, IDC_WBF_ALARM_MASSFLOW_HIGH, m_MassflowHigh);
	DDX_Text(pDX, IDC_WBF_ALARM_MASSFLOW_LOW, m_MassflowLow);
	DDX_Text(pDX, IDC_WBF_ALARM_DRIVECOMMAND_HIGH, m_AlarmDriveCommandHigh);
	DDX_Text(pDX, IDC_WBF_ALARM_DRIVECOMMAND_LOW, m_AlarmDriveCommandLow);
	DDX_Text(pDX, IDC_WBF_ALARM_MINBELTLOAD, m_fMinBeltLoad);
	DDX_Text(pDX, IDC_WBF_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
	DDX_Text(pDX, IDC_WBF_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);
	DDX_Control(pDX, IDC_WBF_ALARM_BITMAP, m_aGrafikContainer);

	DDX_Control(pDX, IDC_WBF_ALARM_MASSFLOW_HIGH_INFO, m_AlarmMassflowHighInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_MASSFLOW_LOW_INFO, m_AlarmMassflowLowInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_DRIVECOMMAND_HIGH_INFO, m_AlarmDriveCommandHighInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_DRIVECOMMAND_LOW_INFO, m_AlarmDriveCommandLowInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);
	DDX_Control(pDX, IDC_WBF_ALARM_MINBELTLOAD_INFO, m_AlarmMinBeltLoadInfoButton);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfAlarmPage::OnNotifyAlarmMassflowHigh()
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
BOOL CWbfAlarmPage::OnNotifyAlarmMassflowLow()
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
BOOL CWbfAlarmPage::OnNotifyAlarmDriveCommandHigh()
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
BOOL CWbfAlarmPage::OnNotifyAlarmDriveCommandLow()
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
BOOL CWbfAlarmPage::OnNotifyAlarmReactionDelay()
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
BOOL CWbfAlarmPage::OnNotifyAlarmStartReactionDelay()
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
BOOL CWbfAlarmPage::OnNotifyAlarmMinBeltLoad()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMinBeltLoad, 0, 10000.0F);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmWbfMinBeltLoad(m_sItem, m_fMinBeltLoad);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
LRESULT CWbfAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			SetValue();
		}
	}
	return 0L;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbfAlarmPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

	REMOTEREF.getDoseAlarmMassflowHigh(m_sItem, m_MassflowHigh);
	REMOTEREF.getDoseAlarmMassflowLow(m_sItem, m_MassflowLow);
	REMOTEREF.getDoseAlarmDriveCommandHigh(m_sItem, m_AlarmDriveCommandHigh);
	REMOTEREF.getDoseAlarmDriveCommandLow(m_sItem, m_AlarmDriveCommandLow);
	REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	REMOTEREF.getDoseAlarmWbfMinBeltLoad(m_sItem, m_fMinBeltLoad);

	ENABLE_ID(IDC_WBF_ALARM_MASSFLOW_HIGH, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_MASSFLOW_LOW, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_REACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_STARTREACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_DRIVECOMMAND_HIGH, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_DRIVECOMMAND_LOW, bPermitted);
	ENABLE_ID(IDC_WBF_ALARM_MINBELTLOAD, bPermitted);

	return CDosePage::OnUpdateControls();
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbfAlarmPage::OnInitDialog()
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
		INITINFOBUTTON(m_AlarmMinBeltLoadInfoButton);

		SetNumberFont(IDC_WBF_ALARM_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmMassflowHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_MASSFLOW_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmMassflowLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_MASSFLOW_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmDriveCommandHigh()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmDriveCommandLow()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmMinBeltLoad()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_ALARM_MINBELTLOAD);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnStnClickedWbfAlarmStartReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBF_ALARM_STARTREACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
HBRUSH CWbfAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBF_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

		case IDC_WBF_ALARM_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;


	case IDC_WBF_ALARM_MASSFLOW_HIGH:
	case IDC_WBF_ALARM_MASSFLOW_LOW:
	case IDC_WBF_ALARM_DRIVECOMMAND_HIGH:
	case IDC_WBF_ALARM_REACTIONDELAY:
	case IDC_WBF_ALARM_STARTREACTIONDELAY:
	case IDC_WBF_ALARM_DRIVECOMMAND_LOW:
	case IDC_WBF_ALARM_MINBELTLOAD:
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
void CWbfAlarmPage::OnClickedWbfAlarmMassflowHighInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_MASSFLOW_HIGH, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmMassflowLowInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_MASSFLOW_LOW, IDS_ALARMERROR_DOSE_MASSFLOW_LOW, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmDriveCommandHighInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_DRIVECOMMAND_HIGH, IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmDriveCommandLowInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_DRIVECOMMAND_LOW, IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_REACTIONDELAY, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmStartReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_STARTREACTIONDELAY, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}
//***************************************************************************************************************
//***************************************************************************************************************
void CWbfAlarmPage::OnClickedWbfAlarmMinBeltLoadInfo()
{
	CreateHelpInfoBox(IDC_WBF_ALARM_MINBELTLOAD, IDS_ALARMERROR_DOSE_DOSEPERFORMANCE, IDS_INFO_ALARMERROR_DOSE_DOSEPERFORMANCE);
}

