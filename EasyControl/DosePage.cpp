//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DosePage.cpp
///
/// @file   DosePage.cpp
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
#include "DosePage.h"
#include "RemoteControl.h"
#include "DataImage.h"
#include "AlarmMap.h"
#include "ToolTipDlg.h"
#include "GlobalConst.h"
#include "ECMessageBox.h"
#include "AgitatorSpeedBoxDlg.h"
#include "ProductDatabaseBox.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"


#define OFFSETCATAGORY 2
//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CDosePage, CMFCPropertyPage)
	ON_MESSAGE(WM_NEW_DOSEITEM, OnDoseNewItem)
	ON_MESSAGE(WM_TIMER_REFRESH, OnTimerRefresh)
	ON_MESSAGE(WM_PRODUCTKEY, OnNewProductKey)
	ON_MESSAGE(WM_SPEEDBOXCLOSED, OnSpeedBoxClosed)
	ON_MESSAGE(WM_HELPINFOCLOSED, OnGainFocus)
	ON_MESSAGE(WM_SCALEBOXCLOSED, OnGainFocus)
	ON_MESSAGE(WM_PRODUCTDATABASEBOXCLOSED, OnGainFocus)
	ON_MESSAGE(WM_REFILLBOXCLOSED, OnGainFocus)
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CDosePage, CMFCPropertyPage)
CDosePage::CDosePage(UINT id): CMFCPropertyPage(id)
	, m_aGrafikContainer()
	, c_EditBrush{ EDITBKCOLOR }
	, m_szTitle(_T(""))
	, m_szName(_T(""))
	, m_QMNumber(0)
	, m_szFeederType(_T(""))
	, m_lNr(0)
	, m_sItem(0)
	, m_lDoseType(base::eDoseType::DOSETYPE_MAX)
{
    VERIFY(m_aFont.CreateFont(
   40,                        // nHeight
   0,                         // nWidth
   0,                         // nEscapement
   0,                         // nOrientation
   FW_BOLD,                   // nWeight
   FALSE,                     // bItalic
   FALSE,                     // bUnderline
   0,                         // cStrikeOut
   ANSI_CHARSET,              // nCharSet
   OUT_DEFAULT_PRECIS,        // nOutPrecision
   CLIP_DEFAULT_PRECIS,       // nClipPrecision
   DEFAULT_QUALITY,           // nQuality
   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
   _T("Arial")));                 // lpszFacename
}
//******************************************************************************************************
//******************************************************************************************************
void CDosePage::SetNumberFont(int32_t nr)
{
	CWnd* pWnd = GetDlgItem(nr);
	ASSERT(pWnd);
	pWnd->SetFont(&m_aFont);
}

//******************************************************************************************************
//******************************************************************************************************
LRESULT CDosePage::OnDoseNewItem(WPARAM , LPARAM)
{
	m_sItem = getActiveItem();
	if ( m_sItem >= 0)
	{
		SetValue();
		Invalidate(TRUE);
	}
	else
	{
		EndDialog(0);
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CDosePage::OnTimerRefresh(WPARAM , LPARAM)
{
	if ( m_sItem < 0)
	{
		return 0L;
	}
	SetValue();
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
void CDosePage::SetValue(void)
{
	if ( m_sItem < 0 )
	{
		return;
	}
	REMOTEREF.getDoseType(m_sItem, m_lDoseType);
	std::string name;
	REMOTEREF.getDoseName(m_sItem, name);
	m_szName					= toCString(name);
	m_lNr                       = m_sItem + 1;

	base::eFeedingType feederstate;
	REMOTEREF.getDoseFeedingType(m_sItem, feederstate);
	m_szFeederType = toCString(c_FeedingTypeMap.get(feederstate));

	REMOTEREF.getDoseQMNumber(m_sItem, m_QMNumber);

	VERIFY(m_szTitle.LoadString(globalconst::g_DoseTitleIDIndex[_S32(m_lDoseType)]));
	OnUpdateControls();
}
//****************************************************************************************************
//****************************************************************************************************
void CDosePage::BuildHeadLine(CListCtrl& iListCtrl)
{
	LV_COLUMN lvc;

	int32_t sMaxCharLength = 0;
	for (int32_t i = 0; i < _S32(base::eAlarmErrorMax); i++)
	{
		auto szTemp = CAlarmMap::GetAlarmString(base::eAlarmError(i));
		sMaxCharLength = __max(sMaxCharLength, _S32(szTemp.GetLength()));
	}
	const int32_t cxwidth = __min(sMaxCharLength * 5, 120);

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.cx = 30;
	lvc.fmt = LVCFMT_RIGHT;
	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_ALARMNR));
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = (int32_t)szTemp.GetLength();
	iListCtrl.InsertColumn(0, &lvc);


	VERIFY(szTemp.LoadString(IDS_ALARMMELDUNG));
	lvc.cx = cxwidth;
	lvc.iSubItem = 1;
	lvc.fmt = LVCFMT_LEFT;
	lvc.pszText = LPTSTR(LPCTSTR(szTemp));
	lvc.cchTextMax = szTemp.GetLength();
iListCtrl.InsertColumn(1, &lvc);


const std::array<CString, base::eAlarmCategoryMax> c_CateorySC{ _T("AM"), _T("S"), _T("FL"), _T("HS"), _T("RA") };
int32_t subitem = OFFSETCATAGORY;
for (int32_t i = 0; i < base::eAlarmCategoryMax; i++)
{
	lvc.cx = 40;
	lvc.iSubItem = subitem;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = LPTSTR(LPCTSTR(c_CateorySC[i]));
	lvc.cchTextMax = c_CateorySC[i].GetLength();
	iListCtrl.InsertColumn(subitem, &lvc);
	subitem++;
}
szTemp = _T("I");
lvc.cx = 36;
lvc.iSubItem = subitem;
lvc.fmt = LVCFMT_CENTER;
lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_IMAGE | LVCF_SUBITEM;
iListCtrl.InsertColumn(subitem, &lvc);
}
//******************************************************************************************************
//******************************************************************************************************
void CDosePage::GetValue(void)
{
	UpdateData(TRUE);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnInitDialog()
{
	m_sItem = getActiveItem();
	return CMFCPropertyPage::OnInitDialog();
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnSetActive()
{
	m_sItem = getActiveItem();
	auto result = CMFCPropertyPage::OnSetActive();
	SetValue();
	return result;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnUpdateControls()
{
	ASSERT(m_sItem >= 0);
	auto& aV = dataimage::getDoseDataImage(m_sItem);
	auto& rItem = aV.m_Container.GetDisplayItem(CDisplayContainer::eDisplayItem::eFeederDisplay);
	auto aP = dynamic_cast<CFeederDisplay*>(rItem.GetGrafikContext());
	ASSERT(aP);
	if (aP)
	{
		m_aGrafikContainer.SetFeederDisplay(aP);
		m_aGrafikContainer.Redraw();
		m_aGrafikContainer.Invalidate();
	}
	UpdateData(FALSE);
	return TRUE;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnKillActive()
{
	CToolTipDlg::DestroyInfo();
	GetValue();
	return CMFCPropertyPage::OnKillActive();
}
//******************************************************************************************************
//******************************************************************************************************
void CDosePage::OnOK()
{
	GetValue();
	CMFCPropertyPage::OnOK();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CDosePage::ToggleLineMode()
{
	REMOTEREF.setDosePBStop(m_sItem);

	BOOL bChange = TRUE;
	base::ProcessStatus pulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, pulStatus);
	const BOOL bLocalMode = pulStatus.flags.lineMode == false;
	if (bLocalMode)
	{
		REMOTEREF.setDosePBInline(m_sItem);
	}
	else
	{
		const float32_t c_epsilon = 1e-03F;

		float32_t dosepercentage = 0.0F;
		REMOTEREF.getDosePercentage(m_sItem, dosepercentage);
		if (dosepercentage > 0.0F)
		{
			bChange = (ECMessageBox(IDS_QUERY_VALIDPERCENTAGE, MB_ICONWARNING | MB_YESNO) == IDYES);
		}
		if ( bChange)
		{
			REMOTEREF.setDosePBLocal(m_sItem);
		}
	}
	if (bChange)
	{
		REMOTEREF.setDosePercentage(m_sItem, 0.0f);
		REMOTEREF.setDoseSetpoint(m_sItem, 0.0f);
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
		SetValue();
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CDosePage::ToggleGravimetricMode()
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bVolumetric = ulStatus.flags.gravimetric == false;
	if (bVolumetric)
	{
		REMOTEREF.setDosePBGravimetric(m_sItem);
	}
	else
	{
		REMOTEREF.setDosePBVolumetric(m_sItem);
	}
	SetValue();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CDosePage::ToggleReleaseMode()
{
	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	REMOTEREF.setDoseRefillRelease(m_sItem, ! status.flags.refillrelease);
	SetValue();
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDosePage::CreateHelpInfoBox(const int32_t InfoId, const int32_t captionId, const int32_t textId)
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, InfoId, captionId, textId);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnNotifyEditName()
{
	auto bModified = CEditCtrlEx::GetString(m_szName);
	if (bModified)
	{
		REMOTEREF.setDoseName(m_sItem, toStdString(m_szName));
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CDosePage::OnNotifyEditQMNumber()
{
	auto value = CEditCtrl::GetUint64();
	BOOL bModified = (value != m_QMNumber);
	if (bModified)
	{
		m_QMNumber = value;
		REMOTEREF.setDoseQMNumber(m_sItem, m_QMNumber);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CDosePage::OnNewProductKey(WPARAM id, LPARAM )
{
	SetFocus();
	auto pItem = PRODUCTDATABASEREF.GetItemID(_S32(id));
	if (pItem)
	{
		REMOTEREF.setDoseName(m_sItem, pItem->GetProductName());
		REMOTEREF.setDoseQMNumber(m_sItem, pItem->GetQMNumber());
		SetValue();
	}
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
void CDosePage::CreateToolTip(UINT ctrl, UINT MessageID)
{
	auto pWnd = GetDlgItem(ctrl);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	const auto& cp = aRect.TopLeft();
	CToolTipDlg::CreateInfo(this, CPoint{ cp.x - 16, cp.y - 40 }, MessageID);
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CDosePage::OnSpeedBoxClosed(WPARAM, LPARAM)
{
	CWnd* pWnd = GetFocus();
	if (pWnd != this)
	{
		SetFocus();
	}
	return 0L;
}
//**************************************************************************************
//**************************************************************************************
LRESULT CDosePage::OnGainFocus(WPARAM, LPARAM)
{
	CWnd* pWnd = GetFocus();
	if (pWnd != this)
	{
		SetFocus();
	}
	PostMessage(WM_TIMER_REFRESH);
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
void CDosePage::CreateAgitatorBox(UINT ctrl, UINT messageID)
{
	auto pWnd = GetDlgItem(ctrl);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CAgitatorSpeedBoxDlg::CreateSpeedBox(this, m_sItem, CPoint{ aRect.left - 140, aRect.top - 80 }, messageID);
}
//***************************************************************************************
//***************************************************************************************
void CDosePage::CreateProductDatabaseBox(UINT ctrl, BOOL bName)
{
	auto pWnd = GetDlgItem(ctrl);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CProductDatabaseBox::CreateProductDatabaseBox(this, bName, CPoint{ aRect.left + 30, aRect.top });
}










