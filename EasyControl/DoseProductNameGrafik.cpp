//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseProductNameGrafik
///
/// @file   DoseProductNameGrafik.cpp
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
#include "DoseProductNameGrafik.h"
#include "StringConvert.h"


//***********************************************************************************************************
//***********************************************************************************************************
CDoseProductNameGrafik* CDoseProductNameGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	auto pBase = new CDoseProductNameGrafik(rRect);
	ASSERT(pBase);
	pBase->SetDoseType(lTyp);
	return pBase;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseProductNameGrafik::OnUpdate(CDC* pDC)
{
	const auto& rName = GetName();
	if ( rName.length() > 0 )
	{
		auto szName = toCString(rName);
		auto length = szName.GetLength();
		CSize aSize;
		getTextMetrics(pDC, aSize);
		const auto dx = length * aSize.cx;
		POINT p0;
		p0.x = (dx < m_DisplayRect.Width()) ? ((m_DisplayRect.Width() - dx) / 2) : 0;
		p0.y = 1;
		TextOut(pDC, p0, szName);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseProductNameGrafik::IsTextUpdate(void) const
{
	return m_StatusFlags.flags.bNameChanged;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseProductNameGrafik::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	m_StatusFlags.flags.bNameChanged = IsNameChanged(rNewData.m_szName);
}

