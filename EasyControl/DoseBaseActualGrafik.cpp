//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DoseBaseActualGrafik
///
/// @file   DoseBaseActualGrafik.cpp
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
#include "DoseBaseActualGrafik.h"
#include "DoseBatcherActualGrafik.h"
#include "DoseBatcherActualGrafikLwb.h"
#include "DoseBatcherActualGrafikGwb.h"
#include "DoseBatcherActualGrafikIcb.h"
#include "DoseContinousActualGrafik.h"
#include "DoseContinousActualGrafikIfl.h"
#include "DoseContinousActualGrafikIfs.h"
#include "DoseContinousActualGrafikLcl.h"
#include "DoseContinousActualGrafikLws.h"
#include "DoseContinousActualGrafikLwp.h"
#include "DoseContinousActualGrafikLwv.h"
#include "DoseContinousActualGrafikVol.h"
#include "DoseContinousActualGrafikPid.h"
#include "DoseContinousActualGrafikIcp.h"
#include "DoseContinousActualGrafikWbf.h"
#include "DoseContinousActualGrafikWbm.h"
#include "RemoteControl.h"
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
CDoseBaseActualGrafik* CDoseBaseActualGrafik::Create(base::eDoseType lTyp, const CRect& rRect)
{
	CDoseBaseActualGrafik* pBase = nullptr;

	switch (lTyp)
	{
		case base::eDoseType::DOSETYPE_IFL:
			pBase = new CDoseContinousActualGrafikIfl(rRect);
			break;

		case base::eDoseType::DOSETYPE_IFSO:
		case base::eDoseType::DOSETYPE_IFSU:
			pBase = new CDoseContinousActualGrafikIfs(rRect);
			break;

		case base::eDoseType::DOSETYPE_LCL:
			pBase = new CDoseContinousActualGrafikLcl(rRect);
			break;

		case base::eDoseType::DOSETYPE_ICB:
			pBase = new CDoseBatcherActualGrafikIcb(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWS:
			pBase = new CDoseContinousActualGrafikLws(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWP:
			pBase = new CDoseContinousActualGrafikLwp(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWV:
			pBase = new CDoseContinousActualGrafikLwv(rRect);
			break;

		case base::eDoseType::DOSETYPE_LWB:
			pBase = new CDoseBatcherActualGrafikLwb(rRect);
			break;

		case base::eDoseType::DOSETYPE_GWB:
			pBase = new CDoseBatcherActualGrafikGwb(rRect);
			break;

		case base::eDoseType::DOSETYPE_VOL:
			pBase = new CDoseContinousActualGrafikVol(rRect);
			break;

		case base::eDoseType::DOSETYPE_PID:
			pBase = new CDoseContinousActualGrafikPid(rRect);
			break;

		case base::eDoseType::DOSETYPE_ICP:
			pBase = new CDoseContinousActualGrafikIcp(rRect);
			break;

		case base::eDoseType::DOSETYPE_WBF:
			pBase = new CDoseContinousActualGrafikWbf(rRect);
			break;

		case base::eDoseType::DOSETYPE_WBM:
			pBase = new CDoseContinousActualGrafikWbm(rRect);
			break;

		case base::eDoseType::DOSETYPE_NONE:
			pBase = nullptr;
			break;

		default:
			ASSERT(FALSE);
			break;
	}
	if ( pBase )
	{
		pBase->SetDoseType(lTyp);
	}
	return pBase;
}
//***********************************************************************************************************
//***********************************************************************************************************
CDoseBaseActualGrafik::CDoseBaseActualGrafik(const CRect& rRect) : CBaseClass(rRect)
	, cOrg{ rRect.TopLeft() }
	, cFeederNamePoint { 16, 27 }
	, cFeederRunPoint { 12, 0 }
{}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseActualGrafik::DrawFeedingType(CDC* pDC)
{
	auto str = m_FeedingTypeMap.get(GetFeedingType());
	CString szValue{ toCString(str) };
	TextOut(pDC, cFeederNamePoint, szValue);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseActualGrafik::DrawStatus(CDC* pDC)
{
	uint32_t    nRessourceID = 0;

	if (GetRunning())
	{
		if (GetLocalMode())
		{
			nRessourceID = IDB_FEEDERSTATUSRUN_BMP;
		}
		else
		{
			nRessourceID = IDB_FEEDERSTATUSLINERUN_BMP;
		}
	}
	else
	{
		if (GetLocalMode())
		{
			nRessourceID = IDB_FEEDERSTATUSOFF_BMP;
		}
		else
		{
			nRessourceID = IDB_FEEDERSTATUSLINEOFF_BMP;
		}
	}
	ShowBitmap(pDC, nRessourceID, cFeederRunPoint);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseActualGrafik::OnRedraw(CDC* pDC)
{
	DrawStatus(pDC);
}

//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseActualGrafik::OnUpdate(CDC* pDC)
{
	DrawFeedingType(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBaseActualGrafik::IsTextUpdate(void) const
{
	return 	 ( m_StatusFlags.flags.bLocalModeChanged == true)  ||
			 ( m_StatusFlags.flags.bFeedingTypeChanged == true);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDoseBaseActualGrafik::IsBitmapUpdate(void) const
{
	return 	((m_StatusFlags.flags.bRunningChanged == true) ||
			 (m_StatusFlags.flags.bLocalModeChanged == true) ||
			 (m_StatusFlags.flags.bFeedingTypeChanged == true));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDoseBaseActualGrafik::OnSetNewData(const base::CDoseDataCache& rNewData)
{
	CBaseClass::OnSetNewData(rNewData);
	m_StatusFlags.flags.bLocalModeChanged	= IsLocalModeChanged();
	m_StatusFlags.flags.bFeedingTypeChanged = IsFeedingTypeChanged(rNewData.m_eFeedingType);
	m_StatusFlags.flags.bRunningChanged		= IsRunningChanged() || IsStartedChanged();
}
