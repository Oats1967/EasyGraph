//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MainFrm.h
///
/// @file   MainFrm.h
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

#include "ViewSelect.h"
#include "Recorder.h"
#include "MainFrmStatusBar.h"

class CToolBarEx : public CMFCToolBar
{
public:
	CToolBarEx()
	{
	}

	virtual BOOL LoadState(LPCTSTR /*lpszProfileName*/ = NULL, int /*nIndex*/ = -1, UINT /*uiID*/ = (UINT)-1) { return FALSE; }
	virtual BOOL SaveState(LPCTSTR /*lpszProfileName*/ = NULL, int /*nIndex*/ = -1, UINT /*uiID*/ = (UINT)-1) { return FALSE; }


};
class CMenuBarEx : public CMFCMenuBar
{
public:
	CMenuBarEx()
	{
	}

	virtual BOOL LoadState(LPCTSTR /*lpszProfileName*/ = NULL, int /*nIndex*/ = -1, UINT /*uiID*/ = (UINT)-1) { return FALSE; }
	virtual BOOL SaveState(LPCTSTR /*lpszProfileName*/ = NULL, int /*nIndex*/ = -1, UINT /*uiID*/ = (UINT)-1) { return FALSE; }


};




class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)


private:
	CMenuBarEx				m_wndMenuBar;
	CToolBarEx				m_wndMainBar;
	CToolBarEx				m_wndToolBar[cViewSelectMax];
	CMainFrameStatusBar		m_wndStatusBar;
	CRecorder				m_Recorder;
	BOOL					m_bRecipeNew;
	BOOL					m_bAllowUserProgramEnd;
	BOOL					m_bLastRecorderActive;
	UINT_PTR				m_aTimer;
	UINT_PTR				m_aLicenceTimer;
	int32_t					m_aPesaCount;
	BOOL					m_bViewInit = FALSE;
	eViewSelect				m_ActiveViewSelect = eViewSelect::eViewMax;
	CRect					m_StaticRect;

private:
	//------------------------------------------------------------------------------------
	///  @brief   SetUser
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetUser(void);

	//------------------------------------------------------------------------------------
	///  @brief   IsLocalDevice
	///
	///           This method ....
	///
	///  @param[in] szFile , LPCSTR
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL IsLocalDevice(LPCSTR szFile);
	CMultiDocTemplate* GetDocTemplate(eViewSelect _view);
	void SendToView(CMultiDocTemplate* pTemplate, int32_t id);
	void SendToView(eViewSelect, int32_t id);
	void ActivateViewSelect(eViewSelect);
	void SendMessageToActiveView(UINT message, WPARAM w = 0, LPARAM l = 0);
	void PostMessageToActiveView(UINT message, WPARAM w = 0, LPARAM l = 0);

	// void DestroyView(eViewSelect);
	CMDIChildWndEx* GetParentFrame(CMultiDocTemplate* pTemplate);
	CMDIChildWndEx* GetParentFrame(eViewSelect);


	BOOL LoginUserName();
	BOOL LoginNewPassword();
	BOOL LoginPassword();

	void FetchData();
	void TKTest();


	// Generierte Funktionen für die Meldungstabellen
protected:
	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @author  Detlef Hafer
	///
	///  @class        CMainFrame
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int
	///
	//------------------------------------------------------------------------------------
	int OnCreate(LPCREATESTRUCT lpCreateStruct);


	//------------------------------------------------------------------------------------
	///  @brief   PreCreateWindow
	///
	///           This method is called by the framework before the creation of the Windows 
	///		      window´attached to this CWnd object
	///
	///  @param[in] cs , CREATESTRUCT&
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	//------------------------------------------------------------------------------------
	///  @brief   ActivateDefault
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void ActivateDefault();

	void AlarmCheck(void);
	void WindowsShutDownCheck(void);
	void RemoteStatusbarCheck(void);
	void RecorderStatusbarCheck(void);


public:
	CMainFrame();
	~CMainFrame() override = default;

	void OpenTabs();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//------------------------------------------------------------------------------------
	///  @brief   OnDestroy
	///
	///           This method is called by the framework before the window is destroyed
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnDestroy();


	//------------------------------------------------------------------------------------
	///  @brief   OnUserLogin
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnUserLogin();
	afx_msg void OnStatusBarIPConnect();
	afx_msg void OnStatusBarRecorder();

	//------------------------------------------------------------------------------------
	///  @brief   OnOnUpdateUserLogin
	///
	///           This method ....
	///
	///  @param[in]  pCmdUI , CCmdUI *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnOnUpdateUserLogin(CCmdUI *pCmdUI);


	//------------------------------------------------------------------------------------
	///  @brief   OnUserLogout
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnUserLogout();


	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateUserLogout
	///
	///           This method ....
	///
	///  @param[in]  pCmdUI , CCmdUI *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnUpdateUserLogout(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnDoseEnd
	///
	///           This method ....
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnDoseEnd           (WPARAM w, LPARAM l = 0);

	//------------------------------------------------------------------------------------
	///  @brief   OnRecipeEnd
	///
	///           This method ....
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	//LRESULT OnRecipeEnd           (WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnProcessLineEnd
	///
	///           This method ....
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnProcessLineEnd           (WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnLayoutChanged
	///
	///           This method ....
	///
	///  @param[in] wparam , LPARAM
	///  @param[in] lparam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnLayoutChanged			(WPARAM, LPARAM);
	LRESULT OnLicenceCheck	        (WPARAM, LPARAM);
	LRESULT OnTimerRefresh			(WPARAM, LPARAM);
	LRESULT OnZoomFactor			(WPARAM, LPARAM);
	LRESULT OnPrepareNewLayout		(WPARAM w, LPARAM l);


	//------------------------------------------------------------------------------------
	///  @brief   OnActivateViewSelect
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnActivateViewSelect(WPARAM w = 0, LPARAM l = 0);
	LRESULT OnInfoBoxAlarmClear(WPARAM w, LPARAM);
	LRESULT OnConnectionLost(WPARAM, LPARAM);
	LRESULT OnRecorderError(WPARAM, LPARAM);



//------------------------------------------------------------------------------------
	///  @brief   OnKeyboard
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnKeyboard();


	//------------------------------------------------------------------------------------
	///  @brief   OnClose
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnClose();

	//------------------------------------------------------------------------------------
	///  @brief   OnSystemSettings
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnSystemSettings();


	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateSystemSettings
	///
	///           This method ....
	///
	///  @param[in]  pCmdUI , CCmdUI *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnUpdateSystemSettings(CCmdUI *pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnTimer
	///
	///           This method ....
	///
	///  @param[in] nIDEvent , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	//------------------------------------------------------------------------------------
	afx_msg void OnApplicationClose();
	afx_msg void OnUpdateApplicationClose(CCmdUI* pCmdUI);



	//------------------------------------------------------------------------------------
	///  @brief   OnSize
	///
	///           This method ....
	///
	///  @param[in] cy , int
	///  @param[in] cx , int
	///  @param[in] nType , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);


	CToolBarEx& GetToolBar(eViewSelect iSelect)
	{	return m_wndToolBar[_S32(iSelect)];	}


	DECLARE_MESSAGE_MAP()
};



