//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmConfigPage.cpp
///
/// @file   AlarmConfigPage.cpp
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
#include "GlobalConst.h"
#include "EasyControl.h"
#include "RemoteControl.h"
#include "AlarmMap.h"
#include "DataImage.h"
#include "AlarmConfigPage.h"
#include "MFCMacros.h"


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CAlarmConfigPage, CDosePage)
    ON_NOTIFY(LVN_ITEMACTIVATE, IDC_DOSE_ALARMCONFIG_LIST, &CAlarmConfigPage::OnLvnItemActivateAlarmList)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_DOSE_ALARMCONFIG_LIST, &CAlarmConfigPage::OnColumnclickAlarmList)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_DOSE_ALARMCONFIG_LIST, &CAlarmConfigPage::OnCustomdraw)
    ON_MESSAGE(WM_NOTIFY_ALARMERROR, OnAlarmError)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CAlarmConfigPage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CAlarmConfigPage::CAlarmConfigPage(): CDosePage(CAlarmConfigPage::IDD)
    , m_aListCtrl{ this, m_AlarmField }
    , m_AlarmField{}
    , m_bPermissionModify(FALSE)
    , m_LastItem(-1)
    , m_LastDoseType(base::eDoseType::DOSETYPE_NONE)
{
}
//****************************************************************************************************
//****************************************************************************************************
void CAlarmConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_DOSE_ALARMCONFIG_TITLE, m_szTitle);
    DDX_TextN(pDX, IDC_DOSE_ALARMCONFIG_NAME, m_szName, 30);
    DDX_Text(pDX, IDC_DOSE_ALARMCONFIG_NR, m_lNr);

    DDX_Control(pDX, IDC_DOSE_ALARMCONFIG_LIST, m_aListCtrl);
    DDX_Control(pDX, IDC_DOSE_ALARMCONFIG_IMAGE, m_aGrafikContainer);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CAlarmConfigPage::OnUpdateControls(void)
{
    if (m_sItem < 0 )
    {
        return FALSE;
    }
    BOOL bNew = FALSE;
    if (m_lDoseType != m_LastDoseType)
    {
        m_LastDoseType = m_lDoseType;
        bNew = TRUE;
    }
    if (m_sItem != m_LastItem)
    {
        m_LastItem = m_sItem;
        bNew = TRUE;
    }
    if ( bNew )
    {
        InitAlarmField();
        m_aListCtrl.FillAlarmCtrl(base::eAlarmErrorBits(m_LastDoseType));
	}
    m_aListCtrl.SetPermissionModify(USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMSETTINGS));
    return CDosePage::OnUpdateControls();
}
//****************************************************************************************************
//****************************************************************************************************
void CAlarmConfigPage::InitAlarmField()
{
    ASSERT(m_sItem >= 0);

    DECLARE_ALARMERROR(alarmerror)
    DECLARE_ALARMCATEGORY(category)
    m_AlarmField = base::CAlarmField();
    for (const auto& alarmItem : alarmerror)
    {
        for (const auto& categoryitem : category)
        {
            BOOL bA = FALSE;
            REMOTEREF.getDoseAlarmCfgBit(m_sItem, categoryitem, alarmItem, bA);
            m_AlarmField.Set(alarmItem, categoryitem, bA);
        }
    }
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CAlarmConfigPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
    if (result)
    {
        m_aListCtrl.BuildHeadLine();

        m_LastItem = -1;
        m_LastDoseType = base::eDoseType::DOSETYPE_NONE;

        SetNumberFont(IDC_DOSE_ALARMCONFIG_NR);
        SetValue();
    }
    return result;
}
//****************************************************************************************************
//****************************************************************************************************
HBRUSH CAlarmConfigPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_DOSE_ALARMCONFIG_NR:
        {
            // Set the text color to red
            pDC->SetTextColor(INDEXCOLOR);

            // Set the background mode for text to transparent 
            // so background will show thru.
            pDC->SetBkMode(TRANSPARENT);
        }
        break;
        case IDC_DOSE_ALARMCONFIG_IMAGE:
        {
            m_aGrafikContainer.Update(pDC);
        }
        break;
        default:
            break;
    }
    return hbr;
}
//****************************************************************************************************
//****************************************************************************************************
LRESULT CAlarmConfigPage::OnAlarmError(WPARAM wp, LPARAM)
{
    CAlarmCtrl::AlarmChange* pAlarmError = (CAlarmCtrl::AlarmChange * )wp;
    ASSERT(pAlarmError);
    REMOTEREF.setDoseAlarmCfgBit(m_sItem, pAlarmError->category, pAlarmError->alarmerror, pAlarmError->value);
    return 0L;
}
//****************************************************************************************************
//****************************************************************************************************
void CAlarmConfigPage::OnLvnItemActivateAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
    BOOL modified = m_aListCtrl.OnLvnItemActivateAlarmList(pNMHDR);
    SetModified(modified);
    *pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CAlarmConfigPage::OnColumnclickAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_aListCtrl.OnColumnclickAlarmList(pNMHDR);
    *pResult = 0;
}
//****************************************************************************************************
//****************************************************************************************************
void CAlarmConfigPage::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_aListCtrl.OnCustomdraw(pNMHDR, pResult);
}



