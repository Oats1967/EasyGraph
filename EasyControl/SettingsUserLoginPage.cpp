//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsUserLoginPage.cpp
///
/// @file   SettingsUserLoginPage.cpp
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
#include "SettingsUserLoginPage.h"
#include "UserNameDlg.h"
#include "GlobalConst.h"
#include "MFCMacros.h"


#define ICON_SIZE 16
#define IMAGE_MAX 1

#define ICON_CB_SIZEX 30
#define ICON_CB_SIZEY 40
#define IMAGE_CB_MAX   1
#define MYID_CB_USERSELECT 1


#define YESCOLOR RGB(0,0,0)
#define NOCOLOR  RGB(200,0,0)


#define INVALIDUSERCLASS static_cast<base::utils::eUserClass>(-1)


IMPLEMENT_DYNAMIC(CSettingsUserLoginPage, CBCGPPropertyPage)
CSettingsUserLoginPage::CSettingsUserLoginPage()
	: CBCGPPropertyPage(CSettingsUserLoginPage::IDD)
	, c_categorymaplist{ {base::utils::eUserCategory::LOGIN_RECIPE_OPEN, IDS_LOGINPERMISSION_RECIPEOEFFNEN},
							{base::utils::eUserCategory::LOGIN_RECIPE_SAVE, IDS_LOGINPERMISSION_RECIPESPEICHERN},
							{base::utils::eUserCategory::LOGIN_RECIPE_ACTIVATE, IDS_LOGINPERMISSION_RECIPEAKTIVIEREN},
							{base::utils::eUserCategory::LOGIN_RECIPE_EDIT, IDS_LOGINPERMISSION_RECIPEBEARBEITEN},
							{base::utils::eUserCategory::LOGIN_RECIPE_DELETE, IDS_LOGINPERMISSION_RECIPELOESCHEN},
							{base::utils::eUserCategory::LOGIN_CALIBRATE, IDS_LOGINPERMISSION_KALIBRIEREN},
							{base::utils::eUserCategory::LOGIN_SYSTEMSETTINGS,IDS_LOGINPERMISSION_SYSTEMEINSTELLUNGEN},
							{base::utils::eUserCategory::LOGIN_REFILL_LIMITS, IDS_LOGINPERMISSION_NACHFUELLGRENZENEINSTELLEN},
							{base::utils::eUserCategory::LOGIN_DOSE_START, IDS_LOGINPERMISSION_DOSIERUNGENSTARTEN},
							{base::utils::eUserCategory::LOGIN_DOSE_ACTIVATE,IDS_LOGINPERMISSION_DOSIERUNGENAKTIVIEREN},
							{base::utils::eUserCategory::LOGIN_LINE_ACTIVATE,IDS_LOGINPERMISSION_EXTRUDERAKTIVIEREN},
							{base::utils::eUserCategory::LOGIN_LINE_EDIT,IDS_LOGINPERMISSION_EXTRUDERBEARBEITEN},
							{base::utils::eUserCategory::LOGIN_TOTALIZER_RESET,IDS_LOGINPERMISSION_VERBRAUCHNULLEN},
							{base::utils::eUserCategory::LOGIN_TARIEREN,IDS_LOGINPERMISSION_TARIEREN},
							{base::utils::eUserCategory::LOGIN_SENSORADRESSE,IDS_LOGINPERMISSION_SENSORADRESSE},
							{base::utils::eUserCategory::LOGIN_SYSTEMFOLDER,IDS_LOGINPERMISSION_SYSTEMFOLDER},
							{base::utils::eUserCategory::LOGIN_TOUCHKALIBRIEREN,IDS_LOGINPERMISSION_TOUCHKALIBRIEREN},
							{base::utils::eUserCategory::LOGIN_SYSTEMZEIT,IDS_LOGINPERMISSION_SYSTEMZEIT},
							{base::utils::eUserCategory::LOGIN_ALARMSETTINGS,IDS_LOGINPERMISSION_ALARMEINSTELLUNGEN},
							{base::utils::eUserCategory::LOGIN_PROGRAMMBEENDEN,IDS_LOGINPERMISSION_PROGRAMMBEENDEN},
							{base::utils::eUserCategory::LOGIN_REMOTECONNECTION,IDS_LOGINPERMISSION_REMOTEVERBINDUNGEN},
							{base::utils::eUserCategory::LOGIN_HOSTINPUTOFFLINE,IDS_LOGINPERMISSION_HOSTINPUTOFFLINE},
							{base::utils::eUserCategory::LOGIN_ANLAGENMAXSETPOINT,IDS_LOGINPERMISSION_ANLAGENMAXSETPOINT},
							{base::utils::eUserCategory::LOGIN_DOSIERTYPENEINSTELLUNGEN,IDS_LOGINPERMISSION_DOSIERTYPENEINSTELLUNGEN},
							{base::utils::eUserCategory::LOGIN_CONTROLSETTINGS,IDS_LOGINPERMISSION_REGLERPARAMETEREINSTELLUNGEN},
							{base::utils::eUserCategory::LOGIN_DOSEPERFORMANCECHANGE,IDS_LOGINPERMISSION_DOSEPERFORMANCEAENDERN},
							{base::utils::eUserCategory::LOGIN_MAXDREHZAHLAENDERN,IDS_LOGINPERMISSION_MAXDREHZAHLAENDERN},
							{base::utils::eUserCategory::LOGIN_SERVICEMODE,IDS_LOGINPERMISSION_SERVICEMODE},
							{base::utils::eUserCategory::LOGIN_MEASURETIME,IDS_LOGINPERMISSION_MEASURETIME},
							{base::utils::eUserCategory::LOGIN_PADDLESPEED,IDS_LOGINPERMISSION_PADDLESPEED },
							{base::utils::eUserCategory::LOGIN_ALARMTHRESHOLDS, IDS_LOGINPERMISSION_ALARMTHRESHOLDS }  }
, m_szEditUser("")
	, m_bModified(FALSE)
{
	ASSERT(_S32(c_categorymaplist.size()) == _S32(base::utils::eUserCategory::LOGIN_MAX));
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETTINGS_USER_USERPROPERTY, aListCtrl);
	DDX_Text(pDX, IDC_SETTINGS_USER_USERLEVEL, m_szUserLevel);
}
//****************************************************************************************************
//****************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsUserLoginPage, CBCGPPropertyPage)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_SETTINGS_USER_USERPROPERTY, OnLvnItemActivateSettingsUserUserproperty)
    ON_CBN_SELCHANGE(MYID_CB_USERSELECT, OnCbnSelchangeSettingsUserUserlist)
	ON_BN_CLICKED(IDC_SETTINGS_USER_NEWUSER, OnBnClickedSettingsUserNewUser)
	ON_BN_CLICKED(IDC_SETTINGS_USER_DELETEUSER, OnBnClickedSettingsUserDeleteUser)
	ON_BN_CLICKED(IDC_SETTINGS_USER_PASSWORD_RESET, OnBnClickedSettingsUserPasswordReset)
    ON_WM_CREATE()
END_MESSAGE_MAP()
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::SetModified(BOOL b)
{
	CBCGPPropertyPage :: SetModified(b);
}
//****************************************************************************************************
//****************************************************************************************************
int32_t CSettingsUserLoginPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBCGPPropertyPage::OnCreate(lpCreateStruct) == -1)
        return -1;

    VERIFY(m_aFont.CreateFont(
   16,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_BOLD,                   // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   _T("MS Sans Serif")));         // lpszFacename

    // Create Imagelist
	m_aImageList.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR, 0, IMAGE_MAX); // 32, 32 for large icons
	HICON  aIcon = AfxGetApp()->LoadIcon(IDI_ICONUSER);
    ASSERT(aIcon);
	VERIFY(m_aImageList.Add(aIcon) >= 0);

    // Comoox
	m_aCBImageList.Create(ICON_CB_SIZEX, ICON_CB_SIZEY, ILC_COLOR8, IMAGE_CB_MAX, IMAGE_CB_MAX); // 32, 32 for large icons
	HICON  aIconUser = AfxGetApp()->LoadIcon(IDI_USERLOGIN);
    ASSERT(aIconUser);
	VERIFY(m_aCBImageList.Add(aIconUser) >= 0);
    m_aUserList.Create(CBS_DROPDOWNLIST, CRect(100,100,200,250), this, MYID_CB_USERSELECT);
    m_aUserList.SetFont(&m_aFont);
    return 0;
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: SetValue ()
{
	BOOL bMinoruser		= IsMinorClass();
	BOOL bCurrentuser   = GetSafeUser() == GetEditUser();
	BOOL bPasswordValid = FALSE;
	const auto pEditItem = GetEditItem();
	ASSERT(pEditItem);
	if (pEditItem)
	{
		bPasswordValid = pEditItem->IsPasswordValid();
	}
	ENABLE_SHOW_ID(IDC_SETTINGS_USER_PASSWORD_RESET, (bMinoruser | bCurrentuser) & bPasswordValid, bMinoruser | bCurrentuser);
	ENABLE_SHOW_ID(IDC_SETTINGS_USER_DELETEUSER, bMinoruser, bMinoruser);
	UpdateData(FALSE);
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: GetValue ()
{
	UpdateData(TRUE);
}
//****************************************************************************************************
//****************************************************************************************************
inline base::utils::CUserRights* CSettingsUserLoginPage::GetSafeItem()
{
	auto pSaveItem = m_aLoginList.GetAktItem();
	ASSERT(pSaveItem);
	return pSaveItem;
}
//****************************************************************************************************
//****************************************************************************************************
inline const base::utils::CUserRights* CSettingsUserLoginPage::GetSafeItem() const
{
	auto pSaveItem = m_aLoginList.GetAktItem();
	ASSERT(pSaveItem);
	return pSaveItem;
}
//****************************************************************************************************
//****************************************************************************************************
inline base::utils::CUserRights* CSettingsUserLoginPage::GetEditItem()
{
	auto pEditItem = m_aLoginList.FindLoginItem(m_szEditUser);
	ASSERT(pEditItem);
	return pEditItem;
}
//****************************************************************************************************
//****************************************************************************************************
inline const base::utils::CUserRights* CSettingsUserLoginPage::GetEditItem() const
{
	auto pEditItem = m_aLoginList.FindLoginItem(m_szEditUser);
	ASSERT(pEditItem);
	return pEditItem;
}
//****************************************************************************************************
//****************************************************************************************************
inline const std::string& CSettingsUserLoginPage::GetSafeUser() const
{
	return m_aLoginList.GetAktUser();
}
//****************************************************************************************************
//****************************************************************************************************
inline const std::string& CSettingsUserLoginPage::GetEditUser() const
{	return m_szEditUser; }
//****************************************************************************************************
//****************************************************************************************************
inline BOOL CSettingsUserLoginPage::IsMinorClass() const
{
	BOOL bErg = (GetEditUser() != GetSafeUser());
	if (bErg)
	{
		const auto pSaveItem = GetSafeItem();
		ASSERT(pSaveItem);
		const auto pEditItem = GetEditItem();
		ASSERT(pEditItem);
		if (pSaveItem && pEditItem)
		{
			bErg = (_S32(pSaveItem->GetUserClass()) < _S32(pEditItem->GetUserClass()));
		}
	}
	return bErg;
}
//****************************************************************************************************
//****************************************************************************************************
BOOL	CSettingsUserLoginPage :: GetShow () const
{
	BOOL bErg = IsMinorClass();
    return bErg;
}
//****************************************************************************************************
//****************************************************************************************************
BOOL	CSettingsUserLoginPage::CanDelete() const
{
	BOOL bErg = IsMinorClass();
	return bErg;
}
//****************************************************************************************************
//****************************************************************************************************
BOOL	CSettingsUserLoginPage :: GetEnable () const
{
	BOOL bErg = IsMinorClass();
	return bErg;
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: MarkField (base::utils::eUserCategory category, BOOL b)
{
	int32_t i = _S32(category);
	aListCtrl.SetItemText(i, 2, (b) ? SZMARKED : SZEMPTY);
	aListCtrl.SetItemText(i, 3, (b) ? SZEMPTY  : SZMARKED);
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: BuildUser (void)
{
	BuildPermissionList();
	//BuildPassword();
	//BuildPasswordValid();

	const auto pEditItem = GetEditItem();
	if (pEditItem)
	{
		if (pEditItem->IsSupervisor() )
		{
			m_szUserLevel = toCString(m_UserClassMap.get(base::utils::eUserClass::eCLASSSUPERVISOR));
		} 
		else if (pEditItem->IsAdministrator())
		{
			m_szUserLevel = toCString(m_UserClassMap.get(base::utils::eUserClass::eCLASSADMINISTRATOR));
		}
		else if (pEditItem->IsStandardUser())
		{
			m_szUserLevel = toCString(m_UserClassMap.get(base::utils::eUserClass::eCLASSSTANDARDUSER));
		} 
		else
		{
			ASSERT(pEditItem->IsUser());
			m_szUserLevel = toCString(m_UserClassMap.get(base::utils::eUserClass::eCLASSUSER));
		} 
	}
    else
    {
        m_szUserLevel = "";
    }
	SetValue();
}
#if 0
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: BuildPasswordValid (void)
{
	BOOL bEnable =  GetEnable();
    if ( ! bEnable )
    {
        bEnable  = BOOL(GetSafeUser() == GetEditUser());
    }
	if ( ! bEnable )
	{
		m_bPasswordValid = FALSE;
	}
	else
	{
		const auto pEditItem = GetEditItem();
		ASSERT(pEditItem);
		if (pEditItem)
		{
			m_bPasswordValid = pEditItem->IsPasswordValid();
		}
	}
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: BuildPassword (void)
{
	BOOL bShow        =  GetShow();
	if ( ! bShow )
	{
		m_szPassword = "*********";
	}
	else
	{
		const auto pEditItem = GetEditItem();
		ASSERT(pEditItem);
		if (pEditItem)
		{
			m_szPassword = toCString(pEditItem->GetPassword());
		}
	}
    m_aPasswordCtrl.ShowWindow(bShow);
}
#endif
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: BuildPermissionList (void)
{
	BOOL bEnable      =  GetEnable();
	BOOL bShow        =  GetShow();
	if ( bShow )
	{
		const auto pEditItem = GetEditItem();
		ASSERT(pEditItem);
		if (pEditItem)
		{
			for (const auto& item : c_categorymaplist)
			{
				MarkField(item.first, pEditItem->IsPermitted(item.first));
			}
		}
	}
    aListCtrl.SetTextColor((bEnable) ? YESCOLOR : NOCOLOR);
    aListCtrl.RedrawItems(0, aListCtrl.GetItemCount()-1);
	aListCtrl.ShowWindow(bShow);
	aListCtrl.EnableWindow(bShow);
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::	BuildUserNameListBox ()
{
	const auto pEditItem = GetEditItem();
	ASSERT(pEditItem);
	if (pEditItem)
	{
		COMBOBOXEXITEM cbi;
		int32_t iMerke = -1;

		m_aUserList.ResetContent();
		for (int32_t pos = 0; pos < _S32(m_aLoginList.GetCount()); pos++)
		{
			auto& rLoginItem = m_aLoginList.GetItem(pos);

			const auto& szUserName = rLoginItem.GetUsername();

			cbi.mask = CBEIF_IMAGE | CBEIF_TEXT | CBEIF_SELECTEDIMAGE | CBEIF_LPARAM;
			cbi.iImage = 0;
			cbi.iItem = pos;
			auto szUser = toCString(szUserName);
			cbi.pszText = LPTSTR(LPCTSTR(szUser));
			cbi.cchTextMax = _S32(szUser.GetLength());
			cbi.iSelectedImage = 0;
			cbi.lParam = LPARAM(&rLoginItem);
			m_aUserList.InsertItem(&cbi);
			if (rLoginItem.GetID() == pEditItem->GetID())
			{
				iMerke = pos;
			}
		}
		m_aUserList.SetCurSel(iMerke);
	}
}
//****************************************************************************************************
//****************************************************************************************************
void	CSettingsUserLoginPage :: InitColumns ()
{
	int32_t i = 0;
	for (const auto& item : c_categorymaplist)
	{
		CString szFormat;
		szFormat.Format(_T("%d"), i+1);
		CString szLoginPermission;
		VERIFY(szLoginPermission.LoadString(item.second));
		LVITEM lvi;
		lvi.mask		= LVIF_TEXT | LVIF_STATE;
		lvi.iImage		= -1;
		lvi.iItem		= i;
		lvi.iSubItem	= 0;
		lvi.pszText    = LPTSTR(LPCTSTR(szFormat));
		lvi.cchTextMax = szFormat.GetLength();;
		lvi.stateMask	= LVIS_STATEIMAGEMASK;
		lvi.state		= INDEXTOSTATEIMAGEMASK(1);
		aListCtrl.InsertItem(&lvi);
		aListCtrl.SetItemText(i, 1, szLoginPermission); 
		i++;
	}
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::Init ()
{
	LV_COLUMN lvc;
	CString szTemp;

	aListCtrl.SetExtendedStyle (aListCtrl.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	aListCtrl.SetImageList(&m_aImageList, LVSIL_SMALL );

	lvc.mask		= LVCF_WIDTH | LVCF_FMT | LVCF_SUBITEM;
	lvc.iSubItem	= 0;
	lvc.cx			= 40;
	lvc.fmt			= LVCFMT_LEFT;
	aListCtrl.InsertColumn(0,&lvc);

	VERIFY(szTemp.LoadString(IDS_LOGINFUNCTION));
	lvc.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx			= 240;
	lvc.iSubItem	= 1;
	lvc.fmt			= LVCFMT_LEFT;
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax  = szTemp.GetLength();
	aListCtrl.InsertColumn(1,&lvc);

	VERIFY(szTemp.LoadString(IDS_LOGINYES));
	lvc.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx			= 40;
	lvc.iSubItem	= 2;
	lvc.fmt		= LVCFMT_CENTER;
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax  = szTemp.GetLength();
	aListCtrl.InsertColumn(2,&lvc);

	VERIFY(szTemp.LoadString(IDS_LOGINNO));
	lvc.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.cx			= 40;
	lvc.iSubItem	= 3;
	lvc.fmt		= LVCFMT_CENTER;
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax  = szTemp.GetLength();
	aListCtrl.InsertColumn(3,&lvc);

	InitColumns ();
	BuildUserNameListBox ();
	BuildUser ();
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CSettingsUserLoginPage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		CWnd* pWnd = GetDlgItem(IDC_SETTINGS_USER_USERLIST);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(&aRect);
		ScreenToClient(aRect);
		m_aUserList.MoveWindow(aRect);
		m_aUserList.SetImageList(&m_aCBImageList);;


		m_aLoginList = USERRIGHTSREF;
		m_szEditUser = m_aLoginList.GetAktUser();
		Init();
		m_aUserList.ShowWindow(TRUE);
	}
	return bResult;
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::Accept()
{
	if (m_bModified)
	{
		USERRIGHTSREF = m_aLoginList;
		BOOL bErg = USERRIGHTSREF.Save(EASYCONTROLREF.m_UserConfigFile);
		if (!bErg)
		{
			ECMessageBox(IDS_ERROR_USERLISTSAVE, MB_ICONSTOP | MB_OK);
		}
		m_bModified = FALSE;
	}
	CBCGPPropertyPage::OnOK();
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnOK()
{
	Accept();
	CBCGPPropertyPage::OnOK();
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CSettingsUserLoginPage::OnApply()
{
	Accept();
	return CBCGPPropertyPage::OnApply();
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnLvnItemActivateSettingsUserUserproperty(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    BOOL bEnable = GetEnable();
    if ( ! bEnable )
    {
        return;
    }
 
	const base::utils::eUserCategory iItem	= static_cast<base::utils::eUserCategory>(pNMIA->iItem);
	const int32_t iSubItem				= pNMIA->iSubItem;
	auto pEditItem = GetEditItem();
	ASSERT(pEditItem);
	BOOL bMarkField = FALSE;
	const BOOL  bCurrentState = pEditItem->IsPermitted(iItem);
	if ( iSubItem == 2) // Ja
	{
		if ( ! bCurrentState)
		{
			const auto pSafeItem = GetSafeItem();
			ASSERT(pSafeItem);
			bMarkField = pSafeItem->IsPermitted(iItem);
		}
	}
	else if ( iSubItem == 3) // Nein
	{
		bMarkField = bCurrentState;
	}
	else
	{}

	if ( bMarkField )
	{
		pEditItem->SetPermitted(iItem, !bCurrentState);
		MarkField(iItem, !bCurrentState);
		SetModified(m_bModified = TRUE);
	}
	*pResult = 0;
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnCbnSelchangeSettingsUserUserlist(void)
{
	int32_t nIndex = m_aUserList.GetCurSel();
	if (nIndex != CB_ERR )
	{
		const auto pEditItem = reinterpret_cast<base::utils::CUserRights*>(m_aUserList.GetItemDataPtr(nIndex));
		ASSERT(pEditItem != NULL);
		m_szEditUser = pEditItem->GetUsername();
		BuildUser ();
	}
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnBnClickedSettingsUserNewUser()
{
	const auto pSafeItem = GetSafeItem();
	CUserNameDlg aDlg(this, pSafeItem->GetUserClass());

	if ( aDlg.DoModal() == IDOK)
	{
		ASSERT(! aDlg.m_szUserName.IsEmpty());
		const auto username = toStdString(aDlg.m_szUserName);
		if (m_aLoginList.IsLoginValid(username) )
		{
			ECMessageBox(IDS_LOGINEXISTINGUSER, MB_ICONSTOP | MB_OK);
		}
		else
		{
			m_szEditUser = username;
			base::utils::CUserRights aItem(-1, aDlg.m_NewUserClass, username);
			aItem.SetDefaultFlags();
			m_aLoginList.AddItem(aItem);
			BuildUserNameListBox();
			BuildUser ();
			SetModified(m_bModified = TRUE);
		}
	}
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnBnClickedSettingsUserDeleteUser()
{
	CString szTemp, szTemp1;

	VERIFY(szTemp1.LoadString(IDS_USERREALLYDELETE));
	szTemp.Format(_T("%s %s"), LPCTSTR(toCString(m_szEditUser)), LPCTSTR(szTemp1));
	if ( ECMessageBox(szTemp, MB_YESNO | MB_ICONQUESTION ) == IDYES)
	{
		const auto item = GetEditItem();
		ASSERT(item);
		if (item)
		{
			VERIFY(m_aLoginList.DeleteItem(item->GetID()));
			m_szEditUser = GetSafeUser();
			BuildUserNameListBox();
			BuildUser();
			SetModified(m_bModified = TRUE);
		}
	}
}
//****************************************************************************************************
//****************************************************************************************************
void CSettingsUserLoginPage::OnBnClickedSettingsUserPasswordReset()
{
	auto pEdititem = GetEditItem();
	if (pEdititem)
	{
		pEdititem->SetPassword("");
		SetValue();
		SetModified(m_bModified = TRUE);
	}
}


