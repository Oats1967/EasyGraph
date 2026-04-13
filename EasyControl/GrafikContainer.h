//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GrafikContainer
///
/// @file   GrafikContainer.h
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

#include "EasyControl.h"
#include "FeederDisplay.h"

class CGrafikContainer : public CStatic
{
    CFeederDisplay* m_pGrafik;

public:
	CGrafikContainer(CFeederDisplay* pGrafik = nullptr) : m_pGrafik(pGrafik)
	{}

	virtual ~CGrafikContainer() = default;

	CFeederDisplay& GetFeederDisplay()
    {  return *m_pGrafik;  }

	void SetFeederDisplay(CFeederDisplay* pGrafik)
	{ m_pGrafik = pGrafik;}

	void Update(CDC* pDC);

	void Invalidate();

	void Redraw();
};

//*******************************************************************************************************
//*******************************************************************************************************
inline void CGrafikContainer::Update(CDC* pDC)
{
	ASSERT(m_pGrafik);
	m_pGrafik->Update(pDC, *this);
}
//*******************************************************************************************************
//*******************************************************************************************************
inline void CGrafikContainer::Invalidate()
{
	ASSERT(m_pGrafik);
	CStatic::Invalidate(m_pGrafik->IsModified());
}
//*******************************************************************************************************
//*******************************************************************************************************
inline void CGrafikContainer::Redraw()
{
	ASSERT(m_pGrafik);
	m_pGrafik->SetAllFlags();
}







