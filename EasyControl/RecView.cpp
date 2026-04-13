//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecView
///
/// @file   RecView.cpp
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
#include "RecView.h"
#include "DiagramView.h"
#include "DiagramDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ICON_SIZE 16
#define IMAGE_MAX 2
/////////////////////////////////////////////////////////////////////////////
// CRecView
BEGIN_MESSAGE_MAP(CRecView, CListView)
	ON_WM_CREATE()
	ON_MESSAGE( WM_NEW_RECITEM,  OnNewRecItem)
	ON_MESSAGE( WM_DIAGRAMBAR_SELECT, OnDiagramSelect)
	ON_MESSAGE( WM_DIAGRAMBAR_DOSEITEM, OnDiagramBarDoseItem)
	ON_MESSAGE( WM_DIAGRAMBAR_TIME, OnDiagramBarTime)
	ON_MESSAGE( WM_DIAGRAMBAR_DIAGRAMTYPE, OnDiagramBarDiagramType)
	
END_MESSAGE_MAP()


IMPLEMENT_DYNCREATE(CRecView, CListView)
//******************************************************************************************************
//******************************************************************************************************
CRecView::CRecView() : m_szNr("")
                     , m_szTime("")
                     , m_szFeeder("")
                     , m_szMassflow("")
                     , m_szSetpoint("")
                     , m_szTotalizer("")
                     , m_szDriveCommand("")
                     , m_szWeight("")
                     , m_szDosePerformance("")
                     , m_szRotationalSpeed("")
{
	m_aImageList.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR, 0, IMAGE_MAX); // 32, 32 for large icons
	HICON aIcon = AfxGetApp()->LoadIcon(IDI_ICONRESET);
    ASSERT(aIcon);
	m_aImageList.Add(aIcon);
	aIcon = AfxGetApp()->LoadIcon(IDI_ICONRESETMESSAGE);
    ASSERT(aIcon);
	m_aImageList.Add(aIcon);
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::InitItem(const base::MassflowBitfield& iSelect)
{
	LV_COLUMN lvc;
    
	CListCtrl& aListCtrl = GetListCtrl();
    aListCtrl.DeleteAllItems();
    const int32_t nColumnCount = aListCtrl.GetHeaderCtrl()->GetItemCount();
    // Delete all of the columns.
    for (int32_t i=0;i < nColumnCount;i++)
    {
        aListCtrl.DeleteColumn(0);
    }

	int32_t iSubItem = 0;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem	= iSubItem;
	lvc.cx			= 50;
	lvc.fmt			= LVCFMT_LEFT;
	lvc.pszText		= LPTSTR(LPCTSTR(m_szNr));
	lvc.cchTextMax  = (int32_t) m_szNr.GetLength(); 
	aListCtrl.InsertColumn(iSubItem,&lvc);
	iSubItem++;

	lvc.cx			= 100;
	lvc.iSubItem	= iSubItem;
	lvc.fmt			= LVCFMT_CENTER;
	lvc.pszText		= LPTSTR(LPCTSTR(m_szTime));
	lvc.cchTextMax  = m_szTime.GetLength();
	aListCtrl.InsertColumn(iSubItem,&lvc);
	iSubItem++;

	if (iSelect.flags.bitVIEWMASSFLOW)
	{
		lvc.cx = 90;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szMassflow));
		lvc.cchTextMax = m_szMassflow.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWSETPOINT)
	{
		lvc.cx = 90;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szSetpoint));
		lvc.cchTextMax = m_szSetpoint.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWDRIVECOMMAND)
	{
		lvc.cx = 95;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szDriveCommand));
		lvc.cchTextMax = m_szDriveCommand.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWWEIGHT)
	{
		lvc.cx = 80;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szWeight));
		lvc.cchTextMax = m_szWeight.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWDOSEPERFORMANCE)
	{
		lvc.cx = 110;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szDosePerformance));
		lvc.cchTextMax = m_szDosePerformance.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWTOTALIZER)
	{
		lvc.cx = 90;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szTotalizer));
		lvc.cchTextMax = m_szTotalizer.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
		iSubItem++;
	}
	if (iSelect.flags.bitVIEWROTSPEED)
	{
		lvc.cx = 100;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.iSubItem = iSubItem;
		lvc.pszText = LPTSTR(LPCTSTR(m_szRotationalSpeed));
		lvc.cchTextMax = m_szRotationalSpeed.GetLength();
		aListCtrl.InsertColumn(iSubItem, &lvc);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::InitAll()
{
	LV_COLUMN lvc;
	CString szTemp;
    
	CListCtrl& aListCtrl = GetListCtrl();
    aListCtrl.DeleteAllItems();
    const int32_t nColumnCount = aListCtrl.GetHeaderCtrl()->GetItemCount();
    // Delete all of the columns.
    for (int32_t i=0;i < nColumnCount;i++)
    {
        aListCtrl.DeleteColumn(0);
    }

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem	= 0;
	lvc.cx			= 50;
	lvc.fmt			= LVCFMT_LEFT;
	lvc.pszText		= LPTSTR(LPCTSTR(m_szNr));
	lvc.cchTextMax  = (int32_t) m_szNr.GetLength(); 
	aListCtrl.InsertColumn(0,&lvc);

	lvc.cx			= 120;
	lvc.iSubItem	= 1;
	lvc.fmt			= LVCFMT_CENTER;
	lvc.pszText		= LPTSTR(LPCTSTR(m_szTime));
	lvc.cchTextMax  = m_szTime.GetLength();
	aListCtrl.InsertColumn(1,&lvc);

	auto* pDoc = (CDiagramDoc*)GetDocument();
	ASSERT(pDoc);
	const auto& recItemList = pDoc->GetRecItemList();
	auto sMaxItems = recItemList.ScanMaxItems();
	for (int32_t i = 0; i < _S32(sMaxItems); i++)
	{
		lvc.fmt		= LVCFMT_RIGHT;
		lvc.iSubItem	= 2+i;
		szTemp.Format(_T("%s %d"), LPCTSTR(m_szFeeder), (i+1));
		lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
		lvc.cchTextMax  = szTemp.GetLength();
		aListCtrl.InsertColumn(i+3,&lvc);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::Init()
{
	auto& rDiagramSettings = GetDiagramConfig();
	rDiagramSettings.SetDiagramType(base::eDiagramType::eTable);
	if ( (rDiagramSettings.GetTimeInterval() != base::eTimeMode::TG_TODAY) &&
		 (rDiagramSettings.GetTimeInterval() != base::eTimeMode::TG_DATE))
	{ 
		rDiagramSettings.SetTimeInterval(base::eTimeMode::TG_TODAY);
	}
	if (rDiagramSettings.GetID() < 0)
	{
		InitAll();
	}
	else
	{
		InitItem(rDiagramSettings.GetDiagramSelect());
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::DrawDate(void)
{
    BuildGrid();
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::Draw(void) 
{
	auto& rDiagramSettings = GetDiagramConfig();
	const auto timeintervall = rDiagramSettings.GetTimeInterval();
	if ((timeintervall == base::eTimeMode::TG_DATE) ||
		(timeintervall == base::eTimeMode::TG_TODAY))
    {
        DrawDate ();
    }
}
//******************************************************************************************************
//******************************************************************************************************
inline void CRecView:: FormatString(const float f, CString& szTemp)
{
    if ( f < 1.0f)
    {
	    szTemp.Format(_T(" %4.2f"), f);
    }
    else
    {
	    szTemp.Format(_T(" %5.1f"), f);
    }
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView:: BuildItem(const base::MassflowBitfield& iSelect, const base::utils::CRecItemList& aR)
{
	auto& rDiagramSettings = GetDiagramConfig();
    ASSERT(rDiagramSettings.GetID() >= 0);
	if (rDiagramSettings.GetID() < 0)
	{
		return;
	}
	const auto lCount = aR.GetCount();
	if ( ! lCount)
		return;


	CListCtrl& aListCtrl = GetListCtrl();
	aListCtrl.DeleteAllItems();
	aListCtrl.Invalidate();

    DECLARE_MASSFLOWSELECT(field);
    for ( uint32_t aPos = 0; aPos < lCount; aPos++ )
	{
		CString szTemp;
		LVITEM lvi;

		const auto& aItem = aR.GetItem(aPos);
		szTemp.Format(_T(" %5d"), _S32(aItem.GetID()));
		lvi.mask		= LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem		= 0;
		lvi.iSubItem	= 0;
		lvi.pszText		= LPTSTR(LPCTSTR(szTemp));
		lvi.iImage		= 0;
		lvi.stateMask	= LVIS_STATEIMAGEMASK;
		lvi.cchTextMax	= szTemp.GetLength();
		lvi.state		= INDEXTOSTATEIMAGEMASK(1);
		aListCtrl.InsertItem(&lvi);

		aListCtrl.SetItemData(0, DWORD_PTR(&aItem));
		szTemp = StdTimeToCTime(aItem.GetTime()).Format( "%d.%m.%y   %H:%M");
		VERIFY(aListCtrl.SetItemText(0,1,LPCTSTR(szTemp)));

        int32_t offset = 2;
		if (iSelect.flags.bitVIEWMASSFLOW)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWMASSFLOW, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWSETPOINT)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWSETPOINT, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWDRIVECOMMAND)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWDRIVECOMMAND, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWWEIGHT)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWWEIGHT, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWDOSEPERFORMANCE)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWDOSEPERFORMANCE, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWTOTALIZER)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWTOTALIZER, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
		if (iSelect.flags.bitVIEWROTSPEED)
		{
			const auto value = aItem.Get(base::eMassflowSelect::eVIEWROTSPEED, rDiagramSettings.GetID());
			FormatString(value, szTemp);
			VERIFY(aListCtrl.SetItemText(0, offset, LPCTSTR(szTemp)));
			offset++;
		}
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView:: BuildDate(const base::utils::CRecItemList& aR)
{
	auto lCount = aR.GetCount();
	if (  lCount <= 0)
		return;

	CListCtrl& aListCtrl = GetListCtrl();
	aListCtrl.DeleteAllItems();
	aListCtrl.Invalidate();
	CString szTime;
	CString szTemp;

	for ( uint32_t aPos = 0; aPos < lCount; aPos++)
	{
		LVITEM lvi;

		const auto& aItem = aR.GetItem(aPos);
		szTemp.Format(_T(" %5d"), _S32(aItem.GetID()));
		lvi.mask		= LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem		= 0;
		lvi.iSubItem	= 0;
		lvi.pszText		= LPTSTR(LPCTSTR(szTemp));
		lvi.iImage		= 0;
		lvi.stateMask	= LVIS_STATEIMAGEMASK;
		lvi.cchTextMax	= szTemp.GetLength();
		lvi.state		= INDEXTOSTATEIMAGEMASK(1);
		aListCtrl.InsertItem(&lvi);

		aListCtrl.SetItemData(0, DWORD_PTR(&aItem));
		szTemp = StdTimeToCTime(aItem.GetTime()).Format( "%d.%m.%y   %H:%M");
		aListCtrl.SetItemText(0,1, LPCTSTR(szTemp));

        const int32_t offset = 2;
		for (int32_t j = 0; j < _S32(aItem.GetMaxItems()); j++)
		{
            const auto actualvalue  = aItem.Get(base::eMassflowSelect::eVIEWMASSFLOW, j);
            const auto nominalvalue = aItem.Get(base::eMassflowSelect::eVIEWSETPOINT, j);
            szTemp.Format(_T(" %5.1f (%5.1f)"), actualvalue, nominalvalue);
			aListCtrl.SetItemText(0, offset + j,LPCTSTR(szTemp));
		}
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::BuildGrid (void)
{
	auto* pDoc = (CDiagramDoc*)GetDocument();
	ASSERT(pDoc);
	auto& rDiagramSettings = pDoc->GetDiagramConfig();
	if (rDiagramSettings.GetID() >= 0)
    {
        BuildItem(rDiagramSettings.GetDiagramSelect(),pDoc->GetRecItemList());
    }
    else
    {
        BuildDate(pDoc->GetRecItemList());
    }
    //pDoc->SetDocTitle();
}
//******************************************************************************************************
//******************************************************************************************************
/////////////////////////////////////////////////////////////////////////////
// CRecView diagnostics
#ifdef _DEBUG
void CRecView::AssertValid() const
{
	CListView::AssertValid();
}

void CRecView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDiagramDoc* CRecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDiagramDoc)));
	return (CDiagramDoc*)m_pDocument;
}
#endif //_DEBUG
//******************************************************************************************************
//******************************************************************************************************
int32_t CRecView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if ( CListView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }
    VERIFY(m_szTime.LoadString(IDS_TIME));
    VERIFY(m_szFeeder.LoadString(IDS_DOSIERUNG));
    VERIFY(m_szMassflow.LoadString(IDS_MASSFLOW_KGH));
    VERIFY(m_szSetpoint.LoadString(IDS_SETPOINT_KGH));
    VERIFY(m_szTotalizer.LoadString(IDS_TOTALIZER_KG));
    VERIFY(m_szDriveCommand.LoadString(IDS_DRIVECOMMAND_PERCENTAGE));
    VERIFY(m_szWeight.LoadString(IDS_WEIGHT_KG));
    VERIFY(m_szDosePerformance.LoadString(IDS_DOSEPERFORMANCE_KGH));
    VERIFY(m_szRotationalSpeed.LoadString(IDS_ROTSPEED_UPM));

	CListCtrl& aListCtrl = GetListCtrl();
	aListCtrl.SetExtendedStyle (aListCtrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	aListCtrl.SetImageList(&m_aImageList, LVSIL_SMALL );
    return 0;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CRecView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}
//******************************************************************************************************
//******************************************************************************************************
void CRecView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
    if ( bActivate )
    {
		LOGDEBUG("Activate CRecView !");
		AfxGetMainWnd()->SendMessage(WM_ACTIVATE_VIEW, WPARAM(eViewSelect::eDiagramView), 0);
		Init();
		Draw();
		BringWindowToTop();
	}
    CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//******************************************************************************************************
//******************************************************************************************************
void CRecView::OnInitialUpdate()
{
	UpdateWindow();
	CListView::OnInitialUpdate();
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecView::OnDiagramSelect(WPARAM wParam, LPARAM )
{
	auto& rDiagramSettings = GetDiagramConfig();
	ASSERT(rDiagramSettings.GetDiagramType() == base::eDiagramType::eTable);
	auto newflags = _U32(wParam);
	if (rDiagramSettings.GetDiagramSelect().allflags != newflags)
	{
		base::MassflowBitfield iSelect;
		iSelect.allflags = newflags;
		rDiagramSettings.SetDiagramSelect(iSelect);
		Init();
		Draw();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecView::OnDiagramBarDoseItem(WPARAM wParam, LPARAM)
{
	auto& rDiagramSettings = GetDiagramConfig();
	ASSERT(rDiagramSettings.GetDiagramType() == base::eDiagramType::eTable);
	if ( IsValidTimeInterval(rDiagramSettings.GetTimeInterval()) )
	{
		auto item = static_cast<int32_t>(wParam);
		if (rDiagramSettings.GetID() != item)
		{
			rDiagramSettings.SetID(item);
			Init();
			Draw();
		}
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecView::OnDiagramBarTime(WPARAM wParam, LPARAM lParam)
{
	auto item = static_cast<base::eTimeMode>(wParam);
	if ( IsValidTimeInterval(item) )
	{
		auto& rDiagramSettings = GetDiagramConfig();
		ASSERT(rDiagramSettings.GetDiagramType() == base::eDiagramType::eTable);
		if (item == base::eTimeMode::TG_DATE)
		{
			LoadRecItemList((time_t)lParam);
		}
		rDiagramSettings.SetTimeInterval(item);
		Init();
		Draw();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CRecView::OnDiagramBarDiagramType(WPARAM wParam, LPARAM )
{
	auto item = static_cast<base::eDiagramType>(wParam);
	auto& rDiagramSettings = GetDiagramConfig();
	if (item != rDiagramSettings.GetDiagramType())
	{
		if (!IsValidTimeInterval(rDiagramSettings.GetTimeInterval()))
		{
			rDiagramSettings.SetTimeInterval(base::eTimeMode::TG_TODAY);
		}
		rDiagramSettings.SetDiagramType(item);
		Init();
		Draw();
	}
	return 0L;
}

//******************************************************************************************************
//******************************************************************************************************
LRESULT CRecView::OnNewRecItem(WPARAM, LPARAM)
{
	auto& rDiagramSettings = GetDiagramConfig();
	ASSERT(rDiagramSettings.GetDiagramType() == base::eDiagramType::eTable);
	if (rDiagramSettings.GetTimeInterval() == base::eTimeMode::TG_TODAY)
	{
		BuildGrid();
	}
	return 0L;
}
