//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramView
///
/// @file   DiagramView.cpp
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
#include "EasyControl.h"
#include "DiagramView.h"
#include "GlobalConst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDiagramView

IMPLEMENT_DYNCREATE(CDiagramView, CDiagramBaseView)


CDiagramView::CDiagramView()
	: CDiagramBaseView(CDiagramView::IDD)
	, c_nLineStyle { 0 }
	, c_nLineWidth { 2 }
	, c_bDisplayShadow { FALSE }
	, m_Feature { CDemoFeature::BCGP_Area }
{
	m_pSeries.fill(nullptr);
}

void CDiagramView::DoDataExchange(CDataExchange* pDX)
{
	CDiagramBaseView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagramView)
	DDX_Control(pDX, IDC_CHART, m_wndChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiagramView, CDiagramBaseView)
	//{{AFX_MSG_MAP(CDiagramView)
	ON_MESSAGE(WM_DIAGRAMBAR_SELECT, OnDiagramSelect)
	ON_MESSAGE(WM_DIAGRAMBAR_DOSEITEM, OnDiagramBarDoseItem)
	ON_MESSAGE(WM_DIAGRAMBAR_TIME, OnDiagramBarTime)
	ON_MESSAGE(WM_TIMER_REFRESH, OnTimerRefresh)
	ON_MESSAGE(WM_DIAGRAMBAR_DIAGRAMTYPE, OnDiagramBarDiagramType)
	ON_WM_DESTROY()
	//ON_BN_DOUBLECLICKED(IDC_CHART, OnDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramView diagnostics

#ifdef _DEBUG
void CDiagramView::AssertValid() const
{
	CDiagramBaseView::AssertValid();
}

void CDiagramView::Dump(CDumpContext& dc) const
{
	CDiagramBaseView::Dump(dc);
}
#endif //_DEBUG
//**************************************************************************************************
//**************************************************************************************************
int32_t CDiagramView::GetMaxCount()
{
	static const int32_t MAXSERIESCOUNT = 8;
	uint32_t uCount = 0;
	REMOTEREF.getMaxDoseCount(uCount);
	int32_t count = __min(_S32(uCount), MAXSERIESCOUNT);
	return count;
}

//**************************************************************************************************
//**************************************************************************************************
void CDiagramView::DeleteSingleSelectedSeries(const int32_t newSeries)
{
	const int32_t count = (newSeries < 0) ? GetMaxCount() : 1;
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	const int32_t seriescount = pChart->GetSeriesCount();
	for (int32_t index = count; index < seriescount; index++)
	{
		pChart->CleanUpChartData(index, TRUE);
		m_pSeries[index] = nullptr;
	}
}
//**************************************************************************************************
//**************************************************************************************************
CString CDiagramView::CreateDoseName(const int32_t index)
{
	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_DOSIERER));
	CString szName;
	szName.Format(_T("%s %d"), LPCTSTR(szTemp), index + 1);
	return szName;
}
//**************************************************************************************************
//**************************************************************************************************
CString CDiagramView::CreateSelectedName(const base::eMassflowSelect iSelect)
{
	const UINT iMap[base::cMassflowSelectMax] = { IDS_MASSFLOW_KGH,
											  IDS_SETPOINT_KGH,
											  IDS_DRIVECOMMAND,
											  IDS_WEIGHT_KG,
											  IDS_DOSEPERFORMANCE_KGH,
											  IDS_TOTALIZER_KG,
											  IDS_ROTSPEED_UPM };

	static_assert(_S32(base::eMassflowSelect::eVIEWMASSFLOW) == 0, "eVIEWMASSFLOW) != 0");
	static_assert(_S32(base::eMassflowSelect::eVIEWSETPOINT) == 1, "eVIEWSETPOINT) != 1");
	static_assert(_S32(base::eMassflowSelect::eVIEWDRIVECOMMAND) == 2, "eVIEWDRIVECOMMAND) != 2");
	static_assert(_S32(base::eMassflowSelect::eVIEWWEIGHT) == 3, "eVIEWWEIGHT) != 3");
	static_assert(_S32(base::eMassflowSelect::eVIEWDOSEPERFORMANCE) == 4, "eVIEWDOSEPERFORMANCE) != 4");
	static_assert(_S32(base::eMassflowSelect::eVIEWTOTALIZER) == 5, "eVIEWTOTALIZER) != 5");
	static_assert(_S32(base::eMassflowSelect::eVIEWROTSPEED) == 6, "eVIEWROTSPEED) != 6");


	CString szName;
	VERIFY(szName.LoadString(iMap[_S32(iSelect)]));
	return szName;
}
//**************************************************************************************************
//**************************************************************************************************
void CDiagramView::InitializeSeries(CBCGPChartSeries& rSeries)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	auto type = pChart->GetChartType();
	if (type == BCGPChartLine)
	{
		BCGPChartFormatSeries style = rSeries.GetSeriesFormat();

		style.m_curveType = BCGPChartFormatSeries::CCT_LINE;
		style.SetSeriesOutlineDashStyle((CBCGPStrokeStyle::BCGP_DASH_STYLE)c_nLineStyle);
		style.SetSeriesLineWidth(c_nLineWidth);

		style.m_shadowType.m_bDisplayShadow = c_bDisplayShadow;
		rSeries.SetSeriesFormat(style);
	}
	else if (type == BCGPChartLine3D)
	{
		const int32_t m_nLineThickness = 1;
		rSeries.SetDataPoint3DLineThickness(m_nLineThickness);
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CDiagramView::CreateSeriesSingleSelected(const int32_t series)
{
	// Definitiver Wechsel
	DeleteSingleSelectedSeries(series);

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	const int32_t seriescount = pChart->GetSeriesCount();
	const int32_t count = (series < 0) ? GetMaxCount() : 1;
	const int32_t mincount = __min(seriescount, count);

	int32_t index = 0;
	for (; index < count; index++)
	{
		const int32_t doseindex = (series < 0) ? index : series;
		if (index < mincount)
		{
			ASSERT(m_pSeries[index] != NULL);
			pChart->SetSeriesName(CreateDoseName(doseindex), index);
		}
		else
		{
			ASSERT(m_pSeries[index] == NULL);
			m_pSeries[index] = pChart->CreateSeries(CreateDoseName(doseindex));
		}
		InitializeSeries(*m_pSeries[index]);
	}
	for ( ; index < seriescount; index++)
	{
		pChart->CleanUpChartData(index, TRUE);
		m_pSeries[index] = NULL;
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::FillSerieSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index, CBCGPChartSeries& rSeries)
{
	const uint32_t datapoints = _U32(rSeries.GetDataPointCount());
	uint32_t massflowcounter = MASSFLOWREF.GetCounter();
	int32_t startpoint = 0;

	rSeries.ClearMinMaxValues();
	if (diagramtype == base::eDiagramType::e3DLine)
	{
		startpoint = __max(_S32(massflowcounter) - 30, 0);
	}
	else
	{
		startpoint = __max(_S32(massflowcounter) - _S32(MASSFLOWREF.GetBufferSize()), 0);
	}
	uint32_t count = _U32(massflowcounter - startpoint);
	uint32_t seriescount = __min(datapoints, count);
	uint32_t offsetx = _U32(startpoint);
	for (uint32_t x = 0; x < seriescount; x++, offsetx++)
	{
		const auto& el = MASSFLOWREF.GetElement(offsetx);
		const float& value = el.Get(iSelect, index);
		rSeries.SetDataPointValue(x, value);
	}
	uint32_t rest = count - seriescount;
	while (rest--)
	{
		const auto& el = MASSFLOWREF.GetElement(offsetx++);
		const float& value = el.Get(iSelect, index);
		rSeries.AddDataPoint(value);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::FillSerieSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index)
{
	if (index < 0)
	{
		const int32_t count = GetMaxCount();
		for (int32_t ix = 0; ix < count; ix++)
		{
			ASSERT(m_pSeries[ix]);
			FillSerieSingleSelected(iSelect, diagramtype, ix, *m_pSeries[ix]);
		}
	}
	else
	{
		ASSERT(m_pSeries[0]);
		FillSerieSingleSelected(iSelect, diagramtype, index, *m_pSeries[0]);
	}

}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::SetYRange(const base::eMassflowSelect iSelect)
{
	static int32_t bAuto = -1;
	static int32_t bMin = INT_MAX;
	static int32_t bMax = INT_MIN;


	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);

	pYAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pYAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	const auto& rScale = GetDiagramConfig().GetAxisScale();
	int32_t iNewAuto = _S32(rScale.GetAuto(iSelect));
	int32_t iNewMin = _S32(rScale.GetMin(iSelect));
	int32_t iNewMax = _S32(rScale.GetMax(iSelect));

	BOOL bNew = (iNewAuto != bAuto);
	if ( (! bNew) && ( ! iNewAuto))
	{
		bNew = (iNewMin != bMin) || (iNewMax != bMax);
	}
	if ( bNew )
	{
		if (iNewAuto)
		{
			pYAxis->SetAutoDisplayRange();
		}
		else
		{
			pYAxis->SetFixedMinimumDisplayValue(iNewMin);
			pYAxis->SetFixedMaximumDisplayValue(iNewMax);
		}
		bAuto = iNewAuto;
		bMin = iNewMin;
		bMax = iNewMax;
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::ChartDoseSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index)
{
	CString szTitle;

	VERIFY(szTitle.LoadString(globalconst::g_MassflowView[_S32(iSelect)]));
	
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	pChart->SetChartTitle(szTitle);
	pChart->ShowAllDataLabels(FALSE);
	pChart->ShowDataMarkers(FALSE);

	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, FALSE);
	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);

	CreateSeriesSingleSelected(index);
	FillSerieSingleSelected(iSelect, diagramtype, index);

	SetYRange(iSelect);

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;


	OnUpdateChart();
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::FillSerieMultipleSelected(const base::tViewMultipleSelect& iSelect, const base::eDiagramType diagramtype, const int32_t index)
{
	ASSERT(index >= 0);

	int32_t ix = 0;
	for (const auto& item : iSelect)
	{
		ASSERT(m_pSeries[ix]);
		FillSerieSingleSelected(item, diagramtype, index,*m_pSeries[ix]);
		ix++;
	}
}
//**************************************************************************************************
//**************************************************************************************************
void CDiagramView::CreateSeriesMultipleSelected(const base::tViewMultipleSelect& iSelect)
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	auto seriescount = pChart->GetSeriesCount();

	int32_t count = _S32(iSelect.size());
	int32_t mincount = __min(seriescount, count);
	int32_t index = 0;
	for (; index < count; index++)
	{
		if (index < mincount)
		{
			ASSERT(m_pSeries[index] != NULL);
			pChart->SetSeriesName(CreateSelectedName(iSelect[index]), index);
		}
		else
		{
			ASSERT(m_pSeries[index] == NULL);
			m_pSeries[index] = pChart->CreateSeries(CreateSelectedName(iSelect[index]));
		}
		InitializeSeries(*m_pSeries[index]);
	}
	for (;  index < seriescount; index++)
	{
		pChart->CleanUpChartData(index, TRUE);
		m_pSeries[index] = NULL;
	}

}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::ChartDoseMultipleSelected(const base::tViewMultipleSelect& iSelect, const base::eDiagramType diagramtype, const int32_t index)
{
	CString szTitle = CreateDoseName(index);

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	pChart->SetChartTitle(szTitle);
	pChart->ShowAllDataLabels(FALSE);
	pChart->ShowDataMarkers(FALSE);

	CreateSeriesMultipleSelected(iSelect);
	FillSerieMultipleSelected(iSelect, diagramtype, index);

	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);

	pYAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pYAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_CROSS;
	pXAxis->m_minorTickMarkType = CBCGPChartAxis::TMT_CROSS;

	OnUpdateChart();
}

//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::ChartDoseItem ()
{
	auto& rDiagramSettings = GetDiagramConfig();
	rDiagramSettings.SetTimeInterval(base::eTimeMode::TG_SECONDS);

	auto selectType = rDiagramSettings.GetSelectionType();
	if (selectType == base::eSelectionType::eSelectionMultiple)
	{
		if (rDiagramSettings.GetID() < 0)
		{
			auto mul = rDiagramSettings.GetSingleSelection();
			rDiagramSettings.SetSingleSelection(mul);
			selectType = base::eSelectionType::eSelectionSingle;
		}
	}
	if (selectType == base::eSelectionType::eSelectionSingle)
	{
		if (rDiagramSettings.GetSelectionCount() > 0)
		{
			ChartDoseSingleSelected(rDiagramSettings.GetSingleSelection(), rDiagramSettings.GetDiagramType(), rDiagramSettings.GetID());
		}
	}
	else
	{
		ASSERT(rDiagramSettings.GetID() >= 0);
		if (rDiagramSettings.GetSelectionCount() > 0)
		{
			auto eViewSelect = rDiagramSettings.ExtractSelection();
			ASSERT(eViewSelect.size() > 1);
			ChartDoseMultipleSelected(eViewSelect, rDiagramSettings.GetDiagramType(), rDiagramSettings.GetID());
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::SetNewChartType(const base::eDiagramType diagramtype)
{
	BCGPChartType type =
	(m_Feature == CDemoFeature::BCGP_StackedLine || m_Feature == CDemoFeature::BCGP_StackedLineSpline) ? BCGP_CT_STACKED :
	(m_Feature == CDemoFeature::BCGP_StackedLine100) ?
	BCGP_CT_100STACKED : BCGP_CT_SIMPLE;

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->CleanUpChartData(-1, TRUE);
	m_pSeries.fill(nullptr);

	if (diagramtype == base::eDiagramType::e2DLine)
	{
		pChart->SetChartType(BCGPChartLine, type);
	}
	else
	{
		pChart->SetChartType(BCGPChartLine3D, type);
	}
	auto& rDiagramSettings = GetDiagramConfig();
	ASSERT(rDiagramSettings.GetDiagramType() != diagramtype);
	rDiagramSettings.SetDiagramType( diagramtype);
}

//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::OnInitialUpdate() 
{
	CDiagramBaseView::OnInitialUpdate();
	
	if (m_bIsReady)
	{
		return;
	}
	m_bIsReady = TRUE;
	m_wndChart.EnableTooltip(FALSE);
	m_wndChart.EnableTrackingToolTips(FALSE);
	SetNewChartType(base::eDiagramType::e2DLine);
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::OnUpdate2DLine()
{
	UpdateData();

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->Redraw();
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::OnUpdate3DLine()
{
	const BOOL m_bShowDataLabels = FALSE;
	const int32_t m_nDataLabelAngle = 3;
	const int32_t m_nTransparency = 25;
	const int32_t m_nLineThickness = 1;
	const BOOL m_bShowWalls = TRUE;
	const BOOL m_bThickWalls = TRUE;
	const BOOL m_bRotateByMouse = TRUE;

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	double dblDataLabelAngle = (m_nDataLabelAngle - 2) * 45;

	pChart->ShowDataLabels(m_bShowDataLabels, TRUE, TRUE, dblDataLabelAngle);

	pChart->SetThemeOpacity(100 - m_nTransparency);

	DWORD dwoFlags = CBCGPChartDiagram3D::DWO_OUTLINE_ALL;

	// Draw walls and floor:
	if (m_bShowWalls)
	{
		dwoFlags |= (CBCGPChartDiagram3D::DWO_DRAW_ALL_WALLS | CBCGPChartDiagram3D::DWO_DRAW_FLOOR);
	}

	//m_wndThickWalls.EnableWindow(m_bShowWalls);

	pChart->GetDiagram3D()->EnableRotationByMouse(m_bRotateByMouse ? BCGPCHART_MOUSE_ROTATE_X | BCGPCHART_MOUSE_ROTATE_Y | BCGPCHART_MOUSE_ROTATE_CHANGE_CURSOR : 0);
	pChart->GetDiagram3D()->EnableChangePerspectiveByMouseWheel(m_bRotateByMouse);
	pChart->GetDiagram3D()->SetDrawWallOptions((CBCGPChartDiagram3D::DrawWallOptions)dwoFlags);
	pChart->GetDiagram3D()->SetThickWallsAndFloor(m_bShowWalls && m_bThickWalls);

	pChart->SetDirty();
	pChart->Redraw();
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::OnUpdateChart() 
{
	const auto& rDiagramSettings = GetDiagramConfig();
	if (rDiagramSettings.GetDiagramType() == base::eDiagramType::e2DLine)
	{
		OnUpdate2DLine();
	}
	else if (rDiagramSettings.GetDiagramType() == base::eDiagramType::e3DLine)
	{
		OnUpdate3DLine();
	}
	else
	{
	}
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDiagramView::OnDiagramSelect(WPARAM wParam, LPARAM )
{
	auto newflags = _U32(wParam);

	auto& rDiagramSettings = GetDiagramConfig();
	if (rDiagramSettings.GetDiagramSelect().allflags != newflags)
	{
		base::MassflowBitfield temp;
		temp.allflags = newflags;
		rDiagramSettings.SetDiagramSelect(temp);

		if ((rDiagramSettings.GetSelectionType() == base::eSelectionType::eSelectionMultiple) &&
			(rDiagramSettings.GetID() < 0) )
		{
			rDiagramSettings.SetID(0);
		}
		ChartDoseItem();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDiagramView::OnDiagramBarDoseItem(WPARAM wParam, LPARAM )
{
	auto item = static_cast<int32_t>(wParam);

	auto& rDiagramSettings = GetDiagramConfig();
	if (rDiagramSettings.GetID() != item)
	{
		rDiagramSettings.SetID(item);
		if ((rDiagramSettings.GetSelectionType() == base::eSelectionType::eSelectionMultiple) &&
			(rDiagramSettings.GetID() < 0))
		{
			auto mul = rDiagramSettings.GetSingleSelection();
			rDiagramSettings.SetSingleSelection(mul);
		}
		ChartDoseItem();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDiagramView::OnDiagramBarTime(WPARAM wParam, LPARAM )
{
	auto item = static_cast<base::eTimeMode>(wParam);

	auto& rDiagramSettings = GetDiagramConfig();
	if ((item == base::eTimeMode::TG_SECONDS) && (rDiagramSettings.GetTimeInterval() != item))
	{
		ChartDoseItem();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDiagramView::OnDiagramBarDiagramType(WPARAM wParam, LPARAM)
{
	auto diagramtype = static_cast<base::eDiagramType>(wParam);
	ASSERT((diagramtype == base::eDiagramType::e2DLine) ||
		   (diagramtype == base::eDiagramType::e3DLine));

	auto& rDiagramSettings = GetDiagramConfig();
	if (rDiagramSettings.GetDiagramType() != diagramtype)
	{
		SetNewChartType(diagramtype);
		ChartDoseItem();
	}
	return 0L;
}
	
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDiagramView::OnTimerRefresh(WPARAM , LPARAM )
{
	auto& rDiagramSettings = GetDiagramConfig();
	if (rDiagramSettings.GetTimeInterval() == base::eTimeMode::TG_SECONDS)
	{
		ChartDoseItem();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
void CDiagramView::OnDestroy()
{
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);
	pChart->CleanUp();
	CWnd::OnDestroy();
}
