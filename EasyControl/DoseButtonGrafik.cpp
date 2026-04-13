//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseButtonGrafik
///
/// @file   DoseButtonGrafik.cpp
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
#include "DoseButtonBatcherGrafik.h"
#include "DoseButtonBatcherGrafikLwb.h"
#include "DoseButtonBatcherGrafikGwb.h"
#include "DoseButtonBatcherGrafikIcb.h"
#include "DoseButtonContinousGrafik.h"
#include "DoseButtonContinousGrafikIfl.h"
#include "DoseButtonContinousGrafikIfs.h"
#include "DoseButtonContinousGrafikLcl.h"
#include "DoseButtonContinousGrafikLws.h"
#include "DoseButtonContinousGrafikLwp.h"
#include "DoseButtonContinousGrafikLwv.h"
#include "DoseButtonContinousGrafikVol.h"
#include "DoseButtonContinousGrafikPid.h"
#include "DoseButtonContinousGrafikIcp.h"
#include "DoseButtonContinousGrafikWbf.h"
#include "DoseButtonContinousGrafikWbm.h"
#include "DoseButtonContinousGrafikNone.h"
#include "EasyControl_DEU/resource.h"
#include "MFCMacros.h"


/////////////////////////////////////////////////////////////////////////////
// CDoseButtonGrafik command target
// 
//***********************************************************************************************************
//***********************************************************************************************************
CDoseButtonGrafik* CDoseButtonGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	CDoseButtonGrafik* pBase = nullptr;

	switch (lTyp)
	{
		case base::eDoseType::DOSETYPE_ICB:
			pBase = new CDoseButtonBatcherGrafikIcb(rRect);
			break;

		case base::eDoseType::DOSETYPE_IFL:
			pBase = new CDoseButtonContinousGrafikIfl(rRect);
			break;

		case base::eDoseType::DOSETYPE_IFSO:
		case base::eDoseType::DOSETYPE_IFSU:
			pBase = new CDoseButtonContinousGrafikIfs(rRect);
			break;

		case base::eDoseType::DOSETYPE_LCL:
			pBase = new CDoseButtonContinousGrafikLcl(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWS:
			pBase = new CDoseButtonContinousGrafikLws(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWP:
			pBase = new CDoseButtonContinousGrafikLwp(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWV:
			pBase = new CDoseButtonContinousGrafikLwv(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWB:
			pBase = new CDoseButtonBatcherGrafikLwb(rRect);
			break;

		case base::eDoseType::DOSETYPE_GWB:
			pBase = new CDoseButtonBatcherGrafikGwb(rRect);
			break;

		case base::eDoseType::DOSETYPE_VOL:
			pBase = new CDoseButtonContinousGrafikVol(rRect);
			break;

		case base::eDoseType::DOSETYPE_PID:
			pBase = new CDoseButtonContinousGrafikPid(rRect);
			break;

		case base::eDoseType::DOSETYPE_ICP:
			pBase = new CDoseButtonContinousGrafikIcp(rRect);
			break;

		case base::eDoseType::DOSETYPE_WBF:
			pBase = new CDoseButtonContinousGrafikWbf(rRect);
			break;

		case base::eDoseType::DOSETYPE_WBM:
			pBase = new CDoseButtonContinousGrafikWbm(rRect);
			break;

		case base::eDoseType::DOSETYPE_NONE:
			pBase = nullptr;
			break;

		default:
			ASSERT(FALSE);
			break;
	}
	if (pBase)
	{
		pBase->SetDoseType(lTyp);
	}
	return pBase;
}

//***********************************************************************************************************
//***********************************************************************************************************
CDoseButtonGrafik::CDoseButtonGrafik(const CRect& rRect) : CBaseClass(rRect)
, cIDPoint{ 19, 11 }
, cDoseButtonPoint{ 0,0 }
, m_OpState{ OpState::eOp_Off }
{
	m_idFont.CreateFont(18, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsVisible() const
{
	const auto dosetype = GetDoseType();
	assert(__ISVALIDDOSETYPE(dosetype));
	const auto bLocalMode			= GetLocalMode();
	const auto bIOStartInputern		= GetIOStartInput();
	const auto bIOLineStartInput	= GetIOLineStartInput();
	const auto bIOLogoStart			= GetIOLogoStart();
	const auto bReady				= GetReady(); // override
	const auto bCalibration			= GetCalibration(); // override
	const auto bEmptyFeeder			= GetEmptyFeeder(); // override
	const auto bTaraState			= GetTaring(); // override
	BOOL bVisible = (!bCalibration) && (!bEmptyFeeder) && (!bTaraState) && bIOLogoStart && (!bIOStartInputern) && ((!bIOLineStartInput) || bLocalMode);
	return bVisible;
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsButtonRun() const
{
	return GetRunning();
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsButtonReady() const
{
	return GetReady();
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsButtonStart() const
{
	return GetStart();
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::DrawStatus(CDC* pDC)
{
	if (IsVisible())
	{
		uint32_t nRessourceID = 0;
		if (IsButtonRun())
		{
			nRessourceID = IDB_DOSEBUTTONRUN;
			m_OpState = OpState::eOp_Run;

		}
		else if (IsButtonStart())
		{
			nRessourceID = IDB_DOSEBUTTONPAUSE;
			m_OpState = OpState::eOp_Ready;
		}
		else if (IsButtonReady())
		{
			nRessourceID = IDB_DOSEBUTTONREADY;
			m_OpState = OpState::eOp_Ready;
		}
		else
		{
			nRessourceID = IDB_DOSEBUTTONOFF;
			m_OpState = OpState::eOp_Off;
		}
		ShowBitmap(pDC, nRessourceID, cDoseButtonPoint);
	}
	else
	{
		m_OpState = OpState::eOp_Off;
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::OnRedraw(CDC* pDC)
{
	DrawStatus(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::DrawID(CDC* pDC, const POINT& p)
{
	CString szValue;
	szValue.Format(_T("%d"), GetID() + 1);
	CenterTextOut(pDC, p, szValue, COLORBLACK, TRANSPARENT);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::CenterTextOut(CDC* pDC, const POINT& p, const CString& sz, COLORREF color, int32_t bkmode)
{
	int32_t iOldCR = pDC->SetTextColor(color);
	int32_t iOldBK = pDC->SetBkMode(bkmode);
	int32_t iOldTA = pDC->SetTextAlign(TA_CENTER | TA_TOP);
	CFont* oldFont = pDC->SelectObject(&m_idFont);
	pDC->TextOut(p.x, p.y, sz);
	pDC->SelectObject(oldFont);
	pDC->SetTextAlign(iOldTA);
	pDC->SetBkMode(iOldBK);
	pDC->SetTextColor(iOldCR);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::OnUpdate(CDC* pDC)
{
	DrawID(pDC, cIDPoint);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsTextUpdate(void) const
{
	return 	(m_StatusFlags.flags.bIdChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseButtonGrafik::IsBitmapUpdate(void) const
{
	return 	(m_StatusFlags.flags.bLineModeChanged == true) ||
			(m_StatusFlags.flags.bReleaseChanged == true) ||
			(m_StatusFlags.flags.bIOStartInputChanged == true) ||
			(m_StatusFlags.flags.bIOLineStartInputChanged == true) ||
			(m_StatusFlags.flags.bIOLogoStartChanged == true) ||
			(m_StatusFlags.flags.bRunningChanged == true) ||
			(m_StatusFlags.flags.bStartedChanged == true) ||
			(m_StatusFlags.flags.bSetpointChanged == true) ||
			(m_StatusFlags.flags.bCalibrationChanged == true) ||
			(m_StatusFlags.flags.bEmptyFeederChanged == true) ||
			(m_StatusFlags.flags.bTaringChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::FlashButton(CDC* pDC)
{
	if (m_OpState != OpState::eOp_Off)
	{
		CBaseClass::FlashButton(pDC);
		CPoint p{ m_DisplayRect.CenterPoint() };
		DrawID(pDC, p + CPoint(0, -10));
	}
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseButtonGrafik::OnSetNewData(const base::CDoseDataCache& rNew)
{
	CBaseClass::OnSetNewData(rNew);

	m_StatusFlags.flags.bIdChanged				 = IsIdChanged(rNew.m_id);
	m_StatusFlags.flags.bLineModeChanged		 = IsLocalModeChanged();
	m_StatusFlags.flags.bReleaseChanged			 = IsReleaseChanged();
	m_StatusFlags.flags.bIOStartInputChanged	 = IsIOStartInputChanged();
	m_StatusFlags.flags.bIOLineStartInputChanged = IsIOLineStartInputChanged();
	m_StatusFlags.flags.bIOLogoStartChanged		 = IsIOLogoStartChanged();
	m_StatusFlags.flags.bRunningChanged			 = IsRunningChanged();
	m_StatusFlags.flags.bStartedChanged			 = IsStartedChanged();
	m_StatusFlags.flags.bSetpointChanged		 = IsSetpointChanged(rNew.m_fSetpoint);
	m_StatusFlags.flags.bCalibrationChanged		 = IsCalibrationChanged();
	m_StatusFlags.flags.bEmptyFeederChanged		 = IsEmptyFeederChanged();
	m_StatusFlags.flags.bTaringChanged			 = IsTaringChanged();
}


