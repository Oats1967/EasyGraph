
#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "EasyGraph.h"
#include "CheckBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd() noexcept
{
	m_nComboHeight = 0;
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

	m_wndObjectCombo.AddString(_T("Anwendung"));
	m_wndObjectCombo.AddString(_T("Eigenschaftenfenster"));
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
#if 0
void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Darstellung"));

	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D-Darstellung"), (_variant_t) false, _T("Gibt an, dass im Fenster eine nicht fette Schriftart verwendet wird und dass Steuerelemente mit einem 3D-Rahmen versehen werden.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Rahmen"), _T("Dialogfeldrahmen"), _T("Eine der folgenden Optionen: 'Keiner', 'Dünn', 'Größe änderbar' oder 'Dialogfeldrahmen'"));
	pProp->AddOption(_T("Keiner"));
	pProp->AddOption(_T("Dünn"));
	pProp->AddOption(_T("Größe änderbar"));
	pProp->AddOption(_T("Dialogfeldrahmen"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Beschriftung"), (_variant_t) _T("Info"), _T("Gibt den Text an, der in der Titelleiste des Fensters angezeigt wird.")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Fenstergröße"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Höhe"), (_variant_t) 250l, _T("Gibt die Höhe des Fensters an."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Breite"), (_variant_t) 150l, _T("Gibt die Breite des Fensters an."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Schriftart"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Schriftart"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Gibt die Standardschriftart für das Fenster an.")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Systemschriftart verwenden"), (_variant_t) true, _T("Gibt an, dass im Fenster die Schriftart 'MS Shell Dlg' verwendet wird.")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Sonstiges"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Anwendung"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Linienfarbe"), RGB(210, 192, 254), nullptr, _T("Gibt die Standardfarbe des Fensters an."));
	pColorProp->EnableOtherButton(_T("Andere..."));
	pColorProp->EnableAutomaticButton(_T("Standard"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Symboldateien(*.ico)|*.ico|Alle Dateien(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Symbol"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Gibt das Fenstersymbol an.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Ordner"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);
#if 0
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchie"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("Erste Unterebene"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Zweite Unterebene"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 1"), (_variant_t) _T("Wert 1"), _T("Dies ist eine Beschreibung.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 2"), (_variant_t) _T("Wert 2"), _T("Dies ist eine Beschreibung.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 3"), (_variant_t) _T("Wert 3"), _T("Dies ist eine Beschreibung.")));

	pGroup4->Expand(FALSE);
#endif
	m_wndPropList.AddProperty(pGroup4);
}
#else
void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Darstellung"));
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Linienfarbe"), RGB(210, 192, 254), nullptr, _T("Gibt die Standardfarbe des Fensters an."));
	pColorProp->EnableOtherButton(_T("Andere..."));
	pColorProp->EnableAutomaticButton(_T("Standard"), ::GetSysColor(COLOR_3DFACE));
	pGroup1->AddSubItem(pColorProp);

	CMFCPropertyGridProperty* pCategoryProp = new CMFCPropertyGridProperty(_T("Kategorie"), _T("Linientyp"), _T("Eine der folgenden Optionen: 'Linie', 'Fläche"));
	pCategoryProp->AddOption(_T("Linie"));

	pCategoryProp->AddOption(_T("Flaeche"));
	pCategoryProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pCategoryProp);

	CMFCPropertyGridProperty* pLineWidthProp = new CMFCPropertyGridProperty(_T("Linienstaerke"), _T("1"), _T("A numeric value"), NULL, NULL, NULL, _T("0123456789"));
	pLineWidthProp->AddOption(_T("1"));
	pLineWidthProp->AddOption(_T("2"));
	pLineWidthProp->AddOption(_T("3"));
	pLineWidthProp->AllowEdit(TRUE);
	pGroup1->AddSubItem(pLineWidthProp);

	auto* pMassflowhProp = new CCheckBoxProp(_T("Linienstaerke"), _T("1"), _T("A numeric value"), NULL, NULL, NULL, _T("0123456789"));
	pGroup1->AddSubItem(pMassflowhProp);

	m_wndPropList.AddProperty(pGroup1);

#if 0
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D-Darstellung"), (_variant_t) false, _T("Gibt an, dass im Fenster eine nicht fette Schriftart verwendet wird und dass Steuerelemente mit einem 3D-Rahmen versehen werden.")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Rahmen"), _T("Dialogfeldrahmen"), _T("Eine der folgenden Optionen: 'Keiner', 'Dünn', 'Größe änderbar' oder 'Dialogfeldrahmen'"));
	pProp->AddOption(_T("Keiner"));
	pProp->AddOption(_T("Dünn"));
	pProp->AddOption(_T("Größe änderbar"));
	pProp->AddOption(_T("Dialogfeldrahmen"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Beschriftung"), (_variant_t)_T("Info"), _T("Gibt den Text an, der in der Titelleiste des Fensters angezeigt wird.")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Fenstergröße"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Höhe"), (_variant_t)250l, _T("Gibt die Höhe des Fensters an."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty(_T("Breite"), (_variant_t)150l, _T("Gibt die Breite des Fensters an."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Schriftart"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Schriftart"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Gibt die Standardschriftart für das Fenster an.")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Systemschriftart verwenden"), (_variant_t)true, _T("Gibt an, dass im Fenster die Schriftart 'MS Shell Dlg' verwendet wird.")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Sonstiges"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Anwendung"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);


	static const TCHAR szFilter[] = _T("Symboldateien(*.ico)|*.ico|Alle Dateien(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Symbol"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Gibt das Fenstersymbol an.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Ordner"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);
	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchie"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("Erste Unterebene"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Zweite Unterebene"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 1"), (_variant_t)_T("Wert 1"), _T("Dies ist eine Beschreibung.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 2"), (_variant_t)_T("Wert 2"), _T("Dies ist eine Beschreibung.")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Element 3"), (_variant_t)_T("Wert 3"), _T("Dies ist eine Beschreibung.")));

	pGroup4->Expand(FALSE);
#endif
}

#endif

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
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CString str = pProp->GetName(); // get the change property name.
	if (str == "Linienfarbe")
	{
		CMFCPropertyGridColorProperty* pColorGrid = (CMFCPropertyGridColorProperty*)pProp;
		auto color = pColorGrid->GetColor();
		AfxGetMainWnd()->SendMessage(WM_LINECOLOR, WPARAM(color));

	}
	else if (str == "Kategorie")
	{
		auto val = pProp->GetValue();
		LPVARIANT pVar = (LPVARIANT)val;
		ASSERT(pVar->vt == VT_BSTR);

		CString str1{ pVar->bstrVal };
		if (str1 == "Linie")
		{
			AfxGetMainWnd()->SendMessage(WM_CATEGORY, WPARAM(BCGPChartCategory::BCGPChartLine));
		}
		else if (str1 == "Flaeche")
		{
			AfxGetMainWnd()->SendMessage(WM_CATEGORY, WPARAM(BCGPChartCategory::BCGPChartArea));
		}
	}
	else if (str == "Linienstaerke")
	{
		auto val = pProp->GetValue();
		LPVARIANT pVar = (LPVARIANT)val;
		ASSERT(pVar->vt == VT_BSTR);
		CString str1{ pVar->bstrVal };
		int32_t iVal = _ttoi(str1);
		AfxGetMainWnd()->SendMessage(WM_LINEWIDTH, WPARAM(iVal));
	}
	{
		int pID = pProp->GetData();

		COleVariant i = pProp->GetValue();// get the change value.

		//below is the code to change COleVariant to other variable type
		LPVARIANT pVar = (LPVARIANT)i;
		int x;
		short y;
		double d;
		float f;
		bool status;
		CString str1;
		switch (pVar->vt)
		{
		case VT_I2:    // short
			y = pVar->iVal;
			break;
		case VT_I4:     // int
			x = pVar->lVal;
			break;
		case VT_R4:    // float
			f = pVar->fltVal;
			break;
		case VT_R8:    // double
			d = pVar->dblVal;
			break;
		case VT_INT:
			x = pVar->lVal;
			break;
		case VT_BOOL:
			status = pVar->boolVal;
			break;
		case VT_BSTR:
			str1 = pVar->bstrVal;
			break;
			// etc.
		}
	}

	return 0;
}
