//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseBaseActualGrafik
///
/// @file   DoseBaseActualGrafik.h
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

#include "DoseGrafikContext.h"
#include "BASE/include/FeedingTypeMap.h"



class CDoseBaseActualGrafik : public CDoseGrafikContext
{
	using CBaseClass = CDoseGrafikContext;

private:
	union DosierBaseStatus
	{
		struct
		{
			uint32_t bLocalModeChanged : 1;
			uint32_t bFeedingTypeChanged : 1;
			uint32_t bRunningChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

private:
	const POINT cOrg;
	const POINT cFeederNamePoint;
	const POINT cFeederRunPoint;
	const base::CFeedingTypeMap m_FeedingTypeMap;

private:
	virtual void DrawStatus(CDC* pDC);
	virtual void DrawFeedingType(CDC* pDC);

protected:
	void OnRedraw(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;


private:
	CDoseBaseActualGrafik(void) = delete;

public:
	static CDoseBaseActualGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseBaseActualGrafik(const CRect& rRect);
	~CDoseBaseActualGrafik(void) override = default;
	CDoseBaseActualGrafik(const CDoseBaseActualGrafik&) = delete;
	CDoseBaseActualGrafik& operator = (const CDoseBaseActualGrafik&) = delete;
};










