//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DisplayContainer
///
/// @file   DisplayContainer.h
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

#include <array>
#include "BASE/include/DoseType.h"
#include "DoseGrafikContext.h"


class CDoseDisplayItem
{
	CRect	m_OrgRect;
	CRect	m_DisplayRect;
	CRect	m_SaveDisplayRect;
	CDoseGrafikContext* m_pGrafikContext;

public:
	CDoseDisplayItem() : m_OrgRect{ 0,0,0,0 }
		, m_DisplayRect{ 0,0,0,0 }
		, m_SaveDisplayRect{ 0,0,0,0 }
		, m_pGrafikContext{ nullptr }
	{}

	CDoseDisplayItem(const CDoseDisplayItem&)			  = delete;
	CDoseDisplayItem& operator = (const CDoseDisplayItem&) = delete;

	virtual ~CDoseDisplayItem()
	{	
		ASSERT(! m_pGrafikContext);
		DeleteGrafikContext();	
	}

	void Reset()
	{
		DeleteGrafikContext();
		m_OrgRect.SetRectEmpty();
		m_DisplayRect.SetRectEmpty();
		m_SaveDisplayRect.SetRectEmpty();
	}

	void DeleteGrafikContext()
	{	DELETE_OBJECT(m_pGrafikContext);	}

	void SetGrafikContext(CDoseGrafikContext* pV)
	{	
		ASSERT(!m_pGrafikContext);
		DeleteGrafikContext();
		m_pGrafikContext = pV;
	}
	const CDoseGrafikContext* GetGrafikContext(void) const
	{	return m_pGrafikContext;	}

	CDoseGrafikContext* GetGrafikContext(void)
	{	return m_pGrafikContext;	}

	SETGET(const CRect&, OrgRect)
	SETGET(const CRect&, DisplayRect)
	SETGET(const CRect&, SaveDisplayRect)
};
//********************************************************************************************************
//********************************************************************************************************
class CDisplayContainer
{
public:
	enum class eDisplayItem : int32_t
	{
		eFeederDisplay = 0,
		eDoseBaseActualGrafik,
		eDoseBaseNominalGrafik,
		eDoseButtonGrafik,
		eDoseRefillReleaseButtonGrafik,
		eDoseRefillButtonGrafik,
		eDoseProductNameGrafik,
		eDoseDisplayMax
	};

private:
	std::array< CDoseDisplayItem, _S32(eDisplayItem::eDoseDisplayMax)> m_DisplayItems;
	base::eDoseType m_DoseType;

private:
	static CRect CalcDisplayRect(const CRect& rRect, const CPoint& cp, int32_t scale);
	void DeleteGrafikContext(const eDisplayItem item);

	void SetGrafikContext(const eDisplayItem item, CDoseGrafikContext*);
	const CDoseGrafikContext* GetGrafikContext(const eDisplayItem item) const;

	void Close();

public:
	CDisplayContainer() : m_DisplayItems{}
		, m_DoseType { base::eDoseType ::DOSETYPE_NONE }
	{}

	CDisplayContainer(const CDisplayContainer&)              = delete;
	CDisplayContainer& operator = (const CDisplayContainer&) = delete;

	virtual ~CDisplayContainer()
	{	Close();	}

	CDoseDisplayItem& GetDisplayItem(const eDisplayItem item)
	{	return m_DisplayItems[_S32(item)];	}

	const CDoseDisplayItem& GetDisplayItem(const eDisplayItem item) const
	{	return m_DisplayItems[_S32(item)];	}

	CRect GetDisplayBoundingRect() const;
	CRect GetSaveDisplayBoundingRect() const;
	void  OffsetDisplayBoundingRect(int32_t x, int32_t y);
	CRect GetBoundingRect() const;
	void  Reset();

	BOOL SearchPoint(const CPoint& aP, eDisplayItem& rItem) const;

	void CalcDisplay(const CPoint& cp, int32_t scale);

	void ResetDisplay();
	void DeleteGrafikContext();

	void SetOrgRect(const eDisplayItem item, const CRect&);
	const CRect& GetOrgRect(const eDisplayItem item) const;

	void SetDisplayRect(const eDisplayItem item, const CRect&);
	const CRect& GetDisplayRect(const eDisplayItem item) const;

	SETGET(base::eDoseType, DoseType);
};
