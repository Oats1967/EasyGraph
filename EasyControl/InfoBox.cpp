//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module InfoBox
///
/// @file   InfoBox.cpp
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
#include "InfoBox.h"
#include "MFCMacros.h"



#define CLR_DEFAULT             0xFF000000L
#define CLR_DEFAULTTEXTCOLOR    COLORREF(RGB(100, 100, 100))
#define CLR_FRAMECOLOR			COLORREF(RGB(100, 100, 100))
#define CLR_CAPTIONCOLOR		COLORREF(RGB(100, 100, 100))
#define CLR_BARCOLOR			COLORREF(RGB(240, 125, 136))


//***********************************************************************************************
//***********************************************************************************************
void CInfoBoxCtrl::SizeToContent(BCGP_SIZE_TO_CONTENT_LINES lines)
{
	UNREFERENCED_PARAMETER(lines);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CBCGPClientDC dc(this);
	SetWindowPos(NULL, -1, -1, m_orgrect.Width(), m_orgrect.Height(), SWP_NOMOVE | SWP_NOZORDER);
	CRect rectWndOld;
	GetWindowRect(rectWndOld);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(rectWndOld);
	pParent->RedrawWindow(rectWndOld, NULL, RDW_ERASE | RDW_UPDATENOW | RDW_INVALIDATE);
	CRect rect = DoDrawCtrl(&dc, FALSE);
	rect.InflateRect(5, 5);
	SetWindowPos(NULL, -1, -1, rect.Width(), rectWndOld.Height(), SWP_NOMOVE | SWP_NOZORDER);

	// Adjust parent window
	pParent->SetWindowPos(NULL, -1, -1, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER);
	pParent->RedrawWindow(rect, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_INVALIDATE);
}
//***********************************************************************************************
//***********************************************************************************************
// CInfoBox-Dialogfeld
IMPLEMENT_DYNAMIC(CInfoBox, CBaseClass)
CInfoBox::CInfoBox()
	: CBaseClass()
	, m_wndInfoBox()
	, m_aLogItem()
	, m_orgrect(0,0,0,0)
{
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_BOX, m_wndInfoBox);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CInfoBox, CBaseClass)
	ON_STN_CLICKED(IDC_INFO_BOX, OnStnClickedInfoMessage)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//***************************************************************************************
//***************************************************************************************
BOOL CInfoBox::OnInitDialog()
{
	const int32_t m_nColorBarWidth = 7;

	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	ASSERT(result);
	if (result)
	{
		m_wndInfoBox.SetWindowOrg();

		m_wndInfoBox.m_clrBackground = CLR_DEFAULT;
		m_wndInfoBox.m_clrForeground = CLR_DEFAULTTEXTCOLOR;
		m_wndInfoBox.m_clrFrame = CLR_FRAMECOLOR;
		m_wndInfoBox.m_clrCaption = CLR_CAPTIONCOLOR;
		m_wndInfoBox.m_clrColorBar = CLR_BARCOLOR;
		m_wndInfoBox.SetLink(_T(""));
		m_wndInfoBox.ModifyStyle(WS_TABSTOP, 0);

		m_wndInfoBox.SetIcon(NULL);
		ASSERT(m_wndInfoBox.m_nIconIndex == -1);
		m_wndInfoBox.m_bIsColorBarOnLeft = TRUE;
		m_wndInfoBox.m_nColorBarWidth = m_nColorBarWidth;
		m_wndInfoBox.m_bDrawShadow = TRUE;
		m_wndInfoBox.m_bRoundedCorners = FALSE;
		m_wndInfoBox.m_bFixedFrameHeight = TRUE;

		SetValue();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::Create(CWnd* pParent, const CPoint& top)
{
	auto result = CBaseClass::Create(IDD, pParent);
	ASSERT(result);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::Move(const CPoint& top)
{
	CBaseClass::Move(top);
	m_wndInfoBox.SetWindowOrg();
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::SetText()
{
	const CString szTime{ StdTimeToCTime(m_aLogItem.GetTime()).Format("%H:%M:%S") };

	// Fehler nr.
	CString szDosierer;
	VERIFY(szDosierer.LoadString(IDS_DOSIERER));
	const CString message{ toCString(m_aLogItem.GetMessage()) };
	CString szFormat;
	szFormat.Format(_T("%s : %s(%d);  [%d] %s"), LPCTSTR(szTime), LPCTSTR(szDosierer), _S32(m_aLogItem.GetItem() + 1),
					_S32(m_aLogItem.GetAlarm())+1, LPCTSTR(message));

	m_wndInfoBox.SetWindowText(szFormat);
	ASSERT(m_wndInfoBox.m_nIconIndex == -1); //  (m_aLogItem.GetAlarmClass() == base::eAlarmClass::eWARNTYP) ? 2 : 1;
	m_wndInfoBox.SizeToContent();
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::SetErrorItem(const base::utils::CLogItem& aLogItem)
{
	if ((aLogItem.GetID() != m_aLogItem.GetID()) || (aLogItem.GetChecked() != m_aLogItem.GetChecked()))
	{
		m_aLogItem = aLogItem;
		if (m_aLogItem.GetID() >= 0)
		{
			SetText();
			ShowWindow(SW_SHOWNORMAL);
			BringWindowToTop();
			SetFocus();
		}
		else
		{
			ShowWindow(SW_HIDE);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::DeleteErrorItem()
{
	m_aLogItem = base::utils::CLogItem();
	ShowWindow(SW_HIDE);
	Invalidate(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::ClearMessage()
{
	auto id = m_aLogItem.GetID();
	if (id >= 0)
	{
		auto* pParent = GetParent();
		if (pParent && pParent->GetSafeHwnd())
		{
			pParent->PostMessage(WM_INFOBOX_ALARMCLEAR, WPARAM(id));
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::OnStnClickedInfoMessage()
{
	ClearMessage();
}
//***********************************************************************************************
//***********************************************************************************************
void CInfoBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	ClearMessage();
	CBaseClass::OnLButtonDown(nFlags, point);
}





