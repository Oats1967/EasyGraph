//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DosePropertySheet.cpp
///
/// @file   DosePropertySheet.cpp
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
#include "RemoteControl.h"
#include "EasyControl.h"
#include "DosePropertySheet.h"
#include "MFCMacros.h"






CDosePropertySheet* CDosePropertySheet ::m_pDoseProperty = NULL;


IMPLEMENT_DYNAMIC(CDosePropertySheet, CMFCPropertySheet)
CDosePropertySheet::CDosePropertySheet(CWnd* pParentWnd, int32_t sID)
	:CMFCPropertySheet(IDS_DOSIEREIGENSCHAFTEN, pParentWnd, 0)
	, m_sItem(sID)
	, m_pParentWnd(pParentWnd)
	, m_ImageId{ ePages::eRecipePage, ePages::eMotorPage, ePages::eAlarmPage,
				 ePages::eAlarmConfigPage, ePages::eAlertPage }
{
	AddPages(m_sItem);
}

CDosePropertySheet::~CDosePropertySheet()
{
	m_pDoseProperty = NULL;
}


BEGIN_MESSAGE_MAP(CDosePropertySheet, CMFCPropertySheet)
	ON_MESSAGE(WM_NEW_DOSEITEM, OnDoseNewItem)
	ON_MESSAGE(WM_TIMER_REFRESH, OnTimerRefresh)
	ON_MESSAGE(WM_NEW_LOGITEM, OnNewLogItem)
	ON_MESSAGE(WM_DOSEEND, OnDoseEnd)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//*****************************************************************************************************************
//*****************************************************************************************************************
LRESULT CDosePropertySheet::OnDoseNewItem(WPARAM wNewItem, LPARAM)
{
	const int32_t sNewItem = _S32(wNewItem);
	ASSERT(_S32(sNewItem) != m_sItem);
	if ((sNewItem == m_sItem) || (sNewItem < 0))
	{
		PostMessage(WM_CLOSE);
	}
	else
	{
		SetNewItem(sNewItem);
	}
	return 0;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
LRESULT CDosePropertySheet::OnDoseEnd(WPARAM, LPARAM)
{
	PostMessage(WM_CLOSE);
	return 0;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
LRESULT CDosePropertySheet::OnTimerRefresh(WPARAM, LPARAM)
{
	auto* p = GetActivePage();
	return (p) ? p->SendMessage(WM_TIMER_REFRESH) : 0L;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
LRESULT CDosePropertySheet::OnNewLogItem(WPARAM, LPARAM)
{
	auto* p = GetActivePage();
	return (p) ? p->SendMessage(WM_NEW_LOGITEM) : 0L;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::MyTitle (void)
{
    if ( m_sItem < 0 )
    {
        return;
    }
    CString szTemp1;
    CString szTemp;

    VERIFY(szTemp1.LoadString(IDS_DOSIERUNG));

    szTemp.Format(_T("%s %d"), LPCTSTR(szTemp1), int32_t(m_sItem+1));
    SetTitle(szTemp);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::FillImageIds()
{
	m_ImageId[0] = ePages::eRecipePage;
	m_ImageId[1] = ePages::eMotorPage;
	m_ImageId[2] = ePages::eAlarmPage;
	m_ImageId[3] = ePages::eAlarmConfigPage;
	m_ImageId[4] = ePages::eAlertPage;
};

//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::AddPages(const int32_t newitem)
{
	base::eDoseType lDoseType = base::eDoseType::DOSETYPE_NONE;

	FillImageIds();
	ASSERT(newitem >= 0);
	REMOTEREF.getDoseType(newitem, lDoseType);
	std::vector<CMFCPropertyPage*> pages;
	switch ( lDoseType )
	{
	case base::eDoseType::DOSETYPE_IFL:
			pages.push_back(&m_aIflRecipePage);
			pages.push_back(&m_aIflMotorPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
			m_ImageId[0] = ePages::eRecipePage;
			m_ImageId[1] = ePages::eMotorPage;
			m_ImageId[2] = ePages::eAlarmConfigPage;
			m_ImageId[3] = ePages::eAlertPage;
			break;
	case base::eDoseType::DOSETYPE_IFSO:
	case base::eDoseType::DOSETYPE_IFSU:
		    pages.push_back(&m_aIfsRecipePage);
			pages.push_back(&m_aIfsAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
			m_ImageId[0] = ePages::eRecipePage;
			m_ImageId[1] = ePages::eAlarmPage;
			m_ImageId[2] = ePages::eAlarmConfigPage;
			m_ImageId[3] = ePages::eAlertPage;

			break;

	case base::eDoseType::DOSETYPE_LCL:
			pages.push_back(&m_aLclRecipePage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
			m_ImageId[0] = ePages::eRecipePage;
			m_ImageId[1] = ePages::eAlarmConfigPage;
			m_ImageId[2] = ePages::eAlertPage;
			break;

		case base::eDoseType::DOSETYPE_LWP:
		case base::eDoseType::DOSETYPE_LWS:
		case base::eDoseType::DOSETYPE_LWV:
		default:
		{
			pages.push_back(&m_aLwfRecipePage);
			pages.push_back(&m_aLwfMotorPage);
			pages.push_back(&m_aLwfAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
		}
		break;

		case base::eDoseType::DOSETYPE_LWB:
		{
			pages.push_back(&m_aLwbRecipePage);
			pages.push_back(&m_aLwbMotorPage);
			pages.push_back(&m_aLwfAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
		}
		break;

		case base::eDoseType::DOSETYPE_ICB:
		{
			pages.push_back(&m_aIcbRecipePage);
			pages.push_back(&m_aLwbMotorPage);
			pages.push_back(&m_aIcpAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
		}
		break;

		case base::eDoseType::DOSETYPE_GWB:
		{
			pages.push_back(&m_aLwbRecipePage);
			pages.push_back(&m_aLwbMotorPage);
			pages.push_back(&m_aLwfAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
		}
		break;

		case base::eDoseType::DOSETYPE_WBF:
		{
			pages.push_back(&m_aWbfRecipePage);
			pages.push_back(&m_aWbfMotorPage);
			pages.push_back(&m_aWbfAlarmPage);
			pages.push_back(&m_aAlarmConfigPage);
			pages.push_back(&m_aAlertPage);
		}
        break;

		case base::eDoseType::DOSETYPE_WBM:
			{
				pages.push_back(&m_aWbmRecipePage);
				pages.push_back(&m_aWbmMotorPage);
				pages.push_back(&m_aWbmAlarmPage);
				pages.push_back(&m_aAlarmConfigPage);
				pages.push_back(&m_aAlertPage);
		}
			break;
		case base::eDoseType::DOSETYPE_PID:
			{
				pages.push_back(&m_aPidRecipePage);
				pages.push_back(&m_aPidMotorPage);
				pages.push_back(&m_aPidAlarmPage);
				pages.push_back(&m_aAlarmConfigPage);
				pages.push_back(&m_aAlertPage);
		}
			break;
		case base::eDoseType::DOSETYPE_ICP:
			{
				pages.push_back(&m_aIcpRecipePage);
				pages.push_back(&m_aIcpMotorPage);
				pages.push_back(&m_aIcpAlarmPage);
				pages.push_back(&m_aAlarmConfigPage);
				pages.push_back(&m_aAlertPage);
		}
			break;
		case base::eDoseType::DOSETYPE_VOL:
			{
				pages.push_back(&m_aVolRecipePage);
				pages.push_back(&m_aVolMotorPage);
				pages.push_back(&m_aVolAlarmPage);
				pages.push_back(&m_aAlarmConfigPage);
				pages.push_back(&m_aAlertPage);
		}
			break;
		case base::eDoseType::DOSETYPE_NONE:
			{
			}
			break;
	}
	BOOL update = FALSE;
	if (pages != m_Pages)
	{
		RemovePages();
		m_Pages = pages;
		update = TRUE;
	}
	m_sItem = newitem;
	if (update)
	{
		for (auto& item : m_Pages)
		{
			AddPage(item);
		}
	}
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::RemovePages (void)
{
	for (auto& item : m_Pages)
	{
		RemovePage(item);
	}
	m_Pages.clear();
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::SetNewItem(int32_t newitem)
{
	const int32_t saveLastIndex = GetActiveIndex();
	AddPages(newitem);
	CTabCtrl* pTab = GetTabControl();
	TC_ITEM tcItem;
	tcItem.mask = TCIF_IMAGE;
	for (int32_t i = 0; i < m_Pages.size(); i++)
	{
		tcItem.iImage = m_ImageId[i];
		pTab->SetItem(i, &tcItem);
	}
	SetActivePage((saveLastIndex < GetPageCount()) ? saveLastIndex :0);
	MyTitle();
	auto* p = GetActivePage();
	if (p)
	{
		p->SendMessage(WM_NEW_DOSEITEM, m_sItem);
	}
}
//*****************************************************************************************************************
//*****************************************************************************************************************
BOOL CDosePropertySheet::OnInitDialog()
{
	m_bModeless = FALSE;
	m_nFlags |= WF_CONTINUEMODAL;

	BOOL bResult = CMFCPropertySheet::OnInitDialog();
	m_bModeless = TRUE;
	m_nFlags &= ~WF_CONTINUEMODAL;
	SortButtons ();

	m_imageTab.Create( IDB_TABIMAGES, 30, ePages::eMaxPages, RGB(250,250,250) );
	CTabCtrl *pTab = GetTabControl();
	ASSERT(pTab);
	pTab->SetImageList( &m_imageTab );
	SetNewItem(m_sItem);
	return bResult;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::OnDestroy()
{
 	if (m_pParentWnd && m_pParentWnd->GetSafeHwnd())
	{
		m_pParentWnd->PostMessage(WM_SUBDIALOG_CLOSED, 0);
	}
	CMFCPropertySheet :: OnDestroy();
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::PostNcDestroy()
{
	ASSERT(m_pDoseProperty != NULL);
	delete this;
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CDosePropertySheet::SortButtons(void)
{
	int32_t ids[] = { IDOK };    //, , IDHELP };
	CRect       rectBtn;
	CRect       aRect;
	const int32_t nSpacing = 6;        // space between two buttons...

  // Hide Apply and Help buttons
	HIDE_ID(IDCANCEL);
	HIDE_ID(ID_APPLY_NOW);
	HIDE_ID(IDHELP);

	CWnd* pWnd = (CWnd*)GetDlgItem(IDHELP);
	ASSERT(pWnd);
	pWnd->GetWindowRect(rectBtn);
	ScreenToClient(&rectBtn);
	rectBtn.left = rectBtn.right;
	rectBtn.right = rectBtn.left + nSpacing;
	for (int32_t i = sizeof(ids) / sizeof(int32_t) - 1; i >= 0; i--)
	{
		pWnd = GetDlgItem(ids[i]);
		ASSERT(pWnd);
		pWnd->GetWindowRect(aRect);
		ScreenToClient(&aRect);
		int32_t btnWidth = aRect.Width();
		CRect aR(rectBtn.left - nSpacing - btnWidth, aRect.top, rectBtn.left - nSpacing, aRect.bottom);
		pWnd->MoveWindow(aR);
		rectBtn = aR;
	}
}





