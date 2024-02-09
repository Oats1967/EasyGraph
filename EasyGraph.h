
// EasyGraph.h: Hauptheaderdatei für die EasyGraph-Anwendung
//
#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"       // Hauptsymbole
#include "global.h"



// CEasyGraphApp:
// Siehe EasyGraph.cpp für die Implementierung dieser Klasse
//

class CEasyGraphApp : public CWinAppEx
{
private:
	CString m_szCMDPath;
	base::CEasyGraphConfig m_SaveCfg;


private:
	BOOL LoadInitFile(void);
	BOOL LoadLineGraphConfig(void);
	BOOL LoadProductDatabase(void);
	BOOL LoadSettings(void);


public:
	CEasyGraphApp() noexcept;




// Überschreibungen
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementierung
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CEasyGraphApp theApp;
