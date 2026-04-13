//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Implementation of module LineTotalizerBox
///
/// @file   LineTotalizerBox.cpp
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
#include "LineTotalizerBox.h"
#include "FormatMetric.h"


#define STARTY		4
#define LEFTCOLUMN	4
#define OFFSETLINE	27

static const POINT cTotalizerPoint		= { LEFTCOLUMN, STARTY 			};
static const POINT cTimePoint			= { LEFTCOLUMN, STARTY  + OFFSETLINE };


//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CLineTotalizerBox :: CLineTotalizerBox() :  m_szProduktionsdauer(L"")
											, m_szTotalizer(L"")
											, m_Dauer(0)
											, m_fTotalizer(0.0f)
											, m_bRepaint(FALSE)
											, m_bInit(FALSE)
{
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CLineTotalizerBox :: 	~CLineTotalizerBox(void)
{}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
CSize  CLineTotalizerBox::CalcBoxSize(CDC* pDC) 
{
	auto pOld = pDC->SelectObject(&m_fontValue);
	TEXTMETRIC textmetrics;
	pDC->GetTextMetrics(&textmetrics);

	pDC->SelectObject(pOld);
	auto iCharHeight = textmetrics.tmHeight;
	auto iCharWidth = textmetrics.tmAveCharWidth;

	CRect aDrawRect;
	auto l1 = m_szProduktionsdauer.GetLength();
	auto l2 = m_szTotalizer.GetLength();
	auto l = max(l1, l2) + 2;

	auto szV1 = FORMATKG(100000000);
	auto szV2 = FORMATTIME(1000*60*60);
	auto v1 = szV1.GetLength();
	auto v2 = szV2.GetLength();
	auto v = max(v1, v2) + 2;

	auto t = l + v;
	auto ix = iCharWidth * t + LEFTCOLUMN;
	auto iy = 2*iCharHeight + OFFSETLINE + STARTY;

	return CSize(ix, iy);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void	CLineTotalizerBox :: Create(CDC* pDC, const CPoint& aP)
{
	ASSERT(!m_bInit);
	m_fontValue.CreateFont(18, 0, 0, 0, 400, FALSE, FALSE, 0, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	VERIFY(m_szProduktionsdauer.LoadString(IDS_PRODUCTIONTIME));
	VERIFY(m_szTotalizer.LoadString(IDS_TOTALTOTALISATOR));


	auto rSize = CalcBoxSize(pDC);
	m_aFillRect = CRect(CPoint(0, 0), rSize);
	m_aRect = m_aFillRect;
	m_aRect.OffsetRect(aP);
	m_dcBackground.Create(pDC, rSize);
	m_bRepaint = TRUE;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CLineTotalizerBox::DrawBackground(CDC* pDC)
{
	pDC->FillSolidRect(m_aFillRect, c_aBkColor);
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CLineTotalizerBox::DrawValues(CDC *pDC)
{
	CString szTemp;
	auto iOldCR = pDC->SetTextColor(COLORBLACK) ;
	auto iOldBK = pDC->SetBkMode(OPAQUE);
	auto iOldTA =  pDC->SetTextAlign(TA_LEFT|TA_TOP) ;
	CFont *oldFont = pDC->SelectObject(&m_fontValue) ;
	auto szValue = FORMATKG(m_fTotalizer);
	szTemp.Format(_T("%s : %s"), LPCTSTR(m_szTotalizer), LPCTSTR(szValue));
	pDC->TextOut(cTotalizerPoint.x, cTotalizerPoint.y, szTemp);

	szValue = FORMATTIME(m_Dauer);
	szTemp.Format(_T("%s : %s"), LPCTSTR(m_szProduktionsdauer), LPCTSTR(szValue));
	pDC->TextOut(cTimePoint.x, cTimePoint.y, szTemp);

	pDC->SelectObject(oldFont) ;
	pDC->SetTextAlign(iOldTA);
	pDC->SetBkMode(iOldBK);
	pDC->SetTextColor(iOldCR) ;
}
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void CLineTotalizerBox::Update(CDC* pDC, const float32_t fTotalizer, const uint32_t Dauer)
{
	CDC& rDC = m_dcBackground.GetDC();
	BOOL bRepaint = m_bRepaint || (_F32(fabs(fTotalizer - m_fTotalizer)) > 1e-02f) || (Dauer != m_Dauer);
	if ( bRepaint )
	{
		m_bRepaint = FALSE;
		m_fTotalizer = fTotalizer;
		m_Dauer = Dauer;

		DrawBackground	(&rDC);
		DrawValues		(&rDC);
	}
	pDC->BitBlt(m_aRect.left, m_aRect.top, m_aRect.Width(), m_aRect.Height(), &rDC, 0, 0, SRCCOPY) ;
} // end UpdateFilling

