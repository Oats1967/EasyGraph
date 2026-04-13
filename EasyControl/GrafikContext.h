//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GrafikContext
///
/// @file   GrafikContext.h
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

#include "BASE/Base.h"
#include "BitmapDC.h"
#include "MFCMacros.h"



class CGrafikContext
{
private:
	CFont m_fontValue;
	BOOL m_TextUpdate;

protected:
	CBitmapDC m_dcMem;
	CBitmapDC m_dcImage;
	CRect	  m_OrgRect;
	CRect	  m_DisplayRect;

protected:
	void TextOut(CDC* pDC, const POINT& p, const CString& sz, COLORREF color = COLORBLACK,  int32_t bkmode = OPAQUE);

	void getTextMetrics(CDC* pDC, CSize&);

private:
	void Update(CDC* pDC, uint32_t bkmode = OPAQUE);

protected:
	void ShowBitmap(CDC* pDC, uint32_t nRessourceID, const CPoint& aP = CPoint(0,0));

protected:
	virtual void OnRedraw(CDC* pDC) = 0;
	virtual void OnUpdate(CDC* pDC) = 0;
	virtual BOOL IsTextUpdate(void) const = 0;
	virtual BOOL IsBitmapUpdate(void) const = 0;

private:
	CGrafikContext() = delete;

public:
	CGrafikContext(const CRect& rSize);
	virtual ~CGrafikContext();
	CGrafikContext(const CGrafikContext&) = delete;
	CGrafikContext& operator = (const CGrafikContext&) = delete;

	void ShowGrafik(CDC* pDC, const CRect& rDisplayrect, uint32_t mode = OPAQUE);
};





