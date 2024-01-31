
// MainFrm.h: Schnittstelle der CMainFrame-Klasse
//

#pragma once

//#include "FileView.h"
#include "CalendarBar.h"
#include "wmuser.h"
#include "PropertiesWnd.h"
#include "WorkspaceBar.h"
#include "Statistics.h"


#ifdef _BCGSUITE_INC_
#define CBCGPStatusBar CMFCStatusBar
#define CBCGPToolbarComboBoxButton CMFCToolBarComboBoxButton
#endif



class CMainFrame : public CBCGPMultiViewFrameWnd
{
public:
	CMainFrame() noexcept;

	DECLARE_DYNCREATE(CMainFrame)

// Attribute

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
	CWorkspaceBar		m_wndWorkSpace;
	// CMFCToolBarImages m_UserImages;
	//COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CCalendarBar      m_wndCalendarView;
	int32_t			  m_ActiveLine;

private:
	int				  m_nActiveLine = 0;
	int				  m_nActiveFeeder = 0;
	BOOL			  m_bIsDarkTheme = FALSE;


// Generierte Funktionen für die Meldungstabellen
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg void OnLineCombo();
	afx_msg void OnDoseSelectCombo();

	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	virtual void OnSelectView(int nView);


public:
	int	GetColorTheme() const
	{
		return m_nActiveLine;
	}

	BOOL IsDarkTheme() const
	{
		return m_bIsDarkTheme;
	}

	virtual CBCGPMultiViewsCollection& GetViewsCollection()
	{
		return m_wndWorkSpace;
	}


	CWorkspaceBar& GetWorkspace()
	{
		return m_wndWorkSpace;
	}

	LRESULT OnNewDate(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetView(WPARAM wParam, LPARAM lParam);

	CView* GetFeatureView(CDemoFeature* pFeature);


};


