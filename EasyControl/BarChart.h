//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Declaration of module BarChart
///
/// @file   BarChart.h
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

#include "Colors.def"
#include <array>
#include "BitmapDC.h"


class CBarChart
{
	enum eGridSymbol
	{
		eSetpoint = 0,
		eMassflow,
		eMax
	};
	CBitmapDC	m_dcBackground;
	CBitmapDC	m_dcCache;
	CBrush		m_BarBrush[eGridSymbol::eMax];
	CPen		m_BlackPen;
	CFont		m_GraphFont;
	CRect		m_aRect;								// Gesamtes Rechteck
	CRect		m_aFillRect;							// Nur zum Füller
	CRect		m_aDrawRect;
    CRect		m_aLegendTextRect;
	CRect		m_BarRect[eGridSymbol::eMax];
	int32_t 	m_iCharHeight;
 	int32_t		m_iCharWidth;
	float32_t	m_fMaxwert;
	float32_t	m_fValue[eGridSymbol::eMax];
	int32_t		m_iyMaxwert;
	int32_t		m_iyGridraster;
	float32_t	m_fyPixel;
	float32_t	m_fxPixel;
	BOOL		m_bRepaint;
	BOOL		m_bInit;
	CString		m_szXLegend[eGridSymbol::eMax];
	CString		m_szValue[eGridSymbol::eMax];

	static const int32_t   c_ixMaxwert			= eGridSymbol::eMax;
	static const COLORREF  c_aColorRef[eGridSymbol::eMax];
	static const COLORREF  c_aGridColor		 = RGBCOLORLIGHTGRAY;
	static const COLORREF  c_aBkColor		 = WHITECOLOR;


private:
	void DrawBackground (CDC*);
	void Draw			(CDC*);
	void DrawGrid		(CDC*);
	void DrawYAxis		(CDC*);
	void DrawXLegend	(CDC*);
	void DrawBar(CDC*);
	void DrawValues(CDC* pDC);

	CRect CalcBarRect		(const int32_t index) const;
	BOOL  CheckBarRect() ;
	int32_t CalcYGridRaster	(const float32_t& fGridStep) const;
	void CalcXAxisGridAndTicks(void);

public:
	CBarChart(void);
	virtual ~CBarChart(void);

	void Create (CDC* pDC, const CRect& aRect);
	void Rescale(const float32_t fMaxwert);
	void Update	(CDC* pDC, const float32_t fSollwert, const float32_t fIstwert);
	void ForceRepaint(void)
	{ m_bRepaint = TRUE; }

	BOOL IsInit() const
	{return m_bInit;	}
};
/// @}
