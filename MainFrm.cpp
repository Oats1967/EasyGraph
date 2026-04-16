
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

#define REFRESHTIMER 1

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
	ON_COMMAND(ID_TB_VIEW, &CMainFrame::OnRealMonitoring)
	ON_UPDATE_COMMAND_UI(ID_TB_VIEW, &CMainFrame::OnUpdateRealMonitoring)


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
	ON_WM_TIMER()
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


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == REFRESHTIMER)
	{
		// Aktion hier ausführen, z.B. StatusBar aktualisieren
		uint32_t bEnable = g_Statistics.GetRealMonitoring();
		DateToShow aDate;
		GetRealMonitoringTimeSpan(aDate);

		g_Statistics.SetDateToShow(aDate);
		g_Statistics.SetRealMonitoring(bEnable);
		UpdateNewData();
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy() 
{
	g_Statistics.SetRealMonitoring(FALSE);
	KillTimer(REFRESHTIMER); // Timer beenden
	CFrameWnd::OnDestroy();
}


LRESULT CMainFrame::OnClosePane(WPARAM, LPARAM lp)
{
	return (LRESULT)TRUE;//prevent close , we already close it
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//lpCreateStruct->style |= WS_MAXIMIZE;
	if (CBCGPMultiViewFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifdef _BCGSUITE_INC_
	BCGPSetVisualManagerSuite(CBCGPVisualManager::BCGPVM_OFFICE_2007_BLUE);
#endif

	SetMenu(NULL);

	//m_wndToolBar.EnableNavigationButtons(ID_VIEW_BACK, ID_VIEW_FORWARD, 0, 0, 5, ID_VIEW_UP, TRUE);


	m_wndToolBar.SetSizes(CSize(24, 24), CSize(24, 24));
	m_wndToolBar.SetMenuSizes(globalUtils.ScaleByDPI(CSize(32, 32), this), CSize(32, 32));

	//m_wndToolBar.EnableNavigationButtons(ID_VIEW_BACK, ID_VIEW_FORWARD, 0, 0, 5, ID_VIEW_UP, TRUE);

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
	

	CString szLine;
	VERIFY(szLine.LoadString(IDS_PW_LINE));
	m_wndToolBar.ReplaceButton(ID_TB_LABEL_LINE, CBCGPToolbarLabel(ID_TB_LABEL_LINE, szLine));

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
	CString szFeeder;
	VERIFY(szFeeder.LoadString(IDS_MF_FEEDER));
	CString szAll;
	VERIFY(szAll.LoadString(IDS_MF_ALL));
	{
		CBCGPToolbarComboBoxButton comboTheme(ID_TB_DOSESELECT_COMBO,
#ifdef _BCGSUITE_INC_
			GetCmdMgr()->GetCmdImage(ID_TB_DOSESELECT_COMBO, FALSE),
#else
			CImageHash::GetImageOfCommand(ID_TB_COLOR_THEME_COMBO, FALSE),
#endif
			CBS_DROPDOWNLIST, globalUtils.ScaleByDPI(120, this));

		CString szTemp;
		szTemp.Format(_T("%s: %s"), (LPCTSTR)szFeeder, LPCTSTR(szAll));
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

		CString szOrder;
		VERIFY(szOrder.LoadString(IDS_MF_ORDER));
		CString szTemp;
		szTemp.Format(_T("%s: %s"), LPCTSTR(szOrder), LPCTSTR(szAll));
		comboTheme.AddItem(szTemp);
		comboTheme.SelectItem(0);
		m_wndToolBar.ReplaceButton(ID_TB_ANNUMBER_COMBO, comboTheme);
	}
	CString szLogMessage;
	VERIFY(szLogMessage.LoadString(IDS_MF_LOGMESSAGE));

	m_wndToolBar.ReplaceButton(ID_TB_LOGMESSAGE_BUTTON, CBCGPToolbarLabel(ID_TB_LOGMESSAGE_BUTTON, szLogMessage));
	m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_TB_LOGMESSAGE_BUTTON), TBBS_CHECKBOX);

	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_CHART_EXPORT), NULL, TRUE, TRUE);
	m_wndToolBar.SetToolBarBtnText(m_wndToolBar.CommandToIndex(ID_TB_CHART_COPY), NULL, TRUE, TRUE);

	//m_wndToolBar.EnableCustomizeButton(TRUE, (UINT)-1, _T("More Items"));

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

void CMainFrame::GetRealMonitoringTimeSpan(DateToShow& rDate)
{
	COleDateTime dateEnd = COleDateTime::GetCurrentTime();
	COleDateTime dateStart = dateEnd;

	COleDateTimeSpan timediff;
	const auto& rS = g_Statistics.GetSettings();
	auto totalminutes = rS.m_RealMonitoringHistoryMinutes;
	auto minutes	= totalminutes % 60U;
	auto totalhours = totalminutes / 60U;
	auto hours	    = totalhours % 60U;
	auto days	    = totalhours / 24U;


	timediff.SetDateTimeSpan(days, hours, minutes, 0);
	dateStart -= timediff;

	rDate.dateStart = dateStart.m_dt;
	rDate.dateEnd = dateEnd.m_dt;
}


void CMainFrame::OnRealMonitoring()
{
	uint32_t bEnable = g_Statistics.GetRealMonitoring();
	bEnable = !bEnable;
	if (bEnable)
	{
		DateToShow aDate;
		GetRealMonitoringTimeSpan(aDate);
		g_Statistics.SetDateToShow(aDate);
		g_Statistics.SetRealMonitoring(bEnable);
		UpdateNewData();
		const auto& rS = g_Statistics.GetSettings();
		auto sec = rS.m_RealMonitoringRefreshTime;
		sec *= 1000;
		sec = RANGE(sec, 10*1000, 300 * 1000);
		SetTimer(REFRESHTIMER, sec, NULL);
	}
	else
	{
		KillTimer(REFRESHTIMER);
		g_Statistics.SetRealMonitoring(bEnable);
	}
}

void CMainFrame::OnUpdateRealMonitoring(CCmdUI* pCmdUI)
{
	uint32_t bEnable = TRUE;
	pCmdUI->Enable(bEnable);
	pCmdUI->SetCheck(g_Statistics.GetRealMonitoring());
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
		CString sz;
		if (sel > 0)
		{
			sz = pCombobox->GetItem(sel);
		}
		g_Statistics.SetANNumber(toStdString(sz));
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
		auto index = m_wndToolBar.CommandToIndex(ID_TB_DOSESELECT_COMBO);
		// Retrieve button
		auto* pButton = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(index));
		ASSERT(pButton);

		auto btcount = _U32(pButton->GetCount());
		if (btcount > 0)
		{
			btcount--;
		}
		auto fdcount = g_Statistics.GetFeederCount();
		if (btcount != fdcount)
		{
			pButton->RemoveAllItems();

			CString szFeeder;
			VERIFY(szFeeder.LoadString(IDS_MF_FEEDER));
			CString szAll;
			VERIFY(szAll.LoadString(IDS_MF_ALL));
			{
				CString szTemp;
				szTemp.Format(_T("%s: %s"), (LPCTSTR)szFeeder, LPCTSTR(szAll));
				pButton->AddItem(szTemp);
			}
			for (uint32_t k = 0; k < fdcount; k++)
			{
				CString szTemp;
				szTemp.Format(_T("%s: %u"), (LPCTSTR)szFeeder, k + 1);
				pButton->AddItem(szTemp);
			}
		}
		auto activeFeeder = g_Statistics.GetActiveFeeder();
		pButton->SelectItem((activeFeeder < 0) ? 0 : activeFeeder + 1);
	}
	// Retrieve button
	// Find button index for command ID
	{
		const auto& rANNumberList = g_Statistics.GetANNumberList();

		int index = m_wndToolBar.CommandToIndex(ID_TB_ANNUMBER_COMBO);
		auto *pButton = DYNAMIC_DOWNCAST(CBCGPToolbarComboBoxButton, m_wndToolBar.GetButton(index));
		auto btcount = _U32(pButton->GetCount());
		if (btcount > 0)
		{
			btcount--;
		}
		auto ancount = _U32(rANNumberList.size());
		if (btcount != ancount)
		{
			pButton->RemoveAllItems();
			CString szOrder;
			VERIFY(szOrder.LoadString(IDS_MF_ORDER));
			CString szAll;
			VERIFY(szAll.LoadString(IDS_MF_ALL));
			CString szTemp;
			szTemp.Format(_T("%s: %s"), LPCTSTR(szOrder), LPCTSTR(szAll));
			pButton->AddItem(szTemp);
			uint32_t sel = 0;
			for (uint32_t k = 0; k < ancount; k++)
			{
				const auto& str = rANNumberList[k];
				if (str == g_Statistics.GetANNumber())
				{
					sel = k + 1U;
				}
				pButton->AddItem(toCString(str));
			}
			pButton->SelectItem(_S32(sel));
		}
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
