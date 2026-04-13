//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsRecipePage
///
/// @file   SettingsRecipePage.cpp
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
#include <process.h>
#include <io.h>
#include "BASE/Utils/public/xml/SystemBackupXml.h"
#include "EasyControl.h"
#include "EasyControlVer.h"
#include "ECMessageBox.h"
#include "SettingsRecipePage.h"
#include "RemoteControl.h"
#include "MFCMacros.h"




#define ICON_SIZEX 50
#define ICON_SIZEY 40
#define MYID_LANGUAGESELECT 1


#define PATHTOTOUCH         _T("C:\\Programme\\EloTouchSystems\\EloVa.exe")
#define RUNDLL32APPLICATION _T("rundll32.exe")
#define RUNDLL32PARAMETER   _T("shell32.dll,Control_RunDLL timedate.cpl,,0")


IMPLEMENT_DYNAMIC(CSettingsRecipePage, CBCGPPropertyPage)
CSettingsRecipePage::CSettingsRecipePage()
	: CBCGPPropertyPage(CSettingsRecipePage::IDD)
	, c_Lang{
				{ base::eLanguage::eGERMAN,    { IDS_LANGGERMAN, IDI_FLAGGERMAN } },
				{ base::eLanguage::eENGLISH,   { IDS_LANGENGLISH, IDI_FLAGENGLISCH } },
				{ base::eLanguage::eFRENCH,    { IDS_LANGFRENCH, IDI_FLAGFRENCH  } },
				{ base::eLanguage::eITALIAN,   { IDS_LANGITALIAN, IDI_FLAGITALIAN } },
				{ base::eLanguage::eDUTCH,     { IDS_LANGDUTCH, IDI_FLAGDUTCH } },
				{ base::eLanguage::eTURKISH,   { IDS_LANGTURKISH, IDI_FLAGTURKISH } },
				{ base::eLanguage::ePOLISH,    { IDS_LANGPOLISH, IDI_FLAGPOLISH } },
				{ base::eLanguage::eSPANISH,   { IDS_LANGSPANISH, IDI_FLAGSPANISH } },
				{ base::eLanguage::eRUSSIAN,   { IDS_LANGRUSSIAN, IDI_FLAGRUSSIAN } },
				{ base::eLanguage::eCHINESE,   { IDS_LANGCHINESE, IDI_FLAGCHINESE } },
				{ base::eLanguage::eUKRAINIAN, { IDS_LANGUKRAINIAN, IDI_FLAGUKRAINIAN } },
				{ base::eLanguage::eHUNGARIAN, { IDS_LANGHUNGARIAN, IDI_FLAGHUNGARIAN } },
				{ base::eLanguage::eGREEK,	   { IDS_LANGGREEK, IDI_FLAGGREEK } } }
	, m_iNewLang(base::eLanguage::eGERMAN)
	, m_iLang(base::eLanguage::eGERMAN)
	, m_bModified(FALSE)
    , m_szVersion("")
{
	ASSERT(c_Lang.size() == _S32(base::eLanguage::eMAXLANGUAGE));
}
//***************************************************************************************************
//***************************************************************************************************
void CSettingsRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SETTINGS_RECIPE_VERSION, m_szVersion);
}
//***************************************************************************************************
//***************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsRecipePage, CBCGPPropertyPage)
    ON_BN_CLICKED(IDC_SETTINGS_RECIPE_TOUCH, OnBnClickedSettingsRecipeTouch)
    ON_BN_CLICKED(IDC_SETTINGS_RECIPE_DATE, OnBnClickedSettingsRecipeDate)
	ON_BN_CLICKED(IDC_SETTINGS_RECIPE_EXPORT, OnBnClickedSettingsRecipeExport)
	ON_BN_CLICKED(IDC_SETTINGS_RECIPE_IMPORT, OnBnClickedSettingsRecipeImport)
	ON_CBN_SELCHANGE(MYID_LANGUAGESELECT, OnCbnSelchangeSettingsRecipeLanguageselect)
    ON_WM_CREATE()
END_MESSAGE_MAP()
//***************************************************************************************************
//***************************************************************************************************
void CSettingsRecipePage::SetModified(BOOL b)
{
	if ( b )
	{
		m_bModified = b;
	}
	CBCGPPropertyPage :: SetModified(b);
}
//***************************************************************************************************
//***************************************************************************************************
BOOL CSettingsRecipePage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		COMBOBOXEXITEM cbi;
		CString szFormat;
		CString aStr;
		CRect aRect;

		{
			CWnd* pWnd = GetDlgItem(IDC_SYSTEM_CONFIGURATION_LANGUAGE_SELECT);
			ASSERT(pWnd);
			pWnd->GetWindowRect(&aRect);
		}
		ScreenToClient(aRect);
		m_aLanguageCtrl.MoveWindow(aRect);
		m_aLanguageCtrl.SetImageList(&m_aImageList);

		for (auto& item : c_Lang)
		{
			cbi.mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
			cbi.iImage = _S32(item.first);
			cbi.iItem = _S32(item.first);
			cbi.iSelectedImage = _S32(item.first);
			VERIFY(szFormat.LoadString(item.second.first));
			cbi.pszText = LPTSTR(LPCTSTR(szFormat));
			cbi.cchTextMax = szFormat.GetLength();;
			(void)m_aLanguageCtrl.InsertItem(&cbi);
		}

		m_szVersion.Format(_T("%s %s %s %s"), LPCTSTR(SZTITLE), LPCTSTR(SZVERSION), LPCTSTR(SZVERSIONNR), LPCTSTR(RELEASENR));
		m_iLang = EASYCONTROLREF.m_Language;
		m_iNewLang = m_iLang;
		m_aLanguageCtrl.SetCurSel(_S32(m_iNewLang));

		BOOL bPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOUCHKALIBRIEREN);
		ENABLE_ID(IDC_SETTINGS_RECIPE_TOUCH, bPermission);
		bPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SYSTEMZEIT);
		ENABLE_ID(IDC_SETTINGS_RECIPE_DATE, bPermission);

		bPermission = USERRIGHTSREF.IsAktSupervisor();
		//bPermission = LAYOUTCONFIGREF.GetLCAuthorize();
		ENABLE_ID(IDC_SETTINGS_RECIPE_EXPORT, bPermission);
		ENABLE_ID(IDC_SETTINGS_RECIPE_IMPORT, bPermission);

		UpdateData(FALSE);
		m_aLanguageCtrl.ShowWindow(TRUE);
	}
	return bResult;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::GetValue ()
{
	UpdateData(TRUE);

	if ( m_iNewLang != m_iLang)
	{
		m_iLang = m_iNewLang;
		ASSERT(_S32(m_iNewLang) < _S32(c_Lang.size()));
		EASYCONTROLREF.m_Language = m_iLang;
		ECMessageBox(IDS_LANUAGEUPDATE, MB_ICONEXCLAMATION | MB_OK);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CSettingsRecipePage::OnApply()
{
	if ( m_bModified )
	{
		m_bModified = FALSE;
		GetValue();
	}
	return CBCGPPropertyPage::OnApply();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnOK()
{
	if ( m_bModified )
	{
		m_bModified = FALSE;
		GetValue();
	}
	CBCGPPropertyPage::OnOK();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnCbnSelchangeSettingsRecipeLanguageselect()
{
	int32_t iID = m_aLanguageCtrl.GetCurSel();
	if ( iID != CB_ERR)
	{
		m_iNewLang = static_cast<base::eLanguage>(iID);
		SetModified(TRUE);
        SetFocus();
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnBnClickedSettingsRecipeTouch()
{
    STARTUPINFO         stgStartupInfo;               // startup information
    PROCESS_INFORMATION stgProcessInfo;
    CString szApplicationName(PATHTOTOUCH);
    CString szCMDLine(PATHTOTOUCH);

    ZeroMemory(&stgStartupInfo, sizeof(stgStartupInfo));
    stgStartupInfo.cb = sizeof(stgStartupInfo);
    
    LPTSTR szCmdline = _tcsdup(LPCTSTR(szCMDLine));
    CreateProcess(szApplicationName, szCmdline, NULL, NULL, false, 
                    CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS, NULL, NULL, 
                        &stgStartupInfo, &stgProcessInfo);
    Sleep(2000);
#if _DEBUG
    DWORD dErg = :: WaitForSingleObject(stgProcessInfo.hProcess, 60000);
    ASSERT(dErg == WAIT_OBJECT_0);
#else
	(void)::WaitForSingleObject(stgProcessInfo.hProcess, 60000);
#endif
	free(szCmdline);
    szCmdline = NULL;
    CloseHandle(stgProcessInfo.hProcess);
    CloseHandle(stgProcessInfo.hThread);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnBnClickedSettingsRecipeDate()
{
    STARTUPINFO         stgStartupInfo;               // startup information
    PROCESS_INFORMATION stgProcessInfo;
    TCHAR szSystemDir[256];


    ZeroMemory(&stgStartupInfo, sizeof(stgStartupInfo));
    stgStartupInfo.cb = sizeof(stgStartupInfo);

#if _DEBUG
    UINT uiCount = GetSystemDirectory(szSystemDir, sizeof(szSystemDir));
    ASSERT(uiCount > 0);
#else
	(void)GetSystemDirectory(szSystemDir, sizeof(szSystemDir));
#endif

	CString szApplicationName;
	szApplicationName.Format(_T("%s\\%s"), LPCTSTR(szSystemDir), LPCTSTR(RUNDLL32APPLICATION));

	CString szCMDLine;
	szCMDLine.Format(_T("%s %s"), LPCTSTR(szApplicationName), LPCTSTR(RUNDLL32PARAMETER));
    LPTSTR szCmdline = _tcsdup(LPCTSTR(szCMDLine));
    CreateProcess(szApplicationName, szCmdline, NULL, NULL, false, 
                    CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS, NULL, NULL, 
                        &stgStartupInfo, &stgProcessInfo);
    Sleep(2000);
#if _DEBUG
    DWORD dErg = :: WaitForSingleObject(stgProcessInfo.hProcess, 60000);
    ASSERT(dErg == WAIT_OBJECT_0);
#else
	(void)::WaitForSingleObject(stgProcessInfo.hProcess, 60000);
#endif
	free(szCmdline);
    szCmdline = NULL;
    CloseHandle(stgProcessInfo.hProcess);
    CloseHandle(stgProcessInfo.hThread);
}
//***********************************************************************************************************
//***********************************************************************************************************
int32_t CSettingsRecipePage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBCGPPropertyPage::OnCreate(lpCreateStruct) == -1)
        return -1;

	const int32_t ImageMax = _S32(c_Lang.size());
	m_aImageList.Create(ICON_SIZEX, ICON_SIZEY, ILC_COLOR8, ImageMax, ImageMax); // 32, 32 for large icons

	for (auto& item : c_Lang)
	{
		HICON  aIcon = AfxGetApp()->LoadIcon(item.second.second);
		ASSERT(aIcon);
		VERIFY(m_aImageList.Add(aIcon) >= 0);
	}
	m_aLanguageCtrl.Create(CBS_DROPDOWNLIST, CRect(100,80,200,270), this, MYID_LANGUAGESELECT);
    return 0;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnBnClickedSettingsRecipeExport()
{
	static TCHAR BASED_CODE szFilter[] = _T("xml - Files (*.xml)|*.xml|");
	CFileDialog dlg(FALSE, _T("*.xml"), NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		base::xml::CSystemBackupXml backup;
		ExportDataItem(backup.Get());
		auto result = backup.Save(toStdString(dlg.GetPathName()));
		if (!result)
		{
			ECMessageBox(IDS_ERROR_EXPORT_DATA, MB_OK);
		}
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::OnBnClickedSettingsRecipeImport()
{
	static TCHAR BASED_CODE szFilter[] = _T("xml - Files (*.xml)|*.xml|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		base::xml::CSystemBackupXml backup;
		auto result = backup.Load(toStdString(dlg.GetPathName()));
		if (!result)
		{
			ECMessageBox(IDS_ERROR_IMPORT_DATA, MB_OK);
		}
		else
		{
			ImportDataItem(backup.Get());
		}
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::ExportDataItem(base::CSystemBackup& field)
{
	CWaitCursor wait;

	uint32_t count = 0;
	REMOTEREF.getMaxDoseCount(count);
	field.resize(count);
	for (int32_t k = 0; k < _S32(count); k++)
	{
		auto& rItem = field[k];
		rItem.ID = k + 1U;
		REMOTEREF.getDoseType(k, rItem.DoseType);
		REMOTEREF.getDoseName(k, rItem.Name);
		REMOTEREF.getDoseQMNumber(k, rItem.QMNumber);
		REMOTEREF.getDoseFeedingType(k, rItem.FeedingType);
		REMOTEREF.getDosePercentage(k, rItem.Percentage);
		REMOTEREF.getDoseSetpoint(k, rItem.NominalSetpoint);
		REMOTEREF.getDoseMaxSetpoint(k, rItem.MaxSetpoint);
		REMOTEREF.getDoseMaxRotationalSpeed(k, rItem.MaxRotationalSpeed);
		REMOTEREF.getDoseMassflowFilter(k, rItem.MassflowFilter);

		REMOTEREF.getDoseRefillLimit(k, rItem.RefillLimitMin, rItem.RefillLimitMax, rItem.RefillLimitMinMin);
		REMOTEREF.getDoseRefillTime(k, rItem.RefillTime);
		REMOTEREF.getDoseRefillSwitchDelay(k, rItem.RefillSwitchDelay);
		REMOTEREF.getDoseRefillDebounceMax(k, rItem.RefillDebounceMax);
		REMOTEREF.getDoseRefillDebounceMin(k, rItem.RefillDebounceMin);
		REMOTEREF.getDoseRefillFeederEmptyStart(k, rItem.RefillFeederEmptyStart);

		REMOTEREF.getDoseAgitatorSetpoint(k, rItem.NominalAgitator);
		REMOTEREF.getDoseMinSetpointChange(k, rItem.MinSetpointChange);
		REMOTEREF.getDoseStartupDelay(k, rItem.StartupDelay);
		REMOTEREF.getDoseStartupRamp(k, rItem.StartupRamp);
		REMOTEREF.getDoseLCCorrectionFactor(k, rItem.LCCorrectionFactor);
		REMOTEREF.getDoseLCTaraWeight(k, rItem.LCTaraWeight);
		REMOTEREF.getDoseEncoderMonitor(k, rItem.EncoderMonitor);
		REMOTEREF.getDoseAlarmNoiseLimit(k, rItem.AlarmNoiseLimit);
		REMOTEREF.getDoseAlarmReactionDelay(k, rItem.AlarmReactionDelay);
		REMOTEREF.getDoseAlarmStartReactionDelay(k, rItem.AlarmStartReactionDelay);
		REMOTEREF.getDoseAlarmMassflowLow(k, rItem.AlarmMassflowLow);
		REMOTEREF.getDoseAlarmMassflowHigh(k, rItem.AlarmMassflowHigh);
		REMOTEREF.getDoseAlarmDriveCommandHigh(k, rItem.AlarmDriveCommandHigh);
		REMOTEREF.getDoseAlarmDriveCommandLow(k, rItem.AlarmDriveCommandLow);
		REMOTEREF.getDoseAlarmDosePerformance(k, rItem.AlarmDosePerformance);
		REMOTEREF.getDoseAlarmMaxBatchTime(k, rItem.AlarmMaxBatchTime);
		REMOTEREF.getDoseMaxDriveCommandChange(k, rItem.MaxDriveCommandChange);
		REMOTEREF.getDoseRegenerat(k, rItem.Regenerat);
		REMOTEREF.getDoseWbfBeltLoadSetpoint(k, rItem.WbfBeltLoadSetpoint);
		REMOTEREF.getDoseWbfBeltLoadVolSwitch(k, rItem.WbfBeltLoadVolSwitch);
		REMOTEREF.getDoseWbfReduction(k, rItem.WbfReduction);
		REMOTEREF.getDoseWbfWeighingLine(k, rItem.WbfWeighingLine);
		REMOTEREF.getDoseWbfWheelSize(k, rItem.WbfWheelSize);
		REMOTEREF.getDoseWbfTareDriveCommand(k, rItem.WbfTareDriveCommand);
		REMOTEREF.getDoseWbfTareTime(k, rItem.WbfTareMeasurementTime);
		REMOTEREF.getDoseAlarmWbfMinBeltLoad(k, rItem.WbfAlarmMinBeltLoad);
		REMOTEREF.getDoseAlarmWbfMaxBeltLoad(k, rItem.WbfAlarmMaxBeltLoad);
		REMOTEREF.getDoseWbfMinDriveCommand(k, rItem.WbfMinDriveCommand);
		REMOTEREF.getDoseWbfVerwiegungFull(k, rItem.WbfWeighingFull);
		REMOTEREF.getDosePidPropGainGross(k, rItem.PidPropGainGross);
		REMOTEREF.getPidSampleInterval(k, rItem.PidSampleInterval);
		REMOTEREF.getDosePidPropGainFine(k, rItem.PidPropGainFine);
		REMOTEREF.getDosePidPropGainSwitchGrossFine(k, rItem.PidPropGainSwitchGrossFine);
		REMOTEREF.getDosePidIntegralGain(k, rItem.PidIntegralGain);
		REMOTEREF.getDoseDriveCommandInverse(k, rItem.PidDriveCommandInv);
		REMOTEREF.getDoseGatefilter(k, rItem.PidGatefilter);
		REMOTEREF.getDoseMassflowSteepness(k, rItem.SteepnessMassflow);
		REMOTEREF.getDoseEmptyFeederSpeed(k, rItem.EmptyFeederSpeed);
		REMOTEREF.getDoseCalibCfg(k, rItem.calibType);

		REMOTEREF.getIfsReduceFactor(k, rItem.IfsReduceFactor);
		REMOTEREF.getIfsGainFactor(k, rItem.IfsGainFactor);
		REMOTEREF.getIfsFeederOverflowTimeOut(k, rItem.IfsFeederOverflowTimeOut);
		REMOTEREF.getIfsFeederEmptyTimeOut(k, rItem.IfsFeederEmptyTimeOut);
		REMOTEREF.getIfsStepTimeGain(k, rItem.IfsStepTimeGain);
		REMOTEREF.getIfsStepTimeReduce(k, rItem.IfsStepTimeReduce);
		REMOTEREF.getIfsSetpointOverflow(k, rItem.IfsSetpointOverflow);
		REMOTEREF.getIfsDebounceMax(k, rItem.IfsDebounceMax);
		REMOTEREF.getIfsDebounceMin(k, rItem.IfsDebounceMin);
	}
}

//***********************************************************************************************************
//***********************************************************************************************************
void CSettingsRecipePage::ImportDataItem(const base::CSystemBackup& field)
{
	CWaitCursor wait;

	uint32_t count = 0;
	REMOTEREF.getMaxDoseCount(count);

	const uint32_t maxcount = __min(_U32(field.size()), count);
	for (uint32_t k = 0; k < maxcount; k++)
	{
		const auto& rItem = field[k];
		REMOTEREF.setDoseName(k, rItem.Name);
		ASSERT(rItem.ID == k + 1U);
		base::eDoseType type = base::eDoseType::DOSETYPE_NONE;
		REMOTEREF.getDoseType(k, type);
		ASSERT(type == rItem.DoseType);
		REMOTEREF.setDoseQMNumber(k, rItem.QMNumber);
		REMOTEREF.setDoseFeedingType(k, rItem.FeedingType);
		REMOTEREF.setDosePercentage(k, rItem.Percentage);
		REMOTEREF.setDoseSetpoint(k, rItem.NominalSetpoint);
		REMOTEREF.setDoseMaxSetpoint(k, rItem.MaxSetpoint);
		REMOTEREF.setDoseMaxRotationalSpeed(k, rItem.MaxRotationalSpeed);
		REMOTEREF.setDoseMassflowFilter(k, rItem.MassflowFilter);
		REMOTEREF.setDoseRefillLimit(k, rItem.RefillLimitMin, rItem.RefillLimitMax, rItem.RefillLimitMinMin);
		REMOTEREF.setDoseRefillTime(k, rItem.RefillTime);
		REMOTEREF.setDoseRefillSwitchDelay(k, rItem.RefillSwitchDelay);
		REMOTEREF.setDoseRefillDebounceMax(k, rItem.RefillDebounceMax);
		REMOTEREF.setDoseRefillDebounceMin(k, rItem.RefillDebounceMin);
		REMOTEREF.setDoseRefillFeederEmptyStart(k, rItem.RefillFeederEmptyStart);
		REMOTEREF.setDoseAgitatorSetpoint(k, rItem.NominalAgitator);
		REMOTEREF.setDoseMinSetpointChange(k, rItem.MinSetpointChange);
		REMOTEREF.setDoseStartupDelay(k, rItem.StartupDelay);
		REMOTEREF.setDoseStartupRamp(k, rItem.StartupRamp);
		REMOTEREF.setDoseLCCorrectionFactor(k, rItem.LCCorrectionFactor);
		REMOTEREF.setDoseLCTaraWeight(k, rItem.LCTaraWeight);
		REMOTEREF.setDoseEncoderMonitor(k, rItem.EncoderMonitor);
		REMOTEREF.setDoseAlarmNoiseLimit(k, rItem.AlarmNoiseLimit);
		REMOTEREF.setDoseAlarmReactionDelay(k, rItem.AlarmReactionDelay);
		REMOTEREF.setDoseAlarmStartReactionDelay(k, rItem.AlarmStartReactionDelay);
		REMOTEREF.setDoseAlarmMassflowLow(k, rItem.AlarmMassflowLow);
		REMOTEREF.setDoseAlarmMassflowHigh(k, rItem.AlarmMassflowHigh);
		REMOTEREF.setDoseAlarmDriveCommandHigh(k, rItem.AlarmDriveCommandHigh);
		REMOTEREF.setDoseAlarmDriveCommandLow(k, rItem.AlarmDriveCommandLow);
		REMOTEREF.setDoseAlarmDosePerformance(k, rItem.AlarmDosePerformance);
		REMOTEREF.setDoseAlarmMaxBatchTime(k, rItem.AlarmMaxBatchTime);
		REMOTEREF.setDoseMaxDriveCommandChange(k, rItem.MaxDriveCommandChange);
		REMOTEREF.setDoseRegenerat(k, rItem.Regenerat);
		REMOTEREF.setDoseWbfBeltLoadSetpoint(k, rItem.WbfBeltLoadSetpoint);
		REMOTEREF.setDoseWbfBeltLoadVolSwitch(k, rItem.WbfBeltLoadVolSwitch);
		REMOTEREF.setDoseWbfReduction(k, rItem.WbfReduction);
		REMOTEREF.setDoseWbfWeighingLine(k, rItem.WbfWeighingLine);
		REMOTEREF.setDoseWbfWheelSize(k, rItem.WbfWheelSize);
		REMOTEREF.setDoseWbfTareDriveCommand(k, rItem.WbfTareDriveCommand);
		REMOTEREF.setDoseWbfTareTime(k, rItem.WbfTareMeasurementTime);
		REMOTEREF.setDoseAlarmWbfMinBeltLoad(k, rItem.WbfAlarmMinBeltLoad);
		REMOTEREF.setDoseAlarmWbfMaxBeltLoad(k, rItem.WbfAlarmMaxBeltLoad);
		REMOTEREF.setDoseWbfMinDriveCommand(k, rItem.WbfMinDriveCommand);
		REMOTEREF.setDoseWbfVerwiegungFull(k, rItem.WbfWeighingFull);
		REMOTEREF.setDosePidPropGainGross(k, rItem.PidPropGainGross);
		REMOTEREF.setPidSampleInterval(k, rItem.PidSampleInterval);
		REMOTEREF.setDosePidPropGainFine(k, rItem.PidPropGainFine);
		REMOTEREF.setDosePidPropGainSwitchGrossFine(k, rItem.PidPropGainSwitchGrossFine);
		REMOTEREF.setDosePidIntegralGain(k, rItem.PidIntegralGain);
		REMOTEREF.setDoseDriveCommandInverse(k, rItem.PidDriveCommandInv);
		REMOTEREF.setDoseGatefilter(k, rItem.PidGatefilter);
		REMOTEREF.setDoseMassflowSteepness(k, rItem.SteepnessMassflow);
		REMOTEREF.setDoseEmptyFeederSpeed(k, rItem.EmptyFeederSpeed);
		REMOTEREF.setDoseCalibCfg(k, rItem.calibType);

		REMOTEREF.setIfsReduceFactor(k, rItem.IfsReduceFactor);
		REMOTEREF.setIfsGainFactor(k, rItem.IfsGainFactor);
		REMOTEREF.setIfsFeederOverflowTimeOut(k, rItem.IfsFeederOverflowTimeOut);
		REMOTEREF.setIfsFeederEmptyTimeOut(k, rItem.IfsFeederEmptyTimeOut);
		REMOTEREF.setIfsStepTimeGain(k, rItem.IfsStepTimeGain);
		REMOTEREF.setIfsStepTimeReduce(k, rItem.IfsStepTimeReduce);
		REMOTEREF.setIfsSetpointOverflow(k, rItem.IfsSetpointOverflow);
		REMOTEREF.setIfsDebounceMax(k, rItem.IfsDebounceMax);
		REMOTEREF.setIfsDebounceMin(k, rItem.IfsDebounceMin);

	}
}


