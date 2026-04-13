//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Implementation of module ProductNameBox
///
/// @file   ProductNameBox.cpp
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
#include "ProductNameBox.h"
#include "FormatMetric.h"


#define STARTY		4
#define LEFTCOLUMN	4
#define OFFSETLINE	1

static const POINT cNamePoint		= { LEFTCOLUMN, STARTY 			};


//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CProductNameBox :: CProductNameBox() :  m_szName(L"")
										, m_bRepaint(FALSE)
										, m_bInit(FALSE)
{
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CProductNameBox :: 	~CProductNameBox(void)
{}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void	CProductNameBox :: Create(CDC* pDC, const CPoint& aP)
{
	ASSERT(!m_bInit);
	m_fontValue.CreateFont(18, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_aP = aP;
	auto rSize = CalcBoxSize(pDC);
	m_aFillRect = CRect(CPoint(0, 0), rSize);
	m_aRect = m_aFillRect;
	m_aRect.OffsetRect(aP);
	m_dcBackground.Create(pDC, rSize);
	m_bRepaint = TRUE;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CProductNameBox::DrawBackground(CDC* pDC)
{
	pDC->FillSolidRect(m_aFillRect, c_aBkColor);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CProductNameBox::DrawValues(CDC *pDC)
{
	CString szTemp;
	auto iOldCR = pDC->SetTextColor(COLORBLACK) ;
	auto iOldBK = pDC->SetBkMode(OPAQUE);
	auto iOldTA =  pDC->SetTextAlign(TA_CENTER|TA_TOP) ;
	CFont *oldFont = pDC->SelectObject(&m_fontValue) ;
	pDC->TextOut(cNamePoint.x, cNamePoint.y, m_szName);

	pDC->SelectObject(oldFont) ;
	pDC->SetTextAlign(iOldTA);
	pDC->SetBkMode(iOldBK);
	pDC->SetTextColor(iOldCR) ;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CSize  CProductNameBox::CalcBoxSize(CDC* pDC)
{
	auto pOld = pDC->SelectObject(&m_fontValue);
	TEXTMETRIC textmetrics;
	pDC->GetTextMetrics(&textmetrics);

	pDC->SelectObject(pOld);
	auto iCharHeight = textmetrics.tmHeight;
	auto iCharWidth = textmetrics.tmAveCharWidth;

	CRect aDrawRect;
	auto l = m_szName.GetLength();
	auto ix = iCharWidth * l + LEFTCOLUMN;
	auto iy = iCharHeight + OFFSETLINE + STARTY;

	return CSize(ix, iy);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void  CProductNameBox::CalcRect(CDC* pDC)
{
	auto rSize = CalcBoxSize(pDC);

	auto aFillRect = CRect(CPoint(0, 0), rSize);
	auto aRect = aFillRect;
	aRect.OffsetRect(m_aP);
	if (aRect.Size() != m_dcBackground.GetBitmapDimensions())
	{
		m_dcBackground.Create(pDC, aRect.Size());
	}
	m_aRect = aRect;
	m_aFillRect = aFillRect;
}

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CProductNameBox::Update(CDC* pDC, const CString& szName)
{
	CDC& rDC = m_dcBackground.GetDC();
	BOOL bRepaint = m_bRepaint || (szName != m_szName);
	if ( bRepaint )
	{
		m_bRepaint = FALSE;
		m_szName = szName;

		CalcRect(pDC);

		DrawBackground	(&rDC);
		DrawValues		(&rDC);
	}
	pDC->BitBlt(m_aRect.left, m_aRect.top, m_aRect.Width(), m_aRect.Height(), &rDC, 0, 0, SRCCOPY) ;
} // end UpdateFilling

