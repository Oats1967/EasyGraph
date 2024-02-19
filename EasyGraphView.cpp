//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a sample for BCGControlBar Library Professional Edition
// Copyright (C) 1998-2023 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************
//
// EasyGraphView.cpp : implementation of the CEasyGraphView class
//

#include "pch.h"
#include "MainFrm.h"
#include "EasyGraphDoc.h"
#include "EasyGraphView.h"
#include "ConfigItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEasyGraphView

IMPLEMENT_DYNCREATE(CEasyGraphView, CBCGPFormView)

BEGIN_MESSAGE_MAP(CEasyGraphView, CBCGPFormView)
	//{{AFX_MSG_MAP(CEasyGraphView)
	ON_WM_NCPAINT()
	ON_COMMAND(ID_TB_CHART_COPY, OnChartCopy)
	ON_COMMAND(ID_TB_CHART_EXPORT, OnChartExport)
	ON_UPDATE_COMMAND_UI(ID_TB_CHART_COPY, OnUpdateChartCopy)
	ON_UPDATE_COMMAND_UI(ID_TB_CHART_EXPORT, OnUpdateChartExport)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CBCGPFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CBCGPFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyGraphView construction/destruction

CEasyGraphView::CEasyGraphView(UINT id)
	: CBCGPFormView(id == 0 ? CEasyGraphView::IDD : id)
	, c_SelectString{ _T("Durchsatz"),
				   _T("Sollwert"),
				   _T("Stellbefehl"),
				   _T("Nettogewicht"),
				   _T("Dosierperformanz"),
				   _T("Verbrauch"),
				   _T("Drehzahl") }

{
	//{{AFX_DATA_INIT(CEasyGraphView)
	m_strTitle = _T("BCGSoft charts");
	//}}AFX_DATA_INIT

	EnableVisualManagerStyle();
	EnableLayout();

	SetWhiteBackground();
	SetControlsDefaultDlgBackground();

	m_bIsReady = FALSE;
	m_bIsDarkBackground = FALSE;
	m_bIsTexturedTheme = FALSE;
	m_bIsFlatTheme = FALSE;

	m_nThumbnailFlags = 0;
	m_wndTitle.SetCaptionMode();
}

CEasyGraphView::~CEasyGraphView()
{
}

void CEasyGraphView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasyGraphView)
	//}}AFX_DATA_MAP
}

BOOL CEasyGraphView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style |= WS_MAXIMIZE;
	return CBCGPFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEasyGraphView diagnostics

#ifdef _DEBUG
void CEasyGraphView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

void CEasyGraphView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}

CEasyGraphDoc* CEasyGraphView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyGraphDoc)));
	return (CEasyGraphDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEasyGraphView message handlers

LRESULT CEasyGraphView::HandleInitDialog(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = CBCGPFormView::HandleInitDialog (wParam, lParam);

#if 0
	if (GetDlgItem(IDC_CIRCULAR_PROGRESS) != NULL && m_wndProgress.GetSafeHwnd() == NULL)
	{
		m_wndProgress.SubclassDlgItem(IDC_CIRCULAR_PROGRESS, this);
		
		CBCGPCircularProgressIndicatorImpl* pProgress = m_wndProgress.GetCircularProgressIndicator();
		ASSERT_VALID(pProgress);
		
		CBCGPCircularProgressIndicatorOptions options = pProgress->GetOptions();
		options.m_strPercentageFormat = _T("%1.0f");
		options.m_strProgressLabelTrailingSign = _T("%");
		
		pProgress->SetOptions(options);
	}
	ShowWindow(SW_SHOWMAXIMIZED);
#endif
	GetParentFrame()->RecalcLayout();
	if (GetLayout() != NULL)
	{
		UpdateData(FALSE);
		SetupLayout();
	}

	return lRes;
}

void CEasyGraphView::OnInitialUpdate() 
{
	CBCGPFormView::OnInitialUpdate();

	if (m_bIsReady)
	{
		return;
	}
	
#if 0
	if (m_arHours[0].IsEmpty())
	{
		for (int i = 0; i < HOURS_NUM; i++)
		{
			m_arHours[i].Format(_T("%2d"), i);
		}
	}
#endif
	GetParentFrame()->RecalcLayout();

	if (AfxGetMainWnd() == NULL)
	{
		return;
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	GetTitle(m_strTitle);
	UpdateData(FALSE);

	UpdateChartColorTheme(pMainFrame->GetColorTheme(), pMainFrame->IsDarkTheme());

	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart != NULL)
	{
		pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS);
	}

	if (GetCtrl() != NULL)
	{
		//GetCtrl()->SetGraphicsManager(CBCGPGraphicsManager::BCGP_GRAPHICS_MANAGER_GDI);
	}
#if 0
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(); // FORCES CMainframe to be as small as the dialog template
#endif
}

void CEasyGraphView::GetTitle(CString& strTitle)
{
	int nIndex = 0;
	if (nIndex >= 0)
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

		strTitle = "SmaDoTec";
	}
}

void CEasyGraphView::SetupLayout()
{
	CBCGPStaticLayout* pLayout = (CBCGPStaticLayout*)GetLayout();
	if (pLayout != NULL && GetCtrl() != NULL)
	{
		pLayout->AddAnchor(GetCtrl()->GetDlgCtrlID(), CBCGPStaticLayout::e_MoveTypeNone, CBCGPStaticLayout::e_SizeTypeBoth);
	}
}

void CEasyGraphView::OnChartCopy() 
{
	CBCGPBaseVisualCtrl* pCtrl = GetCtrl();
	if (pCtrl != NULL)
	{
		pCtrl->CopyToClipboard();
	}
}

void CEasyGraphView::OnChartExport() 
{
	CBCGPBaseVisualCtrl* pCtrl = GetCtrl();
	if (pCtrl == NULL)
	{
		return;
	}

#ifdef _BCGSUITE_INC_
	CFileDialog 
#else
	CBCGPFileDialogEx 
#endif
		dlg (FALSE, _T("bmp"), m_strTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Bitmap Files (*.bmp)|*.bmp|Png Files (*.png)|*.png|All Files (*.*)|*.*||"), this);

	if (dlg.DoModal () == IDOK)
	{
		pCtrl->ExportToFile(dlg.GetPathName());
	}
}

void CEasyGraphView::OnUpdateChartCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsGroupView());
}

void CEasyGraphView::OnUpdateChartExport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsGroupView());
}

void CEasyGraphView::RotateChart(CBCGPRotationObject::RotationElement hit, double xDelta, double yDelta, double persperctiveDelta)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}

	ASSERT_VALID(pChart);

	CBCGPChartDiagram3D* pDiagram3D = pChart->GetDiagram3D();
	if (pDiagram3D == NULL)
	{
		return;
	}

	double xRotation = pDiagram3D->GetXRotation();
	double yRotation = pDiagram3D->GetYRotation();
	double dblPerspectivePercent = pDiagram3D->GetPerspectivePercent();

	switch (hit)
	{
	case CBCGPRotationObject::BCGP_ROTATION_UP:
		yRotation += yDelta;
		break;

	case CBCGPRotationObject::BCGP_ROTATION_DOWN:
		yRotation -= yDelta;
		break;

	case CBCGPRotationObject::BCGP_ROTATION_LEFT:
		xRotation -= xDelta;
		break;

	case CBCGPRotationObject::BCGP_ROTATION_RIGHT:
		xRotation += xDelta;
		break;

	case CBCGPRotationObject::BCGP_ROTATION_RESET:
		pDiagram3D->Reset(TRUE);
		return;

	case CBCGPRotationObject::BCGP_ROTATION_NARROW_FIELD_OF_VIEW:
		dblPerspectivePercent -= persperctiveDelta;
		break;

	case CBCGPRotationObject::BCGP_ROTATION_WIDEN_FIELD_OF_VIEW:
		dblPerspectivePercent += persperctiveDelta;
		break;
	}

	pDiagram3D->SetPosition(xRotation, yRotation, dblPerspectivePercent);

	pChart->SetDirty();
	pChart->Redraw();
}

void CEasyGraphView::SetLineWidth(int32_t lineWidth)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}

	ASSERT_VALID(pChart);

	for (int i = 0; i < pChart->GetSeriesCount(); i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i);
		ASSERT_VALID(pSeries);

		BCGPChartFormatSeries style = pSeries->GetSeriesFormat();
		style.SetSeriesLineWidth(lineWidth);
		pSeries->SetSeriesFormat(style);
	}
}

void CEasyGraphView::SetSeriesLineColor(const CBCGPColor* pColors, int32_t count)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}
	ASSERT_VALID(pChart);

	auto seriescount = __min(pChart->GetSeriesCount(), count);
	for (int i = 0; i < seriescount; i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i);
		ASSERT_VALID(pSeries);
		pSeries->SetSeriesLineColor(CBCGPBrush(pColors[i]));
	}
}


void CEasyGraphView::SetSeriesFillColors(const CBCGPColor::BCGP_COLOR* pColors, int32_t count)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}

	ASSERT_VALID(pChart);

	for (int i = 0; i < __min(pChart->GetSeriesCount(), count); i++)
	{
		CBCGPChartSeries* pSeries = pChart->GetSeries(i);
		ASSERT_VALID(pSeries);

		BCGPChartFormatSeries style = pSeries->GetSeriesFormat();
		style.SetSeriesFill(CBCGPBrush(pColors[i]));
		pSeries->SetSeriesFormat(style);
	}
}

HBITMAP CEasyGraphView::CreateThumbnail(const CSize& size, BOOL bIsDarkTheme)
{
	ASSERT_VALID(this);

	if (GetCtrl() == NULL)
	{
		return NULL;
	}

	CBCGPBrush brFill(bIsDarkTheme ? globalData.clrBarFace : RGB(255, 255, 255));

	CBCGPVisualContainerCtrl* pContainerCtrl = DYNAMIC_DOWNCAST(CBCGPVisualContainerCtrl, GetCtrl());
	if (pContainerCtrl != NULL)
	{
		CBCGPVisualContainer* pContainer = pContainerCtrl->GetVisualContainer();
		if (pContainer != NULL)
		{
			CArray<BOOL, BOOL> arThumbnailModes;
			int i = 0;

			for (i = 0; i < pContainer->GetCount(); i++)
			{
				CBCGPChartVisualObject* pChart = DYNAMIC_DOWNCAST(CBCGPChartVisualObject, pContainer->GetAt(i));
				if (pChart != NULL)
				{
					arThumbnailModes.Add(pChart->IsThumbnailMode());

					pChart->SetThumbnailFontRatio(theConfig.m_dblThumbnailFontRatio);
					pChart->SetThumbnailMode(TRUE, m_nThumbnailFlags | BCGP_CHART_THUMBNAIL_NO_DRAW_AXIS_LABELS);
				}
				else
				{
					CBCGPChartLegendVisualObject* pLegend = DYNAMIC_DOWNCAST(CBCGPChartLegendVisualObject, pContainer->GetAt(i));
					if (pLegend != NULL)
					{
						arThumbnailModes.Add(pLegend->IsThumbnailMode());
						pLegend->SetThumbnailMode(TRUE, theConfig.m_dblThumbnailFontRatio);
					}
				}
			}

			CBCGPRect rectSaved = pContainer->GetRect();

			pContainer->SetRect(CBCGPRect(0, 0, 1.0 + size.cx, 1.0 + size.cy));
			Repos();

			HBITMAP hbmp = pContainer->ExportToBitmap(pContainerCtrl->GetGraphicsManager(), FALSE, brFill);

			int nObjectIndex = 0;
			for (i = 0; i < pContainer->GetCount(); i++)
			{
				CBCGPChartVisualObject* pChart = DYNAMIC_DOWNCAST(CBCGPChartVisualObject, pContainer->GetAt(i));
				if (pChart != NULL)
				{
					pChart->StopAllAnimations();
					pChart->SetThumbnailMode(arThumbnailModes[nObjectIndex++]);
				}
				else
				{
					CBCGPChartLegendVisualObject* pLegend = DYNAMIC_DOWNCAST(CBCGPChartLegendVisualObject, pContainer->GetAt(i));
					if (pLegend != NULL)
					{
						pLegend->SetThumbnailMode(arThumbnailModes[nObjectIndex++], theConfig.m_dblThumbnailFontRatio);
					}
				}
			}

			pContainer->SetRect(rectSaved);
			Repos();

			return hbmp;
		}
	}

	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return NULL;
	}

	pChart->StopAllAnimations();

	BOOL bIsThumbnailMode = pChart->IsThumbnailMode();

	pChart->SetThumbnailFontRatio(theConfig.m_dblThumbnailFontRatio);
	pChart->SetThumbnailMode(TRUE, m_nThumbnailFlags);

	CBCGPRect rectSaved = pChart->GetRect();
	pChart->SetRect(CBCGPRect(0, 0, 0.9 + size.cx, 0.9 + size.cy));

	HBITMAP hbmp = pChart->ExportToBitmap(GetCtrl()->GetGraphicsManager(), brFill);

	pChart->SetRect(rectSaved);
	pChart->SetThumbnailMode(bIsThumbnailMode);

	return hbmp;
}

/////////////////////////////////////////////////////////////////////////////
// CEasyGraphView printing

void CEasyGraphView::OnFilePrintPreview() 
{
#ifdef _BCGSUITE_INC_
    AFXPrintPreview(this);
#else
	BCGPPrintPreview (this);
#endif
}

BOOL CEasyGraphView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEasyGraphView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEasyGraphView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEasyGraphView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CBCGPBaseVisualCtrl* pCtrl = GetCtrl();
	if (pCtrl != NULL)
	{
		// Since D2D has some print problems, activate the GDI+ graphics manager:
		pCtrl->SetGraphicsManager(CBCGPGraphicsManager::BCGP_GRAPHICS_MANAGER_GDI_PLUS);
		
		pCtrl->DoPrint(pDC, pInfo);

		// Restore default graphics manager:
		pCtrl->SetGraphicsManager(CBCGPGraphicsManager::BCGP_GRAPHICS_MANAGER_DEFAULT);
	}
}

void CEasyGraphView::UpdateChartColorTheme(int nTheme, BOOL bIsDarkTheme)
{
	CBCGPChartVisualObject* pChart = GetChart();
	if (pChart == NULL)
	{
		return;
	}

	if (nTheme < 0)
	{
		if (m_bIsDarkBackground)
		{
			m_bIsDarkBackground = FALSE;
			OnChangeBackgroundDarkness();
		}

		pChart->SetColors(theConfig.GetCustomTheme(nTheme), bIsDarkTheme);

		m_bIsTexturedTheme = nTheme == -1;
		m_bIsFlatTheme = FALSE;
	}
	else
	{
		CBCGPChartTheme::ChartTheme theme = (CBCGPChartTheme::ChartTheme)nTheme;

		BOOL bIsDarkBackground = FALSE;

		switch (theme)
		{
		case CBCGPChartTheme::CT_BLACK_AND_GOLD:
		case CBCGPChartTheme::CT_BLACK_AND_RED:
		case CBCGPChartTheme::CT_BLACK_AND_GREEN:
		case CBCGPChartTheme::CT_BLACK_AND_BLUE:
			bIsDarkBackground = TRUE;
			break;
		}

		if (m_bIsDarkBackground != bIsDarkBackground)
		{
			m_bIsDarkBackground = bIsDarkBackground;
			OnChangeBackgroundDarkness();
		}

		pChart->SetColors((CBCGPChartTheme::ChartTheme)nTheme, bIsDarkTheme);
		
		m_bIsTexturedTheme = FALSE;

		m_bIsFlatTheme = theme >= CBCGPChartTheme::CT_FLAT_2014_1 && theme <= CBCGPChartTheme::CT_ICE;
	}
#if 0
	CWnd* pGradientTypeCombo = GetDlgItem(IDC_FILL_GRADIENT_TYPE);
	if (pGradientTypeCombo->GetSafeHwnd() != NULL)
	{
		pGradientTypeCombo->EnableWindow(IsFillGradientAvailable());
	}

	CBCGPRotationCtrl* pRotationCtrl = DYNAMIC_DOWNCAST(CBCGPRotationCtrl, GetDlgItem(IDC_ROTATE));
	if (pRotationCtrl->GetSafeHwnd() != NULL)
	{
		pRotationCtrl->GetRotationObject()->SetColorTheme(CBCGPRotationObject::BCGP_COLOR_THEME_VISUAL_MANAGER);
	}
#endif
}

void CEasyGraphView::OnNcPaint()
{
	if (!CBCGPVisualManager::GetInstance()->OnDrawViewBorder(this))
	{
		Default();
	}
}

void CEasyGraphView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsGroupView());
}

void CEasyGraphView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsGroupView());
}


int CEasyGraphView::GetMarkerSize(int nSel)
{
	return globalUtils.ScaleByDPI(nSel == 0 ? 7 : 5 * (nSel + 1), this);
}

int CEasyGraphView::GetAxisSplitGap() const
{
	return bcg_round(globalUtils.ScaleByDPI(6.0, this));
}

void CEasyGraphView::SetProgress(int nCurr, int nTotal)
{
	CString strDescription;
	strDescription.Format(_T("Creating views: %d of %d"), nCurr + 1, nTotal);
	
	//SetDlgItemText(IDC_DESCRIPTION, strDescription);
	
	if (m_wndProgress.GetSafeHwnd() != NULL)
	{
		if (!m_wndProgress.IsWindowVisible())
		{
			m_wndProgress.SetRange(0.0, (double)nTotal);
			m_wndProgress.ShowWindow(SW_SHOWNOACTIVATE);
		}
		
		m_wndProgress.SetPos((double)nCurr);
	}
}

