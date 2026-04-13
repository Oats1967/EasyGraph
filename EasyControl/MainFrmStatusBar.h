//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MainFrmStatusBar
///
/// @file   MainFrmStatusBar.h
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

#include "EasyControl.h"
#include "RecorderStatus.h"
#include "TextProgressCtrl.h"

enum {
	MBC_ICONNORMAL,
	MBC_ICONCONNECT,
	MBC_ICONDISCONNECT,
	MBC_ICONINPUTOFFLINE,
	MBC_ICONMAX
};

enum {
	MBC_RECORDERDEACTIVE,
	MBC_RECORDERACTIVE,
	MBC_RECORDERERROR,
	MBC_RECORDERICONMAX
};


class CMainFrameStatusBar : public CMFCStatusBar
{
	CTextProgressCtrl		 m_Progress;
	CSliderCtrl				 m_ZoomCtrl;
	CFont					 m_StatusBarFont;
	HICON					 m_hTooltipIcon[MBC_ICONMAX];
	HICON					 m_hRecorderIcon[MBC_RECORDERICONMAX];
	CString					 m_RemoteIP;
	CString					 m_User;
	CString					 m_Company;
	CTime					 m_Time;
	uint32_t				 m_MaxItems;
	BOOL					 m_bProgressBarCreated;
	base::RemoteServerStatus m_RemoteState;
	eRecorderStatus			 m_RecorderState;

private:
	void SetRemoteIcon();
	void SetRecorderIcon();

public:
	CMainFrameStatusBar();

	// void OnUpdateStatusBar(CCmdUI* pCmdUI);
	BOOL Create(CWnd* pParent);

	void SetTime(const CTime& aTime);
	void SetUser(const CString& szUser);
	void SetCompany(const CString& szCompany);
	void SetMaxItems(const uint32_t count);
	void SetRemoteState(const base::RemoteServerStatus state);
	void SetRecorderState(const eRecorderStatus state);
	void SetRemoteIP(const CString& szRemoteIP);
	void Progress();
	void Resize();

	GET(const base::RemoteServerStatus, RemoteState)
	GET(const eRecorderStatus, RecorderState)

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

