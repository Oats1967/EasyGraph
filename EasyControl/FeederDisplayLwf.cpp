//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayLwf
///
/// @file   FeederDisplayLwf.cpp
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
#include "BASE/Base.def"
#include "FeederDisplayLwf.h"
#include "EasyControl_DEU/resource.h"
#include "FormatMetric.h"
#include "MFCMacros.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayLwf::CFeederDisplayLwf(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
												, m_brushBlueSolid(COLORBLUE) 
												, m_brushYellowSolid(COLORYELLOW)
												, m_brushRedDashed(HS_BDIAGONAL, COLORRED)
												, m_brushWhiteDashed(HS_DIAGCROSS, COLORWHITE)
												, m_brushYellowDashed(HS_DIAGCROSS, COLORYELLOW)
												, m_PenBlue(PS_NULL, 0, COLORBLUE)
{
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::OnRedraw(CDC* pDC)
{
	CBaseClass::OnRedraw(pDC);
	DrawGravVol(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::OnUpdate(CDC* pDC)
{
	CBaseClass::OnUpdate(pDC);
	DrawFilling(pDC);
	DrawDriveCommand(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::DrawFilling(CDC* pDC)
{
	if (GetLCEnable())
	{
		DrawFillingScale(pDC);
	}
	else
	{
		DrawFillingNoScale(pDC);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::DrawFillingNoScale(CDC* pDC)
{
	ASSERT(pDC);
	const float32_t fFillGrad = 0.75F;

	auto& hopper = GetGrafikHopper();
	hopper.DrawFilling(pDC, &m_brushYellowDashed, &m_PenBlue, fFillGrad);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::DrawFillingScale(CDC *pDC)
{
	ASSERT(pDC);

	// and a solid red brush
	float32_t fWeight	= GetWeight();
	fWeight = __max(fWeight, 0.0F);
	float32_t fMax		= GetRefillMax();
	float32_t fMinMin	= GetRefillMinMin();

	CBrush* pNewBrush   = (fWeight < fMinMin) ? &m_brushWhiteDashed : &m_brushBlueSolid;
	const float32_t fFillGrad = (fMax > 0) ? (fWeight / fMax) : 0.0F;
	auto& hopper = GetGrafikHopper();
	hopper.DrawFilling(pDC, pNewBrush, &m_PenBlue, __min(fFillGrad, 1.0F) );

    int32_t sdy         = 0;
    if (fFillGrad > 0.0f)
    {
        const float32_t fy  = 100.0f * fFillGrad;
        sdy   = RANGE(_S32(fy), 0, 999);
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
void CFeederDisplayLwf::DrawDriveCommand(CDC* pDC)
{
	TextOut(pDC, GetDriveCommandPoint(), FORMATPERCENT(GetDriveCommand()), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::DrawGravVol(CDC *pDC)
{
	if (GetLCEnable())
	{
		uint32_t nRessourceID = 0;
		if ( ! GetLCActive())
		{
			nRessourceID = IDB_DOSIERERGRAVERROR;
		}
		else if (GetGravimetric())
		{
			nRessourceID = (GetGravVolState()) ? IDB_DOSIERERGRAVVOL : IDB_DOSIERERGRAV;
		}
		else
		{
			nRessourceID = IDB_DOSIERERVOL;
		}
		ShowBitmap(pDC, nRessourceID, GetGravVolPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::DrawRefillState(CDC* pDC)
{
	BOOL bLocked = TRUE;
	if (GetRefillEnable()) // Ist Freigabe
	{
		if (GetRefillControlEnable()) // Ist Refill controlwaagen aktiv
		{
			CBaseClass::DrawRefillState(pDC);

			uint32_t nIDResource = 0;
			if (GetRefillAlarm())
			{
				nIDResource = IDB_LWF_CONTAINER_REFILL_ALARM;
			}
			else if (GetRefillActive())
			{
				nIDResource = IDB_LWF_CONTAINER_REFILL;
			}
			if (nIDResource)
			{
				ShowBitmap(pDC, nIDResource, GetRefillStatePoint());
			}
			bLocked = FALSE;
		}
	}
	if (bLocked)
	{
		ShowBitmap(pDC, IDB_DOSIERERVERRIEGELUNG, GetRefillPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayLwf::IsTextUpdate() const
{
	return (m_StatusFlags.flags.bWeightChanged == true) ||
		(m_StatusFlags.flags.bRefillMinChanged == true) ||
		(m_StatusFlags.flags.bRefillMaxChanged == true) ||
		(m_StatusFlags.flags.bRefillMinMinChanged == true) ||
		(m_StatusFlags.flags.bDriveCommandChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayLwf::IsBitmapUpdate() const
{
	auto bUpdate = CBaseClass::IsBitmapUpdate();
	if ( ! bUpdate)
	{
		bUpdate =	(m_StatusFlags.flags.bRefillEnableChanged == true) ||
					(m_StatusFlags.flags.bGravimetricChanged == true) ||
					(m_StatusFlags.flags.bRefillControlEnableChanged == true) ||
					(m_StatusFlags.flags.bGravVolStateChanged == true) ||
					(m_StatusFlags.flags.bLCEnableChanged == true) ||
					(m_StatusFlags.flags.bLCActiveChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayLwf::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bRefillEnableChanged		= IsRefillEnableChanged();
	m_StatusFlags.flags.bGravimetricChanged			= IsGravimetricChanged();
	m_StatusFlags.flags.bRefillControlEnableChanged = IsRefillControlEnableChanged();
	m_StatusFlags.flags.bGravVolStateChanged		= IsGravVolStateChanged();
	m_StatusFlags.flags.bLCEnableChanged			= IsLCEnableChanged();
	m_StatusFlags.flags.bLCActiveChanged			= IsLCActiveChanged();

	m_StatusFlags.flags.bWeightChanged			= IsWeightChanged(rNewData.m_fActWeight);
	m_StatusFlags.flags.bRefillMinChanged		= IsRefillMinChanged(rNewData.m_fRefillMin);
	m_StatusFlags.flags.bRefillMaxChanged		= IsRefillMaxChanged(rNewData.m_fRefillMax);
	m_StatusFlags.flags.bRefillMinMinChanged	= IsRefillMinMinChanged(rNewData.m_fRefillMinMin);
	m_StatusFlags.flags.bDriveCommandChanged	= IsDriveCommandChanged(rNewData.m_fActualDriveCommand);
}




