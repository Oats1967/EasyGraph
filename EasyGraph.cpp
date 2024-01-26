
// EasyGraph.cpp: Definiert das Klassenverhalten für die Anwendung.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EasyGraph.h"
#include "MainFrm.h"
#include "SplashWindow.h"

#include "ChildFrm.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "ChartLineView.h"
#include "ConfigItem.h"
#include "wmuser.h"
#include "global.h"
#include "StringConvert.h"

#include "BASE/Utils/public/xml/EasyGraphConfigXml.h"
#include "BASE/Utils/public/xml/LineGraphConfigXml.h"
#include "BASE/Utils/public/xml/ProductDatabaseXml.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define EASYGRAPHCONFIGFILE "EasyGraph.xml"

// CEasyGraphApp
struct ConfigItem theConfig;
static base::CEasyGraphConfig g_EasyGraphCfg;


BEGIN_MESSAGE_MAP(CEasyGraphApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEasyGraphApp::OnAppAbout)
	ON_COMMAND(ID_COLOR_THEME_COMBO, &CEasyGraphApp::OnAppAbout)
	// Dateibasierte Standarddokumentbefehle
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standarddruckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEasyGraphApp-Erstellung
static BOOL ReadCMDPath(CString& sz)
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath_s(LPCSTR(sz), drive, dir, fname, ext);
	_makepath_s(path_buffer, drive, dir, "", "");
	int sLen = (int)strlen(path_buffer) - 1;

	while ((sLen > 0) && (path_buffer[sLen] == '\\'))
	{
		path_buffer[sLen] = 0;
		sLen--;
	}
	sz = CString(path_buffer);
	return !sz.IsEmpty();
}


CEasyGraphApp::CEasyGraphApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// Neustart-Manager unterstützen
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// Wenn die Anwendung mit Common Language Runtime-Unterstützung (/clr) erstellt wurde:
	//     1) Diese zusätzliche Eigenschaft ist erforderlich, damit der Neustart-Manager ordnungsgemäß unterstützt wird.
	//     2) Für die Projekterstellung müssen Sie im Projekt einen Verweis auf System.Windows.Forms hinzufügen.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: Ersetzen Sie die Anwendungs-ID-Zeichenfolge unten durch eine eindeutige ID-Zeichenfolge; empfohlen
	// Das Format für die Zeichenfolge ist: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EasyGraph.AppID.NoVersion"));

	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}

// Das einzige CEasyGraphApp-Objekt

CEasyGraphApp theApp;


// CEasyGraphApp-Initialisierung
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyGraphApp::LoadInitFile()
{
	const std::string xmlfile{ toStdString(m_szCMDPath) + "\\" + std::string(EASYGRAPHCONFIGFILE) };
	base::xml::CEasyGraphConfigXml myxml;
	auto result = myxml.Load(xmlfile);
	if (result)
	{
		m_SaveCfg = EASYCGRAPHCONFIGFILE = myxml.Get();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyGraphApp::LoadLineGraphConfig()
{
	const std::string xmlfile{ EASYCGRAPHCONFIGFILE.m_LineConfigFile };
	base::xml::CLineGraphConfigXml myxml;
	auto result = myxml.Load(xmlfile);
	if (result)
	{
		g_Statistics.SetLineGraphConfig(myxml.Get());
	}
	return result;
}
//************************************************************************************************************************
BOOL CEasyGraphApp::LoadProductDatabase(void)
{
	LOGDEBUG("Reading ProductDatabase, " << EASYCGRAPHCONFIGFILE.m_ProductDatabaseFile);
	base::xml::CProductDatabaseXml config;
	auto result = config.Load(EASYCGRAPHCONFIGFILE.m_ProductDatabaseFile);
	if (!result)
	{
		LOGERROR("Error reading ProductDatabase, " << EASYCGRAPHCONFIGFILE.m_ProductDatabaseFile);
	}
	else
	{
		g_Statistics.SetProductDatabase(config.Get());
	}
	return result;
}


BOOL CEasyGraphApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt.  Ansonsten treten beim Erstellen von Fenstern Fehler auf.

	m_szCMDPath = m_pszHelpFilePath;
	VERIFY(ReadCMDPath(m_szCMDPath));

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	auto result = LoadInitFile();
	if (!result)
	{
		AfxMessageBox("Error loading ini-file!", MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}

	result = LoadLineGraphConfig();
	if (!result)
	{
		AfxMessageBox("Error loading LineConfig!", MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}

	result = LoadProductDatabase();
	if (!result)
	{
		AfxMessageBox("Error loading ProductDatabase!", MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}

	// OLE-Bibliotheken initialisieren
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();



	// display final splash screen and have it turn off after 3 seconds (3000 milliseconds)
	//CSplashWindow::ShowSplashScreen(NULL, "http://applehome.com/", 3000);

	EnableTaskbarInteraction();
#if 0
	CSplashWindow::ShowSplashScreen(NULL, "Starting application...");
	Sleep(1000);

	CSplashWindow::ShowSplashScreen(NULL, "Initializing hardware...");
	Sleep(1000);

	CSplashWindow::ShowSplashScreen(NULL, "Initializing database...");
	Sleep(1000);
#endif
	// AfxInitRichEdit2() ist für die Verwendung des RichEdit-Steuerelements erforderlich.
	// AfxInitRichEdit2();

	// Standardinitialisierung
	// Wenn Sie diese Funktionen nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));
	LoadStdProfileSettings();  // Standard INI-Dateioptionen laden (einschließlich MRU)
	SetRegistryBase(_T("Settings"));

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();

	CMFCToolTipInfo params;
	params.m_bVislManagerTheme = TRUE;

	GetTooltipManager()->SetTooltipParams(
		0xFFFF,
		RUNTIME_CLASS(CMFCToolTipCtrl),
		&params);


	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Dokumentvorlagen der Anwendung registrieren.  Dokumentvorlagen
	//  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEasyGraphDoc),
		RUNTIME_CLASS(CMainFrame), // Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CEasyGraphView));

	if (!pDocTemplate)
		return FALSE;

	AddDocTemplate(pDocTemplate);
	 
#if 0
	// Haupt-MDI-Rahmenfenster erstellen
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
#endif


	// Befehlszeile auf Standardumgebungsbefehle überprüfen, DDE, Datei öffnen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Verteilung der in der Befehlszeile angegebenen Befehle.  Gibt FALSE zurück, wenn
	// die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	if ( ! ProcessShellCommand(cmdInfo))
		return FALSE;

	// Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	CSplashWindow::HideSplashScreen();

	::PostMessage(m_pMainWnd->m_hWnd, WM_SETVIEW, WPARAM(0), LPARAM(0));

	return TRUE;
}

int CEasyGraphApp::ExitInstance()
{
	//TODO: Zusätzliche Ressourcen behandeln, die Sie möglicherweise hinzugefügt haben
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEasyGraphApp-Meldungshandler


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Anwendungsbefehl zum Ausführen des Dialogfelds
void CEasyGraphApp::OnAppAbout()
{
	CSplashWindow::ShowSplashScreen(GetMainWnd(), "http://applehome.com/", 10000);
#if 0
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
#endif
}

// CEasyGraphApp-Anpassungen – Methoden zum Laden/Speichern

void CEasyGraphApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CEasyGraphApp::LoadCustomState()
{
}

void CEasyGraphApp::SaveCustomState()
{
}

// CEasyGraphApp-Meldungshandler





BOOL CEasyGraphApp::PreTranslateMessage(MSG* pMsg)
{
#if 0
	if (CSplashWindow::PreTranslateAppMessage(pMsg))
		return TRUE;
#endif
	return CWinAppEx::PreTranslateMessage(pMsg);
}
