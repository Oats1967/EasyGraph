
// MainFrm.cpp: Implementierung der CMainFrame-Klasse
//

#include "pch.h"
#include "wmuser.h"
#include "EasyGraph.h"
#include "MainFrm.h"
#include "StringConvert.h"
#include "EasyGraphView.h"
#include "global.h"

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
	ON_COMMAND(ID_TB_COLOR_THEME_COMBO, &CMainFrame::OnLineCombo)
	ON_CBN_SELENDOK(ID_TB_COLOR_THEME_COMBO, &CMainFrame::OnLineCombo)
	ON_COMMAND(ID_TB_DOSESELECT_COMBO, &CMainFrame::OnDoseSelectCombo)
	ON_CBN_SELENDOK(ID_TB_DOSESELECT_COMBO, &CMainFrame::OnDoseSelectCombo)
	ON_COMMAND(ID_TB_ANNUMBER_COMBO, &CMainFrame::OnANNumberCombo)
	ON_CBN_SELENDOK(ID_TB_ANNUMBER_COMBO, &CMainFrame::OnANNumberCombo)
	ON_COMMAND(ID_TB_LOGMESSAGE_BUTTON, &CMainFrame::OnLogMessages)
	ON_UPDATE_COMMAND_UI(ID_TB_LOGMESSAGE_BUTTON, &CMainFrame::OnUpdateLogMessages)


	//ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_NEWDATE, &CMainFrame::OnNewDate)
	ON_MESSAGE(WM_SETVIEW, &CMainFrame::OnSetView)
	ON_MESSAGE(WM_LINECOLOR, &CMainFrame::OnSetLineColor)
	ON_MESSAGE(WM_CATEGORY, &CMainFrame::OnSetCategory)
	ON_MESSAGE(WM_VISIBLE, &CMainFrame::OnSetVisible)
	ON_MESSAGE(WM_LINEWIDTH, &CMainFrame::OnSetLineWidth)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_PRESS_CLOSE_BUTTON, OnClosePane)
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

LRESULT CMainFrame::OnClosePane(WPARAM, LPARAM lp)
{
#if 0
	CBasePane* pane = (CBasePane*)lp;
	int id = pane->GetDlgCtrlID();
	pane->ShowPane(FALSE, FALSE, FALSE);
	RemovePaneFromDockManager(pane, TRUE, TRUE, TRUE, NULL);
	AdjustDockingLayout();
	pane->PostMessage(WM_CLOSE);
	PostMessage(WM_RESETMEMBER, id, 0);
#endif
	return (LRESULT)TRUE;//prevent close , we already close it
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

	m_wndToolBar.ReplaceButton(ID_TB_VIEW,
		CBCGPToolbarMenuButton((UINT)-1, menuView.Detach(), -1, _T("&View")));

	menuView.LoadMenu(IDR_MENU_MAGNIFY);
	m_wndToolBar.ReplaceButton(ID_TB_MAGNIFY,
		CBCGPToolbarMenuButton((UINT)-1, menuView.GetSubMenu(0)->GetSafeHmenu(), -1, _T("&Zoom")));
	

	m_wndToolBar.ReplaceButton(ID_TB_LABEL_LINE, CBCGPToolbarLabel(ID_TB_LABEL_LINE, _T("Linie:")));

	{
		CBCGPToolbarComboBoxButton comboTheme(ID_TB_COLOR_THEME_COMBO,
#ifdef _BCGSUITE_INC_
			GetCmdMgr()->GetCmdImage(ID_TB_COLOR_THEME_COMBO, FALSE),
#else
			CImageHash::GetImageOfCommand(ID_TB_COLOR_THEME_COMBO, FALSE),
#endif
			CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(120, this));

		auto& rList = g_Statistics.GetLineGraphConfig().m_field;
		for (auto& rItem : rList)
		{
			comboTheme.AddItem(toCString(rItem.m_szName));
		}
		comboTheme.SelectItem(0);

		m_wndToolBar.ReplaceButton(ID_TB_COLOR_THEME_COMBO, comboTheme);
	}
	{
		CBCGPToolbarComboBoxButton comboTheme(ID_TB_DOSESELECT_COMBO,
#ifdef _BCGSUITE_INC_
			GetCmdMgr()->GetCmdImage(ID_TB_DOSESELECT_COMBO, FALSE),
#else
			CImageHash::GetImageOfCommand(ID_TB_COLOR_THEME_COMBO, FALSE),
#endif
			CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(120, this));

		CString szTemp;
		szTemp.Format("Dosierung: all");
		comboTheme.AddItem(szTemp);
		comboTheme.SelectItem(0);
		m_wndToolBar.ReplaceButton(ID_TB_DOSESELECT_COMBO, comboTheme);
	}
	{
		CBCGPToolbarComboBoxButton comboTheme(ID_TB_ANNUMBER_COMBO,
#ifdef _BCGSUITE_INC_
			GetCmdMgr()->GetCmdImage(ID_TB_ANNUMBER_COMBO, FALSE),
#else
			CImageHash::GetImageOfCommand(ID_TB_COLOR_THEME_COMBO, FALSE),
#endif
			CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(140, this));

		CString szTemp;
		szTemp.Format("Auftragsnummer: all");
		comboTheme.AddItem(szTemp);
		comboTheme.SelectItem(0);
		m_wndToolBar.ReplaceButton(ID_TB_ANNUMBER_COMBO, comboTheme);
	}
	m_wndToolBar.ReplaceButton(ID_TB_LOGMESSAGE_BUTTON, CBCGPToolbarLabel(ID_TB_LOGMESSAGE_BUTTON, _T("Logmeldungen:")));
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_TB_LOGMESSAGE_BUTTON), TBBS_CHECKBOX);

	//m_wndToolBar.ReplaceButton(ID_ANIMATION_LABEL, CBCGPToolbarLabel(ID_ANIMATION_LABEL, _T("Animation:")));

	//m_wndToolBar.ReplaceButton(ID_ANIMATION_COMBO, CToolbarAnimationCombo(ID_ANIMATION_COMBO));

	//m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_DARK_THEME), NULL, TRUE, FALSE);
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_CHART_EXPORT), NULL, TRUE, TRUE);
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_CHART_COPY), NULL, TRUE, TRUE);

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

	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCalendarView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndWorkSpace);
	CDockablePane* pTabbedBar = nullptr;
	m_wndCalendarView.AttachToTabWnd(&m_wndWorkSpace, DM_SHOW, TRUE, &pTabbedBar);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	RecalcLayout();
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

	CBCGPToolbarComboBoxButton comboTheme(ID_TB_COLOR_THEME_COMBO,
#ifdef _BCGSUITE_INC_
		GetCmdMgr()->GetCmdImage(ID_TB_COLOR_THEME_COMBO, FALSE),
#else
		CImageHash::GetImageOfCommand(ID_TB_COLOR_THEME_COMBO, FALSE),
#endif
		CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(150));

	comboTheme.m_strText = _T("Color Theme");
	comboTheme.AddItem(_T("Light Theme"));
	comboTheme.AddItem(_T("Dark Theme"));
	comboTheme.AddItem(_T("Blue Theme"));

	comboTheme.SelectItem(m_nActiveLine);
	m_wndToolBar.ReplaceButton(ID_TB_COLOR_THEME_COMBO, comboTheme);

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

void CMainFrame::OnLogMessages()
{
	UINT nID;
	UINT nStyle;
	int iImage;

	m_wndToolBar.GetButtonInfo(m_wndToolBar.CommandToIndex(ID_TB_LOGMESSAGE_BUTTON), nID, nStyle, iImage);
	BOOL bPressed = (nStyle & TBBS_CHECKED) == 0;
	g_Statistics.SetLogMessages(bPressed);
	UpdateNewData();
}

void CMainFrame::OnUpdateLogMessages(CCmdUI* pCmdUI)
{
	uint32_t bEnable = (g_Statistics.GetFeederCount() * g_Statistics.GetLogDaysList().GetCount()) > 0;
	pCmdUI->Enable(bEnable);
}


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
	if (!m_wndWorkSpace.Create(strDoseSelectView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_DOSESELECT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
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
	m_wndWorkSpace.SetIcon(hFileViewIcon, FALSE);

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

void CMainFrame::OnLineCombo()
{
	CBCGPToolbarComboBoxButton* pCombobox = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_COLOR_THEME_COMBO)));
	ASSERT_VALID(pCombobox);
	m_nActiveLine = pCombobox->GetCurSel();
	g_Statistics.SetActiveLine(m_nActiveLine);
	g_Statistics.SetActiveFeeder(-1);
	g_Statistics.SetFeederCount(0);
	g_Statistics.SetANNumber("");
	g_Statistics.GetANNumberList().clear();
	UpdateNewData();
}

void CMainFrame::OnDoseSelectCombo()
{
	CBCGPToolbarComboBoxButton* pCombobox = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_DOSESELECT_COMBO)));
	ASSERT_VALID(pCombobox);
	m_nActiveFeeder = pCombobox->GetCurSel();
	g_Statistics.SetActiveFeeder(m_nActiveFeeder - 1);
	auto pView = GetActiveView();
	if (pView)
	{
		pView->SendMessage(WM_NEWDATE);
	}
}

void CMainFrame::OnANNumberCombo()
{
	CBCGPToolbarComboBoxButton* pCombobox = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_TB_ANNUMBER_COMBO)));
	ASSERT_VALID(pCombobox);
	auto sel = pCombobox->GetCurSel();
	if (sel >= 0)
	{
		std::string sz;
		if (sel > 0)
		{
			sz = pCombobox->GetItem(sel);
		}
		g_Statistics.SetANNumber(sz);
		g_Statistics.SetActiveFeeder(-1);
		g_Statistics.SetFeederCount(0);
		UpdateNewData();
	}
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

void CMainFrame::UpdateNewData()
{
	g_Statistics.LoadRectItemList();

	// Find button index for command ID
	{
		int index = m_wndToolBar.CommandToIndex(ID_TB_DOSESELECT_COMBO);
		// Retrieve button
		auto* pButton = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(index));
		while (pButton->GetCount())
		{
			pButton->DeleteItem(0);
		}

		pButton->ClearData();
		{
			CString szTemp;
			szTemp.Format("Dosierung: all");
			pButton->AddItem(szTemp);
		}
		auto count = g_Statistics.GetFeederCount();
		for (uint32_t k = 0; k < count; k++)
		{
			CString szTemp;
			szTemp.Format("Dosierung: %u", k + 1);
			pButton->AddItem(szTemp);
		}
		auto activeFeeder = g_Statistics.GetActiveFeeder();
		pButton->SelectItem((activeFeeder < 0) ? 0 : activeFeeder + 1);
	}
	// Retrieve button
	// Find button index for command ID
	{
		const auto& rANNumberList = g_Statistics.GetANNumberList();

		int sel = 0;
		int index = m_wndToolBar.CommandToIndex(ID_TB_ANNUMBER_COMBO);
		auto *pButton = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(index));
		while (pButton->GetCount())
		{
			pButton->DeleteItem(0);
		}
		pButton->ClearData();
		pButton->AddItem("Auftragsnummer: all");
		auto count = _U32(rANNumberList.size());
		for (uint32_t k = 0; k < count; k++)
		{
			if (rANNumberList[k] == g_Statistics.GetANNumber())
			{
				sel = k + 1;
			}
			pButton->AddItem(toCString(rANNumberList[k]));
		}
		pButton->SelectItem(sel);
	}

	auto pView = GetActiveView();
	if (pView)
	{
		pView->SendMessage(WM_NEWDATE);
	}
}



LRESULT CMainFrame::OnNewDate(WPARAM wParam, LPARAM lParam)
{
	auto pDate = (DateToShow*)wParam;
	if (pDate != nullptr)
	{
		g_Statistics.SetActiveFeeder(-1);
		g_Statistics.SetFeederCount(0);
		g_Statistics.SetANNumber("");
		g_Statistics.GetANNumberList().clear();
		g_Statistics.SetDateToShow(*pDate);
		UpdateNewData();
	}
	return 0L;
}

LRESULT CMainFrame::OnSetView(WPARAM wParam, LPARAM lParam)
{
	SelectView(_S32(wParam));
	return 0L;
}

LRESULT CMainFrame::OnSetLineColor(WPARAM wParam, LPARAM lParam)
{
	CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, GetActiveView());
	if (pChartView != NULL)
	{
		pChartView->OnUpdateLineColor(base::eMassflowSelect(wParam));
	}
	return 0L;
}


LRESULT CMainFrame::OnSetCategory(WPARAM wParam, LPARAM lParam)
{
	CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, GetActiveView());
	if (pChartView != NULL)
	{
		pChartView->OnUpdateCategory(base::eMassflowSelect(wParam));
	}
	return 0L;
}

LRESULT CMainFrame::OnSetLineWidth(WPARAM wParam, LPARAM lParam)
{
	CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, GetActiveView());
	if (pChartView != NULL)
	{
		pChartView->OnUpdateLineWidth(base::eMassflowSelect(wParam));
	}
	return 0L;
}

LRESULT CMainFrame::OnSetVisible(WPARAM wParam, LPARAM lParam)
{
	CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, GetActiveView());
	if (pChartView != NULL)
	{
		pChartView->OnUpdateVisible(base::eMassflowSelect(wParam));
	}
	return 0L;
}


void CMainFrame::OnSelectView(int nView)
{
	BOOL bFirstTime = m_wndWorkSpace.GetSelected() <= 0;

	m_wndWorkSpace.SelectView(nView);

	if (!bFirstTime)
	{
		CEasyGraphView* pChartView = DYNAMIC_DOWNCAST(CEasyGraphView, GetActiveView());
		if (pChartView != NULL)
		{
			ASSERT_VALID(pChartView);
		}
	}

	theApp.WriteInt(_T("GroupID"), m_nSelGroup);
	theApp.WriteInt(_T("ViewType"), m_nSelView);
}


CView* CMainFrame::GetFeatureView(CDemoFeature* pFeature)
{
	return GetView(pFeature->GetIndex());
}
