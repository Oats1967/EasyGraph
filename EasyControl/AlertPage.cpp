//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlertPage.cpp
///
/// @file   AlertPage.cpp
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
#include "AlertPage.h"
#include "MFCMacros.h"
#include "BASE/include/AlarmTypes.def"




#define LISTCHANGED ((LOGREF.GetCount() !=  m_lLastCount) || (LOGREF.IsChanged()))


//*****************************************************************************************************
//*****************************************************************************************************
BEGIN_MESSAGE_MAP(CAlertPage, CDosePage)
	ON_MESSAGE(WM_NEW_LOGITEM, OnNewLogItem)
	ON_BN_CLICKED(IDC_DOSE_ALERT_ALARM_CLEAR, &CAlertPage::OnBnClickedDoseAlertClearAlarm)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_DOSE_ALERT_LIST, &CAlertPage::OnColumnclickAlarmList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_DOSE_ALERT_LIST, &CAlertPage::OnLvnItemActivateAlarmList)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//*****************************************************************************************************
//*****************************************************************************************************
IMPLEMENT_DYNAMIC(CAlertPage, CDosePage)
CAlertPage::CAlertPage()	: CDosePage(CAlertPage::IDD)
			, m_lLastCount(_U32(-1))
			, m_aListCtrl(this, LOGREF)
{
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlertPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DOSE_ALERT_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_DOSE_ALERT_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_DOSE_ALERT_NR, m_lNr);

	DDX_Control(pDX, IDC_DOSE_ALERT_LIST, m_aListCtrl);
	DDX_Control(pDX, IDC_DOSE_ALERT_IMAGE, m_aGrafikContainer);
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CAlertPage::OnNewLogItem(WPARAM , LPARAM)
{
	if ( m_sItem < 0)
	{
		return 0L;
	}
	SetValue();
	return 0L;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CAlertPage::OnUpdateControls ()
{
	if (m_sItem < 0)
	{
		return FALSE;
	}

    if ( getListChanged() )
    {
		m_aListCtrl.FillAlertCtrl(m_sItem);
    }
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CAlertPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		SetNumberFont(IDC_DOSE_ALERT_NR);
		m_aListCtrl.BuildHeadLine();
		m_lLastCount = _U32(-1);
		SetValue();
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CAlertPage::OnBnClickedDoseAlertClearAlarm()
{
	// Loeschen von allen Warnings und Alarmen
	REMOTEREF.setDosePBAlarmClear(m_sItem);
	REMOTEREF.setLinePBAlarm1Quit();
	REMOTEREF.setLinePBAlarm03Quit();
	SetValue();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CAlertPage::getListChanged() const
{
	return LISTCHANGED;
}

//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CAlertPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_DOSE_ALERT_NR:
	{
		// Set the text color to red
		pDC->SetTextColor(INDEXCOLOR);

		// Set the background mode for text to transparent 
		// so background will show thru.
		pDC->SetBkMode(TRANSPARENT);
	}
	break;

	case IDC_DOSE_ALERT_IMAGE:
	{
		m_aGrafikContainer.Update(pDC);
	}
	break;
	default:
		break;
	}
	return hbr;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlertPage::OnColumnclickAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_aListCtrl.OnColumnclickAlarmList(pNMHDR);
	*pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlertPage::OnLvnItemActivateAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
	(void) m_aListCtrl.OnLvnItemActivateAlarmList( pNMHDR);
	*pResult = 0;
}



