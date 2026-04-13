//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MainFrm.cpp
///
/// @file   MainFrm.cpp
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
#include "EasyControlVer.h"
#include "ECMessageBox.h"
#include "MainFrm.h"
#include "RemoteControl.h"
#include "AlarmArchiveView.h"
#include "AlarmArchiveDoc.h"
#include "AlarmArchiveFrame.h"
#include "DiagramView.h"
#include "DiagramDoc.h"
#include "DiagramFrame.h"
#include "DosePropertySheet.h"
#include "SettingsPropertySheet.h"
#include "AlarmLog.h"
#include "PasswordDlg.h"
#include "NewPasswordDlg.h"
#include "SingleUserDlg.h"
#include "DataImage.h"
#include "TKBoardInterface.h"


#define TLICENCEID 37
#define TLCID	    1


enum
{
	eParamNone	    = 0,
	eParamChanged , 
	eParamSaveError
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_TB_MAINFRAME_LOGIN, OnUserLogin)
	ON_UPDATE_COMMAND_UI(ID_TB_MAINFRAME_LOGIN, OnOnUpdateUserLogin)
	ON_COMMAND(ID_TB_MAINFRAME_CLOSE, OnApplicationClose)
	ON_UPDATE_COMMAND_UI(ID_TB_MAINFRAME_CLOSE, OnUpdateApplicationClose)
	ON_COMMAND(ID_TB_MAINFRAME_LOGOUT, OnUserLogout)
	ON_UPDATE_COMMAND_UI(ID_TB_MAINFRAME_LOGOUT, OnUpdateUserLogout)
	ON_COMMAND(ID_TB_MAINFRAME_SETTINGS, OnSystemSettings)
	ON_UPDATE_COMMAND_UI(ID_TB_MAINFRAME_SETTINGS, OnUpdateSystemSettings)
	ON_COMMAND(ID_TB_MAINFRAME_KEYBOARD, OnKeyboard)
	ON_COMMAND(ID_SB_INDICATOR_ICONCONNECT, OnStatusBarIPConnect)
	ON_COMMAND(ID_SB_INDICATOR_RECORDER, OnStatusBarRecorder)
	ON_MESSAGE(WM_LICENCECHECK, OnLicenceCheck)
	ON_MESSAGE(WM_DOSEEND, OnDoseEnd)
	ON_MESSAGE(WM_TIMER_REFRESH, OnTimerRefresh)
	ON_MESSAGE(WM_LAYOUTCHANGED, OnLayoutChanged)
	ON_MESSAGE(WM_ACTIVATE_VIEW, OnActivateViewSelect) // Callback from active view
	ON_MESSAGE(WM_CONNECTIONLOST, OnConnectionLost)
	ON_MESSAGE(WM_RECORDERERROR, OnRecorderError)
	ON_MESSAGE(WM_INFOBOX_ALARMCLEAR, OnInfoBoxAlarmClear)
	ON_MESSAGE(WM_ZOOMFACTOR, OnZoomFactor)
	ON_MESSAGE(WM_PREPARENEWLAYOUT, OnPrepareNewLayout)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOVE()
END_MESSAGE_MAP()
//****************************************************************************************************************
//****************************************************************************************************************
CMainFrame::CMainFrame() :  m_bRecipeNew(FALSE)
                , m_bAllowUserProgramEnd(FALSE)
                , m_aTimer(NULL)
                , m_aLicenceTimer(NULL)
                , m_aPesaCount(0)
				, m_bViewInit(FALSE)
				, m_bLastRecorderActive(FALSE)
				, m_ActiveViewSelect{ eViewSelect::eViewMax }
				, m_StaticRect{ 0,0,0,0 }
{}
//****************************************************************************************************************
//****************************************************************************************************************
//------------------------------------------------------------------------------------
///  @brief   OnLicenceCheck
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CMainFrame
///
///  @param[in] w , WPARAM
///  @return LRESULT
///
//------------------------------------------------------------------------------------
LRESULT CMainFrame::OnLicenceCheck(WPARAM , LPARAM)
{
	if ( m_aLicenceTimer )
	{
		return 0L;
	}
	long lTime = 0L;
	srand( (unsigned)time( NULL ) );
	lTime = rand() % 200;
	lTime = (lTime + 100) *23;
	m_aLicenceTimer = SetTimer(TLICENCEID, lTime, NULL);
	return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnStatusBarIPConnect()
{
	auto permitted = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_HOSTINPUTOFFLINE);
	if (permitted)
	{
		auto lModBusResult = REFREMOTESERVER.getRemoteState();
		if (lModBusResult == base::RemoteServerStatus::REMOTE_STATUS_INPUTOFFLINE)
		{
			REFREMOTESERVER.SetInputOffline(FALSE);
		}
		else if (lModBusResult == base::RemoteServerStatus::REMOTE_STATUS_CONNECTED)
		{
			if (ECMessageBox(IDS_QUERY_INPUTOFFLINE, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				REFREMOTESERVER.SetInputOffline(TRUE);
			}
		}
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnStatusBarRecorder()
{
	auto permitted = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SYSTEMSETTINGS);
	if (permitted)
	{
		if (RECORDERCONFIGREF.m_RecorderActive)
		{
			if (ECMessageBox(IDS_RECORDER_STOP, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				RECORDERCONFIGREF.m_RecorderActive = FALSE;
			}
		}
		else
		{
			if (ECMessageBox(IDS_RECORDER_START, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				RECORDERCONFIGREF.m_RecorderActive = TRUE;
			}
		}
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
LRESULT CMainFrame::OnConnectionLost(WPARAM, LPARAM)
{
	static BOOL bShow = FALSE;

	if (!bShow)
	{
		bShow = TRUE;
		ECMessageBox(IDS_ERROR_CONNECTIONLOST, MB_ICONSTOP | MB_OK);
		bShow = FALSE;
	}
	return 0L;
}
//****************************************************************************************************************
//****************************************************************************************************************
LRESULT CMainFrame::OnRecorderError(WPARAM, LPARAM)
{
	static BOOL bShow = FALSE;

	if (!bShow)
	{
		bShow = TRUE;
		ECMessageBox(IDS_ERROR_RECORDERERROR, MB_ICONSTOP | MB_OK);
		bShow = FALSE;
	}
	return 0L;
}
//****************************************************************************************************************
//****************************************************************************************************************
CMultiDocTemplate* CMainFrame::GetDocTemplate(eViewSelect _view)
{
	CEasyControlApp* pAdosys = (CEasyControlApp*)AfxGetApp();
	CMultiDocTemplate* pTemplate = nullptr;
	switch (_view)
	{
	default:
	case eViewSelect::eDefaultView:
		pTemplate = pAdosys->pDocAnlagenTemplate;
		break;
	case eViewSelect::eRecipeView:
		pTemplate = pAdosys->pRezeptTemplate;
		break;
	case eViewSelect::eAlarmArchiveView:
		pTemplate = pAdosys->pLogTemplate;
		break;
	case eViewSelect::eDiagramView:
		pTemplate = pAdosys->pDiagramTemplate;
		break;
	}
	return pTemplate;
}
//****************************************************************************************************************
//****************************************************************************************************************
CMDIChildWndEx* CMainFrame::GetParentFrame(CMultiDocTemplate* pTemplate)
{
	if (pTemplate == NULL)
		return nullptr;

	POSITION aPos = pTemplate->GetFirstDocPosition();
	if (aPos == NULL)
		return nullptr;
	aPos = pTemplate->GetFirstDocPosition();
	if (aPos == NULL)
		return nullptr;
	CDocument* aDoc = pTemplate->GetNextDoc(aPos);
	if (aDoc == NULL)
		return nullptr;
	aPos = aDoc->GetFirstViewPosition();
	if (aPos == NULL)
		return nullptr;
	CView* aView = aDoc->GetNextView(aPos);
	if (aView == NULL)
		return nullptr;
	return (CMDIChildWndEx*)aView->GetParentFrame();
}

//****************************************************************************************************************
//****************************************************************************************************************
void CMainFrame::SendToView(CMultiDocTemplate* pTemplate, int32_t id)
{
	POSITION aPos = pTemplate->GetFirstDocPosition();
	if (aPos != NULL)
	{
		CDocument* aDoc = pTemplate->GetNextDoc(aPos);
		ASSERT(aDoc);
		POSITION aViewPos = aDoc->GetFirstViewPosition();
		ASSERT(aViewPos != NULL);
		CView* aView = aDoc->GetNextView(aViewPos);
		ASSERT(aView != NULL);
		aView->SendMessage(id);
	}
}

//****************************************************************************************************************
//****************************************************************************************************************
void CMainFrame::SendToView(eViewSelect _view, int32_t id)
{
	auto pTemplate = GetDocTemplate(_view);
	if (pTemplate)
	{
		SendToView(pTemplate, id);
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
void CMainFrame::ActivateViewSelect(eViewSelect _view)
{
	auto pWnd = GetParentFrame(_view);
	if (pWnd)
	{
		pWnd->MDIActivate();
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
CMDIChildWndEx* CMainFrame::GetParentFrame(eViewSelect _view)
{
	auto pTemplate = GetDocTemplate(_view);
	return GetParentFrame(pTemplate);
}
//****************************************************************************************************************
//****************************************************************************************************************
void CMainFrame::SendMessageToActiveView(UINT message, WPARAM w, LPARAM l)
{
	auto pWnd = GetParentFrame(m_ActiveViewSelect);
	if (pWnd)
	{
		auto aView = pWnd->GetActiveView();
		aView->SendMessage(message, w, l);
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
void CMainFrame::PostMessageToActiveView(UINT message, WPARAM w, LPARAM l)
{
	auto pWnd = GetParentFrame(m_ActiveViewSelect);
	if (pWnd)
	{
		auto aView = pWnd->GetActiveView();
		aView->PostMessage(message, w, l);
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
LRESULT CMainFrame::OnProcessLineEnd(WPARAM , LPARAM)
{
	return 0L;
}
//****************************************************************************************************************
//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnLayoutChanged(WPARAM , LPARAM )
{
	BeginWaitCursor();
	SendToView(eViewSelect::eDefaultView, WM_LAYOUTCHANGED);
	SendToView(eViewSelect::eRecipeView, WM_LAYOUTCHANGED);

	// Update Menu
	// Customize toolbarbutton with Menubutton
	CString szDose;
	VERIFY(szDose.LoadString(IDS_DOSIERUNG));

	CMenu menuBackward;
	menuBackward.CreatePopupMenu();
	MENUITEMINFO mii = { 0 };
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_STRING;
	mii.wID = ID_MN_DOSEITEM00;
	CString szTemp;
	szTemp.Format(_T("%s:all"), LPCTSTR(szDose));
	mii.dwTypeData = LPTSTR(LPCTSTR(szTemp));
	menuBackward.InsertMenuItem(0, &mii, FALSE);

	uint32_t count;
	REMOTEREF.getMaxDoseCount(count);
	for (uint32_t ix = 0; ix < count; ix++)
	{
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_ID | MIIM_STRING;
		mii.wID = ID_MN_DOSEITEM00 + (ix+1);
		szTemp.Format(_T("%s:%d"), LPCTSTR(szDose), (ix + 1));
		mii.dwTypeData = LPTSTR(LPCTSTR(szTemp));
		menuBackward.InsertMenuItem(0, &mii, FALSE);
	}
	auto hdl = menuBackward.GetSafeHmenu();
	m_wndToolBar[_S32(eViewSelect::eDiagramView)].ReplaceButton(ID_TB_DIAGRAMBAR_DOSEITEM,
		CMFCToolBarMenuButton(ID_TB_DIAGRAMBAR_DOSEITEM, hdl,
			GetCmdMgr()->GetCmdImage(ID_TB_DIAGRAMBAR_DOSEITEM), NULL));

	m_wndStatusBar.SetMaxItems(count);

	EndWaitCursor();
	return 0L;
}

//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnZoomFactor(WPARAM w, LPARAM l)
{
	SendMessageToActiveView(WM_ZOOMFACTOR, w, l);
	return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnPrepareNewLayout(WPARAM , LPARAM )
{
	auto& aV = dataimage::getLineDataImage();
	aV.m_Cache.m_count = 0;
	return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::SetUser(void)
{
	CString szUser{ toCString(USERRIGHTSREF.GetAktUser()) };

    m_bRecipeNew = FALSE;
    m_bRecipeNew |= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_EDIT);
    m_bRecipeNew |= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_OPEN);
    m_bRecipeNew |= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_SAVE);
    m_bRecipeNew |= USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_RECIPE_DELETE);
    m_bAllowUserProgramEnd   = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_PROGRAMMBEENDEN);

	if (USERRIGHTSREF.IsAktSupervisor())
	{
		ModifyStyle(0, WS_OVERLAPPEDWINDOW); //  WS_SYSMENU | WS_MAXIMIZEBOX);
	
	}
	else
	{
		ModifyStyle(WS_OVERLAPPEDWINDOW & (~WS_CAPTION), 0);
		ShowWindow(SW_SHOWMAXIMIZED);
	}
    m_wndStatusBar.SetUser(szUser);
	 
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CMainFrame::IsLocalDevice(LPCSTR szFile)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

   _splitpath_s( (LPCSTR)szFile, drive, dir, fname, ext );
   CString szDrive(drive);
   szDrive.Trim(' ');
   szDrive = szDrive.Left(1);
   szDrive.MakeUpper();
   return BOOL(szDrive == "C");
}
//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnActivateViewSelect(WPARAM w, LPARAM )
{
	if (m_bViewInit)
	{
		auto iViewSelect = static_cast<eViewSelect>(w);
		if (iViewSelect != m_ActiveViewSelect)
		{
			SendToView(m_ActiveViewSelect, WM_DEACTIVATE_VIEW);
			m_ActiveViewSelect = iViewSelect;
			for (int32_t ix = 0; ix < _S32(eViewSelect::eViewMax); ix++)
			{
				ShowPane(&m_wndToolBar[ix], _S32(iViewSelect) == ix, FALSE, _S32(iViewSelect) == ix);
			}
			AdjustDockingLayout();
			RecalcLayout();
			SendToView(m_ActiveViewSelect, WM_ACTIVATE_VIEW);
		}
	}
	return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnInfoBoxAlarmClear(WPARAM w, LPARAM)
{
	SendMessageToActiveView(WM_INFOBOX_ALARMCLEAR ,w);
	return 0L;
}

//**************************************************************************************************
//**************************************************************************************************
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bLastRecorderActive = FALSE;
	m_Recorder.SetTempFolder(theApp.GetTempRecorderFolder());

	m_StaticRect.SetRectEmpty();

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // Weitere Stile verfügbar...
	mdiTabParams.m_bActiveTabCloseButton = FALSE;      // Auf "FALSE" festlegen, um die Schaltfläche "Schließen" rechts auf der Registerkarte zu platzieren
	mdiTabParams.m_bTabCloseButton = FALSE;
	mdiTabParams.m_bTabIcons = FALSE;    // Auf "TRUE" festlegen, um Dokumentsymbole auf MDI-Registerkarten zu aktivieren
	mdiTabParams.m_bAutoColor = TRUE;    // Auf "FALSE" festlegen, um automatische Farben für MDI-Registerkarten zu deaktivieren
	mdiTabParams.m_bDocumentMenu = FALSE; // Dokumentmenü am rechten Rand des Registerkartenbereichs aktivieren
	mdiTabParams.m_tabLocation = CMFCTabCtrl::Location::LOCATION_BOTTOM;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Fehler beim Erstellen der Menüleiste.\n");
		return -1;      // Fehler beim Erstellen
	}
	m_wndMenuBar.ShowPane(FALSE, 0, 0);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	EnableDocking(CBRS_ALIGN_ANY);

	if (!m_wndMainBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS) ||
		!m_wndMainBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung  CBRS_FLYBY
	}

	const uint32_t cIDR[cViewSelectMax]		= { IDR_OVERVIEWFRAME, IDR_RECIPEFRAME, IDR_ALARMARCHIVEFRAME, IDR_DIAGRAMFRAME };
	for (uint32_t index = 0; index < cViewSelectMax; index++)
	{
		auto& rToolbar = m_wndToolBar[index];
		if (!rToolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_TOOLTIPS) ||
			!rToolbar.LoadToolBar(cIDR[index]))
		{
			TRACE0("Symbolleiste konnte nicht erstellt werden\n");
			return -1;      // Fehler bei Erstellung  CBRS_FLYBY
		}
		rToolbar.EnableDocking(CBRS_ALIGN_ANY);
	}

	//----------------------------------------------------------------------------------------
	// Change Toolbar to 256 colors
	CMFCToolBar::ResetAllImages();
	const uint32_t cBitmap[cViewSelectMax] = { IDB_OVERVIEW256, IDB_RECIPEFRAME256, IDB_ALARMARCHIVEFRAME256, IDB_DIAGRAMFRAME256 };

	VERIFY(m_wndMainBar.LoadToolBar(IDR_MAINFRAME));
	VERIFY(m_wndMainBar.LoadBitmap(IDB_MAINFRAME256));

	for (uint32_t index = 0; index < cViewSelectMax; index++)
	{
		auto& rToolbar = m_wndToolBar[index];
		VERIFY(rToolbar.LoadToolBar(cIDR[index]));
		VERIFY(rToolbar.LoadBitmap(cBitmap[index]));
	}
	//----------------------------------------------------------------------------------------
	// Dock all toolbars
	int32_t activeViewSelect = cViewSelectMax - 1;
	DockPane(&m_wndToolBar[activeViewSelect]);
	while (activeViewSelect)
	{
		DockPaneLeftOf(&m_wndToolBar[activeViewSelect - 1], &m_wndToolBar[activeViewSelect]);
		activeViewSelect--;
	}
	DockPaneLeftOf(&m_wndMainBar, &m_wndToolBar[activeViewSelect]);
	m_ActiveViewSelect = eViewSelect::eDefaultView;

	//----------------------------------------------------------------------------------------
	// Customize toolbarbutton with Menubutton
	{
		CMenu menuBackward;
		menuBackward.LoadMenu(IDR_DIAGRAMSELECT);
		m_wndToolBar[_S32(eViewSelect::eDiagramView)].ReplaceButton(ID_TB_DIAGRAMBAR_DIAGRAMSELECT,
			CMFCToolBarMenuButton(ID_TB_DIAGRAMBAR_DIAGRAMSELECT, menuBackward.GetSubMenu(0)->GetSafeHmenu(),
				GetCmdMgr()->GetCmdImage(ID_TB_DIAGRAMBAR_DIAGRAMSELECT), NULL));
	}

	//----------------------------------------------------------------------------------------
	// Customize Sekunden/Datum für Diagrammauswahl
	{
		CMenu menuBackward;
		menuBackward.LoadMenu(IDR_TIMESELECT);
		m_wndToolBar[_S32(eViewSelect::eDiagramView)].ReplaceButton(ID_TB_DIAGRAMBAR_TIMESELECT,
			CMFCToolBarMenuButton(ID_TB_DIAGRAMBAR_TIMESELECT, menuBackward.GetSubMenu(0)->GetSafeHmenu(),
				GetCmdMgr()->GetCmdImage(ID_TB_DIAGRAMBAR_TIMESELECT), NULL));
	}

	VERIFY(m_wndStatusBar.Create(this) == 0);
	m_wndStatusBar.SetMaxItems(0);

	{
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_IDLEMESSAGEEASYCONTROL));
		CString szTemp1;
		szTemp1.Format(_T("%s  %s"), LPCTSTR(szTemp), LPCTSTR(RELEASENR));
		m_wndStatusBar.SetCompany(szTemp1);
	}
	m_wndStatusBar.SetRemoteIP(theApp.GetRemoteIP());

	// Andockfensterverhalten wie in Visual Studio 2005 aktivieren
	CDockingManager::SetDockingMode(DT_SMART);
	// Automatisches Ausblenden von Andockfenstern wie in Visual Studio 2005 aktivieren
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

    SetUser();

	//FetchData();

	ASSERT(m_aTimer == NULL);
	if ( m_aTimer == NULL)
	{
		m_aTimer = SetTimer(TLCID, 1000, NULL);
	}
	return 0;
}

//**************************************************************************************************
//**************************************************************************************************
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.x = cs.y = 0;

	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	//cs.style &= ~WS_CAPTION;
	 return TRUE;
}


// CMainFrame Diagnose

#ifdef _DEBUG
//------------------------------------------------------------------------------------
///  @brief   AssertValid
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CMainFrame
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

//------------------------------------------------------------------------------------
///  @brief   Dump
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CMainFrame
///
///  @param[in] dc , CDumpContext&
///  @return none
///
//------------------------------------------------------------------------------------
void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG


//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnDestroy()
{
	if ( m_aTimer != NULL)
	{
		KillTimer(m_aTimer);
		m_aTimer = NULL;
	}
	if ( m_aLicenceTimer != NULL)
	{
		KillTimer(m_aLicenceTimer);
		m_aLicenceTimer = NULL;
	}
	//CMDIFrameWndEx::OnDestroy();
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CMainFrame::LoginUserName()
{
	CSingleUserDlg aDlg(this);
	aDlg.DoModal();

	CString szUser{ aDlg.m_szUserName };
	BOOL bFound = USERRIGHTSREF.IsLoginValid(toStdString(szUser));
	if (!bFound)
	{
		ECMessageBox(IDS_LOGINUNKNOWNUSER, MB_ICONSTOP | MB_OK);
		return FALSE;
	}
	USERRIGHTSREF.SetAktUser(toStdString(szUser));
	return TRUE;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CMainFrame::LoginNewPassword()
{
	BOOL result = FALSE;

	while ( ! result)
	{
		CNewPasswordDlg aDlg(this);
		if (aDlg.DoModal() != IDOK)
		{
			break;
		}
		(void) USERRIGHTSREF.SetAktPassword(toStdString(aDlg.m_szPassword));
		if (USERRIGHTSREF.IsAktPasswordValid())
		{
			auto bErg = USERRIGHTSREF.Save(EASYCONTROLREF.m_UserConfigFile);
			if (!bErg)
			{
				ECMessageBox(IDS_ERROR_USERLISTSAVE, MB_ICONSTOP | MB_OK);
			}
			result = TRUE;
		}
		else
		{
			USERRIGHTSREF.SetAktPassword("");
			if (ECMessageBox(IDS_LOGININVALIDPASSWORD, MB_ICONSTOP | MB_YESNO) == IDNO)
			{
				break;
			}
		}
	}
	return result;
}
//**************************************************************************************************
//**************************************************************************************************
BOOL CMainFrame::LoginPassword()
{
	BOOL bFound = FALSE;

	while (!bFound)
	{
		CPasswordDlg aDlg(this);
		aDlg.DoModal();
		CString password{ aDlg.m_szPassword };

		if (password.IsEmpty())
		{
			if (ECMessageBox(IDS_LOGININVALIDPASSWORD, MB_ICONSTOP | MB_YESNO) == IDNO)
			{
				break;
			}
		}
		else
		{
			bFound = USERRIGHTSREF.CheckAktPassword(toStdString(password));
			if ( ! bFound)
			{
				bFound = USERRIGHTSREF.IsDailyPassword(toStdString(password));
				if (!bFound)
				{
					if (ECMessageBox(IDS_LOGININVALIDPASSWORD, MB_ICONSTOP | MB_YESNO) == IDNO)
					{
						break;
					}
				}
			}
		}
	}
	return bFound;
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnApplicationClose()
{
	OnDoseEnd(0,0);
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnUpdateApplicationClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAllowUserProgramEnd);
}

//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnUserLogin()
{
	TKB.TK_Show();

	BOOL password = FALSE;
	auto username = LoginUserName();
	if (username)
	{
		BOOL result = USERRIGHTSREF.IsAktPasswordValid();
		if (!result)
		{
			password = LoginNewPassword();
			if (!password)
			{
				username = FALSE;
			}
		}
	}
	if (username)
	{
		if (! password)
		{
			password = LoginPassword();
			if (! password)
			{
				username = FALSE;
			}
		}
	}
	TKB.TK_Hide();

	if ( ! (username && password))
	{
		USERRIGHTSREF.SetAktUser("");
	}
	else
	{
		SetUser();
	}
	SendToView(eViewSelect::eRecipeView, WM_LOGON);
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnOnUpdateUserLogin(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( ! USERRIGHTSREF.IsAktUser());
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnUserLogout()
{
	CString szName{ toCString(USERRIGHTSREF.GetAktUser()) };

	CString szTemp1, szTemp;
	VERIFY(szTemp1.LoadString(IDS_REALLYLOGOUT));
    szTemp.Format(_T("%s %s"), LPCTSTR(szName), LPCTSTR(szTemp1));
    if ( ECMessageBox(szTemp, MB_ICONQUESTION | MB_YESNO) != IDYES)
    {
        return;
    }

	USERRIGHTSREF.SetAktUser("");
	REMOTEREF.setLinePBServiceMode(FALSE);
    SetUser ();
	SendToView(eViewSelect::eRecipeView, WM_LOGOUT);
	SendToView(eViewSelect::eDefaultView, WM_LOGOUT);
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnUpdateUserLogout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(USERRIGHTSREF.IsAktUser());
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnKeyboard()
{
    BOOL bErg = TKB.TK_IsHide();
    if ( bErg )
    {
		TKB.TK_Show ();
    }
    else
    {
		TKB.TK_Hide ();
    }
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::ActivateDefault()
{
	ActivateViewSelect(eViewSelect::eDefaultView);
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OpenTabs()
{
	auto doctemplate = GetDocTemplate(eViewSelect::eDefaultView);
	ASSERT_VALID(doctemplate);
	doctemplate->OpenDocumentFile(NULL);
	if (LAYOUTCONFIGREF.GetLayoutRecipe())
	{
		doctemplate = GetDocTemplate(eViewSelect::eRecipeView);
		ASSERT_VALID(doctemplate);
		doctemplate->OpenDocumentFile(NULL);
	}
	doctemplate = GetDocTemplate(eViewSelect::eAlarmArchiveView);
	ASSERT_VALID(doctemplate);
	doctemplate->OpenDocumentFile(NULL);
	doctemplate = GetDocTemplate(eViewSelect::eDiagramView);
	ASSERT_VALID(doctemplate);
	doctemplate->OpenDocumentFile(NULL);
	m_bViewInit = TRUE;
	ActivateDefault();
}
//**************************************************************************************************
//**************************************************************************************************
LRESULT CMainFrame::OnDoseEnd(WPARAM , LPARAM l)
{
	BOOL forceshutdown = (l < 0);
	if ( ! forceshutdown )
	{
#ifndef DEBUG
		if ( ! m_bAllowUserProgramEnd)
		{
			ECMessageBox(IDS_PROGRAMMEXITADMIN, MB_ICONEXCLAMATION | MB_OK);
		}
		else if (ECMessageBox(IDS_QUERY_END_EASYCONTROL, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			forceshutdown = TRUE;
			if (ECMessageBox(L"Shutdown windows ?", MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				((CEasyControlApp*)AfxGetApp())->m_bShutdown = TRUE;
			}
		}
#else
		forceshutdown = TRUE;
#endif

	}
	if (forceshutdown)
	{
		TKB.TK_Close();

		if (m_aTimer != NULL)
		{
			KillTimer(m_aTimer);
			m_aTimer = NULL;
		}
		if (m_aLicenceTimer != NULL)
		{
			KillTimer(m_aLicenceTimer);
			m_aLicenceTimer = NULL;
		}
		SendMessageToActiveView(WM_DOSEEND);
		((CEasyControlApp*)AfxGetApp())->m_bClose = TRUE;
		m_bViewInit = FALSE;
		CMDIFrameWndEx::OnClose();
	}
    return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnClose()
{
	OnDoseEnd(0, 0);
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnSystemSettings()
{
	CSettingsPropertySheet(IDS_SYSTEMSETTINGS, this).DoModal();
	TKB.TK_Hide();
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrame::OnUpdateSystemSettings(CCmdUI *pCmdUI)
{
	BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SYSTEMSETTINGS);
	pCmdUI->Enable(bLogin);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
    switch ( nIDEvent )
    {
        case TLICENCEID :
                {
		            KillTimer(m_aLicenceTimer);
		            m_aLicenceTimer = NULL;

#ifndef _DEBUG
		            ReadLicence ();
#endif
	            }
                break;

        case TLCID :
                {
					m_wndStatusBar.Progress();
					PostMessage(WM_TIMER_REFRESH);
				}
                break;

        
            default:
                {
                    ASSERT(FALSE);
                }
                break;
	}
	CMDIFrameWndEx::OnTimer(nIDEvent);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);
	m_wndStatusBar.Resize();
	if (nType == SIZE_MAXIMIZED || nType == SIZE_MAXSHOW)
	{
		GetWindowRect(m_StaticRect);
	}
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CMainFrame::OnMove(int x, int y)
{
	CMDIFrameWndEx::OnMove(x, y);
	if ( ( !m_StaticRect.IsRectEmpty()) && (!USERRIGHTSREF.IsAktSupervisor()))
	{
		MoveWindow(m_StaticRect);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::FetchData()
{
	auto& aL = dataimage::getLineDataImage();
	if ( aL.m_LayoutChanged )
	{
		return;
	}

	uint32_t count = aL.m_Cache.m_count;
	{
		REMOTEREF.getLineDataCache(aL.m_Cache);
		aL.m_LayoutChanged = aL.m_Cache.m_count != count;

		std::string svalue;
		REMOTEREF.getLineRecipeName(svalue);
		aL.m_szName = toCString(svalue);
		REMOTEREF.getLineANNumber(svalue);
		aL.m_szANNumber = toCString(svalue);
	}
	count = aL.m_Cache.m_count;

	base::CMassflowEntry massflow;
	for (uint32_t i = 0; i < count; i++)
	{
		auto& aV = dataimage::getDoseDataImage(i);
		auto& cache = aV.m_Cache;
		auto lastype = cache.m_DoseType;
		REMOTEREF.getDoseDataCache(i, cache);
		aL.m_LayoutChanged |= (cache.m_DoseType != lastype);
#if 0
		std::string szName;
		REMOTEREF.getDoseName(i, szName);
#endif
		massflow.Set(base::eMassflowSelect::eVIEWSETPOINT, i, cache.m_fSetpoint);
		massflow.Set(base::eMassflowSelect::eVIEWMASSFLOW, i, cache.m_fMassflow);
		massflow.Set(base::eMassflowSelect::eVIEWWEIGHT, i, cache.m_fActWeight);
		massflow.Set(base::eMassflowSelect::eVIEWDRIVECOMMAND, i, cache.m_fActualDriveCommand);
		massflow.Set(base::eMassflowSelect::eVIEWROTSPEED, i, cache.m_fIstRotationalSpeed);
		massflow.Set(base::eMassflowSelect::eVIEWDOSEPERFORMANCE, i, cache.m_fQMaxFaktor);
		massflow.Set(base::eMassflowSelect::eVIEWTOTALIZER, i, cache.m_fTotalizer);
	}
	MASSFLOWREF.Add(massflow);

	if (aL.m_LayoutChanged)
	{
		PostMessage(WM_LAYOUTCHANGED);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::AlarmCheck(void)
{
	auto count = dataimage::getLineDataImage().m_Cache.m_count;
	LOGREF.SetChanged(FALSE);
	for (int32_t i = 0; i < _S32(count); i++)
	{
		auto& aV = dataimage::getDoseDataImage(i);
		auto& cache = aV.m_Cache;
		// Übergang von Warnings auf Alarm
		CAlarmLog::AlarmCheck(i, cache.m_ulDoseWarning, aV.m_ulLastDoseWarning, cache.m_ulDoseAlarm, aV.m_ulLastDoseAlarm);
	}
	if (LOGREF.IsChanged())
	{
		PostMessageToActiveView(WM_NEW_LOGITEM);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::WindowsShutDownCheck(void)
{
	auto bWindowsShutDown = dataimage::getLineDataImage().m_Cache.m_ProcessStatus.flags.IOWindowsShutDown;
	if (bWindowsShutDown)
	{
		((CEasyControlApp*)AfxGetApp())->m_bShutdown = TRUE;
		OnDoseEnd(0, -1);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::RemoteStatusbarCheck(void)
{
	base::RemoteServerStatus remoteState = REFREMOTESERVER.getRemoteState();
	auto lastremote = m_wndStatusBar.GetRemoteState();
	if (lastremote != remoteState)
	{
		m_wndStatusBar.SetRemoteState(REFREMOTESERVER.getRemoteState());
		if (remoteState == base::RemoteServerStatus::REMOTE_STATUS_ERROR)
		{
			PostMessage(WM_CONNECTIONLOST);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::RecorderStatusbarCheck(void)
{
	auto newstate = m_Recorder.GetRecorderState();
	auto oldstate = m_wndStatusBar.GetRecorderState();
	if (oldstate != newstate)
	{
		m_wndStatusBar.SetRecorderState(newstate);
		if ((oldstate == eRecorderStatus::eRecorderActive) &&
			(newstate == eRecorderStatus::eRecorderError))
		{
			PostMessage(WM_RECORDERERROR);
		}
	}
}
//#define __TK_TEST
#ifdef __TK_TEST
//***********************************************************************************************
//***********************************************************************************************
void CMainFrame::TKTest()
{
	static BOOL bInit = FALSE;
	static FILE * out = nullptr;
	static int counter = 0;


	if (! bInit)
	{
		bInit = TRUE;
		counter = 0;
		auto erg = fopen_s(&out, "C:\\temp\\kyboardcounter.text", "a+");
		assert(erg == 0);
		std::srand(std::time(0)); // use cur
	}
	int k = std::rand() % 4;
	counter++;

	fprintf(out, "TK-Counter = %d, mode = %d\n", counter, k);

	switch (k)
	{
	case 0:
		TKB.TK_Hide();
		break;

	case 1:
		TKB.TK_Show();
		break;

	case 2:
		TKB.TK_Hide();
		break;

	case 3:
		TKB.TK_ShowLayout(ITKBoardInterface::eLayout::TKL_NUMERIC, ITKBoardInterface::TKPoint(100, 100));
		break;

	default:
		break;

	}
}
#endif
//***********************************************************************************************
//***********************************************************************************************
LRESULT CMainFrame::OnTimerRefresh(WPARAM, LPARAM)
{
#ifdef __TK_TEST
	TKTest();
#endif
	FetchData();
	AlarmCheck();
	RemoteStatusbarCheck();
	if (RECORDERCONFIGREF.m_RecorderActive || m_bLastRecorderActive )
	{
		m_bLastRecorderActive = RECORDERCONFIGREF.m_RecorderActive;
		m_Recorder.SetConfig(RECORDERCONFIGREF);
		auto newiten = m_Recorder.WriteRecorder();
		if (newiten)
		{
			SendMessageToActiveView(WM_NEW_RECITEM);
		}
	}
	RecorderStatusbarCheck();
	SendMessageToActiveView(WM_TIMER_REFRESH);
	m_wndMainBar.OnUpdateCmdUI(this, TRUE);
	for (int32_t i = 0; i < cViewSelectMax; i++)
	{
		m_wndToolBar[i].OnUpdateCmdUI(this, TRUE);
	}
	WindowsShutDownCheck();
	return 0L;
}

