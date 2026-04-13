//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTabTotalizer
///
/// @file   LineTabTotalizer.cpp
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
#include "LineTabTotalizer.h"
#include "ECMessageBox.h"
#include "DataImage.h"
#include "FormatMetric.h"
#include "MFCMacros.h"



// CLineTabTotalizer-Dialogfeld

IMPLEMENT_DYNAMIC(CLineTabTotalizer, CDialog)
CLineTabTotalizer::CLineTabTotalizer(CWnd* pParent)
	: CDialog(CLineTabTotalizer::IDD, pParent)
	, m_TotalizerLine{ 0.0F }
	, m_TotalizerAll { 0.0F }
	, m_ProductionTime{ 0 }
	, m_TotalizerResetBt { IDS_TOTALIZER_RESET, IDS_TOTALIZER_RESET }
{
	SetTitle(eTotFeedingType::FT_MF,  L"MF");
	SetTitle(eTotFeedingType::FT_SF1, L"SF1");
	SetTitle(eTotFeedingType::FT_SF2, L"SF2");
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineTabTotalizer, CDialog)
	ON_MESSAGE(WM_TIMER_REFRESH, &CLineTabTotalizer::OnTimerRefresh)
	ON_BN_CLICKED(IDC_LINETAB_TOTALIZER_CLEAR, &CLineTabTotalizer::OnBnClickedLineTabTotalizerClear)
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
void CLineTabTotalizer::SetValue(CFeederInfo& rInfo)
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
void CLineTabTotalizer::SetValue()
{
	const auto& rL = dataimage::getLineDataImage();
	const auto& cache = rL.m_Cache;
	SetTotalizerAll(cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_ALL)]);
	SetTotalizerLine(cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_LINE)]);
	SetProductionTime(cache.m_ulProductionTime);

	SetTotalizer(eTotFeedingType::FT_MF, cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_MF)]);
	SetTotalizer(eTotFeedingType::FT_SF1, cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_SF1)]);
	SetTotalizer(eTotFeedingType::FT_SF2, cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_SF2)]);

	SetRun(eTotFeedingType::FT_MF, cache.m_fLineMassflowMF > 0.0F);
	SetRun(eTotFeedingType::FT_SF1, cache.m_fLineMassflowSF1 > 0.0F);
	SetRun(eTotFeedingType::FT_SF2, cache.m_fLineMassflowSF2 > 0.0F);

	m_Impuls.SetState(cache.m_ProcessStatus.flags.IOTotalizerPulseOutput);

	for (auto& item : m_FeederInfo)
	{
		SetValue(item);
	}
	UpdateData(FALSE);
}
//******************************************************************************************************
//******************************************************************************************************
void CLineTabTotalizer::GetValue()
{
	UpdateData(TRUE);
}

//***********************************************************************************************
//***********************************************************************************************
void CLineTabTotalizer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	for (int32_t k = 0; k < eTotFeedingType::FT_MAX; k++)
	{
		auto& rItem = m_FeederInfo[k];
		DDX_Control(pDX, IDC_LINETAB_TOTALIZER_STATUS1    + k, rItem.m_wndLineStatus);
		DDX_FloatKg(pDX, IDC_LINETAB_TOTALIZER_TOTALIZER1 + k, rItem.m_Totalizer);
		DDX_Text(pDX, IDC_LINETAB_TOTALIZER_TITLE1 + k, rItem.m_Title);
	}
	DDX_FloatKg(pDX, IDC_LINETAB_TOTALIZER_TOTALIZERALL, m_TotalizerAll);
	DDX_FloatKg(pDX, IDC_LINETAB_TOTALIZER_TOTALIZERLINE, m_TotalizerLine);
	DDX_TimeH(pDX, IDC_LINETAB_TOTALIZER_PRODUCTIONTIME, m_ProductionTime);
	DDX_Control(pDX, IDC_LINETAB_TOTALIZER_CLEAR, m_TotalizerResetBt);
	DDX_Control(pDX, IDC_LINETAB_TOTALIZER_IMPULSE, m_Impuls);

}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabTotalizer::ReposLineStatus(CFeederInfo& rInfo)
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
void CLineTabTotalizer::InitLineStatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndLineStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);
	ASSERT(rInfo.m_pLineStatusIndicator == NULL);
	rInfo.m_pLineStatusIndicator = new CLineStatusIndicator(pContainer);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabTotalizer::ReposFeederStatus(CFeederInfo& rInfo)
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
void CLineTabTotalizer::InitFeedertatus(CFeederInfo& rInfo)
{
	CBCGPVisualContainer* pContainer = rInfo.m_wndFeederStatus.GetVisualContainer();
	ASSERT_VALID(pContainer);
	ASSERT(rInfo.m_pFeederStatusIndicator == NULL);
	rInfo.m_pFeederStatusIndicator = new CFeederStatusIndicator(pContainer);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTabTotalizer::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		for (auto& item : m_FeederInfo)
		{
			InitLineStatus(item);
			InitFeedertatus(item);
		}
		const auto& rL = dataimage::getLineDataImage();
		const auto& cache = rL.m_Cache;
		BOOL bLineMode = cache.m_ProcessStatus.flags.ModuleLineSetpoint;
		ENABLE_SHOW_ID(IDC_LINETAB_TOTALIZER_TOTALIZERLINE, bLineMode, bLineMode);
		ENABLE_SHOW_ID(IDC_LINETAB_TOTALIZER_TOTALIZERLINE_STATIC, bLineMode, bLineMode);
		auto bChange = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET);
		ENABLE_SHOW_ID(IDC_LINETAB_TOTALIZER_CLEAR, bChange, bChange);
		BOOL bTotalizerPulse = cache.m_ProcessStatus.flags.ModuleTotalizerPulse;
		ENABLE_SHOW_ID(IDC_LINETAB_TOTALIZER_IMPULSE, bTotalizerPulse, bTotalizerPulse);

		m_TotalizerResetBt.Init(FALSE);

		SetValue();
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
LRESULT CLineTabTotalizer::OnTimerRefresh(WPARAM, LPARAM)
{
	SetValue();
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabTotalizer::OnBnClickedLineTabTotalizerClear()
{
	ASSERT(USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET));
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		REMOTEREF.setLinePBClearTotalizer();
	}
}

