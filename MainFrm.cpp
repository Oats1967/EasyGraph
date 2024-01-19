
// MainFrm.cpp: Implementierung der CMainFrame-Klasse
//

#include "pch.h"
#include "wmuser.h"
#include "EasyGraph.h"
#include "MainFrm.h"
#include "StringConvert.h"
#include "BASE/Utils/public/RecItemList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPMultiViewFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMultiViewFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	//ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_NEWDATE, &CMainFrame::OnNewDate)
	ON_MESSAGE(WM_SETVIEW, &CMainFrame::OnSetView)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // Statusleistenanzeige
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame-Erstellung/Zerstörung

CMainFrame::CMainFrame() noexcept : m_ActiveLine (0)
{
	// TODO: Hier Code für die Memberinitialisierung einfügen
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
}
#if 1
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//lpCreateStruct->style |= WS_MAXIMIZE;
	if (CBCGPMultiViewFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef _BCGSUITE_INC_
	BCGPSetVisualManagerSuite(CBCGPVisualManager::BCGPVM_OFFICE_2007_BLUE);
#endif

	SetMenu(NULL);

	m_wndToolBar.SetSizes(CSize(32, 32), CSize(24, 24));
	m_wndToolBar.SetMenuSizes(globalUtils.ScaleByDPI(CSize(32, 32), this), CSize(24, 24));

	m_wndToolBar.EnableNavigationButtons(ID_VIEW_BACK, ID_VIEW_FORWARD, 0, 0, 5, ID_VIEW_UP, TRUE);

	// Detect color depth. 256 color toolbars can be used in the
	// high or true color modes only (bits per pixel is > 8):
	CClientDC dc(this);
	BOOL bIsHighColor = dc.GetDeviceCaps(BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_TOOLBAR_HC : 0;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//-----------------------
	// Setup toolbar buttons:
	//-----------------------
	CMenu menuView;
	menuView.LoadMenu(IDR_MENU_VIEW);

	m_wndToolBar.ReplaceButton(ID_TOOLBAR_VIEW,
		CBCGPToolbarMenuButton((UINT)-1, menuView.Detach(), -1, _T("&View")));

	m_wndToolBar.ReplaceButton(ID_COLOR_LABEL, CBCGPToolbarLabel(ID_COLOR_LABEL, _T("Color theme:")));

	CBCGPToolbarComboBoxButton comboTheme(ID_COLOR_THEME_COMBO,
#ifdef _BCGSUITE_INC_
		GetCmdMgr()->GetCmdImage(ID_COLOR_THEME_COMBO, FALSE),
#else
		CImageHash::GetImageOfCommand(ID_COLOR_THEME_COMBO, FALSE),
#endif
		CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(150, this));

	auto& rList = theApp.m_LineConfig.GetLineList();
	for (auto& rItem : rList)
	{
		comboTheme.AddItem(toCString(rItem.m_szName));
	}
	comboTheme.SelectItem(0);

	m_wndToolBar.ReplaceButton(ID_COLOR_THEME_COMBO, comboTheme);

	//m_wndToolBar.ReplaceButton(ID_ANIMATION_LABEL, CBCGPToolbarLabel(ID_ANIMATION_LABEL, _T("Animation:")));

	//m_wndToolBar.ReplaceButton(ID_ANIMATION_COMBO, CToolbarAnimationCombo(ID_ANIMATION_COMBO));

	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_DARK_THEME), NULL, TRUE, FALSE);
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_CHART_EXPORT), NULL, TRUE, TRUE);
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_CHART_COPY), NULL, TRUE, TRUE);

	m_wndToolBar.EnableCustomizeButton(TRUE, (UINT)-1, _T("More Items"));

	//-------------------
	// Create status bar:
	//-------------------
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
			sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
#if 0

	if (!m_wndWorkSpace.Create(_T("Features"), this, CRect(0, 0, globalUtils.ScaleByDPI(300, this), globalUtils.ScaleByDPI(300, this)),
		TRUE, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
#ifndef _BCGSUITE_INC_
		CBRS_BCGP_REGULAR_TABS, CBRS_BCGP_RESIZE | CBRS_BCGP_AUTOHIDE))
#else
		AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE | AFX_CBRS_AUTOHIDE))
#endif
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}

#ifndef _BCGSUITE_INC_
	m_wndWorkSpace.SetIconIndex(0);
#endif
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
#endif

	if (!CreateDockingWindows())
	{
		TRACE0("Fehler beim Erstellen der Andockfenster.\n");
		return -1;
	}

	m_wndDoseSelectView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCalendarView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndDoseSelectView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndCalendarView.AttachToTabWnd(&m_wndDoseSelectView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);


	return 0;
}
#else
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMultiViewFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // Weitere Stile verfügbar...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // Auf "FALSE" festlegen, um die Schaltfläche "Schließen" rechts auf der Registerkarte zu platzieren
	mdiTabParams.m_bTabIcons = FALSE;    // Auf "TRUE" festlegen, um Dokumentsymbole auf MDI-Registerkarten zu aktivieren
	mdiTabParams.m_bAutoColor = TRUE;    // Auf "FALSE" festlegen, um automatische Farben für MDI-Registerkarten zu deaktivieren
	mdiTabParams.m_bDocumentMenu = TRUE; // Dokumentmenü am rechten Rand des Registerkartenbereichs aktivieren
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Fehler beim Erstellen der Menüleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// Verhindern, dass die Menüleiste beim Aktivieren den Fokus erhält
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Fehler beim Erstellen der Symbolleiste.\n");
		return -1;      // Fehler beim Erstellen
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	CBCGPToolbarComboBoxButton comboTheme(ID_COLOR_THEME_COMBO,
#ifdef _BCGSUITE_INC_
		GetCmdMgr()->GetCmdImage(ID_COLOR_THEME_COMBO, FALSE),
#else
		CImageHash::GetImageOfCommand(ID_COLOR_THEME_COMBO, FALSE),
#endif
		CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(150));

	comboTheme.m_strText = _T("Color Theme");
	comboTheme.AddItem(_T("Light Theme"));
	comboTheme.AddItem(_T("Dark Theme"));
	comboTheme.AddItem(_T("Blue Theme"));

	comboTheme.SelectItem(m_nColorTheme);
	m_wndToolBar.ReplaceButton(ID_COLOR_THEME_COMBO, comboTheme);

	// Benutzerdefinierte Symbolleistenvorgänge zulassen:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Fehler beim Erstellen der Statusleiste.\n");
		return -1;      // Fehler beim Erstellen
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Löschen Sie diese fünf Zeilen, wenn Sie nicht möchten, dass die Symbolleiste und die Menüleiste andockbar sind
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Andockfensterverhalten wie in Visual Studio 2005 aktivieren
	CDockingManager::SetDockingMode(DT_SMART);
	// Automatisches Ausblenden von Andockfenstern wie in Visual Studio 2005 aktivieren
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Menüelementbild laden (nicht auf Standardsymbolleisten platziert):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// Andockfenster erstellen
	if (!CreateDockingWindows())
	{
		TRACE0("Fehler beim Erstellen der Andockfenster.\n");
		return -1;
	}

	m_wndDoseSelectView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCalendarView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndDoseSelectView);
	CDockablePane* pTabbedBar = nullptr;
	m_wndCalendarView.AttachToTabWnd(&m_wndDoseSelectView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// Visuellen Manager und Stil auf Basis eines persistenten Werts festlegen
	OnApplicationLook(theApp.m_nAppLook);

	// Dialogfeld für erweiterte Fensterverwaltung aktivieren
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Umpositionieren des Menüs für Symbolleisten und Andockfenster aktivieren
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// Schnelles Anpassen von Symbolleisten mit Alt+Ziehen aktivieren
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// Benutzerdefinierte Symbolleistenbilder laden
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// Menüpersonalisierung aktivieren (zuletzt verwendete Befehle)
	// TODO: Definieren Sie eigene Basisbefehle, wobei jedes Pulldownmenü mindestens einen Basisbefehl enthalten muss.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Vertauschen Sie die Reihenfolge von Dokumentname und Anwendungsname auf der Titelleiste des Fensters. Hierdurch
	// wird die Verwendbarkeit der Taskleiste verbessert, da der Dokumentname in der Miniaturansicht sichtbar ist.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}
#endif

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~FWS_ADDTOTITLE;

	if (!CBCGPMultiViewFrameWnd::PreCreateWindow(cs))
		return FALSE;

	WNDCLASS wndcls;

	if (GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls))
	{
		HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME), RT_GROUP_ICON);
		HICON hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME));

		// register a very similar WNDCLASS but without CS_HREDRAW and CS_VREDRAW styles:
		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, wndcls.hCursor, wndcls.hbrBackground, hIcon);
	}

	return TRUE;
}


BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Klassenansicht erstellen
	CString strCalendarView;
	bNameValid = strCalendarView.LoadString(IDS_CALENDAR_VIEW);
	ASSERT(bNameValid);
	if (!m_wndCalendarView.Create(strCalendarView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CALENDAR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Fehler beim Erstellen des Klassenansichtsfensters.\n");
		return FALSE; // Fehler beim Erstellen
	}

	// Dateiansicht erstellen
	CString strDoseSelectView;
	bNameValid = strDoseSelectView.LoadString(IDS_GRAFIKSELECT_VIEW);
	ASSERT(bNameValid);
	if (!m_wndDoseSelectView.Create(strDoseSelectView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_DOSESELECT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Fehler beim Erstellen des Dateiansichtsfensters.\n");
		return FALSE; // Fehler beim Erstellen
	}
#if 0
	// Ausgabefenster erstellen
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Fehler beim Erstellen des Ausgabefensters.\n");
		return FALSE; // Fehler beim Erstellen
	}
#endif
	// Eigenschaftenfenster erstellen
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenfensters.\n");
		return FALSE; // Fehler beim Erstellen
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndDoseSelectView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndCalendarView.SetIcon(hClassViewIcon, FALSE);

#if 0
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
#endif

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	//UpdateMDITabbedBarsIcons();
}

// CMainFrame-Diagnose

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMultiViewFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMultiViewFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame-Meldungshandler

void CMainFrame::OnWindowManager()
{
	//ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* Menüs überprüfen */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CBCGPMultiViewFrameWnd::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	//m_wndOutput.UpdateFonts();
	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	if (!CBCGPMultiViewFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Anpassungsschaltfläche für alle benutzerdefinierten Symbolleisten aktivieren
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}
	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CBCGPMultiViewFrameWnd::OnSettingChange(uFlags, lpszSection);
	//m_wndOutput.UpdateFonts();
}


LRESULT CMainFrame::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	auto pDate = (DateToShow*)wParam;
	if (pDate != nullptr)
	{
		LoadRectItemList(*pDate);
		auto pView = GetActiveView();
		if (pView)
		{
			pView->SendMessage(WM_NEWDATE, wParam);
		}
	}
	return 0L;
}

LRESULT CMainFrame::OnSetView(WPARAM wParam, LPARAM lParam)
{
	SelectView(wParam);
	return 0L;
}



void CMainFrame::LoadRectItemList(DateToShow& rDate)
{
	base::utils::CRecItemList gList;

	auto pLineItem = theApp.m_LineConfig[m_ActiveLine];
	if (pLineItem != nullptr)
	{
		COleDateTime dSO(rDate.dateStart);
		COleDateTime dEO(rDate.dateEnd);

		COleDateTimeSpan difftime = dEO - dSO;
		int32_t days = difftime.GetDays();
		COleDateTimeSpan dayskip(1, 0, 0, 0);

		for (int32_t k = 0; k < days; k++)
		{
			tm date_tm;
			memset(&date_tm, 0, sizeof(date_tm));
			date_tm.tm_year = dSO.GetYear() - 1900;
			date_tm.tm_mon = dSO.GetMonth() - 1;
			date_tm.tm_mday = dSO.GetDay();
			date_tm.tm_isdst = 0;
			time_t aTime = std::mktime(&date_tm);

			base::utils::CRecItemList tempList;
			tempList.SetPath(pLineItem->m_szPath);
			tempList.SetFilename(aTime);
			tempList.Load();
			gList += tempList;
			dSO += dayskip;
		}
	}
}
