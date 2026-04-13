//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SpeedBoxDlg
///
/// @file   SpeedBoxDlg.cpp
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
#include "EasyControl.h"
#include "SpeedBoxDlg.h"
#include "MFCMacros.h"



#define CLR_DEFAULT             0xFF000000L
#define CLR_DEFAULTTEXTCOLOR    COLORREF(RGB(100, 100, 100))
#define CLR_FRAMECOLOR			COLORREF(RGB(100, 100, 100))
#define CLR_CAPTIONCOLOR		COLORREF(RGB(100, 100, 100))
#define CLR_BARCOLOR			COLORREF(RGB(38, 213, 82))
#define TIMERID 1

CSpeedBoxDlg* CSpeedBoxDlg:: g_SpeedBoxDlg = NULL;


//***********************************************************************************************
//***********************************************************************************************
// CSpeedBoxDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CSpeedBoxDlg, CBaseClass)
CSpeedBoxDlg::CSpeedBoxDlg()
	: CBaseClass()
	, m_pParent{ nullptr }
	, m_wndKnob{ this }
	, m_szCaption { "" }
	, m_Tracking{ FALSE }
	, m_id{ 0 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPEEDBOX_KNOB, m_wndKnob);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CSpeedBoxDlg, CBaseClass)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_START_TRACK, OnTrackKnob)
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_FINISH_TRACK, OnTrackKnob)
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_TRACK, OnTrackKnob)
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_CANCEL_TRACK, OnTrackKnob)
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
BOOL CSpeedBoxDlg::OnInitDialog()
{
	const int32_t m_nColorBarWidth = 7;

	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	ASSERT(result);
	if (result)
	{
		auto value = GetStartValue();
		ShowHeader(value);
		m_wndKnob.SetSpeed(value);
		m_wndKnob.Update();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if ( ! g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::DestroySpeedBox(void)
{
	if (g_SpeedBoxDlg && g_SpeedBoxDlg->GetSafeHwnd())
	{
		g_SpeedBoxDlg->PostMessageW(WM_DESTROY);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::Create(CWnd* pParent, const int32_t id, const CPoint& top, const CString& szCaption)
{
	m_pParent = pParent;
	m_id = id;
	m_szCaption = szCaption;

	mfcmacros::SendMessage(pParent, WM_NOTIFYSPEEDBOX);
	auto result = CBaseClass::Create(IDD, pParent);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 1000, NULL);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::OnTimer(UINT_PTR )
{
	SetFocus();
	KillTimer(TIMERID);
	UnlockWindow();
}
//***********************************************************************************************
//***********************************************************************************************
void CSpeedBoxDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_SPEEDBOXCLOSED, m_id);
	}
	delete this;
}
//*************************************************************************************
//*************************************************************************************
void CSpeedBoxDlg::ShowHeader(const float32_t value)
{
	CString szTemp;
	szTemp.Format(_T("%s - %3d %%"), LPCTSTR(m_szCaption), NEXTLONG(value));
	CBaseClass::ShowHeader(szTemp);
}
//*************************************************************************************
//*************************************************************************************
LRESULT CSpeedBoxDlg::OnTrackKnob(WPARAM, LPARAM lp)
{
	CBCGPGaugeTrackingData* pData = (CBCGPGaugeTrackingData*)lp;

	if (!pData)
	{
		return 0L;
	}
	BOOL bChanged = FALSE;
	if (pData->m_nMessage == BCGM_ON_GAUGE_TRACK)
	{
		float32_t value = RANGE(_F32(pData->m_Value), 0.0f, 100.0f);
		ShowHeader(value);
		m_Tracking = TRUE;
	}
	else if (pData->m_nMessage == BCGM_ON_GAUGE_START_TRACK)
	{
		float32_t value = RANGE(_F32(pData->m_Value), 0.0f, 100.0f);
		ShowHeader(value);
		m_Tracking = TRUE;
	}
	else if (pData->m_nMessage == BCGM_ON_GAUGE_FINISH_TRACK)
	{
		float32_t value = RANGE(_F32(pData->m_Value), 0.0f, 100.0f);
		OnNewValue(value);
		ShowHeader(value);
		bChanged = TRUE;
		m_Tracking = FALSE;
	}
	else if (pData->m_nMessage == BCGM_ON_GAUGE_CANCEL_TRACK)
	{
		m_Tracking = FALSE;
	}
	else
	{}

	return bChanged;
}




