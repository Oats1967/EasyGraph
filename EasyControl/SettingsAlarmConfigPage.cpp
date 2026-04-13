//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsAlarmConfigPage.cpp
///
/// @file   SettingsAlarmConfigPage.cpp
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
#include "BASE/Base.def"
#include "AlarmMap.h"
#include "AlarmCtrl.h"
#include "SettingsAlarmConfigPage.h"
#include "RemoteControl.h"
#include "BASE/Utils/public/xml/SystemAlarmXml.h"
#include "MFCMacros.h"





IMPLEMENT_DYNAMIC(CSettingsAlarmConfigPage, CBCGPPropertyPage)
CSettingsAlarmConfigPage::CSettingsAlarmConfigPage()
	: CBCGPPropertyPage(CSettingsAlarmConfigPage::IDD)
	, m_aListCtrl{ this, m_AlarmField }
    , m_bModified(FALSE)
{
	base::xml::CSystemAlarmXml config;
	BOOL result = config.Load(EASYCONTROLREF.m_SystemAlarmFile);
	if (!result)
	{
		LOGERROR("Cannot Load " << EASYCONTROLREF.m_SystemAlarmFile);
	}
	m_AlarmField = config.Get();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsAlarmConfigPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS_ALARMCONFIG_LIST, m_aListCtrl);
}
//*********************************************************************************************************
//*********************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsAlarmConfigPage, CBCGPPropertyPage)
	ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, OnPageChanged)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_SETTINGS_ALARMCONFIG_LIST, &CSettingsAlarmConfigPage::OnLvnItemActivateAlarmList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SETTINGS_ALARMCONFIG_LIST, &CSettingsAlarmConfigPage::OnColumnclickAlarmList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SETTINGS_ALARMCONFIG_LIST, &CSettingsAlarmConfigPage::OnCustomdraw)
	ON_BN_CLICKED(IDC_SETTINGS_ALARMCONFIG_ACCEPT, OnBnClickedSettingsAccept)
END_MESSAGE_MAP()
//*********************************************************************************************************
//*********************************************************************************************************
LRESULT CSettingsAlarmConfigPage::OnPageChanged(WPARAM, LPARAM)
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		if (ECMessageBox(IDS_QUERY_ALARMLISTACCEPT, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			GetValue();
			SaveItems();
		}
	}
	return 0L;
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsAlarmConfigPage::GetValue()
{
	UpdateData(TRUE);
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsAlarmConfigPage::SaveItems()
{
	base::xml::CSystemAlarmXml config;
	config.Set(m_AlarmField);
	auto result = config.Save(EASYCONTROLREF.m_SystemAlarmFile);
	if (!result)
	{
		LOGERROR("Cannot save " << EASYCONTROLREF.m_SystemAlarmFile);
		ECMessageBox(IDS_ERROR_ALARMLISTSAVE, MB_ICONSTOP | MB_OK);
	}
	m_AlarmField.SetModified(FALSE);
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CSettingsAlarmConfigPage::OnApply()
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	return CBCGPPropertyPage::OnApply();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsAlarmConfigPage::OnOK()
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	CBCGPPropertyPage::OnOK();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsAlarmConfigPage::Init ()
{
	base::eAlarmErrorBits rMask;
	rMask.SetAll();
	m_AlarmField.Set(base::eAlarmError::ERROR_DOSE_SHUTDOWN, base::eAlarmCategory::categoryAM, TRUE);
	m_AlarmField.Set(base::eAlarmError::ERROR_DOSE_SHUTDOWN, base::eAlarmCategory::categoryFL, FALSE);
	m_AlarmField.Set(base::eAlarmError::ERROR_DOSE_SHUTDOWN, base::eAlarmCategory::categoryRA, FALSE);
	m_AlarmField.Set(base::eAlarmError::ERROR_DOSE_SHUTDOWN, base::eAlarmCategory::categoryS, FALSE);
	m_AlarmField.Set(base::eAlarmError::ERROR_DOSE_SHUTDOWN, base::eAlarmCategory::categorySD, FALSE);
	m_aListCtrl.FillAlarmCtrl(rMask);
	SetModified(m_bModified = FALSE);
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CSettingsAlarmConfigPage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		m_aListCtrl.SetPermissionModify(USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ALARMSETTINGS));
		m_aListCtrl.BuildHeadLine();
		Init();
	}
	return bResult;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsAlarmConfigPage::OnLvnItemActivateAlarmList(NMHDR *pNMHDR, LRESULT *pResult)
{
	BOOL modified = m_aListCtrl.OnLvnItemActivateAlarmList(pNMHDR);
	m_bModified |= modified;
	SetModified(m_bModified);
	*pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsAlarmConfigPage::OnColumnclickAlarmList(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_aListCtrl.OnColumnclickAlarmList(pNMHDR);
	*pResult = 0;
}
//*********************************************************************************************************
//*********************************************************************************************************
BOOL CSettingsAlarmConfigPage::OnSetActive()
{
	GetParent()->SendMessage(WM_PROPERTYPAGE_CHANGED, WPARAM(this));
	Init();
	return CBCGPPropertyPage::OnSetActive();
}
//*********************************************************************************************************
//*********************************************************************************************************
void CSettingsAlarmConfigPage::OnBnClickedSettingsAccept(void)
{
    if ( ECMessageBox(IDS_SYSTEM_ALARMCFG_ACCEPT_SETTINGS, MB_ICONQUESTION  | MB_YESNO) == IDYES)
    {
		uint32_t count = 0;
		REMOTEREF.getMaxDoseCount(count);
		DECLARE_ALARMERROR(alarmfield);
		DECLARE_ALARMCATEGORY(categoryfield)
		for (uint32_t i = 0; i < count; i++)
        {
			base::eDoseType lDoseType;
			REMOTEREF.getDoseType(_S32(i), lDoseType);
			base::eAlarmErrorBits mask(lDoseType);
			for (const auto& alarmindex : alarmfield)
            {
				auto alarmerrorbits = mask.IsAlarmErrorBits(alarmindex);
				for (const auto& category : categoryfield)
				{
					BOOL bActive = FALSE;
					if (alarmerrorbits)
					{
						bActive = m_AlarmField.Get(alarmindex, category);
					}
					REMOTEREF.setDoseAlarmCfgBit(i, category, alarmindex, bActive);
				}
            }
        }
    }
	SetModified(m_bModified = TRUE);
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsAlarmConfigPage::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_aListCtrl.OnCustomdraw(pNMHDR, pResult);
}


