//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Declaration of module LineTotalizerBox
///
/// @file   LineTotalizerBox.h
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
#include "BitmapDC.h"

class CLineTotalizerBox
{
	CBitmapDC		m_dcBackground;
	CRect			m_aRect;
	CRect			m_aFillRect;
	CFont			m_fontValue;
	CString			m_szProduktionsdauer;
	CString			m_szTotalizer;
	uint32_t		m_Dauer;
	float32_t		m_fTotalizer;
	BOOL			m_bRepaint;
	BOOL			m_bInit;

	static const COLORREF  c_aBkColor = WHITECOLOR;

private:
	void	DrawBackground(CDC* pDC);
	void	DrawValues(CDC* pDC);
	CSize   CalcBoxSize(CDC* pDC) ;

public:
	void Create(CDC* pDC, const CPoint& aP);

	CLineTotalizerBox();
	~CLineTotalizerBox(void);

	void Update(CDC* pDC, const float32_t fTotalizer, const uint32_t Dauer);
	void ForceRepaint(void)
	{ m_bRepaint = TRUE;	}

};




/// @}
