//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbmAlarmPage.cpp
///
/// @file   WbmAlarmPage.cpp
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
#include "WbmAlarmPage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"




//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbmAlarmPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CWbmAlarmPage::OnNotifyEdit)
	ON_STN_CLICKED(IDC_WBM_ALARM_MINBELTLOAD, &CWbmAlarmPage::OnStnClickedWBMAlarmMinBeltLoad)
	ON_STN_CLICKED(IDC_WBM_ALARM_MAXBELTLOAD, &CWbmAlarmPage::OnStnClickedWBMAlarmMaxBeltLoad)
	ON_STN_CLICKED(IDC_WBM_ALARM_REACTIONDELAY, &CWbmAlarmPage::OnStnClickedWBMAlarmReactionDelay)
	ON_STN_CLICKED(IDC_WBM_ALARM_STARTREACTIONDELAY, &CWbmAlarmPage::OnStnClickedWBMAlarmStartReactionDelay)

	ON_BN_CLICKED(IDC_WBM_ALARM_MINBELTLOAD_INFO, &CWbmAlarmPage::OnClickedWBMAlarmMinBeltLoadInfo)
	ON_BN_CLICKED(IDC_WBM_ALARM_MAXBELTLOAD_INFO, &CWbmAlarmPage::OnClickedWBMAlarmMaxBeltLoadInfo)
	ON_BN_CLICKED(IDC_WBM_ALARM_REACTIONDELAY_INFO, &CWbmAlarmPage::OnClickedWBMAlarmReactionDelayInfo)
	ON_BN_CLICKED(IDC_WBM_ALARM_STARTREACTIONDELAY_INFO, &CWbmAlarmPage::OnClickedWBMAlarmStartReactionDelayInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbmAlarmPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CWbmAlarmPage::CWbmAlarmPage(): CDosePage(CWbmAlarmPage::IDD)
	, m_AlarmReactionDelayInfoButton()
	, m_AlarmStartReactionDelayInfoButton()
	, m_AlarmAlarmMinBeltLoadInfoButton()
	, m_AlarmAlarmMaxBeltLoadInfoButton()
	, m_fMinBeltLoad(0)
	, m_fMaxBeltLoad(0)
	, m_AlarmReactionDelay(0)
	, m_lAlarmStartReactionDelay(0)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WBM_ALARM_NR, m_lNr);
	DDX_Text(pDX, IDC_WBM_ALARM_NAME, m_szName);
	DDX_Text(pDX, IDC_WBM_ALARM_MINBELTLOAD, m_fMinBeltLoad);
	DDX_Text(pDX, IDC_WBM_ALARM_MAXBELTLOAD, m_fMaxBeltLoad);
	DDX_Text(pDX, IDC_WBM_ALARM_REACTIONDELAY, m_AlarmReactionDelay);
	DDX_Text(pDX, IDC_WBM_ALARM_STARTREACTIONDELAY, m_lAlarmStartReactionDelay);
	DDX_Control(pDX, IDC_WBM_ALARM_BITMAP, m_aGrafikContainer);

	DDX_Control(pDX, IDC_WBM_ALARM_MINBELTLOAD_INFO, m_AlarmAlarmMinBeltLoadInfoButton);
	DDX_Control(pDX, IDC_WBM_ALARM_MAXBELTLOAD_INFO, m_AlarmAlarmMaxBeltLoadInfoButton);
	DDX_Control(pDX, IDC_WBM_ALARM_REACTIONDELAY_INFO, m_AlarmReactionDelayInfoButton);
	DDX_Control(pDX, IDC_WBM_ALARM_STARTREACTIONDELAY_INFO, m_AlarmStartReactionDelayInfoButton);

}
//******************************************************************************************************
//******************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbmAlarmPage::OnNotifyAlarmReactionDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_AlarmReactionDelay, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbmAlarmPage::OnNotifyAlarmStartReactionDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_lAlarmStartReactionDelay, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbmAlarmPage::OnNotifyAlarmWbfMinBeltLoad()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMinBeltLoad, 0.0F, 10000.0F);
	if (bModified)
	{
		if (m_fMaxBeltLoad < m_fMinBeltLoad)
		{
			ECMessageBox(IDS_MINBELTLOADGROESSERMAXBELTLOAD, MB_OK | MB_ICONSTOP);
		}
		REMOTEREF.setDoseAlarmWbfMinBeltLoad(m_sItem, m_fMinBeltLoad);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
BOOL CWbmAlarmPage::OnNotifyAlarmWbfMaxBeltLoad()
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fMaxBeltLoad, 0.0F, 10000.0F);
	if (bModified)
	{
		if (m_fMaxBeltLoad < m_fMinBeltLoad)
		{
			ECMessageBox(IDS_MINBELTLOADGROESSERMAXBELTLOAD, MB_OK | MB_ICONSTOP);
		}
		REMOTEREF.setDoseAlarmWbfMaxBeltLoad(m_sItem, m_fMaxBeltLoad);
	}
	return bModified;
}
//***************************************************************************************************************
//***************************************************************************************************************
LRESULT CWbmAlarmPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;

		switch (id)
		{
		case IDC_WBM_ALARM_MINBELTLOAD:	    bModified = OnNotifyAlarmWbfMinBeltLoad();
			break;
		case IDC_WBM_ALARM_MAXBELTLOAD:	    bModified = OnNotifyAlarmWbfMaxBeltLoad();
			break;
		case IDC_WBM_ALARM_REACTIONDELAY:			bModified = OnNotifyAlarmReactionDelay();
			break;
		case IDC_WBM_ALARM_STARTREACTIONDELAY:	bModified = OnNotifyAlarmStartReactionDelay();
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		if (bModified)
		{
			SetValue();
		}
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmAlarmPage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	auto bPermitted = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS));

	REMOTEREF.getDoseAlarmWbfMinBeltLoad(m_sItem, m_fMinBeltLoad);
	REMOTEREF.getDoseAlarmWbfMaxBeltLoad(m_sItem, m_fMaxBeltLoad);
	REMOTEREF.getDoseAlarmStartReactionDelay(m_sItem, m_lAlarmStartReactionDelay);
	REMOTEREF.getDoseAlarmReactionDelay(m_sItem, m_AlarmReactionDelay);

	ENABLE_ID(IDC_WBM_ALARM_MINBELTLOAD, bPermitted);
	ENABLE_ID(IDC_WBM_ALARM_MAXBELTLOAD, bPermitted);
	ENABLE_ID(IDC_WBM_ALARM_REACTIONDELAY, bPermitted);
	ENABLE_ID(IDC_WBM_ALARM_STARTREACTIONDELAY, bPermitted);

	return CDosePage::OnUpdateControls();
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbmAlarmPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	INITINFOBUTTON(m_AlarmReactionDelayInfoButton)
	INITINFOBUTTON(m_AlarmStartReactionDelayInfoButton);
	INITINFOBUTTON(m_AlarmAlarmMinBeltLoadInfoButton);
	INITINFOBUTTON(m_AlarmAlarmMaxBeltLoadInfoButton);

	SetNumberFont(IDC_WBM_ALARM_NR);
	SetValue();
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnStnClickedWBMAlarmMinBeltLoad()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_ALARM_MINBELTLOAD);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnStnClickedWBMAlarmMaxBeltLoad()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBM_ALARM_MAXBELTLOAD);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnStnClickedWBMAlarmReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBM_ALARM_REACTIONDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnStnClickedWBMAlarmStartReactionDelay()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WBM_ALARM_STARTREACTIONDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CWbmAlarmPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBM_ALARM_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

	case IDC_WBM_ALARM_REACTIONDELAY:
	case IDC_WBM_ALARM_STARTREACTIONDELAY:
	case IDC_WBM_ALARM_MAXBELTLOAD:
	case IDC_WBM_ALARM_MINBELTLOAD:
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
	case IDC_WBM_ALARM_BITMAP:
	{
		m_aGrafikContainer.Update(pDC);
	}
	break;

       default:    
                            break;
    }
    return hbr;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnClickedWBMAlarmReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_WBM_ALARM_REACTIONDELAY, IDS_ALARMERROR_DOSE_REACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_REACTIONDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnClickedWBMAlarmStartReactionDelayInfo()
{
	CreateHelpInfoBox(IDC_WBM_ALARM_STARTREACTIONDELAY, IDS_ALARMERROR_DOSE_STARTREACTIONDELAY_S, IDS_INFO_ALARMERROR_DOSE_STARTREACTIONDELAY);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnClickedWBMAlarmMinBeltLoadInfo()
{
	CreateHelpInfoBox(IDC_WBM_ALARM_MINBELTLOAD, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmAlarmPage::OnClickedWBMAlarmMaxBeltLoadInfo()
{
	CreateHelpInfoBox(IDC_WBM_ALARM_MAXBELTLOAD, IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH);
}

