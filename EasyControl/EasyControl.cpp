//------------------------------------------------------------------------------------
///
/// @brief  Implementation of module EasyControl
///
/// @file   EasyControl.cpp
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
#include "Shutdown.h"
#include <direct.h>
#include <map>
#include "EasyControl.h"
#include "EasyControlVer.h"
#include <locale.h>
#include <time.h>
#include "MainFrm.h"
#include "ChildFrm.h"
#include "EasyControlDoc.h"
#include "EasyControlView.h"
#include "remote/include/RemoteInterface.h"
#include "splash.h"
#include "FileHelper.h"

#include "AlarmArchiveView.h"
#include "AlarmArchiveDoc.h"
#include "AlarmArchiveFrame.h"
#include "AlarmLog.h"

#include "DiagramFrame.h"
#include "DiagramDoc.h"
#include "DiagramView.h"

#include "RecipeFrame.h"
#include "RecipeDoc.h"
#include "RecipeView.h"
#include "DoseServer.h"

#include "Xlicense/LicenseLib/License.h"
#include "Xlicense/LicenseLib/hostid.h"
#include "LoadResource.h"
#include "BASE/Utils/public/xml/EasyControlConfigXml.h"
#include "BASE/Utils/public/xml/RecorderConfigXml.h"
#include "BASE/Utils/public/xml/RecorderDescriptionXml.h"
#include "BASE/Utils/public/xml/LayoutConfigXml.h"
#include "BASE/Utils/public/xml/DiagramConfigXml.h"
#include "BASE/Utils/public/xml/IOConfigXml.h"
#include "BASE/Utils/public/xml/DoseServerXml.h"
#include "BASE/Utils/public/xml/ProductDatabaseXml.h"
#include "LogSave.h"

#ifdef __TKBOARD
#include "TKBoard64.h"
#else
#include "VKBoard64.h"
#endif
#include "RemoteControl.h"
   
#define EASYCONTROLCONFIGFILE "EasyControl.xml"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define MAKEPATH(a,b) a << "\\" << b

#define XMLEXT ".XML"
#define DBEXT  ".DB"
#define LOGEXT ".log"
#define RECEXT ".rec"
#define TEMPRECORDFOLDER "C:\\temp\\_EasyControl\\recorder"
#define TEMPLOGFOLDER "C:\\temp\\_EasyControl\\log"


// CEasyControlApp
BEGIN_MESSAGE_MAP(CEasyControlApp, CWinAppEx)
    //	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Dateibasierte Standarddokumentbefehle
	ON_COMMAND(ID_FILE_NEW, CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


time_t CEasyControlApp::m_ProductDatabaseLastModifiedFileTime = 0;

CEasyControlApp::CEasyControlApp() : CWinAppEx()
	, m_hIcon(HICON(0))
	, m_szCMDPath{}
	, m_RemoteIP{}
	, pDocAnlagenTemplate(nullptr)
	, pDiagramTemplate(nullptr)
	, pLogTemplate(nullptr)
	, pRezeptTemplate(nullptr)
	, m_bClose{ FALSE }
	, m_TempRecorderFolder { TEMPRECORDFOLDER }
	, m_TempLogFolder	   { TEMPLOGFOLDER }
	, m_hMutex{ NULL }
{}


CEasyControlApp::~CEasyControlApp()
{
	BCGCBProCleanUp();
}
 
// Das einzige CEasyControlApp-Objekt
CEasyControlApp theApp;

//**************************************************************************************************************
//**************************************************************************************************************
template < class T>
static BOOL IdentFunc(const std::string& srcfile, const std::string& destfile)
{
	auto bLoadLambda = [](T& list, const std::string& szFile) {
		list.SetPath(CFileHelper::GetPath(szFile));
		list.SetFilename(CFileHelper::GetFileName(szFile));
		auto result = list.LoadAll();
		if (!result)
		{
			LOGERROR("Error loading " << szFile);
		}
		return result;
	};

	T src;
	auto result = bLoadLambda(src, srcfile);
	if (result)
	{
		T dest;
		result = bLoadLambda(dest, destfile);
		if (result)
		{
			T merge;
			merge.Merge(src, dest);
			result = merge.Save(destfile);
			if (!result)
			{
				LOGERROR("Error saving " << destfile);
			}
		}
	}
	return result;
}
//------------------------------------------------------------------------------------
///  @brief   ReadSystemPath
///
///            This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] pszHelpFilePathof type
///  @param[in] iLicence       of type
///  @return BOOL
///
//------------------------------------------------------------------------------------
static BOOL ReadSystemPath (const std::string& pszHelpFilePath, const LICENCETYPE iLicence)
{
	time_t aTime = time(NULL);
	auto iErg = license :: CheckLicenceFile	(pszHelpFilePath, iLicence, aTime);
	if ( iErg != L_OK)
	{
		return FALSE;
	}
	return TRUE;
}

//------------------------------------------------------------------------------------
///  @brief   ReadLicence
///
///           This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
BOOL ReadLicence ( void )
{
	CEasyControlApp *pApp = (CEasyControlApp*) AfxGetApp();
	return pApp->CheckLicence ();
}



//------------------------------------------------------------------------------------
///  @brief   CheckLicence
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CEasyControlApp
///
///  @param[in] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
BOOL CEasyControlApp :: CheckLicence ( void )
{
	if (!ReadSystemPath(m_szCMDPath, E_EASYCONTROL))
	{
		PostQuitMessage(-1);
		return FALSE;
	}
	return TRUE;
 }
//************************************************************************************************************************
//************************************************************************************************************************
BOOL CEasyControlApp::IsLocalDrive(const CString& szFolder)
{
	return CFileHelper::IsLocalDrive(toStdString(szFolder));
}
//************************************************************************************************************************
//************************************************************************************************************************
BOOL CEasyControlApp :: LoadModule ( void )
{
	const std::map< base::eLanguage, int32_t> clangmap = { { base::eLanguage::eGERMAN, LANGUAGE_GERMAN },
															   { base::eLanguage::eENGLISH, LANGUAGE_ENGLISH },
															   { base::eLanguage::eFRENCH, LANGUAGE_FRENCH },
															   { base::eLanguage::eITALIAN, LANGUAGE_ITALIAN },
															   { base::eLanguage::eDUTCH, LANGUAGE_DUTCH },
															   { base::eLanguage::ePOLISH, LANGUAGE_POLISH },
															   { base::eLanguage::eTURKISH, LANGUAGE_TURKISH },
															   { base::eLanguage::eSPANISH, LANGUAGE_SPANISH },
															   { base::eLanguage::eRUSSIAN, LANGUAGE_RUSSIAN },
															   { base::eLanguage::eCHINESE, LANGUAGE_CHINESE },
															   { base::eLanguage::eUKRAINIAN, LANGUAGE_UKRAINIAN },
															   { base::eLanguage::eHUNGARIAN, LANGUAGE_HUNGARIAN },
															   { base::eLanguage::eGREEK, LANGUAGE_GREEK }
	};

	ASSERT(clangmap.size() == _S32(base::eLanguage::eMAXLANGUAGE));
	BOOL result = FALSE;
	const auto it = clangmap.find(EASYCONTROLREF.m_Language);
	if (it != clangmap.cend())
	{
		auto lLanguageID = it->second;
		HINSTANCE aResInstance = LoadResourceDLL(LANGID(lLanguageID), toCString(m_szCMDPath));
		ASSERT(aResInstance != 0);
		AfxSetResourceHandle(aResInstance);
		//BCGCBProSetResourceHandle(aResInstance);
		result = BOOL(aResInstance != 0);
	}
	return result;
}
//************************************************************************************************************************
//************************************************************************************************************************
BOOL CEasyControlApp::LoadLogItem(void)
{
	const std::string szExt{ LOGEXT };

	assert(! EASYCONTROLREF.m_AlarmLogPath.empty());
	CFileHelper::CreateFolder(m_TempLogFolder);
	CFileHelper::MoveFilesToFolder(m_TempLogFolder, EASYCONTROLREF.m_AlarmLogPath, szExt, IdentFunc<base::utils::CLogItemList>);

	if (CFileHelper::IsLocalDrive(EASYCONTROLREF.m_AlarmLogPath))
	{
		CFileHelper::ClearOldFiles(EASYCONTROLREF.m_AlarmLogPath, szExt);
	}
	LOGREF.SetPath(EASYCONTROLREF.m_AlarmLogPath);
	LOGREF.SetFilename();
	LOGDEBUG("Reading Logitems, " << MAKEPATH( EASYCONTROLREF.m_AlarmLogPath, LOGREF.GetFilename()));
	BOOL result = LOGREF.LoadAll();
	if (! result)
	{
		LOGERROR("Error reading Logitems! " << MAKEPATH(EASYCONTROLREF.m_AlarmLogPath, LOGREF.GetFilename()));
	}
	else
	{
		LOGREF.CheckAllItems();
	}
	return result;
}
//************************************************************************************************************************
//************************************************************************************************************************
BOOL CEasyControlApp::LoadProductDatabase(void)
{
	BOOL  result = FALSE;

	const auto& rFile = EASYCONTROLREF.m_ProductDatabaseFile;
	ASSERT( !rFile.empty());
	if (rFile.empty())
	{
		LOGERROR("Productdatabase-file empty " << rFile);
	}
	else
	{
		struct stat buffer;
		BOOL fileexists = stat(rFile.c_str(), &buffer) == 0;
		if (!fileexists)
		{
			LOGERROR("Productdatabase-file does not exist ... overwriting " << rFile);
		}
		else
		{
			if ( buffer.st_mtime != m_ProductDatabaseLastModifiedFileTime )
			{
				m_ProductDatabaseLastModifiedFileTime = buffer.st_mtime;
				LOGDEBUG("Reading ProductDatabase, " << rFile);
				auto bXMLFile = CFileHelper::IsFileExtension(rFile, XMLEXT);
				if (bXMLFile)
				{
					LOGDEBUG("Reading ProductDatabase-XML-File, " << rFile);
					base::xml::CProductDatabaseXml config;
					result = config.Load(rFile);
					if ( ! result)
					{
						LOGERROR("Error reading ProductDatabase-XML-File, " << rFile);
					}
					else
					{
						auto& rData = config.Get().getMap();
						if ( rData.empty())
						{
							LOGERROR("Error ProductDatabase is empty, " << rFile);
							result = FALSE;
						}
						else
						{
							PRODUCTDATABASEREF.Clear();
							for (const auto& rItem : rData)
							{
								PRODUCTDATABASEREF.AddItem(base::utils::CProductItem{ -1, rItem.second, rItem.first });
							}
						}
					}
				}
				else 
				{
					auto bDBFile = CFileHelper::IsFileExtension(rFile, DBEXT);
					if (bDBFile)
					{
						result = PRODUCTDATABASEREF.Load(rFile);
						if ( ! result)
						{
							LOGERROR("Error reading ProductDatabase-DB, " << rFile);
						}
					}
				}
				if ( result )
				{
					PRODUCTDATABASEREF.SetChanged(FALSE);
					m_ProductDatabaseLastModifiedFileTime = buffer.st_mtime;
				}
			}
		}
	}
	return result;
}

//*************************************************************************************************
//*************************************************************************************************
BOOL CEasyControlApp::SaveProductDatabase(void)
{
	BOOL result = FALSE;
	const auto& rFile = EASYCONTROLREF.m_ProductDatabaseFile;
	ASSERT(!rFile.empty());
	if (rFile.empty())
	{
		LOGERROR("Productdatabase-file empty " << rFile);
		result = FALSE;
	}
	else if ( ! PRODUCTDATABASEREF.IsChanged())
	{
		result = TRUE;
	}
	else
	{
		struct stat buffer;
		BOOL fileexists = ( stat(rFile.c_str(), &buffer) == 0);
		if ( ! fileexists)
		{
			LOGERROR("Productdatabase-file does not exist ... creating " << rFile);
		}
		else
		{
			LOGDEBUG("Overwriting Productdatabase-file " << rFile);
		}
		auto bXMLFile = CFileHelper::IsFileExtension(rFile, XMLEXT);
		if (bXMLFile)
		{
			LOGDEBUG("Saving ProductDatabase-XML-File, " << rFile);
			base::xml::CProductDatabaseXml config;
			auto& rData = config.Get();
			auto count = PRODUCTDATABASEREF.GetCount();
			if (count == 0)
			{
				LOGERROR("ProductDatabase is empty ... nothing to save, " << rFile);
				result = FALSE;
			}
			else
			{
				for (uint32_t index = 0; index < count; index++)
				{
					const auto& rItem = PRODUCTDATABASEREF.GetItem(index);
					rData.insert({ rItem.GetQMNumber(), rItem.GetProductName() });
				}
				result = config.Save(rFile);
				if ( ! result)
				{
					LOGERROR("ProductDatabase-XML-File ... error saving " << rFile);
				}
			}
		}
		else
		{
			auto bDBFile = CFileHelper::IsFileExtension(rFile, DBEXT);
			if (bDBFile)
			{
				LOGDEBUG("Saving ProductDatabase-DB-File, " << rFile);
				result = PRODUCTDATABASEREF.Save(rFile);
				if ( ! result)
				{
					LOGERROR("ProductDatabase-DB-File ... error saving " << rFile);
				}
			}
		}
		if (result)
		{
			PRODUCTDATABASEREF.SetChanged(FALSE);
			m_ProductDatabaseLastModifiedFileTime = buffer.st_mtime;
		}
	}
	return result;
}
//************************************************************************************************************************
//************************************************************************************************************************
BOOL CEasyControlApp::LoadLayoutConfig(void)
{
	BOOL  result = FALSE;
	const auto& rFile = EASYCONTROLREF.m_LayoutConfigFile;

	ASSERT(!rFile.empty());
	if (rFile.empty())
	{
		LOGERROR("xml-file empty " << rFile);
	}
	else
	{
		LOGDEBUG("Reading LayoutConfig, " << rFile);
		base::xml::CLayoutConfigXml config;
		result = config.Load(rFile);
		if (!result)
		{
			LOGERROR("Error reading LayoutConfig, " << rFile);
		}
		else
		{
			LAYOUTCONFIGREF = config.Get();
		}
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadRecorderConfig(void)
{
	BOOL  result = FALSE;
	const std::string szExt{ RECEXT };

	const auto& rFile = EASYCONTROLREF.m_RecorderConfigFile;
	ASSERT(!rFile.empty());
	if (rFile.empty())
	{
		LOGERROR("xml-file empty " << rFile);
	}
	else
	{
		// Loading recorder xml
		LOGDEBUG("Reading recorder items, " << rFile);
		base::xml::CRecorderConfigXml config;
		result = config.Load(rFile);
		if ( ! result)
		{
			LOGERROR("Error reading RecorderConfig, " << rFile);
		}
		else
		{
			m_SaveRecorderCfg = RECORDERCONFIGREF = config.Get();
			ASSERT(!RECORDERCONFIGREF.m_RecorderPath.empty());

			CFileHelper::CreateFolder(m_TempRecorderFolder);
			CFileHelper::MoveFilesToFolder(m_TempRecorderFolder, RECORDERCONFIGREF.m_RecorderPath, szExt, IdentFunc<base::utils::CRecItemList>);
			if (CFileHelper::IsLocalDrive (RECORDERCONFIGREF.m_RecorderPath) )
			{
				CFileHelper::ClearOldFiles(RECORDERCONFIGREF.m_RecorderPath, szExt);
			}
			RECORDERREF.SetPath(RECORDERCONFIGREF.m_RecorderPath);
			RECORDERREF.CreateFilename();
			auto erg = RECORDERREF.LoadAll();
			if ( ! erg)
			{
				LOGERROR("Recorder file cannot be loaded ! " << MAKEPATH(RECORDERCONFIGREF.m_RecorderPath, RECORDERREF.GetFilename()));
			}
		}
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::SaveRecorderConfig(void)
{
	// Loading recorder xml
	BOOL result = FALSE;
	if (m_SaveRecorderCfg != RECORDERCONFIGREF)
	{
		const auto& rFile = EASYCONTROLREF.m_RecorderConfigFile;
		ASSERT( ! rFile.empty());
		if ( rFile.empty() )
		{
			LOGERROR("xml-file empty " << rFile);
		}
		else
		{
			LOGDEBUG("Saving recorder items, " << rFile);
			CLogSave < base::xml::CRecorderConfigXml > aFile;
			auto& ref = aFile.Get();
			ref.Set(RECORDERCONFIGREF);
			result = aFile.Save(toCString(rFile));
			ASSERT(result);
			if ( ! result)
			{
				LOGERROR("Error saving RecorderConfig, " << rFile);
			}
		}
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadRecorderDescription(void)
{
	BOOL  result = FALSE;
	const auto& rFile = EASYCONTROLREF.m_RecorderDescriptionFile;

	ASSERT(!rFile.empty());
	if (rFile.empty())
	{
		LOGERROR("xml-file empty " << rFile);
	}
	else
	{
		// Loading recorder xml
		LOGDEBUG("Reading recorder description, " << rFile);
		base::xml::CRecorderDescriptionXml config;
		result = config.Load(rFile);
		if (!result)
		{
			LOGERROR("Error reading RecorderDescription, " << rFile);
		}
		else
		{
			base::utils::CRecItem::SetDescriptor(config.Get());
		}
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadDiagramConfig(void)
{
	BOOL result = FALSE;

	const auto& rFile = EASYCONTROLREF.m_DiagramConfigFile;
	ASSERT( ! rFile.empty() );
	if ( rFile.empty() )
	{
		LOGERROR("Error reading DiagramConfig ! Path is empty !");
	}
	else
	{
		LOGDEBUG("Reading DiagramConfig, " << rFile);
		base::xml::CDiagramConfigXml config;
		result = config.Load(rFile);
		if ( ! result)
		{
			LOGERROR("Error reading DiagramConfig, " << rFile);
		}
		else
		{
			m_SaveDiagramCfg = DIAGRAMCONFIGREF = config.Get();
		}
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::SaveDiagramConfig(void)
{
	BOOL result = FALSE;
	if (m_SaveDiagramCfg != DIAGRAMCONFIGREF)
	{
		const auto& rFile = EASYCONTROLREF.m_DiagramConfigFile;
		ASSERT( ! rFile.empty() ); 
		if ( rFile.empty() )
		{
			LOGERROR("Error writing DiagramConfig ! Path is empty !");
		}
		else
		{
			LOGDEBUG("Saving DiagramConfig, " << rFile);
			CLogSave < base::xml::CDiagramConfigXml > aFile;
			auto& ref = aFile.Get();
			ref.Set(DIAGRAMCONFIGREF);
			result = aFile.Save(toCString(rFile));
			ASSERT(result);
			if ( ! result)
			{
				LOGERROR("Error saving DiagramConfig, " << rFile);
			}
		}
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadUserAccounts(void)
{
	// Loading useraccounts ....
	LOGDEBUG("Reading User accounts, " << EASYCONTROLREF.m_UserConfigFile);
#if 0
	USERRIGHTSREF.Create();
	BOOL result = USERRIGHTSREF.Save(EASYCONTROLREF.m_UserConfigFile);
#else
	BOOL result = USERRIGHTSREF.Load(EASYCONTROLREF.m_UserConfigFile);
#endif
	if (!result)
	{
		LOGERROR("Error reading user accounts, " << EASYCONTROLREF.m_UserConfigFile);
	}
#ifdef _DEBUG
	USERRIGHTSREF.SetAktUser("smadotec");
#else
	auto pUser = USERRIGHTSREF.GetStandardUser();
	if (pUser)
	{
		USERRIGHTSREF.SetAktUser(pUser->GetUsername());
	}
#endif
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CEasyControlApp::DoseServerMessage(ErrorCodes id)
{
	if (id != ErrorCodes::OK)
	{
		const std::map< ErrorCodes, UINT> c_Errormap
		{
			{ ErrorCodes::Xml_DoseServer_LoadError, IDS_ERRORCODES_XMLDOSESERVER_LOADERROR },
			{ ErrorCodes::Xml_Logo_LoadError, IDS_ERRORCODES_XMLLOGO_LOADERROR },
			{ ErrorCodes::Xml_SystemConfig_LoadError, IDS_ERRORCODES_XMSYSTEMCONFIG_LOADERROR },
			{ ErrorCodes::RemoteServer_InitError, IDS_ERRORCODES_REMOTESERVER_INITERROR },
			{ ErrorCodes::RemoteServer_OpenError, IDS_ERRORCODES_REMOTESERVER_OPENERROR },
			{ ErrorCodes::DoseData_NotOpen, IDS_ERRORCODES_DOSEDATA_OPENERROR },
			{ ErrorCodes::DoseData_SaveError, IDS_ERRORCODES_DOSEDATA_SAVEERROR },
			{ ErrorCodes::DoseData_LoadError, IDS_ERRORCODES_DOSEDATA_LOADERROR },
			{ ErrorCodes::DoseData_CrcError, IDS_ERRORCODES_DOSEDATA_CRCERROR },
			{ ErrorCodes::DoseData_InvalidLength, IDS_ERRORCODES_DOSEDATA_INVALIDLENGTH },
		};
		auto sid = c_Errormap.find(id);
		if (sid != c_Errormap.cend())
		{
			CString szTemp;
			VERIFY(szTemp.LoadString(sid->second));
			AfxMessageBox(szTemp);
		}
	}
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadInitFile()
{
	const std::string xmlfile{ EASYCONTROLCONFIGFILE };
	base::xml::CEasyControlConfigXml myxml;
	auto result = myxml.Load(m_szCMDPath + "\\" + xmlfile);
	ASSERT(result);
	if (result)
	{
		m_SaveCfg = EASYCONTROLREF = myxml.Get();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::SaveInitFile()
{  
	BOOL result = FALSE;
	if (m_SaveCfg != EASYCONTROLREF)
	{
		const std::string xmlfile{ EASYCONTROLCONFIGFILE };
		const std::string xmlfilePath = m_szCMDPath + "\\" + xmlfile;

		CLogSave< base::xml::CEasyControlConfigXml>  aFile;
		auto& ref = aFile.Get();
		ref.Set(EASYCONTROLREF);
		result = aFile.Save(toCString(xmlfilePath));
		ASSERT(result);
		if ( ! result)
		{
			LOGERROR("Error saving Ini-file, " << xmlfilePath);
		}

	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::LoadApplication()
{
#ifndef _DEBUG
	if (!ReadLicence())
	{
		AfxMessageBox(_T("Invalid license!"), MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}
#endif
	(void)LoadProductDatabase();
	(void)LoadLayoutConfig();
	(void)LoadLogItem();
	(void)LoadDiagramConfig();

	auto result = LoadRecorderDescription();
	if (!result)
	{
		AfxMessageBox(_T("Error loading RecorderDescription!"), MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	result = LoadRecorderConfig();
	if (!result)
	{
		AfxMessageBox(_T("Error loading RecorderConfig!"), MB_ICONSTOP | MB_OK);
		return FALSE;
	}

	// Loading useraccounts ....
	result = LoadUserAccounts();
	if (!result)
	{
		AfxMessageBox(_T("Userconfig cannot be loaded!"), MB_ICONSTOP | MB_OK);
		return FALSE;
	}
	IN_ADDR	localIP;
	localIP.S_un.S_addr = GetHostID();
	m_RemoteIP.Format(_T("%u.%u.%u.%u"), localIP.s_net, localIP.s_host, localIP.s_lh, localIP.s_impno);

	GetDoseServerInterface(&g_DoseServerInterface);
	result = REFDOSESERVER.Open(EASYCONTROLREF.m_DoseServerFile);
	DoseServerMessage(REFDOSESERVER.GetLastErrorCode());
	if (!result)
	{
		AfxMessageBox(_T("Doseserver cannot be started !"), MB_ICONSTOP | MB_OK);
		return FALSE;
	}
	SYSTEMCONFIGREF = REFDOSESERVER.GetSystemConfig();
	g_RemoteInterface = REFDOSESERVER.GetRemoteInterface();
	g_RemoteServerInterface = REFDOSESERVER.GetRemoteServerInterface();
	(void)REFDOSESERVER.Start();
	return TRUE;
}

//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::InitInstance()
{
	// Mutex erstellen
	m_hMutex = CreateMutex(NULL, FALSE, m_strMutexName);

	if (m_hMutex == NULL)
	{
		// Fehler beim Erstellen des Mutex
		AfxMessageBox(_T("Error creating mutex!"));
		return FALSE;
	}

	// Prüfen, ob der Mutex bereits existiert
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		// Mutex existiert bereits, also beenden
		AfxMessageBox(_T("Application already running!"));
		CloseHandle(m_hMutex);
		return FALSE;
	}

	CWinAppEx::InitInstance();

	//---------------------------------------------------------------------------------------------------
	m_szCMDPath = toStdString(m_pszHelpFilePath);
	CFileHelper::ReadCMDPath(m_szCMDPath);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	auto result = LoadInitFile();
	if ( ! result )
	{
		AfxMessageBox(_T("Error loading ini-file!"), MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}
	result = LoadModule();
	if (!result)
	{
		AfxMessageBox(_T("No resource file found!"), MB_ICONSTOP | MB_ICONSTOP);
		return FALSE;
	}
	result = LoadApplication();
	if (!result)
	{
		return FALSE;
	}

	CStartupWnd::EnableSplashScreen(TRUE);
	CMainFrame* pMainFrame = new CMainFrame;
	CStartupWnd::ShowSplashScreen(pMainFrame);

#ifdef __TKBOARD
	// Loading touchkeyboard ....
	(void)GetTKBoard64Interface(&g_TKBoard);
#else
	// Loading touchkeyboard ....
	(void)GetVKBoard64Interface(&g_TKBoard);
#endif
	TKB.TK_SetLanguage(EASYCONTROLREF.m_Language);
	auto ret = TKB.TK_Open(EASYCONTROLREF.m_TouckKeyboardConfigFile);
	if (ret != ITKBoardInterface::TK_OK)
	{
		AfxMessageBox(_T("Touchkeyboard cannot be loaded!"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	//! Standardinitialisierung
	//! Wenn Sie diese Features nicht verwenden und die Größe
	//! der ausführbaren Datei verringern möchten, entfernen Sie
	//! die nicht erforderlichen Initialisierungsroutinen.
	//! Ändern Sie den Registrierungsschlüssel unter dem Ihre Einstellungen gespeichert sind.
	//! TODO: Ändern Sie diese Zeichenfolge entsprechend,
	//! z.B. zum Namen Ihrer Firma oder Organisation.
	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));
	LoadStdProfileSettings(4);  //! Standard INI-Dateioptionen laden (einschließlich MRU)
	//! Dokumentvorlagen der Anwendung registrieren. Dokumentvorlagen
	//!  dienen als Verbindung zwischen Dokumenten, Rahmenfenstern und Ansichten.
	//! 
	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();

	CMultiDocTemplate* pDocTemplate;
	pDocAnlagenTemplate = pDocTemplate = new CMultiDocTemplate(IDR_OVERVIEWFRAME,
		RUNTIME_CLASS(CEasyControlDoc),
		RUNTIME_CLASS(CChildFrame), //! Benutzerspezifischer MDI-Child-Rahmen
		RUNTIME_CLASS(CEasyControlView));
	AddDocTemplate(pDocAnlagenTemplate);
	pLogTemplate = new CMultiDocTemplate(
		IDR_ALARMARCHIVEFRAME,
		RUNTIME_CLASS(CAlarmArchiveDoc),
		RUNTIME_CLASS(CAlarmArchiveFrame), //! custom MDI child frame
		RUNTIME_CLASS(CAlarmArchiveView));
	AddDocTemplate(pLogTemplate);
	pDiagramTemplate = new CMultiDocTemplate(IDR_DIAGRAMFRAME,
		RUNTIME_CLASS(CDiagramDoc),
		RUNTIME_CLASS(CDiagramFrame), // custom MDI child frame
		RUNTIME_CLASS(CDiagramView));
	AddDocTemplate(pDiagramTemplate);
	pRezeptTemplate = new CMultiDocTemplate(IDR_RECIPEFRAME,
		RUNTIME_CLASS(CRecipeDoc),
		RUNTIME_CLASS(CRecipeFrame), // custom MDI child frame
		RUNTIME_CLASS(CRecipeView));
	AddDocTemplate(pRezeptTemplate);
	Sleep(1000);
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMainWnd->SetIcon(m_hIcon, FALSE);

	//! Rufen Sie DragAcceptFiles nur auf, wenn eine Suffix vorhanden ist.
	//!  In einer MDI-Anwendung ist dies unmittelbar nach dem Festlegen von m_pMainWnd erforderlich
	//! Befehlszeile parsen, um zu prüfen auf Standardumgebungsbefehle DDE, Datei offen
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	//! Verteilung der in der Befehlszeile angegebenen Befehle. Es wird FALSE zurückgegeben, wenn
	//! die Anwendung mit /RegServer, /Register, /Unregserver oder /Unregister gestartet wurde.
	//! Das Hauptfenster ist initialisiert und kann jetzt angezeigt und aktualisiert werden.
	//! 	ShowWindow();
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();
	pMainFrame->OpenTabs();
	return TRUE;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CEasyControlApp::ShutDown()
{
	static BOOL bShutdown = FALSE;

	if (bShutdown)
	{
		return;
	}
	if (g_DoseServerInterface)
	{
		REFDOSESERVER.Close();
		g_DoseServerInterface = nullptr;
	}
	(void)ReleaseDoseServerInterface();

	LOGREF.SaveAll();

	SaveRecorderConfig();
	SaveDiagramConfig();
	SaveInitFile();

	if (g_TKBoard)
	{
		TKB.TK_Close();
		g_TKBoard = nullptr;
	}
#ifdef __TKBOARD
	(void)ReleaseTKBoard64Interface();
#else
	(void)ReleaseVKBoard64Interface();
#endif
	bShutdown = TRUE;
}
//**************************************************************************************************************
//**************************************************************************************************************
int CEasyControlApp::ExitInstance()
{
	// Mutex freigeben
	if (m_hMutex != NULL)
	{
		ReleaseMutex(m_hMutex);
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
	ShutDown();
	BCGCBProCleanUp();
	auto result = CWinAppEx::ExitInstance();
	if (m_bShutdown)
	{
		MySystemShutdown();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CEasyControlApp::SaveAllModified()
{
	if (! m_bClose)
	{
		AfxGetMainWnd()->SendMessage(WM_DOSEEND, 0, -1);
	}
#if 0  // def _DEBUG
	AfxMessageBox("Save all modified files!", MB_OK | MB_ICONSTOP);
#endif
	ShutDown();
	return TRUE;
}



