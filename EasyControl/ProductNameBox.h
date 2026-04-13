//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y T O U C H   >>>
///
///
/// @brief  Declaration of module ProductNameBox
///
/// @file   ProductNameBox.h
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

class CProductNameBox
{
	CBitmapDC		m_dcBackground;
	CRect			m_aRect;
	CRect			m_aFillRect;
	CFont			m_fontValue;
	CString			m_szName;
	CPoint			m_aP;
	BOOL			m_bRepaint;
	BOOL			m_bInit;

	static const COLORREF  c_aBkColor = WHITECOLOR;

private:
	void	DrawBackground(CDC* pDC);
	void	DrawValues(CDC* pDC);
	CSize   CalcBoxSize(CDC* pDC) ;
	void    CalcRect(CDC* pDC);

public:
	void Create(CDC* pDC, const CPoint& aP);

	CProductNameBox();
	~CProductNameBox(void);

	void Update(CDC* pDC, const CString& szName);
	void ForceRepaint(void)
	{ m_bRepaint = TRUE;	}

};




/// @}
