//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DisplayContainer
///
/// @file   DisplayContainer.cpp
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
#include "DisplayContainer.h"

//*****************************************************************************************************************
//*****************************************************************************************************************
CRect CDisplayContainer::GetDisplayBoundingRect() const
{
	CRect aRect;
	aRect.SetRectEmpty();
	for (const auto& rItem : m_DisplayItems)
	{
		CRect bRect;
		bRect.UnionRect(aRect, rItem.GetDisplayRect());
		aRect = bRect;
	}
	return aRect;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
CRect CDisplayContainer::GetSaveDisplayBoundingRect() const
{
	CRect aRect;
	aRect.SetRectEmpty();
	for (const auto& rItem : m_DisplayItems)
	{
		CRect bRect;
		bRect.UnionRect(aRect, rItem.GetSaveDisplayRect());
		aRect = bRect;
	}
	return aRect;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
CRect CDisplayContainer::GetBoundingRect() const
{
	CRect aRect;
	aRect.SetRectEmpty();
	for (const auto& rItem : m_DisplayItems)
	{
		CRect bRect;
		aRect.UnionRect(aRect, rItem.GetOrgRect());
		aRect = bRect;
	}
	return aRect;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDisplayContainer::Reset()
{
	for (auto& rItem : m_DisplayItems)
	{
		rItem.Reset();
	}
	SetDoseType(base::eDoseType::DOSETYPE_NONE);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDisplayContainer::Close()
{
	for (auto& rItem : m_DisplayItems)
	{
		rItem.DeleteGrafikContext();
	}
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDisplayContainer::SearchPoint(const CPoint& aP, eDisplayItem& rItem) const
{
	BOOL result = FALSE;

	rItem = eDisplayItem::eDoseDisplayMax;
	for (int32_t index = 0; index < _S32(eDisplayItem::eDoseDisplayMax); index++)
	{
		const auto& rC = m_DisplayItems[index];
		if (rC.GetGrafikContext() &&
			rC.GetDisplayRect().PtInRect(aP))
		{
			result = TRUE;
			rItem = static_cast<eDisplayItem>(index);
			break;
		}
	}
	return result;
}
//***********************************************************************************************************
//***********************************************************************************************************
CRect CDisplayContainer::CalcDisplayRect(const CRect& rRect, const CPoint& cp, int32_t scale)
{
	CRect rect{ rRect };

	scale = RANGE(scale, 100, 200);
	rect.OffsetRect(-cp.x, -cp.y);
	auto top = NEXTLONG((rect.top * scale) / 100.0F);
	auto left = NEXTLONG((rect.left * scale) / 100.0F);
	auto right = NEXTLONG((rect.right * scale) / 100.0F);
	auto bottom = NEXTLONG((rect.bottom * scale) / 100.0F);
	rect = CRect(left, top, right, bottom);
	rect.OffsetRect(cp.x, cp.y);
	return std::move(rect);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::CalcDisplay(const CPoint& cp, int32_t scale)
{
	for (auto& rItem : m_DisplayItems)
	{
		auto aRect{ CalcDisplayRect(rItem.GetOrgRect(), cp, scale) };
		rItem.SetDisplayRect(aRect);
		rItem.SetSaveDisplayRect(aRect);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::ResetDisplay()
{
	for (auto& rItem : m_DisplayItems)
	{
		rItem.SetDisplayRect(rItem.GetOrgRect());
		rItem.SetSaveDisplayRect(rItem.GetOrgRect());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::DeleteGrafikContext()
{
	for (auto& rItem : m_DisplayItems)
	{
		rItem.DeleteGrafikContext();
	}
	SetDoseType(base::eDoseType::DOSETYPE_NONE);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::DeleteGrafikContext(const eDisplayItem item)
{
	auto& rItem = GetDisplayItem(item);
	rItem.DeleteGrafikContext();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::SetGrafikContext(const eDisplayItem item, CDoseGrafikContext* pV)
{
	auto& rItem = GetDisplayItem(item);
	ASSERT(!rItem.GetGrafikContext());
	rItem.SetGrafikContext(pV);
}
//***********************************************************************************************************
//***********************************************************************************************************
const CDoseGrafikContext* CDisplayContainer::GetGrafikContext(const eDisplayItem item) const
{
	const auto& rItem = GetDisplayItem(item);
	return rItem.GetGrafikContext();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::SetOrgRect(const eDisplayItem item, const CRect& rRect)
{
	auto& rItem = GetDisplayItem(item);
	rItem.SetOrgRect(rRect);
}
//***********************************************************************************************************
//***********************************************************************************************************
const CRect& CDisplayContainer::GetOrgRect(const eDisplayItem item) const
{
	const auto& rItem = GetDisplayItem(item);
	return rItem.GetOrgRect();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDisplayContainer::SetDisplayRect(const eDisplayItem item, const CRect& rRect)
{
	auto& rItem = GetDisplayItem(item);
	rItem.SetDisplayRect(rRect);
}
//***********************************************************************************************************
//***********************************************************************************************************
const CRect& CDisplayContainer::GetDisplayRect(const eDisplayItem item) const
{
	const auto& rItem = GetDisplayItem(item);
	return rItem.GetDisplayRect();
}
//***********************************************************************************************************
//***********************************************************************************************************
void  CDisplayContainer::OffsetDisplayBoundingRect(int32_t x, int32_t y)
{
	for (auto& rItem : m_DisplayItems)
	{
		CRect aRect{ rItem.GetDisplayRect() };
		aRect.OffsetRect(x, y);
		rItem.SetSaveDisplayRect(aRect);
		rItem.SetDisplayRect(aRect);
	}
}





