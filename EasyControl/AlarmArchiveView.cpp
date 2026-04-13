//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmArchiveView
///
/// @file   AlarmArchiveView.cpp
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
#include "AlarmArchiveView.h"
#include "AlarmArchiveDoc.h"
#include "RemoteControl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ICON_SIZE 16
#define IMAGE_MAX 3
/////////////////////////////////////////////////////////////////////////////
// CAlarmArchiveView

IMPLEMENT_DYNCREATE(CAlarmArchiveView, CListView)


CAlarmArchiveView::CAlarmArchiveView() : c_ImageArray{ IDI_ICONRESET, IDI_ICONWARN , IDI_ICONALARM }
{
	static_assert(_S32(base::eAlarmClass::eNOTYP) == 0, "Invalid c_ImageArray configuration! ");
	static_assert(_S32(base::eAlarmClass::eWARNTYP) == 1, "Invalid c_ImageArray configuration! ");
	static_assert(_S32(base::eAlarmClass::eALARMTYP) == 2, "Invalid c_ImageArray configuration! ");
}


BEGIN_MESSAGE_MAP(CAlarmArchiveView, CListView)
	//{{AFX_MSG_MAP(CAlarmArchiveView)
	ON_WM_CREATE()
	ON_MESSAGE(WM_LOGTIME, OnLogTime)
	ON_MESSAGE(WM_NEW_LOGITEM, OnNewLogItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//*************************************************************************************
//*************************************************************************************
LRESULT CAlarmArchiveView::OnNewLogItem(WPARAM, LPARAM )
{
	auto* pDoc = (CAlarmArchiveDoc*)GetDocument();
	ASSERT(pDoc);
	if (pDoc->GetZeitraster() == base::eTimeMode::TG_TODAY)
	{
		SetValue();
	}
	return 0L;
}
//*************************************************************************************
//*************************************************************************************
LRESULT CAlarmArchiveView::OnLogTime(WPARAM wParam, LPARAM lParam)
{
	auto item = static_cast<base::eTimeMode>(wParam);

	if ((item == base::eTimeMode::TG_DATE) ||
		(item == base::eTimeMode::TG_TODAY))
	{
		if (item == base::eTimeMode::TG_DATE)
		{
			LoadLogItemList((time_t)lParam);
		}
		auto* pDoc = (CAlarmArchiveDoc*)GetDocument();
		ASSERT(pDoc);
		pDoc->SetZeitraster(item);
		SetValue();
	}
	return 0L;
}

//*************************************************************************************
//*************************************************************************************
void CAlarmArchiveView::SetValue ()
{
	auto* pDoc = (CAlarmArchiveDoc*)GetDocument();
	ASSERT(pDoc);
	const auto& logItemList = pDoc->GetLogItemList();
	auto lMaxCount = logItemList.GetCount();
	if (lMaxCount <= 0)
    {
		return;
    }

	CListCtrl& aListCtrl = GetListCtrl();
	aListCtrl.DeleteAllItems();

	auto lCount =__min(lMaxCount, 50);
	for ( uint32_t aPos = 0; aPos < lCount; aPos++ )
	{
		CString szTemp;
		LVITEM lvi;

		const auto& aItem = logItemList.GetItem((lMaxCount - 1) - aPos);
		//szTemp.Format("%4d", _S32(aItem.GetID())+1);
		szTemp.Format(_T("%4d"), aPos + 1);

		lvi.mask		= LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.iItem		= aPos;
		lvi.iSubItem	= 0;
		lvi.pszText		= LPTSTR(LPCTSTR(szTemp));

        const auto lAT  = aItem.GetAlarmClass();
		lvi.iImage		= aItem.GetChecked() ? 0 : _S32(lAT);

		lvi.stateMask	= LVIS_STATEIMAGEMASK;
		lvi.cchTextMax	= szTemp.GetLength();
		lvi.state		= INDEXTOSTATEIMAGEMASK(1);
		aListCtrl.InsertItem(&lvi);

		auto starttime = [&aItem](const std::time_t& aTime)
		{	return (aTime == 0) ? CString(_T("-")) : StdTimeToCTime(aTime).Format("%d.%m.%y   %H:%M:%S"); };
		szTemp = starttime(aItem.GetTime());
		aListCtrl.SetItemText(aPos,1,LPCTSTR(szTemp));
		szTemp.Format(_T("%d"), int32_t(aItem.GetItem() + 1));
		aListCtrl.SetItemText(aPos, 2, LPCTSTR(szTemp));
		szTemp = toCString(aItem.GetMessage());
		aListCtrl.SetItemText(aPos,3,LPCTSTR(szTemp));


		auto endtime = [&aItem](const std::time_t& aTime)
		{ return ((aTime == 0) || (!aItem.GetChecked())) ? CString(_T("-")) : StdTimeToCTime(aTime).Format("%d.%m.%y   %H:%M:%S"); };
		szTemp = endtime(aItem.GetCheckedTime());
		aListCtrl.SetItemText(aPos, 4, LPCTSTR(szTemp));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAlarmArchiveView diagnostics

#ifdef _DEBUG
void CAlarmArchiveView::AssertValid() const
{
	CListView::AssertValid();
}

void CAlarmArchiveView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAlarmArchiveView message handlers
void CAlarmArchiveView::OnInitialUpdate() 
{
	UpdateWindow();

	CListView::OnInitialUpdate();
}
//************************************************************************************
//************************************************************************************
void CAlarmArchiveView::OnUpdate(CView* , LPARAM , CObject* ) 
{
    ASSERT((GetStyle() & WS_VISIBLE) != 0);
	ASSERT(GetSafeHwnd() != NULL);
    SetValue ();
}
//************************************************************************************
//************************************************************************************
BOOL CAlarmArchiveView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}
//************************************************************************************
//************************************************************************************
int32_t CAlarmArchiveView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	LV_COLUMN lvc;
	CString szTemp;

	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_aImageList.Create(ICON_SIZE, ICON_SIZE, ILC_COLOR, 0, _S32(c_ImageArray.size())); // 32, 32 for large icons
	for (const auto& item : c_ImageArray)
	{
		HICON aIcon = AfxGetApp()->LoadIcon(item);
		int32_t sErg = m_aImageList.Add(aIcon);
		ASSERT(sErg >= 0);
	}
	CListCtrl& aListCtrl = GetListCtrl();
	aListCtrl.SetExtendedStyle (aListCtrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	aListCtrl.SetImageList(&m_aImageList, LVSIL_SMALL );

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	VERIFY(szTemp.LoadString(IDS_ALARMNR));
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cx			= 50;
	lvc.fmt			= LVCFMT_CENTER;
	aListCtrl.InsertColumn(0,&lvc);

	lvc.iSubItem = 0;
	VERIFY(szTemp.LoadString(IDS_TIME));
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cx			= 120;
	lvc.fmt			= LVCFMT_CENTER;
	aListCtrl.InsertColumn(1,&lvc);

	lvc.iSubItem = 0;
	VERIFY(szTemp.LoadString(IDS_DOSIERER));
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cx			= 60;
	lvc.fmt			= LVCFMT_CENTER;
	aListCtrl.InsertColumn(3,&lvc);

	lvc.iSubItem	= 0;
	VERIFY(szTemp.LoadString(IDS_MELDUNG));
	lvc.pszText		= LPTSTR(LPCTSTR(szTemp));
	lvc.cx			= 300;
	lvc.fmt			= LVCFMT_LEFT;
	lvc.cchTextMax	= szTemp.GetLength();
	aListCtrl.InsertColumn(3, &lvc);

	lvc.iSubItem = 0;
	VERIFY(szTemp.LoadString(IDS_BEENDET));
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cx = 120;
	lvc.fmt = LVCFMT_CENTER;
	aListCtrl.InsertColumn(4, &lvc);

	return 0;
}
//***********************************************************************************************
//***********************************************************************************************
void CAlarmArchiveView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		LOGDEBUG("Activate CAlarmArchiveView !");

		AfxGetMainWnd()->SendMessage(WM_ACTIVATE_VIEW, WPARAM(eViewSelect::eAlarmArchiveView), 0);
		auto* pDoc = (CAlarmArchiveDoc*)GetDocument();
		ASSERT(pDoc);
		pDoc->SetZeitraster(base::eTimeMode::TG_TODAY);
		SetValue();
		BringWindowToTop();
	}

	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}



