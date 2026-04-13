//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControl.h
///
/// @file   EasyControl.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "EasyControl_DEU/resource.h"
#include "wmuser.h"
#include "EasyControlCountry.h"
#include "RemoteControl.h"
#include "ViewSelect.h"
#include "ErrorCodes.h"


enum class eLC_Address : uint32_t
{
	eLC_Address_Feeder = 0,
	eLC_Address_Refill = 0,
};


class CEasyControlApp : public CWinAppEx
{
private:
	HICON   m_hIcon;
	CString m_RemoteIP;
	std::string m_szCMDPath;
	const std::string m_TempRecorderFolder;
	const std::string m_TempLogFolder;

	base::CEasyControlConfig m_SaveCfg;
	base::CDiagramConfig m_SaveDiagramCfg;
	base::CRecorderConfig m_SaveRecorderCfg;

	static time_t m_ProductDatabaseLastModifiedFileTime;

public:
	CMultiDocTemplate* pDocAnlagenTemplate;
	CMultiDocTemplate* pDiagramTemplate;
	CMultiDocTemplate* pLogTemplate;
	CMultiDocTemplate* pRezeptTemplate;

	BOOL m_bClose	 = FALSE;
	BOOL m_bShutdown = FALSE;

	HANDLE m_hMutex;
	const CString m_strMutexName = _T("EasyControlMutex"); // Eindeutiger Name

private:
	//------------------------------------------------------------------------------------
	///  @brief   LoadModule
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL LoadModule ( void );

	//------------------------------------------------------------------------------------
	///  @brief   LoadLogItem
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------

	void DoseServerMessage(ErrorCodes id);
	BOOL LoadApplication();
	BOOL LoadInitFile();
	BOOL SaveInitFile();



	//------------------------------------------------------------------------------------
	///  @brief   LoadrecorderConfig
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL LoadLogItem(void);
	BOOL LoadRecorderConfig(void);
	BOOL SaveRecorderConfig(void);
	BOOL LoadRecorderDescription(void);
	BOOL LoadDiagramConfig(void);
	BOOL SaveDiagramConfig(void);
	//------------------------------------------------------------------------------------
	///  @brief   LoadSystemConfig
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	//static BOOL LoadSystemConfig(void);
	//------------------------------------------------------------------------------------
	///  @brief   LoadLayoutConfig
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	static BOOL LoadLayoutConfig(void);
	static BOOL LoadUserAccounts(void);

// Überschreibungen
public:
	CEasyControlApp();
	~CEasyControlApp();

	BOOL CheckLicence(void);

	//------------------------------------------------------------------------------------
	///  @brief   InitInstance
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL InitInstance() override;

	//------------------------------------------------------------------------------------
	///  @brief   ExitInstance
	///
	///           This method ....
	///
	///  @author  Detlef Hafer
	///
	///  @class        CEasyControlApp
	///
	///  @param[in] none
	///  @return int
	///
	//------------------------------------------------------------------------------------
	int ExitInstance() override;

	BOOL SaveAllModified() override;

	void ShutDown();

	static BOOL IsLocalDrive(const CString&);
	static BOOL LoadProductDatabase(void);

	static BOOL SaveProductDatabase(void);

	GET(const std::string&, TempRecorderFolder);
	GET(const std::string&, TempLogFolder);

	SETGET(const CString&, RemoteIP);

	// Implementierung
	DECLARE_MESSAGE_MAP()
};

//extern int lDosierTitleID[];
extern BOOL ReadLicence	( void );


extern CEasyControlApp theApp;

