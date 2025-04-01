
#include "pch.h"
#include "EasyGraph.h"
#include "framework.h"
#include "PropertiesWnd.h"
#include "MainFrm.h"
#include "StringConvert.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ID_LINEWIDTH 1
#define ID_VISIBLE   2
#define ID_CATEGORY  3
#define ID_COLOR	 4

struct ItemProperty
{
	int32_t c_linewidth;
	int32_t c_color;
	int32_t c_visible;
	int32_t c_category;
};


/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept :
	c_MassflowSelectMap{ { base::eMassflowSelect::eVIEWWEIGHT, IDS_WB_NETWEIGHT },
						 { base::eMassflowSelect::eVIEWDOSEPERFORMANCE, IDS_WB_DOSEPERFORMANCE },
						 { base::eMassflowSelect::eVIEWDRIVECOMMAND, IDS_WB_DRIVECOMMAND },
						 { base::eMassflowSelect::eVIEWMASSFLOW, IDS_WB_MASSFLOW },
						 { base::eMassflowSelect::eVIEWROTSPEED, IDS_WB_ROTSPEED },
						 { base::eMassflowSelect::eVIEWSETPOINT, IDS_WB_SETPOINT },
						 { base::eMassflowSelect::eVIEWTOTALIZER, IDS_WB_TOTALIZER }
						}
	, m_nComboHeight{ 0 }
	, m_LinienColorPos{ 0 }
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar-Meldungshandler

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() -(m_nComboHeight+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Kombinationsfeld erstellen:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenkombinationsfelds. \n");
		return -1;      // Fehler beim Erstellen
	}
	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_P_APPLICATION));
	m_wndObjectCombo.AddString(szTemp);
	VERIFY(szTemp.LoadString(IDS_P_PROPERTYWINDOW));
	m_wndObjectCombo.AddString(szTemp);
	m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Fehler beim Erstellen des Eigenschaftenrasters. \n");
		return -1;      // Fehler beim Erstellen
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Ist gesperrt */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Gesperrt */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// Alle Befehle werden über dieses Steuerelement geleitet, nicht über den übergeordneten Rahmen:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Fügen Sie hier Ihren Befehlshandlercode ein.
	int k = 0;
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Fügen Sie hier Ihren Befehlshandlercode ein.
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Fügen Sie hier Ihren Befehlsaktualisierungs-UI-Befehlshandlercode ein.
}


CPropertyGrid* CPropertiesWnd::CreateProperty(const base::eMassflowSelect select)
{
	CString szTemp;
	CString szYes;
	CString szNo;
	CString szOptions;
	const auto& attrib = g_Statistics.GetLineAttribute(select);
	auto it = c_MassflowSelectMap.find(select);
	ASSERT(it != c_MassflowSelectMap.cend());
	VERIFY(szTemp.LoadString(it->second));
	CPropertyGrid* pGroupProp = new CPropertyGrid(select, 0, szTemp);

	VERIFY(szTemp.LoadString(IDS_P_VIEW));
	VERIFY(szYes.LoadString(IDS_P_YES));
	VERIFY(szNo.LoadString(IDS_P_NO));
	VERIFY(szOptions.LoadString(IDS_P_OPTIONS));

	CPropertyGrid* pElProp	 = new CPropertyGrid(select, ID_VISIBLE, szTemp, szYes, szOptions);
	pElProp->AddOption(szYes);
	pElProp->AddOption(szNo);
	pElProp->AllowEdit(FALSE);
	pElProp->SetValue(COleVariant((attrib.m_Visible) ? szYes : szNo, VT_BSTR));
	pGroupProp->AddSubItem(pElProp);

	CString szLineColor;
	VERIFY(szLineColor.LoadString(IDS_PW_LINECOLOR));
	CString szLineColorInfo;
	VERIFY(szLineColorInfo.LoadString(IDS_PW_LINECOLOR_INFO));
	CPropertyColorGrid* pColorProp = new CPropertyColorGrid(select, ID_COLOR, szLineColor, RGB(210, 192, 254), nullptr, szLineColorInfo);
	pColorProp->EnableOtherButton(_T("Andere..."));
	pColorProp->EnableAutomaticButton(_T("Standard"), ::GetSysColor(COLOR_3DFACE));
	pColorProp->SetColor(attrib.m_Color);
	pGroupProp->AddSubItem(pColorProp);

	CString szCategory;
	VERIFY(szCategory.LoadString(IDS_PW_CATEGORY));
	CString szLine;
	VERIFY(szLine.LoadString(IDS_PW_LINE));
	CString szLineCategoryOptions;
	VERIFY(szLineCategoryOptions.LoadString(IDS_PW_CATEGORY_OPTIONS));
	CPropertyGrid* pCategoryProp = new CPropertyGrid(select, ID_CATEGORY, szCategory, szLine, szLineCategoryOptions);
	pCategoryProp->AddOption(szLine);
	CString szFlaeche;
	VERIFY(szFlaeche.LoadString(IDS_PW_FLAECHE));
	pCategoryProp->AddOption(szFlaeche);
	pCategoryProp->AllowEdit(FALSE);
	pCategoryProp->SetValue(COleVariant((attrib.m_Category == base::LineCategory::eLine) ? szLine : szFlaeche, VT_BSTR));
	pGroupProp->AddSubItem(pCategoryProp);

	CString szLinienstaerke;
	VERIFY(szLinienstaerke.LoadString(IDS_PW_LINIENSTAERKE));
	CPropertyGrid* pLineWidthProp = new CPropertyGrid(select, ID_LINEWIDTH, szLinienstaerke, _T("1"), _T("A numeric value"), NULL, NULL, NULL, _T("0123456789"));
	pLineWidthProp->AddOption(_T("1"));
	pLineWidthProp->AddOption(_T("2"));
	pLineWidthProp->AddOption(_T("3"));
	pLineWidthProp->AllowEdit(FALSE);
	CString str;
	str.Format(_T("%d"), RANGE(attrib.m_LineWidth, 1, 3));
	pLineWidthProp->SetValue(COleVariant(str, VT_BSTR));
	pGroupProp->AddSubItem(pLineWidthProp);

	return pGroupProp;
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CString szTemp;
	VERIFY(szTemp.LoadStringW(IDS_P_LINESELECT));
	CMFCPropertyGridProperty* pGroup = new CMFCPropertyGridProperty(szTemp);
	DECLARE_MASSFLOWSELECT(field);
	for (const auto& rItem : field)
	{
		if (rItem != base::eMassflowSelect::eVIEWTOTALIZER)
		{
			auto prop = CreateProperty(rItem);
			pGroup->AddSubItem(prop);
		}
	}
	m_wndPropList.AddProperty(pGroup);
	m_wndPropList.ExpandAll(FALSE);
}

void CPropertiesWnd::OnSetColor(CPropertyColorGrid* pGrid)
{
	auto lineAttrib = g_Statistics.GetLineAttribute(pGrid->GetSelect());
	lineAttrib.m_Color = CBCGPColor(pGrid->GetColor());
	g_Statistics.SetLineAttribute(pGrid->GetSelect(), lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_LINECOLOR, WPARAM(pGrid->GetSelect()));
}


void CPropertiesWnd::OnSetLineWidth(CPropertyGrid* pGrid)
{
	auto lineAttrib = g_Statistics.GetLineAttribute(pGrid->GetSelect());
	COleVariant i = pGrid->GetValue();// get the change value.

	//below is the code to change COleVariant to other variable type
	LPVARIANT pVar = (LPVARIANT)i;
	switch (pVar->vt)
	{
		case VT_I2:    // short
			lineAttrib.m_LineWidth = pVar->iVal;
			break;
		case VT_I4:     // int
			lineAttrib.m_LineWidth = pVar->lVal;
			break;
		case VT_R4:    // float
			lineAttrib.m_LineWidth = _S32(pVar->fltVal);
			break;
		case VT_R8:    // double
			lineAttrib.m_LineWidth = _S32(pVar->dblVal);
			break;
		case VT_INT:
			lineAttrib.m_LineWidth = pVar->lVal;
			break;
		case VT_BOOL:
			ASSERT(FALSE);
			break;
		case VT_BSTR:
			{
				CString str1{ pVar->bstrVal };
				lineAttrib.m_LineWidth = _ttoi(str1);

			}
			break;
		default:
			ASSERT(FALSE);
			break;
			// etc.
	}
	g_Statistics.SetLineAttribute(pGrid->GetSelect(), lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_LINEWIDTH, WPARAM(pGrid->GetSelect()));
}

void CPropertiesWnd::OnSetCategory(CPropertyGrid* pGrid)
{
	auto lineAttrib = g_Statistics.GetLineAttribute(pGrid->GetSelect());
	COleVariant i = pGrid->GetValue();// get the change value.
	auto val = pGrid->GetValue();
	LPVARIANT pVar = (LPVARIANT)val;
	ASSERT(pVar->vt == VT_BSTR);
	CString str1{ pVar->bstrVal };
	lineAttrib.m_Category = ( str1 == "Linie") ? base::LineCategory::eLine : base::LineCategory::eArea;
	g_Statistics.SetLineAttribute(pGrid->GetSelect(), lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_CATEGORY, WPARAM(pGrid->GetSelect()));
}


void CPropertiesWnd::OnSetVisible(CPropertyGrid* pGrid)
{
	auto lineAttrib = g_Statistics.GetLineAttribute(pGrid->GetSelect());
	COleVariant i = pGrid->GetValue();// get the change value.
	auto val = pGrid->GetValue();
	LPVARIANT pVar = (LPVARIANT)val;
	ASSERT(pVar->vt == VT_BSTR);
	CString str1{ pVar->bstrVal };
	lineAttrib.m_Visible = (str1 == "Ja") ? TRUE : FALSE;
	g_Statistics.SetLineAttribute(pGrid->GetSelect(), lineAttrib);
	AfxGetMainWnd()->SendMessage(WM_VISIBLE, WPARAM(pGrid->GetSelect()));
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}


void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}


LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lParam)
{
	// pProp will have all the variables and info of the active or change property
	CMFCPropertyGridProperty* pGrid = (CMFCPropertyGridProperty*)lParam;
	auto pRuntime = pGrid->GetRuntimeClass();
	if (pRuntime->IsDerivedFrom(RUNTIME_CLASS(CMFCPropertyGridColorProperty)))
	{
		CPropertyColorGrid* pUnique = (CPropertyColorGrid*)pGrid;
		OnSetColor(pUnique);
	}
	else
	{
		CPropertyGrid* pUnique = (CPropertyGrid*)pGrid;
		switch (pUnique->GetId())
		{
		case ID_LINEWIDTH:
			OnSetLineWidth(pUnique);
			break;
		case ID_CATEGORY:
			OnSetCategory(pUnique);
			break;
		case ID_VISIBLE:
			OnSetVisible(pUnique);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
	return 0;
}
