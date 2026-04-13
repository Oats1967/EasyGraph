//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module GrafikContext
///
/// @file   GrafikContext.cpp
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
#include "GrafikContext.h"
#include "MFCMacros.h"
#include "FormatMetric.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//***********************************************************************************************************
//***********************************************************************************************************
CGrafikContext::CGrafikContext(const CRect& rRect) :
	m_TextUpdate(FALSE)
	, m_dcMem()
	, m_dcImage()
	, m_OrgRect{ rRect }
	, m_DisplayRect{ rRect }
{
	m_fontValue.CreateFont(14, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
}
//***********************************************************************************************************
//***********************************************************************************************************
CGrafikContext::~CGrafikContext()
{
}
//***********************************************************************************************************
//***********************************************************************************************************
void CGrafikContext::TextOut(CDC* pDC, const POINT& p, const CString& sz, COLORREF color,  int32_t bkmode)
{
	int32_t iOldCR = pDC->SetTextColor(color);
	int32_t iOldBK = pDC->SetBkMode(bkmode);
	int32_t iOldTA = pDC->SetTextAlign(TA_LEFT | TA_TOP);
	CFont* oldFont = pDC->SelectObject(&m_fontValue);
	pDC->TextOut(p.x, p.y, sz);
	pDC->SelectObject(oldFont);
	pDC->SetTextAlign(iOldTA);
	pDC->SetBkMode(iOldBK);
	pDC->SetTextColor(iOldCR);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CGrafikContext::ShowBitmap(CDC* pDC, uint32_t nRessourceID, const CPoint& aP)
{
	CBitmapDC aBitmap(pDC, nRessourceID);

	auto& dc = aBitmap.GetDC();
	auto bmsize = aBitmap.GetBitmapDimensions();
	TransparentBlt(pDC->GetSafeHdc(), aP.x, aP.y, bmsize.cx, bmsize.cy, dc, 0, 0, bmsize.cx, bmsize.cy, RGB(255,255,255));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CGrafikContext::Update(CDC* pDC, uint32_t mode)
{
	auto displaysize = m_DisplayRect.Size();
	auto orgsize	 = m_OrgRect.Size();
	auto topleft	  = m_DisplayRect.TopLeft();

	const int32_t rectWidth  = displaysize.cx;
	const int32_t rectHeight = displaysize.cy;
	ASSERT((rectWidth * rectHeight > 0));
	ASSERT(m_dcMem.IsBitmapDC());

	if ( ! m_dcImage.IsBitmapDC())
	{
		m_dcImage.Create(pDC, displaysize);
	}
	ASSERT(m_dcImage.IsBitmapDC());
	auto& dc = m_dcImage.GetDC();
	if (m_TextUpdate)
	{
		dc.BitBlt(0, 0, rectWidth, rectHeight, &m_dcMem.GetDC(), 0, 0, SRCCOPY);
		OnUpdate(&dc);
	}
	if (mode == OPAQUE)
	{
		BOOL zoom = displaysize != m_OrgRect.Size();
		if (zoom)
		{
			pDC->StretchBlt(topleft.x, topleft.y, rectWidth, rectHeight, &dc, 0, 0, orgsize.cx, orgsize.cy, SRCCOPY);
		}
		else
		{
			pDC->BitBlt(topleft.x, topleft.y, rectWidth, rectHeight, &dc, 0, 0, SRCCOPY);
		}
	}
	else
	{
		pDC->TransparentBlt( topleft.x, topleft.y, rectWidth, rectHeight, &dc, 0, 0, orgsize.cx, orgsize.cy, WHITECOLOR);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CGrafikContext::getTextMetrics(CDC* pDC, CSize& size) 
{
	TEXTMETRIC textmetrics;

	CFont* poldfont = pDC->SelectObject(&m_fontValue);
	pDC->GetTextMetrics(&textmetrics);
	size = CSize(textmetrics.tmAveCharWidth, textmetrics.tmHeight);
	pDC->SelectObject(poldfont);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CGrafikContext::ShowGrafik(CDC* pDC, const CRect& rDisplayRect, uint32_t mode )
{
	BOOL bitmapUpdate = FALSE;
	auto newdisplaysize = rDisplayRect.Size();
	ASSERT(newdisplaysize.cx * newdisplaysize.cy > 0);
	if (newdisplaysize != m_DisplayRect.Size())
	{
		m_dcMem.Release();
		m_dcImage.Release();
	}
	m_DisplayRect = rDisplayRect;
	if (! m_dcMem.IsBitmapDC() )
	{
		m_dcMem.Create(pDC, newdisplaysize);
		ASSERT(m_dcMem.IsBitmapDC());
		bitmapUpdate = TRUE;
	}
	if (! bitmapUpdate)
	{
		bitmapUpdate = IsBitmapUpdate();
	}
	if (bitmapUpdate)
	{
		// Hintergrund
		auto& dc = m_dcMem.GetDC();
		dc.FillSolidRect(CRect(CPoint(0, 0), newdisplaysize), COLORBKWHITE);
		OnRedraw(&dc);
		m_TextUpdate = TRUE;
	}
	else
	{
		m_TextUpdate = IsTextUpdate();
	}
	Update(pDC, mode);
}










