//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseButtonGrafikContext
///
/// @file   DoseButtonGrafikContext.cpp
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
#include "DoseButtonGrafikContext.h"
#include "EasyControl_DEU/resource.h"
#include "MFCMacros.h"


//**************************************************************************************************
void CDoseButtonGrafikContext::FlashButton(CDC* pDC)
{
	CBitmapDC aBitmap(pDC, IDB_DOSEBUTTONPRESSED);

	auto& dc = aBitmap.GetDC();
	auto bmsize = aBitmap.GetBitmapDimensions();
	TransparentBlt(pDC->GetSafeHdc(), m_DisplayRect.left, m_DisplayRect.top, m_DisplayRect.Width(), m_DisplayRect.Height(), dc, 0,0, bmsize.cx, bmsize.cy, RGB(255, 255, 255));
}











