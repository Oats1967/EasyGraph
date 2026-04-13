//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Implementation of module BarChart
///
/// @file   BarChart.cpp
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
#include <math.h>
#include "EasyControl.h"
#include "BarChart.h"
#include "FormatMetric.h"

#define MAXREPAINT 5

const COLORREF  CBarChart:: c_aColorRef[eGridSymbol::eMax]	= { RGBCOLORBLUE, RGBCOLORGREEN };

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CBarChart::CBarChart(void):
	  m_aRect()
    , m_aFillRect()
    , m_aDrawRect()
    , m_aLegendTextRect()
    , m_iCharHeight(0)
    , m_iCharWidth(0)
    , m_fMaxwert(-1.0f)
    , m_iyMaxwert(0)
    , m_iyGridraster(0)
    , m_fyPixel(0.0f)
    , m_fxPixel(0.0f)
	, m_bRepaint(TRUE)
	, m_bInit(FALSE)
{
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CBarChart ::~CBarChart(void)
{}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::Create (CDC *pDC, const CRect& aRect)
{
	ASSERT(!m_bInit);

	for (int32_t index = 0; index < eGridSymbol::eMax; index++)
	{
		m_fValue[index] =  0.0f;
		m_BarBrush[index].CreateSolidBrush(c_aColorRef[index]);
		m_BarRect[index].SetRectEmpty();
	}
	VERIFY(m_szXLegend[eGridSymbol::eSetpoint].LoadString(IDS_SETPOINT_KGH));
	VERIFY(m_szXLegend[eGridSymbol::eMassflow].LoadString(IDS_MASSFLOW_KGH));

	VERIFY(m_BlackPen.CreatePen(PS_SOLID, 1, RGBCOLORBLACK));

	m_aRect		= aRect;
	m_aFillRect = CRect(CPoint(0,0),aRect.Size());
	m_dcBackground.Create(pDC, aRect.Size());
	DrawBackground(&m_dcBackground.GetDC());
	m_dcCache.Create(pDC, aRect.Size());

	// create and select a solid blue brush
	// get our client rectangle
	// draw a thick black rectangle filled with blue
	// put back the old objects
	CDC& memDC = m_dcBackground.GetDC();
	m_GraphFont.CreatePointFont(9*10,_T("Arial"), &memDC);
	memDC.SelectObject(&m_GraphFont);
	TEXTMETRIC textmetrics;
	memDC.GetTextMetrics(&textmetrics);
 	m_iCharHeight=textmetrics.tmHeight;
 	m_iCharWidth=textmetrics.tmAveCharWidth;
	const int32_t strlen = max(m_szXLegend[eGridSymbol::eSetpoint].GetLength(), m_szXLegend[eGridSymbol::eMassflow].GetLength()) + 2;
	m_aDrawRect = CRect(m_aFillRect.left + 7 * m_iCharWidth, m_aFillRect.top + m_iCharHeight, m_aFillRect.right - 1 * m_iCharWidth, m_aFillRect.bottom - 3 * m_iCharHeight);
	m_aLegendTextRect = CRect(CPoint(0, 0), CSize(strlen * m_iCharWidth, m_iCharHeight));
	m_bInit = TRUE;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawBackground (CDC* pDC)
{
	pDC->FillSolidRect(m_aFillRect, c_aBkColor);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::Draw(CDC* pDC)
{
	DrawBackground (pDC);
	CalcXAxisGridAndTicks ();
	DrawGrid(pDC);
	DrawYAxis(pDC);
	DrawXLegend(pDC);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
int32_t CBarChart::CalcYGridRaster(const float32_t& fGridStep) const
{
	if ( fGridStep < 1.0f)
		return 1;
	if ( fGridStep < 2.0f)
		return 2;
	if ( fGridStep < 5.0f)
		return 5;
	if ( fGridStep < 10.0f)
		return 10;
	if ( fGridStep < 20.0f)
		return 20;
	if ( fGridStep < 50.0f)
		return 50;
	if ( fGridStep < 100.0f)
		return 100;
	if ( fGridStep < 150.0f)
		return 150;
	if ( fGridStep < 200.0f)
		return 200;
	if ( fGridStep < 500.0f)
		return 500;
	if ( fGridStep < 1000.0f)
		return 1000;
	if ( fGridStep < 1500.0f)
		return 1500;
	if ( fGridStep < 2000.0f)
		return 2000;
	if ( fGridStep < 5000.0f)
		return 5000;
	return 10000;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::CalcXAxisGridAndTicks()
{
	if ( m_fMaxwert < 10.0f )
	{
		m_iyMaxwert				= 10;
	}
	else if ( m_fMaxwert < 100.0f )
	{
		m_iyMaxwert = _S32((m_fMaxwert + 9.99f) / 10.0f) * 10;
	}
	else if ( m_fMaxwert < 1000.0f )
	{
		m_iyMaxwert = _S32((m_fMaxwert + 99.99f) / 100.0f) * 100;
	}
	else
	{
		m_iyMaxwert = _S32((m_fMaxwert + 999.99f) / 1000.0f) * 1000;
	}
	const float32_t ik = _F32(m_aDrawRect.Height()) / (2*m_iCharHeight);
	const float32_t fGridStep	= (ik)?(float32_t)m_iyMaxwert / ik : 0;
	m_iyGridraster = CalcYGridRaster(fGridStep);

	m_fyPixel = _F32(m_aDrawRect.Height()) / m_iyMaxwert;
	m_fxPixel = _F32(m_aDrawRect.Width())  / c_ixMaxwert;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawGrid(CDC* pDC)
{
	//need a pen of colour m_crGridColor

	CPen pen1(PS_DOT,1,c_aGridColor);
	CPen   *poldpen	  = pDC->SelectObject(&pen1);

	for ( int32_t iyGrid = m_iyGridraster; iyGrid < m_iyMaxwert; iyGrid += m_iyGridraster)
	{
		const int32_t y = _S32(m_aDrawRect.bottom-m_fyPixel*iyGrid);
		pDC->MoveTo(m_aDrawRect.left, y);
		pDC->LineTo(m_aDrawRect.right,y);
	}
	for ( int32_t index = 0; index < c_ixMaxwert; index++)
	{
		const int32_t x = _S32(m_aDrawRect.left+m_fxPixel*index);
		pDC->MoveTo( x, m_aDrawRect.top);
		pDC->LineTo( x, m_aDrawRect.bottom);
	}

	CPen pen2(PS_SOLID,1,c_aGridColor);
	pDC->SelectObject(&pen2);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(m_aDrawRect);
	// cleanup
	pDC->SelectObject(poldpen);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawXLegend(CDC* pDC)
{
	auto oldBkMode       = pDC->SetBkMode(TRANSPARENT);
	auto oldTextColor   = pDC->SetTextColor(RGBCOLORBLACK);
	CPen   *poldpen	    = pDC->SelectObject(&m_BlackPen);
	CFont *poldfont     = pDC->SelectObject(&m_GraphFont);

	CRect aLegendRect(CPoint(0, 0), CSize(m_iCharHeight / 2, m_iCharHeight / 2));
	for ( int32_t i = 0; i < c_ixMaxwert; i++)
	{
		CBrush aBrush(c_aColorRef[i]);
		CBrush   *poldbrush	  = pDC->SelectObject(&aBrush);
		CPoint p1(m_aDrawRect.left , m_aDrawRect.bottom + m_iCharHeight/2 + i*m_iCharHeight);

		CRect aRect{ aLegendRect };
		aRect.OffsetRect(p1);
		pDC->Rectangle( &aRect);
		pDC->SelectObject(poldbrush);

		aRect = m_aLegendTextRect;
		CPoint p2(p1.x + 2*m_iCharWidth, p1.y-m_iCharHeight/4);
		aRect.OffsetRect(p2);
		pDC->DrawText(m_szXLegend[i],&aRect,DT_LEFT/*|DT_WORDBREAK|DT_END_ELLIPSIS*/);
    }
	pDC->SelectObject(poldfont);
	pDC->SelectObject(poldpen);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawYAxis(CDC* pDC)
{
	CRect aRect;
	CString m_szYLegendText;
	//draw the title using the specified colorscheme
	//using the graph font
	//note we must clear of any old stuff crap fom this area
	aRect.left  = m_aDrawRect.left-7*m_iCharWidth;
	aRect.right = m_aDrawRect.left-1*m_iCharWidth;

	auto oldBkMode = pDC->SetBkMode(TRANSPARENT);
	auto oldTextColor = pDC->SetTextColor(RGBCOLORBLACK);
	CFont *poldfont = pDC->SelectObject(&m_GraphFont);
	
	for ( int32_t iyGrid = 0; iyGrid < m_iyMaxwert; iyGrid += m_iyGridraster)
	{
		const int32_t y = _S32(m_aDrawRect.bottom-m_fyPixel*iyGrid);
		aRect.top    = y - m_iCharHeight/2;
		aRect.bottom = y + m_iCharHeight/2;
		m_szYLegendText.Format(_T("%d"), iyGrid);
		pDC->DrawText(m_szYLegendText,&aRect,DT_RIGHT|DT_WORDBREAK|DT_END_ELLIPSIS);
	}
	//letzter Eintrag
	const int32_t y = _S32(m_aDrawRect.top);
	aRect.top    = y - m_iCharHeight/2;
	aRect.bottom = y + m_iCharHeight/2;
	m_szYLegendText.Format(_T("%d"), m_iyMaxwert);
	pDC->DrawText(m_szYLegendText,&aRect,DT_RIGHT|DT_WORDBREAK|DT_END_ELLIPSIS);

	pDC->SelectObject(poldfont);
	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(oldBkMode);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CRect CBarChart::CalcBarRect(const int32_t index) const
{
	const float32_t c_spacing = 0.2F;
	const int32_t c_Offset = _S32(c_spacing * m_fxPixel);
	const int32_t c_left = m_aDrawRect.left + _S32(index * m_fxPixel);
	CRect aRect;
	aRect.left		= c_left + c_Offset;
	aRect.right		= c_left + _S32(m_fxPixel) - c_Offset;
	aRect.top		= m_aDrawRect.bottom  - _S32(m_fyPixel * m_fValue[index]);
	aRect.top		= max(m_aDrawRect.top, aRect.top);
	aRect.bottom	= m_aDrawRect.bottom;
	return aRect;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
BOOL  CBarChart::CheckBarRect()
{
	BOOL bChanged = FALSE;
	for (int32_t index = 0; index < c_ixMaxwert; index++)
	{
		CRect aRect{ CalcBarRect(index) };
		if (aRect != m_BarRect[index])
		{
			m_BarRect[index] = aRect;
			bChanged = TRUE;
		}
		auto szValue { FormatMetric2(m_fValue[index]) };
		if (szValue != m_szValue[index])
		{
			m_szValue[index] = szValue;
			bChanged = TRUE;
		}
	}
	return bChanged;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawBar(CDC *pDC)
{
	CPen   *poldpen	   = pDC->SelectObject(&m_BlackPen);
	CBrush * poldbrush = pDC->SelectObject(&m_BarBrush[0]);
	for ( int32_t index = 0; index < c_ixMaxwert; index++)
	{
		if (index > 0)
		{
			pDC->SelectObject(&m_BarBrush[index]);
		}
		pDC->Rectangle( &m_BarRect[index]);
	}
	pDC->SelectObject(poldbrush);
	pDC->SelectObject(poldpen);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::DrawValues(CDC* pDC)
{
	CFont* poldfont = pDC->SelectObject(&m_GraphFont);
	auto iOldCR = pDC->SetTextColor(COLORBLACK);
	auto iOldBK = pDC->SetBkMode(TRANSPARENT);
	for (int32_t index = 0; index < c_ixMaxwert; index++)
	{
		if (m_fValue[index] > 0.0F)
		{
			CRect aRect;
			aRect.left = m_aDrawRect.left + _S32(index * m_fxPixel);
			aRect.right = aRect.left + _S32(m_fxPixel);
			aRect.bottom = m_BarRect[index].top;
			aRect.top = aRect.bottom - m_iCharHeight;
			pDC->DrawText(m_szValue[index], &aRect, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
		}
	}
	pDC->SelectObject(poldfont);
	pDC->SetTextColor(iOldCR);
	pDC->SetBkMode(iOldBK);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart::Rescale(const float32_t fMaxwert)
{
	if ( fMaxwert != m_fMaxwert )
	{
		m_fMaxwert = fMaxwert;
		Draw	 (&m_dcBackground.GetDC());
		m_bRepaint = TRUE;
	}
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CBarChart:: Update	(CDC* pDC, const float32_t fSettpoint, const float32_t fMassflow)
{
	BOOL bRepaint = ( m_bRepaint) || ( fabs(fSettpoint - m_fValue[eGridSymbol::eSetpoint]) > 1e-02) || ( fabs(fMassflow-m_fValue[eGridSymbol::eMassflow]) > 1e-02 );
	if (bRepaint)
	{
		m_fValue[eGridSymbol::eSetpoint]  = fSettpoint;
		m_fValue[eGridSymbol::eMassflow]   = fMassflow;

		m_bRepaint |= CheckBarRect();
		if (m_bRepaint)
		{
			auto& rDC = m_dcCache.GetDC();
			rDC.BitBlt(0, 0, m_aRect.Width(), m_aRect.Height(), &m_dcBackground.GetDC(), 0, 0, SRCCOPY);
			DrawBar(&rDC);
			DrawValues(&rDC);
			m_bRepaint = FALSE;
		}
	}
	pDC->BitBlt(m_aRect.left, m_aRect.top, m_aRect.Width(), m_aRect.Height(), &m_dcCache.GetDC(), 0, 0, SRCCOPY);
}
/// @}
