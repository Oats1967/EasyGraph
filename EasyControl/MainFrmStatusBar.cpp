//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MainFrmStatusBar
///
/// @file   MainFrmStatusBar.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "EasyControlVer.h"
#include "MainFrmStatusBar.h"
#include "LCStatus.h"



enum {
	ST_INDICATOR_COMPANY = 0,
	ST_INDICATOR_LOCALIP,
	ST_INDICATOR_ICONCONNECT,
	ST_INDICATOR_RECORDER,
	ST_INDICATOR_LC_ONLINE,
	ST_INDICATOR_LC_OFFLINE,
	ST_INDICATOR_ZOOM,
	ST_INDICATOR_USER,
	ST_INDICATOR_DATE,
};


static UINT indicators[] =
{
	ID_SB_INDICATOR_COMPANY,				// Statusleistenanzeige
	ID_SB_INDICATOR_LOCALIP,		// Statusleistenanzeige
	ID_SB_INDICATOR_ICONCONNECT,				// Statusleistenanzeige
	ID_SB_INDICATOR_RECORDER,				// Statusleistenanzeige
	ID_SB_INDICATOR_LC_ONLINE,
	ID_SB_INDICATOR_LC_OFFLINE,
	ID_SB_INDICATOR_ZOOM,
	ID_SB_INDICATOR_USER,
	ID_SB_INDICATOR_DATE,
};


//**********************************************************************************************************
BEGIN_MESSAGE_MAP(CMainFrameStatusBar, CMFCStatusBar)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()
//**********************************************************************************************************
//**********************************************************************************************************
CMainFrameStatusBar::CMainFrameStatusBar() : CMFCStatusBar()
, m_Progress{}
, m_ZoomCtrl{}
, m_StatusBarFont{}
, m_RemoteIP (_T(""))
, m_User(_T(""))
, m_Company(_T(""))
, m_Time{ 0 }
, m_MaxItems{ 0 }
, m_bProgressBarCreated{ FALSE }
, m_RemoteState{ base::RemoteServerStatus::REMOTE_STATUS_DISABLED }
, m_RecorderState { eRecorderStatus::eRecorderDeactive }
{
	VERIFY(m_StatusBarFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename
}
//**********************************************************************************************************
//**********************************************************************************************************
BOOL CMainFrameStatusBar :: Create(CWnd* pParent)
{
	auto result = CMFCStatusBar::Create(pParent);
	if (result)
	{
		result = CMFCStatusBar::SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	}
	if ( ! result ) 
#if 0
	if (! CMFCStatusBar::Create(pParent) ||
		! CMFCStatusBar::SetIndicators(indicators,
			sizeof(indicators) / sizeof(UINT)))
#endif
	{
		TRACE0("Statusleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	SetFont(&m_StatusBarFont);

	auto hInstance = AfxGetResourceHandle();
	m_hTooltipIcon[MBC_ICONNORMAL] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONCONNECTNORMAL), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));
	m_hTooltipIcon[MBC_ICONCONNECT] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONCONNECTCONNECT), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));
	m_hTooltipIcon[MBC_ICONDISCONNECT] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONCONNECTDISCONNECT), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));
	m_hTooltipIcon[MBC_ICONINPUTOFFLINE] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONCONNECTINPUTOFFLINE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));

	m_hRecorderIcon[MBC_RECORDERDEACTIVE] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONRECORDERDEACTIVE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));
	m_hRecorderIcon[MBC_RECORDERACTIVE] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONRECORDERACTIVE), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));
	m_hRecorderIcon[MBC_RECORDERERROR] = static_cast<HICON>(::LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICONRECORDERERROR), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED));


	int32_t cxWidth = GetSystemMetrics(SM_CXSCREEN); 
	BOOL bSmallScreen = cxWidth == 800;

	UINT uID;
	UINT uStyle;
	int32_t nWidth;


	GetPaneInfo(ST_INDICATOR_COMPANY, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_COMPANY, uID, SBPS_NORMAL, (bSmallScreen) ? 130 : 140);

	GetPaneInfo(ST_INDICATOR_ICONCONNECT, uID, uStyle, nWidth);
	SetPaneStyle(ST_INDICATOR_ICONCONNECT, CBRS_ORIENT_ANY);
	SetPaneText(ST_INDICATOR_ICONCONNECT, _T(""));
	SetPaneInfo(ST_INDICATOR_ICONCONNECT, uID, SBPS_NORMAL, 0);
	SetPaneIcon(ST_INDICATOR_ICONCONNECT, m_hTooltipIcon[MBC_ICONNORMAL]);

	GetPaneInfo(ST_INDICATOR_RECORDER, uID, uStyle, nWidth);
	SetPaneStyle(ST_INDICATOR_RECORDER, CBRS_ORIENT_ANY);
	SetPaneText(ST_INDICATOR_RECORDER, _T(""));
	SetPaneInfo(ST_INDICATOR_RECORDER, uID, SBPS_NORMAL, 0);
	SetPaneIcon(ST_INDICATOR_RECORDER, m_hRecorderIcon[MBC_RECORDERDEACTIVE]);


	GetPaneInfo(ST_INDICATOR_LOCALIP, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_LOCALIP, uID, SBPS_NORMAL, (bSmallScreen) ? 100 : 110);

	GetPaneInfo(ST_INDICATOR_LC_ONLINE, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_LC_ONLINE, uID, SBPS_DISABLED, 240);

	GetPaneInfo(ST_INDICATOR_LC_OFFLINE, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_LC_OFFLINE, uID, SBPS_NORMAL, 40);

	GetPaneInfo(ST_INDICATOR_DATE, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_DATE, uID, SBPS_NORMAL, 110);

	GetPaneInfo(ST_INDICATOR_USER, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_USER, uID, SBPS_NORMAL, (bSmallScreen) ? 80 : 110);

	GetPaneInfo(ST_INDICATOR_ZOOM, uID, uStyle, nWidth);
	SetPaneInfo(ST_INDICATOR_ZOOM, uID, SBPS_NORMAL, (bSmallScreen) ? 140 : 160);

	CRect MyRect;
	GetItemRect(ST_INDICATOR_LC_ONLINE, &MyRect);
	m_ZoomCtrl.Create(WS_CHILD | WS_VISIBLE, MyRect, this, ID_SB_INDICATOR_ZOOM);
	m_ZoomCtrl.SetRange(100, 140);
	m_ZoomCtrl.SetTic(100);
	m_ZoomCtrl.SetTic(110);
	m_ZoomCtrl.SetTic(120);
	m_ZoomCtrl.SetTic(130);
	m_ZoomCtrl.SetTic(140);
	m_ZoomCtrl.SetLineSize(10);

	GetItemRect(ST_INDICATOR_LC_ONLINE, &MyRect);
	m_Progress.Create(WS_VISIBLE | WS_CHILD, MyRect, this, ID_SB_INDICATOR_PROGRESS);
	m_bProgressBarCreated = TRUE;

	EnablePaneDoubleClick(TRUE);
	return 0;
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetMaxItems(uint32_t count)
{
	if (count != m_MaxItems)
	{
		m_MaxItems = count;
		const uint32_t size = m_MaxItems * 20;
		SetPaneInfo(ST_INDICATOR_LC_ONLINE, ID_SB_INDICATOR_LC_ONLINE, SBPS_NORMAL, size);

		m_Progress.SetRange(0, __max(_S16(m_MaxItems - 1), 1));
		m_Progress.SetBarColor(RGB(0, 200, 0));
		m_Progress.SetStep(1);
		m_Progress.OffsetPos(0);

		m_ZoomCtrl.ShowWindow((m_MaxItems > 4 ) ? SW_HIDE:SW_SHOWNORMAL);
		if (m_MaxItems <= 4)
		{
			int32_t curpos = 100;
			m_ZoomCtrl.SetPos(curpos);
			CWnd* pWnd = GetParent();
			ASSERT(pWnd);
			if (pWnd)
				pWnd->SendMessage(WM_ZOOMFACTOR, WPARAM(curpos), 0);
		}
		Resize();
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetUser(const CString& szUser)
{
	if (szUser != m_User)
	{
		m_User = szUser;
		SetPaneText(ST_INDICATOR_USER, m_User);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetTime(const CTime& aTime)
{
	if (aTime != m_Time)
	{
		m_Time = aTime;
		CString sz = m_Time.Format("%d.%m.%y  %H:%M:%S");
		SetPaneText(ST_INDICATOR_DATE, sz);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetCompany(const CString& szCompany)
{
	if (szCompany != m_Company)
	{
		m_Company = szCompany;
		SetPaneText(ST_INDICATOR_COMPANY, m_Company);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetRemoteIP(const CString& szRemoteIP)
{
	if (szRemoteIP != m_RemoteIP)
	{
		m_RemoteIP = szRemoteIP;
		SetPaneText(ST_INDICATOR_LOCALIP, m_RemoteIP);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetRemoteIcon()
{
	switch (m_RemoteState)
	{
	case base::RemoteServerStatus::REMOTE_STATUS_DISABLED:
	case base::RemoteServerStatus::REMOTE_STATUS_CLOSED:
	case base::RemoteServerStatus::REMOTE_STATUS_OPEN:
	case base::RemoteServerStatus::REMOTE_STATUS_DISCONNECTED:
		SetPaneIcon(ST_INDICATOR_ICONCONNECT, m_hTooltipIcon[MBC_ICONNORMAL]);
		break;
	case base::RemoteServerStatus::REMOTE_STATUS_CONNECTED:
		SetPaneIcon(ST_INDICATOR_ICONCONNECT, m_hTooltipIcon[MBC_ICONCONNECT]);
		break;
	case base::RemoteServerStatus::REMOTE_STATUS_INPUTOFFLINE:
		SetPaneIcon(ST_INDICATOR_ICONCONNECT, m_hTooltipIcon[MBC_ICONINPUTOFFLINE]);
		break;
	default:
	case base::RemoteServerStatus::REMOTE_STATUS_ERROR:
		SetPaneIcon(ST_INDICATOR_ICONCONNECT, m_hTooltipIcon[MBC_ICONDISCONNECT]);
		break;
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetRecorderIcon()
{
switch (m_RecorderState)
	{
	case eRecorderStatus::eRecorderActive:
		SetPaneIcon(ST_INDICATOR_RECORDER, m_hRecorderIcon[MBC_RECORDERACTIVE]);
		break;
	case eRecorderStatus::eRecorderError:
		SetPaneIcon(ST_INDICATOR_RECORDER, m_hRecorderIcon[MBC_RECORDERERROR]);
		break;
	default:
	case eRecorderStatus::eRecorderDeactive:
		SetPaneIcon(ST_INDICATOR_RECORDER, m_hRecorderIcon[MBC_RECORDERDEACTIVE]);
		break;
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetRemoteState(const base::RemoteServerStatus state)
{
	if (state != m_RemoteState)
	{
		m_RemoteState = state;
		SetRemoteIcon();
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::SetRecorderState(const eRecorderStatus state)
{
	if (state != m_RecorderState)
	{
		m_RecorderState = state;
		SetRecorderIcon();
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::Progress()
{
	m_Progress.StepIt();
	auto& lcstatus = GetLCStatus();
	lcstatus.UpdateLCStatus();
	m_Progress.SetWindowText(lcstatus.GetLConline());

	SetPaneText(ST_INDICATOR_LC_OFFLINE, lcstatus.GetLCoffline());
	SetPaneTextColor(ST_INDICATOR_LC_OFFLINE, (lcstatus.IsLCoffline()) ? RGB(180, 180, 0) : RGB(200, 200, 200));

	SetTime(CTime::GetCurrentTime());
	Invalidate();
}
//**************************************************************************************************
//**************************************************************************************************
void CMainFrameStatusBar::Resize()
{
	if (m_bProgressBarCreated)
	{
		CRect MyRect;
		GetItemRect(ST_INDICATOR_LC_ONLINE, &MyRect);
		m_Progress.SetWindowPos(&wndTop, MyRect.left, MyRect.top, MyRect.Width(), MyRect.Height(), 0);

		GetItemRect(ST_INDICATOR_ZOOM, &MyRect);
		// Reposition the slider control correctly!
		m_ZoomCtrl.SetWindowPos(&wndTop, MyRect.left + 1, MyRect.top + 1, MyRect.Width() - 1, MyRect.Height() - 1, 0);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMainFrameStatusBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (pScrollBar->GetDlgCtrlID())
	{
		case ID_SB_INDICATOR_ZOOM:
		{
			int32_t minpos = 0;
			int32_t maxpos = 0;

			CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
			pSlider->GetRange(minpos, maxpos);
			int32_t curpos = pSlider->GetPos();
			CWnd* pWnd = GetParent();
			ASSERT(pWnd);
			if (pWnd)
				pWnd->SendMessage(WM_ZOOMFACTOR, WPARAM(curpos), 0);
		}
		break;
	default:
		return;
	}
	CMFCStatusBar::OnHScroll(nSBCode, nPos, pScrollBar);
}



