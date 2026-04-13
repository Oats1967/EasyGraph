//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EditCtrl
///
/// @file   EditCtrl.cpp
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
#include "BASE/Utils/public/StringUtil.h"
#include "EasyControl.h"
#include "EditCtrl.h"
#include "MFCMacros.h"


#ifndef RANGE
#define RANGE(a, b,c)			(min(max(a,b),c))
#endif

// CEditCtrl
#define VALIDPARENT(p) ((p) && (p)->GetSafeHwnd())

CEditCtrl* CEditCtrl  :: m_pEdit = NULL;
int32_t CEditCtrl :: m_cx            = 0;
int32_t CEditCtrl :: m_cy            = 0;

DATATYPE	CEditCtrl :: uVal;


IMPLEMENT_DYNAMIC(CEditCtrl, CEdit)

BEGIN_MESSAGE_MAP(CEditCtrl, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::ShowAlphaKeyboard()
{
	TKB.TK_Show();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::ShowNumericKeyboard (CWnd *pParent, int32_t id)
{
	ASSERT(VALIDPARENT(pParent));
	if (VALIDPARENT(pParent))
	{

		if (m_cx <= 0)
		{
			m_cx = GetSystemMetrics(SM_CXSCREEN);
			m_cy = GetSystemMetrics(SM_CYSCREEN);
		}
		ITKBoardInterface::eLayout nLayout = TKB.TK_GetNumericLayout();
		ITKBoardInterface::TKSize aTKSize;
		TKB.TK_GetSize(nLayout, aTKSize);

		ASSERT(pParent);
		CDialog* pDlg = (CDialog*)pParent;
		ASSERT(pDlg);
		CWnd* pStatic = pDlg->GetDlgItem(id);
		ASSERT(pStatic);
		CRect aRect;
		pStatic->GetWindowRect(aRect);

		const int32_t left = aRect.left;
		const int32_t top = aRect.top;
		const int32_t right = aRect.right;
		const int32_t bottom = aRect.bottom;
		int32_t x = (right > m_cx - aTKSize.Width() - 10) ? left - aTKSize.Width() - 10 : right + 10;
		int32_t y = (bottom > m_cy - aTKSize.Height() - 10) ? top - aTKSize.Height() - 10 : top;

		x = RANGE(x, 0, m_cx);
		y = RANGE(y, 0, m_cy);

		TKB.TK_ShowLayout(nLayout, ITKBoardInterface::TKPoint(x, y));
	}
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::Init(CWnd* pParent)
{
	ASSERT(VALIDPARENT(pParent));
	if (VALIDPARENT(pParent))
	{
		CDialog* pDlg = (CDialog*)pParent;
		ASSERT(pDlg);
		CWnd* pStatic = pDlg->GetDlgItem(m_ID);
		ASSERT(pStatic);
		CRect aRect;
		pStatic->GetWindowRect(aRect);
		pParent->ScreenToClient(aRect);
		CString aText;
		pStatic->GetWindowText(aText);
		Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, aRect, pParent, 1);

		SetWindowText(aText);
		SetSel(0, -1);
		ShowWindow(TRUE);
		SetFocus();
	}
}

//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t	CEditCtrl  :: GetLong   ( void)
{ return uVal.iInt; }
//**********************************************************************************************************************
//**********************************************************************************************************************
int32_t	CEditCtrl::GetLongAbs(void)
{	return _S32(labs(uVal.iInt));
}
//**********************************************************************************************************************
//**********************************************************************************************************************
float32_t	CEditCtrl  :: GetFloat   ( void)
{ return uVal.fFloat; }
//**********************************************************************************************************************
float32_t	CEditCtrl::GetFloatAbs(void)
{ return _F32(fabs(uVal.fFloat)); }
//**********************************************************************************************************************
//**********************************************************************************************************************
CString	CEditCtrl  :: GetString   ( void)
{ return CString(uVal.szBuff); }
//**********************************************************************************************************************
//**********************************************************************************************************************
uint64_t	CEditCtrl::GetUint64(void)
{	return _U64(uVal.iUInt64); }
//**********************************************************************************************************************
//**********************************************************************************************************************
CStringCtrl::CStringCtrl(CWnd* pParent, int32_t _id) : CEditCtrl(_id)
{
	ShowAlphaKeyboard();
	Init(pParent);
}
//**********************************************************************************************************************
//**********************************************************************************************************************
CNumericCtrl::CNumericCtrl(CWnd* pParent, int32_t _id) : CEditCtrl(_id)
{
	ShowNumericKeyboard(pParent, m_ID);
	Init(pParent);
}
//**********************************************************************************************************************
//**********************************************************************************************************************
CEditCtrl::~CEditCtrl()
{
	m_pEdit = NULL;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
CEditCtrl* CEditCtrl::GetInput (CWnd *pParent, E_TYPCTRL eCtrl, int32_t id)
{
	// ASSERT(m_pEdit == NULL);
	if ( m_pEdit )
	{
        delete m_pEdit;
		m_pEdit = NULL;
	}
	ASSERT(VALIDPARENT(pParent));
	if (VALIDPARENT(pParent))
	{
		mfcmacros::SendMessage(pParent, WM_NOTIFYEDITBOX);
		switch (eCtrl)
		{
		default:
		case E_TYPCTRL::E_STRINGCTRL:
			m_pEdit = new CStringCtrl(pParent, id);
			break;
		case E_TYPCTRL::E_INTCTRL:
			m_pEdit = new CNumericCtrl(pParent, id);
			break;
		case E_TYPCTRL::E_UINT64CTRL:
			m_pEdit = new CUint64Ctrl(pParent, id);
			break;
		case E_TYPCTRL::E_FLOATCTRL:
			m_pEdit = new CFloatCtrl(pParent, id);
			break;
		}
	}
	return m_pEdit;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::Destroy()
{
	if (m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
	TKB.TK_Hide();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CEditCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CWnd* pParent = GetParent();

	if (VALIDPARENT(pParent))
	{
		pParent->PostMessage(WM_NOTIFYEDIT, WPARAM(m_ID), LPARAM(m_bValue));
	}
	CEdit::OnKillFocus(pNewWnd);
	delete this;
	TKB.TK_Hide();
}
//**********************************************************************************************************************
//**********************************************************************************************************************
BOOL CEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			if ( pMsg->wParam == VK_RETURN )
			{
                GetValue();
			}
            ShowWindow(SW_HIDE);
			return TRUE;
		}
	}
	// be a good citizen - call the base class
	return CEdit::PreTranslateMessage(pMsg);
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CStringCtrl :: GetValue(void)
{
	CString szBuff;

	GetWindowText(szBuff);
	std::string val = toStdString(szBuff);
	const auto length = __min(sizeof(uVal.szBuff)-1, val.length());
	strncpy_s(uVal.szBuff, val.c_str(), length);
	uVal.szBuff[length] = 0;
    m_bValue = TRUE;
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CNumericCtrl :: GetValue(void)
{
	CString szBuff;

	GetWindowText(szBuff);
	try
	{
		uVal.iInt= std::stoi(LPCTSTR(szBuff));
		m_bValue = TRUE;
	}
	catch (...)
	{
		uVal.iInt = 0;
	}
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CUint64Ctrl::GetValue(void)
{
	CString szBuff;

	GetWindowText(szBuff);
	try
	{
		uVal.iUInt64 = std::stoull(LPCTSTR(szBuff));
		m_bValue = TRUE;
	}
	catch (...)
	{
		uVal.iUInt64 = 0;
	}
}
//**********************************************************************************************************************
//**********************************************************************************************************************
void CFloatCtrl :: GetValue(void)
{
	CString szBuff;

	GetWindowText(szBuff);
	try
	{
		uVal.fFloat = _F32(std::stof(LPCTSTR(szBuff)));
		m_bValue = TRUE;
	}
	catch (...)
	{
		uVal.fFloat = 0.0F;
	}
}


