// TextProgressCtrl.cpp : implementation file
//
// Written by Chris Maunder (chris@codeproject.com)
// Copyright 1998-2005.
//
// TextProgressCtrl is a drop-in replacement for the standard
// CProgressCtrl that displays text in a progress control.
//
// Homepage: http://www.codeproject.com/miscctrl/text_progressctrl.asp
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the author's written consent, and providing that this
// notice and the author's name is included. If the source code in
// this file is used in any commercial application then an email to
// me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer or anything else vaguely within its vicinity.
//
// Expect bugs.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.

////////////////////////////////////////////////////////////////////////////////
// Revision History		- by PJ Arends (pja@telus.net)
// July 1, 2001			- added functionality to set font to parent windows font
// July 15, 2001		- added SetTextColor() and GetTextColor() functions
// August 2, 2001		- Added functionality where the control now responds to
//						  PBM_* progress bar messages. Now the control can be
//						  controlled by sending messages to its HWND.
//						- added PBM_SETTEXTCOLOR and PBM_SETSHOWTEXT messages
//						- added the ability to call GetPos() via OnGetPos()
// January 3, 2005		- By Kriz: Added two basic methods some other changes to
//						  allow the user to switch between the three alignment
//						  styles LEFT, CENTER and RIGHT - even on the fly if that's
//						  needed. Methods: AlignText and AlignTextInvalidate
//						- By C Maunder: updated the code so it compiles in VC7
// March 6, 2006		- By A. Bommarito: changed to allow text in vertical
//						  progress bars; added method to change text background
//						  color; added capability to independently control display
//						  of text and percentage complete; added marquee mode;
//						  changed to exclusively use messaging interface
// February 22, 2007	- By A. Bommarito: changed to fix problem where progress bar
//						  border didn't display on XP and above when a manifest file
//						  specified use of Version 6 of the Common Controls; fixed
//						  problem with AppendFormat method being used without an
//						  _T() around the format string - caused compile error when
//						  using Unicode (message from dev75040); added complete turn
//						  off of Marquee style (message from Robert Pickford)
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <cassert>
#include <vector>
#include "TextProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl

#define	EDGE_SPACE			2					// number of pixels to space text away from edge of progress bar

CTextProgressCtrl::CTextProgressCtrl() : CProgressCtrl()
, m_lastList{}
, m_lastString{ L"" }
, m_crBarClr{ RGB(0, 200, 0) }
, m_crBarBkClr{ CLR_DEFAULT }
, m_crTextClr{ CLR_DEFAULT }
, m_crTextBkClr{ CLR_DEFAULT }
, m_dwTextStyle{ DT_CENTER }
, m_nPos{ 0 }
, m_nStepSize{ 1 }
, m_nMin{ 0 }
, m_nMax{ 1 }
{
}

CTextProgressCtrl::~CTextProgressCtrl()
{
}

BEGIN_MESSAGE_MAP(CTextProgressCtrl, CProgressCtrl)
//{{AFX_MSG_MAP(CTextProgressCtrl)
ON_WM_ERASEBKGND()
ON_WM_PAINT()
ON_MESSAGE(PBM_SETRANGE, OnSetRange)
ON_MESSAGE(PBM_SETPOS, OnSetPos)
ON_MESSAGE(PBM_DELTAPOS, OnOffsetPos)
ON_MESSAGE(PBM_SETSTEP, OnSetStep)
ON_MESSAGE(PBM_STEPIT, OnStepIt)
ON_MESSAGE(PBM_SETRANGE32, OnSetRange32)
ON_MESSAGE(PBM_GETRANGE, OnGetRange)
ON_MESSAGE(PBM_GETPOS, OnGetPos)
ON_MESSAGE(PBM_SETBARCOLOR, OnSetBarColor)
ON_MESSAGE(PBM_SETBKCOLOR, OnSetBarBkColor)

ON_MESSAGE(PBM_GETBARCOLOR, OnGetBarColor)
ON_MESSAGE(PBM_GETBKCOLOR, OnGetBarBkColor)
ON_MESSAGE(PBM_SETTEXTCOLOR, OnSetTextColor)
ON_MESSAGE(PBM_GETTEXTCOLOR, OnGetTextColor)
ON_MESSAGE(PBM_SETTEXTBKCOLOR, OnSetTextBkColor)
ON_MESSAGE(PBM_GETTEXTBKCOLOR, OnGetTextBkColor)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl message handlers

BOOL CTextProgressCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

const CTextProgressCtrl::StringList& CTextProgressCtrl::SplitString(const CString& sz)
{
	if (sz != m_lastString)
	{
		m_lastString = sz;
		m_lastList.clear();

		assert(sz.GetLength() < 200);
		TCHAR str[200];
		_tcscpy_s(str, (LPCTSTR)sz);
		const TCHAR* delim = _T(" ");
		TCHAR* next_token = NULL;

		/* get the first token */
		auto token =   _tcstok_s(str, delim, &next_token);

		/* walk through other tokens */
		while (token != NULL) 
		{
			m_lastList.push_back(token);
			token = _tcstok_s(NULL, delim, &next_token);
		}
	}
	return m_lastList;
}


void CTextProgressCtrl::OnPaint()
{
	if (m_nMin >= m_nMax)
		return;

	CPaintDC PaintDC(this);						// device context for painting
	CMemDC aDC(PaintDC, this);
	auto& dc = aDC.GetDC();
	//CProgressDC dc(&PaintDC);						// memory device context

	// get colors to use
	COLORREF crBarColor		= (COLORREF)OnGetBarColor(0, 0);
	COLORREF crBarBkColor	= (COLORREF)OnGetBarBkColor(0, 0);
	COLORREF crTextColor	= (COLORREF)OnGetTextColor(0, 0);
	//COLORREF crTextBkColor	= (COLORREF)OnGetTextBkColor(0, 0);;

	// select progress bar font if there is one, else select parent font
	if (GetFont())
		dc.SelectObject(GetFont());
	else
		dc.SelectObject(GetParent()->GetFont());

	// get bar dimensions and draw bar outline
	CRect ClientRect;
	GetClientRect(&ClientRect);
	DrawEdge(dc, ClientRect, EDGE_SUNKEN, BF_ADJUST | BF_RECT | BF_FLAT);

	// compute the fractional position
	double dFraction = 1.0 / ((double)(m_nMax - m_nMin + 1));
	int sWidth = ClientRect.Width();
	//int sHeight = ClientRect.Height();
	int dx = static_cast<int>(sWidth * dFraction + 0.5);

	if (m_nPos <= m_nMin)
	{
		CRect aM{ ClientRect.left, ClientRect.top, ClientRect.left + dx, ClientRect.bottom };
		CRect aR{ aM.right, aM.top, ClientRect.right, ClientRect.bottom };

		dc.FillSolidRect(aM, crBarColor);
		dc.FillSolidRect(aR, crBarBkColor);
	}
	else if ((m_nPos >= m_nMax))
	{ 
		CRect aL{ ClientRect.left, ClientRect.top, ClientRect.left + m_nPos *dx, ClientRect.bottom };
		CRect aM{ aL.right, aL.top, ClientRect.right, ClientRect.bottom };

		dc.FillSolidRect(aM, crBarColor);
		dc.FillSolidRect(aL, crBarBkColor);
	}
	else
	{
		CRect aL{ ClientRect.left, ClientRect.top, ClientRect.left + m_nPos * dx, ClientRect.bottom };
		CRect aM{ aL.right, aL.top, aL.right + dx, aL.bottom };
		CRect aR{ aM.right, aM.top, ClientRect.right, ClientRect.bottom };

		dc.FillSolidRect(aL, crBarBkColor);
		dc.FillSolidRect(aM, crBarColor);
		dc.FillSolidRect(aR, crBarBkColor);
	}

	// draw text if needed
	CString str;
	GetWindowText(str);
	if (str.GetLength())
	{
		CPoint ptStart;

		const StringList& rList = SplitString(str);
		const auto length = rList.size();
		ptStart.x = 0;
		ptStart.y = 10;
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(crTextColor);
		for (int i = 0; i < length; i++)
		{
			CRect aRect{ ptStart.x , ptStart.y, ptStart.x + dx, ClientRect.bottom };
			dc.DrawText(rList[i],  aRect, DT_CENTER | DT_VCENTER);
			ptStart.x += dx;
		}
	}
}


LRESULT CTextProgressCtrl::OnSetRange(WPARAM, LPARAM lparamRange)
{
	// set new 16-bit range, returning old one
	return (OnSetRange32(LOWORD(lparamRange), HIWORD(lparamRange)));
}

LRESULT CTextProgressCtrl::OnSetPos(WPARAM nNewPos, LPARAM)
{
	// save old position for return
	int nOldPos = m_nPos;

	// set new position
	m_nPos = (int)nNewPos;

	// limit position to inside of range
#if _DEBUG
	DWORD dwStyle = GetStyle();
	ASSERT((dwStyle & PBS_MARQUEE) == 0);
#endif

	if (m_nPos < m_nMin)
		m_nPos = m_nMin;
	if (m_nPos > m_nMax)
		m_nPos = m_nMax;

	// call a common routine to redraw window
	CommonPaint();
	
	// return old position
	return ((LRESULT)nOldPos);
}

LRESULT CTextProgressCtrl::OnOffsetPos(WPARAM nIncrement, LPARAM)
{
	// set new position, returning old one
	return (OnSetPos((WPARAM)(m_nPos + (int)nIncrement), 0));
}

LRESULT CTextProgressCtrl::OnSetStep(WPARAM nStepInc, LPARAM)
{
	// save old step size for return
	int nOldStep = m_nStepSize;

	// set new step size
	m_nStepSize = (int)nStepInc;

	// return old step size
	return ((LRESULT)nOldStep);
}

LRESULT CTextProgressCtrl::OnStepIt(WPARAM, LPARAM)
{
	int lower, upper;

	GetRange(lower, upper);
	int pos = GetPos();
	if (++pos > upper)
	{
		pos = lower;
	}
	return SetPos(pos);
}

LRESULT CTextProgressCtrl::OnSetRange32(WPARAM nLowLim, LPARAM nHighLim)
{
	// save old range for return
	int nOldMin = m_nMin;
	int nOldMax = m_nMax;

	// set new range
	m_nMin = (int)nLowLim;
	m_nMax = (int)nHighLim;

	// return old range (16 bit only)
	return (MAKELRESULT(LOWORD(nOldMin), LOWORD(nOldMax)));
}

LRESULT CTextProgressCtrl::OnGetRange(WPARAM bWhichLimit, LPARAM pPBRange)
{
	// get arguments into proper types
	BOOL bType = (BOOL)bWhichLimit;
	PPBRANGE pRange = (PPBRANGE)pPBRange;

	// return both range limits if a pointer was provided
	if (pRange)
	{
		pRange->iLow = m_nMin;
		pRange->iHigh = m_nMax;
	}

	// return selected range limit
	return ((LRESULT)(bType ? m_nMin : m_nMax));
}

LRESULT CTextProgressCtrl::OnGetPos(WPARAM, LPARAM)
{
	// return current position
	return ((LRESULT)m_nPos);
}

LRESULT CTextProgressCtrl::OnSetBarColor(WPARAM, LPARAM crBar)
{
	// call a common routine to redraw window
	CommonPaint();

	// set new bar color, returning old one
	COLORREF crOldBarClr = m_crBarClr;
	m_crBarClr = (COLORREF)crBar;
	return ((LRESULT)crOldBarClr);
}

LRESULT CTextProgressCtrl::OnSetBarBkColor(WPARAM, LPARAM crBarBk)
{
	// call a common routine to redraw window
	CommonPaint();

	// set new bar background color, returning old one
	COLORREF crOldBarBkClr = m_crBarBkClr;
	m_crBarBkClr = (COLORREF)crBarBk;
	return ((LRESULT)crOldBarBkClr);
}

LRESULT CTextProgressCtrl::OnGetBarColor(WPARAM, LPARAM)
{
	// return current bar color
	return ((LRESULT)((m_crBarClr == CLR_DEFAULT)? ::GetSysColor(COLOR_HIGHLIGHT) : m_crBarClr));
}

LRESULT CTextProgressCtrl::OnGetBarBkColor(WPARAM, LPARAM)
{
	// return current bar background color
	return ((LRESULT)((m_crBarBkClr == CLR_DEFAULT)? ::GetSysColor(COLOR_BTNFACE) : m_crBarBkClr));
}

LRESULT CTextProgressCtrl::OnSetTextColor(WPARAM, LPARAM crText)
{
	// call a common routine to redraw window
	CommonPaint();

	// set new text color, returning old one
	COLORREF crOldTextClr = m_crTextClr;
	m_crTextClr = (COLORREF)crText;
	return ((LRESULT)crOldTextClr);
}

LRESULT CTextProgressCtrl::OnGetTextColor(WPARAM, LPARAM)
{
	// return current text color
	return ((LRESULT)((m_crTextClr == CLR_DEFAULT ? ::GetSysColor(COLOR_WINDOWTEXT) : m_crTextClr)));
}

LRESULT CTextProgressCtrl::OnSetTextBkColor(WPARAM, LPARAM crTextBkClr)
{
	// call a common routine to redraw window
	CommonPaint();

	// set new text background color, returning old one
	COLORREF crOldTextBkClr = m_crTextBkClr;
	m_crTextBkClr = (COLORREF)crTextBkClr;
	return ((LRESULT)crOldTextBkClr);
}

LRESULT CTextProgressCtrl::OnGetTextBkColor(WPARAM, LPARAM)
{
	// return current text background color
	return ((LRESULT)((m_crTextBkClr == CLR_DEFAULT ? ::GetSysColor(COLOR_WINDOW) : m_crTextBkClr)));
}
////////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl helper functions
void CTextProgressCtrl::CommonPaint()
{
	//
	// common place to mess with redraw options
	//
	// note that Invalidate() by itself won't cause
	// a repaint when this control is used in a modeless
	// dialog box
	//
	// also note that the marquee timer won't work in a modeless
	// dialog - to use marquee mode in a modeless dialog the code
	// that is running will have to pump Windows messages
	//
	
	RedrawWindow();
}
