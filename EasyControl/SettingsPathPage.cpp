//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsPathPage
///
/// @file   SettingsPathPage.cpp
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
#include "RemoteControl.h"
#include "SettingsPathPage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



//*****************************************************************************************************
//*****************************************************************************************************
IMPLEMENT_DYNAMIC(CSettingsPathPage, CBCGPPropertyPage)
CSettingsPathPage::CSettingsPathPage()
	: CBCGPPropertyPage(CSettingsPathPage::IDD)
	, m_szLogfilePath(toCString(EASYCONTROLREF.m_AlarmLogPath))
	, m_szRecorderPath(toCString(RECORDERCONFIGREF.m_RecorderPath))
	, m_szRecipePath(toCString(EASYCONTROLREF.m_RecipePath))
	, m_szProductDatabaseFile(toCString(EASYCONTROLREF.m_ProductDatabaseFile))
	, m_bModified(FALSE)
    , m_bRecorderAktiv(RECORDERCONFIGREF.m_RecorderActive)
    , m_lRecorderInterval(RECORDERCONFIGREF.m_RecorderSampeTime)
    , c_EditBrush(EDITBKCOLOR)
{
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETTINGS_PATH_PRODUCTDATABASEFILE, m_szProductDatabaseFile);
	DDX_Text(pDX, IDC_SETTINGS_PATH_LOGPATH, m_szLogfilePath);
	DDX_Text(pDX, IDC_SETTINGS_PATH_RECORDERPATH, m_szRecorderPath);
	DDX_Text(pDX, IDC_SETTINGS_PATH_RECORDER_INTERVAL, m_lRecorderInterval);
	DDX_Text(pDX, IDC_SETTINGS_PATH_RECIPEPATH, m_szRecipePath);
}
//*****************************************************************************************************
//*****************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsPathPage, CBCGPPropertyPage)
	ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, OnPageChanged)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_SETTINGS_PATH_PRODUCTDATABASEFILE, OnBnClickedSettingsPathProductDatabaseFileButton)
	ON_STN_CLICKED(IDC_SETTINGS_PATH_LOGPATH, OnBnClickedSettingsPathLogfilePathButton)
	ON_STN_CLICKED(IDC_SETTINGS_PATH_RECORDERPATH, OnBnClickedSettingsPathRecorderPathButton)
	ON_STN_CLICKED(IDC_SETTINGS_PATH_RECORDER_INTERVAL, OnBnClickedSettingsPathRecorderPathIntervalButton)
	ON_STN_CLICKED(IDC_SETTINGS_PATH_RECIPEPATH, OnBnClickedSettingsPathRecipePathButton)
    ON_BN_CLICKED(IDC_SETTINGS_PATH_RECORDER_ACTIVE, OnBnClickedSettingsPathRecorderPathActiveButton)
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CSettingsPathPage::OnPageChanged(WPARAM, LPARAM)
{
	if ( m_bModified )
	{
		if ( ECMessageBox(IDS_SYSTEM_QUERY_CHANGED, MB_ICONQUESTION  | MB_YESNO) == IDYES)
		{
			GetValue();
		}
		m_bModified = FALSE;
	}
	return 0L;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::SetModified(BOOL b)
{
    m_bModified = b;
	CBCGPPropertyPage :: SetModified(b);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CSettingsPathPage::OnNotifyRecorderInterval()
{
    BOOL bModified = FALSE;
    auto value = CEditCtrl::GetLong();
    if (value < 5 || value > 3600)
    {
        ECMessageBox(IDS_ERROR_RECORDERINTERVAL, MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        m_lRecorderInterval = value;
        bModified = TRUE;
    }
    return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CSettingsPathPage::OnNotifyEdit(WPARAM id, LPARAM)
{
    BOOL bModified = FALSE;

    switch(id)
	{
    case IDC_SETTINGS_PATH_RECORDER_INTERVAL:	bModified = OnNotifyRecorderInterval();
					                            break;

		default:
					ASSERT(FALSE);
					break;
	}
    if (bModified)
    {
        SetModified(TRUE);
        UpdateData(FALSE);
    }
	return 0L;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::SetControlStyle()
{
    BOOL bPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SYSTEMFOLDER);
	ENABLE_ID(IDC_SETTINGS_PATH_PRODUCTDATABASEFILE, bPermission);
	ENABLE_ID(IDC_SETTINGS_PATH_LOGPATH, bPermission);
    ENABLE_ID(IDC_SETTINGS_PATH_RECORDERPATH, bPermission  && m_bRecorderAktiv);
    ENABLE_ID(IDC_SETTINGS_PATH_RECORDER_INTERVAL, bPermission && m_bRecorderAktiv);
    ENABLE_ID(IDC_SETTINGS_PATH_RECORDER_ACTIVE, bPermission);
    CHECK_ID(IDC_SETTINGS_PATH_RECORDER_ACTIVE, m_bRecorderAktiv);
    ENABLE_ID(IDC_SETTINGS_PATH_RECIPEPATH, bPermission);
    UpdateData(FALSE);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::GetValue ()
{
	UpdateData(TRUE);

	std::string szLogPath = toStdString(m_szLogfilePath);
	LOGREF.SetPath(szLogPath);
	EASYCONTROLREF.m_AlarmLogPath = szLogPath;
	{
		auto szTemp = toStdString(m_szProductDatabaseFile);
		if (EASYCONTROLREF.m_ProductDatabaseFile != szTemp)
		{
			EASYCONTROLREF.m_ProductDatabaseFile = szTemp;
			CEasyControlApp::LoadProductDatabase();
		}
	}
	RECORDERCONFIGREF.m_RecorderPath = toStdString(m_szRecorderPath);
    RECORDERCONFIGREF.m_RecorderActive = m_bRecorderAktiv;
    RECORDERCONFIGREF.m_RecorderSampeTime = m_lRecorderInterval;
	EASYCONTROLREF.m_RecipePath = toStdString(m_szRecipePath);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathProductDatabaseFileButton()
{
	static TCHAR BASED_CODE szFilter[] = _T("ProductDatabase.xml (ProductDatabase.xml)|ProductDatabase.xml|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		m_szProductDatabaseFile = dlg.GetPathName();
		SetModified(TRUE);
		SetControlStyle();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathLogfilePathButton()
{
	CFolderPickerDialog dlg(m_szLogfilePath, 0, this);
	if (dlg.DoModal() == IDOK)
	{
		m_szLogfilePath = dlg.GetPathName();
        SetModified(TRUE);
        SetControlStyle();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathRecorderPathButton()
{
	CFolderPickerDialog dlg(m_szRecorderPath, 0, this);
	if (dlg.DoModal() == IDOK)
	{
		if (CEasyControlApp::IsLocalDrive(dlg.GetFolderPath()) )
		{
			ECMessageBox(IDS_WARNING_RECORDERPATH, MB_ICONEXCLAMATION | MB_OK);
		}
        if (m_szRecorderPath != dlg.GetPathName())
        {
            m_szRecorderPath = dlg.GetPathName();
            SetModified(TRUE);
            SetControlStyle();
        }
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathRecorderPathIntervalButton()
{
	CEditCtrl :: GetInput (this, E_TYPCTRL::E_INTCTRL, IDC_SETTINGS_PATH_RECORDER_INTERVAL);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathRecorderPathActiveButton()
{
    m_bRecorderAktiv = ! m_bRecorderAktiv;
	SetModified(TRUE);
    //Invalidate(FALSE);
    SetControlStyle();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnBnClickedSettingsPathRecipePathButton()
{
	CFolderPickerDialog dlg(m_szRecipePath, 0, this);
	if ( dlg.DoModal() == IDOK)
	{
		m_szRecipePath = dlg.GetPathName();
		SetModified(TRUE);
        SetControlStyle();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::OnOK()
{
	if ( m_bModified )
	{
		m_bModified = FALSE;
		GetValue();
	}
	CBCGPPropertyPage::OnOK();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CSettingsPathPage::OnApply()
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		GetValue();
	}
	return CBCGPPropertyPage::OnApply();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CSettingsPathPage::Init()
{
	m_szProductDatabaseFile = toCString(EASYCONTROLREF.m_ProductDatabaseFile);
	m_szLogfilePath = toCString(EASYCONTROLREF.m_AlarmLogPath);
	m_szRecorderPath = toCString(RECORDERCONFIGREF.m_RecorderPath);
	m_szRecipePath = toCString(EASYCONTROLREF.m_RecipePath);
	m_bRecorderAktiv = RECORDERCONFIGREF.m_RecorderActive;
	m_lRecorderInterval = RECORDERCONFIGREF.m_RecorderSampeTime;
	m_bModified = FALSE;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CSettingsPathPage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		Init();
		SetControlStyle();
	}
	return bResult;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CSettingsPathPage::OnSetActive()
{
	GetParent()->SendMessage(WM_PROPERTYPAGE_CHANGED, WPARAM(this));
	Init ();
	return CBCGPPropertyPage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CSettingsPathPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CBCGPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
		case IDC_SETTINGS_PATH_PRODUCTDATABASEFILE:
		case IDC_SETTINGS_PATH_LOGPATH                 :
	    case IDC_SETTINGS_PATH_RECORDERPATH            :
	    case IDC_SETTINGS_PATH_RECIPEPATH            :
        case IDC_SETTINGS_PATH_RECORDER_INTERVAL  :
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
    }
    return hbr;
}




