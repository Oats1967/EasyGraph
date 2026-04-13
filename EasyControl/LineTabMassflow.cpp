//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTabMassflow
///
/// @file   LineTabMassflow.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "RemoteControl.h"
#include "LineTabMassflow.h"
#include "ECMessageBox.h"
#include "DataImage.h"
#include "FormatMetric.h"
#include "MFCMacros.h"



// CLineTabMassflow-Dialogfeld

IMPLEMENT_DYNAMIC(CLineTabMassflow, CDialog)
CLineTabMassflow::CLineTabMassflow(CWnd* pParent)
	: CDialog(CLineTabMassflow::IDD, pParent)
{
	SetTitle(eTotFeedingType::FT_MF, L"MF");
	SetTitle(eTotFeedingType::FT_SF1, L"SF1");
	SetTitle(eTotFeedingType::FT_SF2, L"SF2");
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineTabMassflow, CDialog)
	ON_MESSAGE(WM_TIMER_REFRESH, &CLineTabMassflow::OnTimerRefresh)
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
void CLineTabMassflow::SetValue(CFeederInfo& rInfo)
{
	const auto oldlinestatus = rInfo.m_pLineStatusIndicator->GetLineStatus();
	const auto newlinestatus = rInfo.m_Run ? CLineStatusIndicator::eStatus::ON : CLineStatusIndicator::eStatus::OFF;
	if (oldlinestatus != newlinestatus)
	{
		rInfo.m_pLineStatusIndicator->SetLineStatus(newlinestatus);
		ReposLineStatus(rInfo);
	}
	const auto oldfeederstatus = rInfo.m_pFeederStatusIndicator->GetFeederStatus();
	auto newfeederstatus = CFeederStatusIndicator::eStatus::OFF;
	if (rInfo.m_ExtInput)
	{
		newfeederstatus = (rInfo.m_Massflow > 0) ? CFeederStatusIndicator::eStatus::ON : CFeederStatusIndicator::eStatus::READY;
	}
	if (newfeederstatus != oldfeederstatus)
	{
		rInfo.m_pFeederStatusIndicator->SetFeederStatus(newfeederstatus);
		ReposFeederStatus(rInfo);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::SetValue()
{
	const auto& rL = dataimage::getLineDataImage();
	const auto& cache = rL.m_Cache;

	SetMassflow(eTotFeedingType::FT_MF, cache.m_fLineMassflowMF);
	SetMassflow(eTotFeedingType::FT_SF1, cache.m_fLineMassflowSF1);
	SetMassflow(eTotFeedingType::FT_SF2, cache.m_fLineMassflowSF2);

	const BOOL bMFExtInput = cache.m_ProcessStatus.flags.IOmainfeeder;
	const BOOL bSF1ExtInput = cache.m_ProcessStatus.flags.IOsidefeeder1;
	const BOOL bSF2ExtInput = cache.m_ProcessStatus.flags.IOsidefeeder2;
	SetExtInput(eTotFeedingType::FT_MF, bMFExtInput);
	SetExtInput(eTotFeedingType::FT_SF1, bSF1ExtInput);
	SetExtInput(eTotFeedingType::FT_SF2, bSF2ExtInput);

	SetRun(eTotFeedingType::FT_MF, cache.m_fLineMassflowMF > 0.0F);
	SetRun(eTotFeedingType::FT_SF1, cache.m_fLineMassflowSF1 > 0.0F);
	SetRun(eTotFeedingType::FT_SF2, cache.m_fLineMassflowSF2 > 0.0F);

	for (auto& item : m_FeederInfo)
	{
		SetValue(item);
	}
	UpdateData(FALSE);
}
//******************************************************************************************************
//******************************************************************************************************
void CLineTabMassflow::GetValue()
{
	UpdateData(TRUE);
}

//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for (int32_t k = 0; k < eTotFeedingType::FT_MAX; k++)
	{
		auto& rItem = m_FeederInfo[k];
		DDX_Control(pDX, IDC_LINETAB_MASSFLOW_STATUS1    + k, rItem.m_wndLineStatus);
		DDX_Control(pDX, IDC_LINETAB_MASSFLOW_RELEASE1   + k, rItem.m_wndFeederStatus);
		DDX_FloatKgh(pDX, IDC_LINETAB_MASSFLOW_MASSFLOW1 + k, rItem.m_Massflow);
		DDX_Text(pDX, IDC_LINETAB_MASSFLOW_TITLE1 + k, rItem.m_Title);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::ReposLineStatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndLineStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);

	auto& rect = pContainer->GetRect();
	auto indicatorSize = globalUtils.ScaleByDPI(CBCGPSize(77, 65), this);

	CBCGPGraphicsManager* pGM = rInfo.m_wndLineStatus.GetGraphicsManager();
	if (pGM != NULL)
	{
		indicatorSize = rInfo.m_pLineStatusIndicator->GetDefaultSize(pGM, NULL);
	}
	auto rectIndicator{ rect };
	double marginHorz = (rectIndicator.Width() - indicatorSize.cx) / 2;
	double marginVert = (rectIndicator.Height() - indicatorSize.cy) / 2;
	rectIndicator.SetSize(indicatorSize);
	rectIndicator.OffsetRect(marginHorz, marginVert);
	rInfo.m_pLineStatusIndicator->SetRect(rectIndicator);
	rInfo.m_wndLineStatus.RedrawWindow();
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::InitLineStatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndLineStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);
	ASSERT(rInfo.m_pLineStatusIndicator == NULL);
	rInfo.m_pLineStatusIndicator = new CLineStatusIndicator(pContainer);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::ReposFeederStatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndFeederStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);

	auto& rect = pContainer->GetRect();
	CBCGPSize indicatorSize = globalUtils.ScaleByDPI(CBCGPSize(77, 65), this);

	CBCGPGraphicsManager* pGM = rInfo.m_wndFeederStatus.GetGraphicsManager();
	if (pGM != NULL)
	{
		indicatorSize = rInfo.m_pFeederStatusIndicator->GetDefaultSize(pGM, NULL);
	}
	auto rectIndicator{ rect };
	double marginHorz = (rectIndicator.Width() - indicatorSize.cx) / 2;
	double marginVert = (rectIndicator.Height() - indicatorSize.cy) / 2;
	rectIndicator.SetSize(indicatorSize);
	rectIndicator.OffsetRect(marginHorz, marginVert);
	rInfo.m_pFeederStatusIndicator->SetRect(rectIndicator);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabMassflow::InitFeedertatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndFeederStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);
	ASSERT(rInfo.m_pFeederStatusIndicator == NULL);
	rInfo.m_pFeederStatusIndicator = new CFeederStatusIndicator(pContainer);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTabMassflow::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		for (auto& item : m_FeederInfo)
		{
			InitLineStatus(item);
			InitFeedertatus(item);
		}
		SetValue();
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
LRESULT CLineTabMassflow::OnTimerRefresh(WPARAM, LPARAM)
{
	SetValue();
	return 0L;
}

