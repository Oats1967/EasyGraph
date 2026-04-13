//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLcl
///
/// @file   FeederDisplayLcl.h
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
#include "EasyControl_DEU/resource.h"
#include "FeederDisplayLcl.h"
#include "FormatMetric.h"


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayLcl::CFeederDisplayLcl(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
, cCalibratePoint{ 0, 0 }
, cTarePoint{ 76, 0 }
, cEmptyFeederPoint{ 0, 0 }
, cReleasePoint{ 0, 0 }
, cFillPoint{ 34, 48 }
, cGravVolPoint{ 0, 0 }
, cWeightPoint{ 34, 32 }
, cRefillPoint{ 0, 0 }
, cDriveCommandPoint{ 0, 0 }
, cRefillStatePoint{ 0, 0 }
, m_Hopper{}
{
    SetDoseType(base::eDoseType::DOSETYPE_LCL);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLcl::DrawFilling(CDC* pDC)
{
	ASSERT(pDC);

	// and a solid red brush
	auto fMax	 = GetLclWeightMaxLevel();
	auto fMinMin = GetLclWeightMinLevel();
	auto fWeight  = GetWeight();
	fWeight = __max(fWeight, 0.0F);

	CBrush* pNewBrush = (fWeight < fMinMin) ? &m_brushWhiteDashed : &m_brushBlueSolid;
	const float32_t fFillGrad = (fMax > 0) ? (fWeight / fMax) : 0.0F;
	auto& hopper = GetGrafikHopper();
	hopper.DrawFilling(pDC, pNewBrush, &m_PenBlue, __min(fFillGrad, 1.0F) );

	int32_t sdy = 0;
	if (fFillGrad > 0.0f)
	{
		const float32_t fy = 100.0f * fFillGrad;
		sdy = RANGE(_S32(fy), 0, 999);
	}
	CString sz;
	sz.Format(_T("%d%%"), sdy);
	// grab the font and set the text color
	// show the title
	TextOut(pDC, GetFillPoint(), sz, COLORBLACK, TRANSPARENT);
	TextOut(pDC, GetWeightPoint(), FORMATKG(fWeight), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLcl::DrawBitmap(CDC* pDC)
{
	ShowBitmap(pDC, IDB_LCL_CONTAINER);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLcl::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bLclMaxLevelChanged = IsLclWeightMaxLevelChanged(rNewData.m_LclWeightMaxLevel);
	m_StatusFlags.flags.bLclMinLevelChanged = IsLclWeightMinLevelChanged(rNewData.m_LclWeightMinLevel);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayLcl::IsTextUpdate() const
{
	auto update = CBaseClass::IsTextUpdate();
	if (!update)
	{
		update = (m_StatusFlags.flags.bLclMaxLevelChanged == true) ||
				 (m_StatusFlags.flags.bLclMinLevelChanged == true);
	}
	return update;
}





