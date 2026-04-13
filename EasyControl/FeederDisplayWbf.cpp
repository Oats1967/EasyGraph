 //------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayWbf.cpp
///
/// @file   FeederDisplayWbf.cpp
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
#include "FeederDisplayWbf.h"
#include "EasyControl_DEU/resource.h"
#include "FormatMetric.h"
#include "MFCMacros.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//							          { FX0, FY0 } \            /  { FX1, FY1 }
//                                                  \         /
//                                                   \      /
//                                    { FX3, FY3 }    \   /    { FX2, FY2 }
//                                                     | |  
//                                                         

#define FPOLY_0 { 4, 20 }
#define FPOLY_1 { 80, 20 }
#define FPOLY_2 { 33, 55 }
#define FPOLY_3 { 24, 55 }


#define BCX 0
#define BCY 0
#define BCDX 96
#define BCDY 74



//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayWbf::CFeederDisplayWbf(const int32_t sID, const CRect& rRect) :	CFeederDisplay(sID, rRect)
, cCalibratePoint { 70, 0 }
, cTarePoint { 70, 0 }
, cEmptyFeederPoint{ 70, 0 }
, cGravVolPoint	{ 38, 88 }
, cFillPoint{ 45-25, 40 -6 }
, cWeightPoint{ 54-38, 26 -6}
, cReleasePoint{ 15, 66 }
, cBeltLoadPoint  { 50, 52 }
, cRefillStatePoint{ 0 , 0}
, cBackgroundRect{ BCX, BCY, BCX + BCDX, BCY + BCDY }
, m_Hopper{ base::C2DTrapeze::TrapezePoints{ base::IPoint(FPOLY_0), base::IPoint(FPOLY_1), base::IPoint(FPOLY_2), base::IPoint(FPOLY_3) } }
, m_brushBlueSolid(COLORBLUE)
, m_brushYellowSolid(COLORYELLOW)
, m_brushRedDashed(HS_BDIAGONAL, COLORRED)
, m_brushWhiteDashed(HS_DIAGCROSS, COLORWHITE)
, m_PenBlue(PS_NULL, 0, COLORBLUE)
{
	SetDoseType(base::eDoseType::DOSETYPE_WBF);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::DrawBitmap(CDC* pDC)
{
	ShowBitmap(pDC, IDB_WBF_CONTAINER);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::OnRedraw(CDC* pDC)
{
	CBaseClass::OnRedraw(pDC);
	DrawGravVol(pDC);
	if (GetMinBeltLoadAlarm())
	{
		CPen pen(PS_SOLID, 3, RGB(250, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(80, 64);
		pDC->LineTo(80 + 56,64);
		pDC->SelectObject(pOldPen);
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::OnUpdate(CDC* pDC)
{
	CFeederDisplay::OnUpdate(pDC);
	DrawFilling(pDC);
	DrawBeltLoad(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::DrawBeltLoad(CDC *pDC)
{
	const float32_t fBeltLoad = GetBeltLoadMassflow();
	TextOut(pDC, cBeltLoadPoint, FORMATKGM(fBeltLoad), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::DrawGravVol(CDC *pDC)
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
void CFeederDisplayWbf::DrawFilling(CDC* pDC)
{
	ASSERT(pDC);

	// and a solid red brush
	float32_t fMax = GetRefillMax();
	float32_t fWeight = GetWeight();
	fWeight = __max(fWeight, 0.0F);

	const float32_t fFillGrad = (fMax > 0) ? (fWeight / fMax) : 0.0F;
	m_Hopper.DrawFilling(pDC, &m_brushBlueSolid, &m_PenBlue, __min(fFillGrad, 1.0F));

	TextOut(pDC, GetWeightPoint(), FORMATKG(fWeight), COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayWbf::IsTextUpdate(void) const
{
	return  (m_StatusFlags.flags.bWeightChanged == true) ||
			(m_StatusFlags.flags.bRefillMaxChanged == true) ||
			(m_StatusFlags.flags.bBeltLoadMassflowChanged == true) ||
			(m_StatusFlags.flags.bDriveCommandChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayWbf::IsBitmapUpdate(void) const
{
	auto bUpdate = CBaseClass::IsBitmapUpdate();
	if (!bUpdate)
	{
		bUpdate = (m_StatusFlags.flags.bGravimetricChanged == true) ||
					(m_StatusFlags.flags.bMinBeltLoadAlarmChanged == true) ||
					(m_StatusFlags.flags.bGravVolStateChanged == true) ||
					(m_StatusFlags.flags.bMinBeltLoadStatusChanged == true);
	}
	return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayWbf::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);

	m_StatusFlags.flags.bGravimetricChanged = IsGravimetricChanged();
	m_StatusFlags.flags.bMinBeltLoadAlarmChanged = IsMinBeltLoadAlarmChanged();
	m_StatusFlags.flags.bGravVolStateChanged = IsGravVolStateChanged();
	m_StatusFlags.flags.bMinBeltLoadStatusChanged = IsMinBeltLoadStatusChanged(rNewData.m_sMinBeltLoadStatus);
	m_StatusFlags.flags.bWeightChanged = IsWeightChanged(rNewData.m_fActWeight);
	m_StatusFlags.flags.bRefillMaxChanged = IsRefillMaxChanged(rNewData.m_fRefillMax);
	m_StatusFlags.flags.bDriveCommandChanged = IsDriveCommandChanged(rNewData.m_fActualDriveCommand);
	m_StatusFlags.flags.bBeltLoadMassflowChanged = IsBeltloadMassflowChanged(rNewData.m_fBandlastMassflow);

}


