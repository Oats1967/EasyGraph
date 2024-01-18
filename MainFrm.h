
// MainFrm.h: Schnittstelle der CMainFrame-Klasse
//

#pragma once
#include "FileView.h"
#include "CalendarBar.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"


#ifdef _BCGSUITE_INC_
#define CBCGPStatusBar CMFCStatusBar
#define CBCGPToolbarComboBoxButton CMFCToolBarComboBoxButton
#endif

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// Attribute
public:

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// Implementierung
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Eingebettete Member der Steuerleiste
	//CMFCMenuBar       m_wndMenuBar;
	CMFCStatusBar     m_wndStatusBar;
	CBCGPExplorerToolBar       m_wndToolBar;
	// CMFCToolBarImages m_UserImages;
	CFileView         m_wndDoseSelectView;
	//COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CCalendarBar      m_wndCalendarView;

private:
	int				  m_nColorTheme = 0;
	BOOL			  m_bIsDarkTheme = FALSE;


// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	int	GetColorTheme() const
	{
		return m_nColorTheme;
	}

	BOOL IsDarkTheme() const
	{
		return m_bIsDarkTheme;
	}

};


