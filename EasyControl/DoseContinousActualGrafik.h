//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseContinousActualGrafik
///
/// @file   DoseContinousActualGrafik.h
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

#include "DoseBaseActualGrafik.h"



class CDoseContinousActualGrafik : public CDoseBaseActualGrafik
{
	using CBaseClass = CDoseBaseActualGrafik;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bMassflowChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus m_StatusFlags;

private:
	const POINT cFeederMassflowPoint;

protected:
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNew) override;

protected:
	virtual void DrawMassflow(CDC* pDC);


private:
	CDoseContinousActualGrafik(void) = delete;

public:
	CDoseContinousActualGrafik(const CRect& rRect);
	~CDoseContinousActualGrafik(void) override = default;
	CDoseContinousActualGrafik(const CDoseContinousActualGrafik&) = delete;
	CDoseContinousActualGrafik& operator = (const CDoseContinousActualGrafik&) = delete;
};










