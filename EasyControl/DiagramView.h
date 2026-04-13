//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramView
///
/// @file   DiagramView.h
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

#include <vector>
#include <array>
#include "EasyControl.h"
#include "DiagramDoc.h"
#include "DiagramBaseView.h"
#include "WorkspaceBar.h"



class CDiagramView : public CDiagramBaseView
{
	DECLARE_DYNCREATE(CDiagramView)

	enum { IDD = IDD_CHART_LINE };


// Form Data
private:
	CBCGPChartCtrl			m_wndChart;
	const int32_t 			c_nLineStyle;
	const int32_t			c_nLineWidth;
	const BOOL				c_bDisplayShadow;
	CDemoFeature::Feature	m_Feature;
	std::array<CBCGPChartSeries*, GLOBALDOSEMAXCOUNT> m_pSeries;
private:
	void InitializeSeries(CBCGPChartSeries& rSeries);
	void CreateSeriesSingleSelected(const int32_t index);
	void DeleteSingleSelectedSeries(const int32_t index);
	void ChartDoseSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index);
	void FillSerieSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index, CBCGPChartSeries& rSeries);
	void FillSerieSingleSelected(const base::eMassflowSelect iSelect, const base::eDiagramType diagramtype, const int32_t index);

	void CreateSeriesMultipleSelected(const base::tViewMultipleSelect& iSelect);
	void ChartDoseMultipleSelected(const base::tViewMultipleSelect& iSelect, const base::eDiagramType diagramtype, const int32_t index);
	void FillSerieMultipleSelected(const base::tViewMultipleSelect& iSelect, const base::eDiagramType diagramtype, const int32_t index);

	void ChartDoseItem();
	void OnUpdate2DLine();
	void OnUpdate3DLine();
	void DeleteSeries();
	void SetYRange(const base::eMassflowSelect iSelect);
	void SetNewChartType(const base::eDiagramType diagramtype);

	static int32_t GetMaxCount();
	static CString CreateDoseName(const int32_t index);
	static CString CreateSelectedName(const base::eMassflowSelect iSelect);

	CBCGPChartVisualObject* GetChart()
	{ return m_wndChart.GetChart();	}

	CBCGPBaseVisualCtrl* GetCtrl()
	{	return &m_wndChart;	}

	afx_msg void OnUpdateChart();

	base::CDiagramConfig& GetDiagramConfig();

protected:
	void OnInitialUpdate() override;
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
public:
	LRESULT OnDiagramSelect(WPARAM wParam, LPARAM lParam);
	LRESULT OnDiagramBarDiagramType(WPARAM wParam, LPARAM);
	LRESULT OnTimerRefresh(WPARAM wParam, LPARAM lParam);
	LRESULT OnDiagramBarDoseItem(WPARAM wParam, LPARAM lParam);
	LRESULT OnDiagramBarTime(WPARAM wParam, LPARAM lParam);

	afx_msg void OnDestroy();

// Operations
public:
	CDiagramView();           // protected constructor used by dynamic creation
	~CDiagramView() override = default;


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
inline base::CDiagramConfig& CDiagramView::GetDiagramConfig()
{
	auto* pDoc = (CDiagramDoc*)GetDocument();
	ASSERT(pDoc);
	return pDoc->GetDiagramConfig();
}
