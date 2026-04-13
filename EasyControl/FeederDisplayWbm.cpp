//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayWbm.cpp
///
/// @file   FeederDisplayWbm.cpp
///
///
/// @coypright(c)  Ing.b�ro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "FeederDisplayWbm.h"
#include "EasyControl_DEU/resource.h"
#include "FormatMetric.h"
#include "MFCMacros.h"


//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayWbm::CFeederDisplayWbm(const int32_t sID, const CRect& rRect) : CFeederDisplay(sID, rRect)
, cCalibratePoint { 30, 4 }
, cTarePoint{ 70, 0 }
, cEmptyFeederPoint{ 70, 0 }
, cReleasePoint{ 15 - 6, 66 }
, cGravVolPoint{ 38-10, 88 }
, cWeightPoint{ 54 - 18, 36 - 6 }
, cBeltLoadPoint{ 50-20, 52 }
, cRefillStatePoint{ 35, 4 }
, cBackgroundRect{ 7, 3, 7 + 70, 3 + 70 }
, m_PenBlue(PS_NULL, 0, COLORBLUE)
, m_brushBlueSolid(COLORBLUE)

, m_Hopper{ }
{
	SetDoseType(base::eDoseType::DOSETYPE_WBM);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::DrawBitmap(CDC* pDC)
{
	ShowBitmap(pDC, IDB_WBM_CONTAINER);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::OnRedraw(CDC* pDC)
{
	CBaseClass::OnRedraw(pDC);
	DrawGravVol(pDC);
	if (GetMinBeltLoadAlarm())
	{
		CPen pen(PS_SOLID, 3, RGB(250, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(80, 64);
		pDC->LineTo(80 + 56, 64);
		pDC->SelectObject(pOldPen);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::OnUpdate(CDC* pDC)
{
	CFeederDisplay::OnUpdate(pDC);
	DrawFilling(pDC);
	DrawBeltLoad(pDC);
	DrawGravVol(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::DrawBeltLoad(CDC *pDC)
{
	TextOut(pDC, cBeltLoadPoint, FORMATKGM(GetBeltLoadMassflow()), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::DrawGravVol(CDC *pDC)
{
	uint32_t nRessourceID = 0;

	if (GetGravimetric())
	{
		if (!GetLCActive())
		{
			nRessourceID = IDB_DOSIERERGRAVERROR;
		}
		else
		{
			nRessourceID = (GetGravVolState()) ? IDB_DOSIERERGRAVVOL : IDB_DOSIERERGRAV;
		}
		ShowBitmap(pDC, nRessourceID, GetGravVolPoint());
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::DrawFilling(CDC* pDC)
{
	ASSERT(pDC);

	// and a solid red brush
	auto fMax	 = GetRefillMax();
	auto fWeight = GetWeight();
	fWeight = __max(fWeight, 0.0F);

	const float32_t fFillGrad = (fMax > 0) ? (fWeight / fMax) : 0.0F;
	m_Hopper.DrawFilling(pDC, &m_brushBlueSolid, &m_PenBlue, __min(fFillGrad, 1.0F));
	TextOut(pDC, GetWeightPoint(), FORMATKG(fWeight), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayWbm::IsTextUpdate(void) const
{
	return  (m_StatusFlags.flags.bWeightChanged == true) ||
			(m_StatusFlags.flags.bRefillMaxChanged == true) ||
			(m_StatusFlags.flags.bBeltLoadMassflowChanged == true) ||
			(m_StatusFlags.flags.bDriveCommandChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayWbm::IsBitmapUpdate(void) const
{
	auto bUpdate = CFeederDisplay::IsBitmapUpdate();
	if (!bUpdate)
	{
		bUpdate =	(m_StatusFlags.flags.bGravimetricChanged == true) ||
					(m_StatusFlags.flags.bMinBeltLoadAlarmChanged == true) ||
					(m_StatusFlags.flags.bGravVolStateChanged == true) ||
					(m_StatusFlags.flags.bMinBeltLoadStatusChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbm::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bGravimetricChanged		  = IsGravimetricChanged();
	m_StatusFlags.flags.bMinBeltLoadAlarmChanged  = IsMinBeltLoadAlarmChanged();
	m_StatusFlags.flags.bGravVolStateChanged	  = IsGravVolStateChanged();
	m_StatusFlags.flags.bMinBeltLoadStatusChanged = IsMinBeltLoadStatusChanged(rNewData.m_sMinBeltLoadStatus);
	m_StatusFlags.flags.bWeightChanged			  = IsWeightChanged(rNewData.m_fActWeight);
	m_StatusFlags.flags.bRefillMaxChanged		  = IsRefillMaxChanged(rNewData.m_fRefillMax);
	m_StatusFlags.flags.bDriveCommandChanged	  = IsDriveCommandChanged(rNewData.m_fActualDriveCommand);
	m_StatusFlags.flags.bBeltLoadMassflowChanged  = IsBeltloadMassflowChanged(rNewData.m_fBandlastMassflow);

}




