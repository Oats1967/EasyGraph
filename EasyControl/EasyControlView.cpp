//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyControlView
///
/// @file   EasyControlView.cpp
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
#include "EasyControlDoc.h"
#include "EasyControlView.h"
#include "ECMessageBox.h"
#include "FeederDisplay.h"
#include "DataImage.h"
#include "RemoteControl.h"
#include "DosePropertySheet.h"
#include "LineTabCtrl.h"
#include "AlarmLog.h"
#include "TKBoardInterface.h"
#include "BASE/EasyTime/public/EasyTime.h"

#if _DEBUG
#include "WeightDemoDlg.h"
#endif



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXBUTTONALIVECOUNTER 10U


// CEasyControlView

IMPLEMENT_DYNCREATE(CEasyControlView, CView)

BEGIN_MESSAGE_MAP(CEasyControlView, CView)
	//ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
#if __SIM_WEIGHT_GUI
	ON_WM_RBUTTONDOWN()
#endif
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_SUBDIALOG_CLOSED, &CEasyControlView::OnDoseSubDialogClosed)
	ON_MESSAGE(WM_TIMER_REFRESH, &CEasyControlView::OnTimerRefresh)
	ON_MESSAGE(WM_DOSEEND, &CEasyControlView::OnDoseEnd)
	ON_MESSAGE(WM_LAYOUTCHANGED, &CEasyControlView::OnLayoutChanged)
	ON_MESSAGE(WM_DEACTIVATE_VIEW, &CEasyControlView::OnDeactivateView)
	ON_MESSAGE(WM_ACTIVATE_VIEW, &CEasyControlView::OnActivateView)
	ON_MESSAGE(WM_INFOBOX_ALARMCLEAR, &CEasyControlView::OnInfoBoxAlarmClear)
	ON_MESSAGE(WM_INFOBOX_FLASHLIGHT, &CEasyControlView::OnInfoBoxFlashlight)
	ON_MESSAGE(WM_NEW_LOGITEM, &CEasyControlView::OnNewLogItem)
	ON_MESSAGE(WM_LOGOUT, &CEasyControlView::OnUserLogOut)
	ON_MESSAGE(WM_ZOOMFACTOR, &CEasyControlView::OnZoomFactor)
	ON_COMMAND(ID_TB_OVERVIEW_ALARM03_CLEAR, &CEasyControlView::OnAlarm03Clear)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_ALARM03_CLEAR, &CEasyControlView::OnUpdateAlarm03Clear)
	ON_COMMAND(ID_TB_OVERVIEW_ALARM1_CLEAR, &CEasyControlView::OnAlarm1Clear)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_ALARM1_CLEAR, &CEasyControlView::OnUpdateAlarm1Clear)
	ON_COMMAND(ID_TB_OVERVIEW_LINESTATUS, &CEasyControlView::OnLineStatus)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_LINESTATUS, &CEasyControlView::OnUpdateLineStatus)
	ON_COMMAND(ID_TB_OVERVIEW_DOSE_PREVIOUS, &CEasyControlView::OnDosiererPrevious)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_DOSE_PREVIOUS, &CEasyControlView::OnUpdateDosiererPrevious)
	ON_COMMAND(ID_TB_OVERVIEW_DOSE_NEXT, &CEasyControlView::OnDosiererNext)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_DOSE_NEXT, &CEasyControlView::OnUpdateDosiererNext)
	ON_COMMAND(ID_TB_OVERVIEW_SERVICEMODE, &CEasyControlView::OnServiceMode)
	ON_UPDATE_COMMAND_UI(ID_TB_OVERVIEW_SERVICEMODE, &CEasyControlView::OnUpdateOnServiceMode)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//***********************************************************************************************
//***********************************************************************************************
CEasyControlView::CEasyControlView() : m_aInfoBox()
, m_LinieDC()
, m_DoseItem(-1)
, m_sMaxItems(0)
, m_zoomfactor { 100 }
, m_CustomerLogoCfg{ LAYOUTCONFIGREF.GetCustomerLogo() }
, m_LogoPos{ CPoint(LAYOUTCONFIGREF.GetPointLogo().x, LAYOUTCONFIGREF.GetPointLogo().y) }
, m_InfoBoxPos{ CPoint(LAYOUTCONFIGREF.GetPointMessageBox().x, LAYOUTCONFIGREF.GetPointMessageBox().y) }
, m_LineBoxPos{ CPoint(LAYOUTCONFIGREF.GetPointLineBox().x, LAYOUTCONFIGREF.GetPointLineBox().y) }
, m_ServiceBoxPos{ CPoint(LAYOUTCONFIGREF.GetPointServiceBox().x, LAYOUTCONFIGREF.GetPointServiceBox().y) }
, m_LineTotalizerBoxPos{ CPoint(LAYOUTCONFIGREF.GetPointTotalizerBox().x, LAYOUTCONFIGREF.GetPointTotalizerBox().y) }
, m_bLineTotalizerBoxEnable { m_LineTotalizerBoxPos.x > 0 && m_LineTotalizerBoxPos.y > 0}
, m_BarChartRect{ LAYOUTCONFIGREF.GetRectBarChart().topleft.x, LAYOUTCONFIGREF.GetRectBarChart().topleft.y,
				  LAYOUTCONFIGREF.GetRectBarChart().bottomright.x, LAYOUTCONFIGREF.GetRectBarChart().bottomright.y }
, m_bBarChartEnable { ! m_BarChartRect.IsRectNull() }
, m_bAlarm1{ FALSE }
, m_bAlarm03{ FALSE }
, m_ButtonAliveCounter { 0 }
, m_bProductNameEnable { LAYOUTCONFIGREF.GetProductNameEnable() }
, m_bDrawRefillButtonsEnable { LAYOUTCONFIGREF.GetRefillButtonEnable() }

{
	VERIFY(m_ServiceModeFont.CreateFont(72, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial")));

	VERIFY(m_LineTotalizerBoxFont.CreateFont(28, 0, 0, 0, FW_BOLD,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial")));

	CString szTemp;
	VERIFY(szTemp.LoadString(IDS_SERVICEMODE));
	m_szServiceMode = szTemp + _T(" !");
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::SetNewDoseItem(int32_t item)
{
	if (item != m_DoseItem)
	{
		setActiveItem(item);
		m_DoseItem = item;
		if (DOSEPROPERTYREF)
		{
			DOSEPROPERTYREF->SendMessage(WM_NEW_DOSEITEM, WPARAM(m_DoseItem));
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnDoseSubDialogClosed(WPARAM , LPARAM)
{
	m_DoseItem = -1;

	if (DOSEPROPERTYREF)
	{
		DOSEPROPERTYREF->Invalidate(FALSE);
	}
	if (LINETABREF)
	{
		LINETABREF->Invalidate(FALSE);
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::SetNewInfoMessage()
{
	auto pLogItem = LOGREF.GetLastUnCheckedItem();
	if (pLogItem)
	{
		m_aInfoBox.SetErrorItem(*pLogItem);
	}
	else
	{
		m_aInfoBox.DeleteErrorItem();
	}
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnNewLogItem(WPARAM , LPARAM)
{
	if (LOGREF.IsChanged())
	{
		SetNewInfoMessage();
	}
	if (DOSEPROPERTYREF)
	{
		DOSEPROPERTYREF->SendMessage(WM_NEW_LOGITEM);
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnUserLogOut(WPARAM, LPARAM)
{
	CloseAllOpenDialogs();
	return 0L;
}
//**************************************************************************************************
//**************************************************************************************************
LRESULT CEasyControlView::OnZoomFactor(WPARAM w, LPARAM)
{
	auto change = SetZoomFactor(_S32(w));
	if (change)
	{
		RecalcLayout();
		Invalidate(FALSE);
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnTimerRefresh(WPARAM , LPARAM)
{
	if (m_ButtonAliveCounter)
		m_ButtonAliveCounter--;

	if ( DOSEPROPERTYREF && DOSEPROPERTYREF->GetSafeHwnd())
    {
		DOSEPROPERTYREF->PostMessage(WM_TIMER_REFRESH);
    }
	if (LINETABREF)
	{
		LINETABREF->PostMessage(WM_TIMER_REFRESH);
	}
	Invalidate(FALSE);
	UpdateWindow();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::CloseAllOpenDialogs()
{
	if (DOSEPROPERTYREF)
	{
		DOSEPROPERTYREF->DestroyWindow();
		ASSERT(!DOSEPROPERTYREF);
	}
	if (LINETABREF)
	{
		LINETABREF->DestroyWindow();
		ASSERT(!LINETABREF);
	}
	m_DoseItem = -1;
	m_ButtonAliveCounter = 0;
	m_aInfoBox.ShowWindow(SW_HIDE);
}

//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnDoseEnd(WPARAM , LPARAM)
{
	CloseAllOpenDialogs();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnLayoutChanged(WPARAM , LPARAM)
{
	m_zoomfactor = 100;
	BOOL bPaintUpdate = CheckLayout ();
	Invalidate(bPaintUpdate);
	UpdateWindow();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnInfoBoxAlarmClear(WPARAM id, LPARAM)
{
	AlarmClear(_S32(id));
	SetNewInfoMessage();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnInfoBoxFlashlight(WPARAM, LPARAM)
{
	OnAlarmBits();
	if (m_bAlarm1)
	{
		REMOTEREF.setLinePBAlarm1Quit();
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnDeactivateView(WPARAM, LPARAM )
{
	CloseAllOpenDialogs();
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CEasyControlView::OnActivateView(WPARAM, LPARAM)
{
	if (m_aInfoBox.IsErrorItem())
	{
		m_aInfoBox.ShowWindow(SW_SHOWNORMAL);
	}
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CEasyControlView::OnLineStatus()
{
	if ( ! LINETABREF)
	{
		const auto& aV = dataimage::getLineDataImage();
		const auto& cache = aV.m_Cache;
		BOOL bEnable = cache.m_ProcessStatus.flags.ModuleExternalSetpoint ||
			cache.m_ProcessStatus.flags.ModuleSetpointRamp ||
			cache.m_ProcessStatus.flags.ModuleLineSetpoint ||
			cache.m_ProcessStatus.flags.ModuleLocalSetpoint ||
			cache.m_ProcessStatus.flags.ModuleBatcher ||
			cache.m_ProcessStatus.flags.ModuleTotalizer ||
			cache.m_ProcessStatus.flags.ModuleTotalizerPulse;
		if (bEnable)
		{
			LINETABREF = new CLineTabCtrl;
			LINETABREF->Create(this, m_LineBoxPos);
		}
	}
	else
	{
		LINETABREF->DestroyWindow();
	}
	PostMessage(WM_TIMER_REFRESH);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CEasyControlView::OnUpdateLineStatus(CCmdUI* pCmdUI)
{
	//BOOL result = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck(LINETABREF != nullptr);
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnAlarmBits()
{
	const auto& vAlarm = dataimage::getLineDataImage().m_Cache.m_Alarmstatus;
	m_bAlarm1 = vAlarm.flags.alarm1;
	m_bAlarm03 = vAlarm.flags.alarm0 | vAlarm.flags.alarm1 | vAlarm.flags.alarm2 | vAlarm.flags.alarm3;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnAlarm03Clear()
{
	OnAlarmBits();
	if (m_bAlarm03)
	{
		REMOTEREF.setLinePBAlarm03Quit();
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnUpdateAlarm03Clear(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bAlarm03);
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnAlarm1Clear()
{
	OnAlarmBits();
	if (m_bAlarm1)
	{
		REMOTEREF.setLinePBAlarm1Quit();
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnUpdateAlarm1Clear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bAlarm1);
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnDosiererNext()
{
	int32_t i = m_DoseItem + 1;

	for ( ; i < m_sMaxItems; i++)
	{
		base::eDoseType ltyp;
		REMOTEREF.getDoseType(i, ltyp);
		if ( ! base::__ISNONETYPE(ltyp))
		{
			break;
		}
	}
	if ( i < m_sMaxItems)
	{
		SetNewDoseItem(i);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnUpdateDosiererNext(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_DoseItem >= 0) && (m_DoseItem < m_sMaxItems-1));
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnDosiererPrevious()
{
	int32_t i = m_DoseItem - 1;

	for ( ; i >= 0; i--)
	{
		base::eDoseType ltyp;
		REMOTEREF.getDoseType(i, ltyp);
		if ( ! base::__ISNONETYPE(ltyp))
		{
			break;
		}
	}
	if ( i >= 0 )
	{
		SetNewDoseItem(i);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnUpdateDosiererPrevious(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((m_DoseItem > 0) && (m_DoseItem < m_sMaxItems));
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnServiceMode()
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;
	const BOOL bServiceMode = cache.m_ProcessStatus.flags.servicemode;

	if (! bServiceMode)
	{
		if (ECMessageBox(IDS_WARN_SERVICEMODE, MB_ICONSTOP | MB_YESNO) == IDYES)
		{
			REMOTEREF.setLinePBServiceMode(TRUE);
		}
	}
	else
	{
		LOGERROR("Servicemode already set!")
		REMOTEREF.setLinePBServiceMode(FALSE);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnUpdateOnServiceMode(CCmdUI* pCmdUI)
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;
	const BOOL bServiceMode		= cache.m_ProcessStatus.flags.servicemode;
	const BOOL bAnyDoseRunning	= cache.m_ProcessStatus.flags.anydosestarted;

	BOOL bEnable  = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_SERVICEMODE);
		 bEnable &= (bServiceMode) || (!bAnyDoseRunning);

	pCmdUI->SetCheck(bEnable && bServiceMode);
	pCmdUI->Enable(bEnable);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CEasyControlView::SetZoomFactor(int32_t zoomfactor)
{
	if (m_sMaxItems > 4)
	{
		zoomfactor = 100;
	}
	zoomfactor = zoomfactor / 10;
	zoomfactor *= 10;
	BOOL bChanged = zoomfactor != m_zoomfactor;
	m_zoomfactor = zoomfactor;
	return bChanged;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEasyControlView::CheckLayout(void)
{
	BOOL bNew = FALSE;
	uint32_t count = 0;

	REMOTEREF.getMaxDoseCount(count);
	ASSERT(count >= 0  && count <= GLOBALDOSEMAXCOUNT);
	if ( _S32(count) != m_sMaxItems)
	{
		m_sMaxItems = _S32(count);
	    LoadLayout (m_sMaxItems);
		bNew = TRUE;
	}
	bNew |= CheckFeederDisplay ();
	if (bNew)
	{
		CreateMessageBox();
	}
	auto& aL = dataimage::getLineDataImage();
	aL.m_LayoutChanged = FALSE;
	return bNew;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::DrawItems(CDC* pDC)
{
	if (! GetSafeHwnd() ||
		(dataimage::getLineDataImage().m_LayoutChanged))
	{
		return;
	}

	OnAlarmBits();

	auto SetGrafik = [pDC](const int32_t item, const CDisplayContainer::eDisplayItem index)
	{
		auto& aV = dataimage::getDoseDataImage(item);
		auto& rI = aV.m_Container.GetDisplayItem(index);
		auto aA  = rI.GetGrafikContext();
		if (aA)
		{
			auto& aCache = aV.m_Cache;
			ASSERT(aA->GetDoseType() == aCache.m_DoseType);
			const auto& linestatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
			aA->SetData(aCache, linestatus);
			aA->ShowGrafik(pDC, rI.GetDisplayRect());
		}
	};

	auto SetProductName = [pDC](const int32_t item, const BOOL bVisible, const CDisplayContainer::eDisplayItem index)
	{
		auto& aV = dataimage::getDoseDataImage(item);
		auto& rI = aV.m_Container.GetDisplayItem(index);
		auto aA = rI.GetGrafikContext();
		if (aA)
		{
			std::string szSaveName;
			auto& aCache = aV.m_Cache;
			ASSERT(aA->GetDoseType() == aCache.m_DoseType);
			if ( ! bVisible)
			{
				szSaveName = aCache.m_szName;
				aCache.m_szName = "";
			}
			const auto& linestatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
			aA->SetData(aCache, linestatus);
			aA->ShowGrafik(pDC, rI.GetDisplayRect());
			if ( ! bVisible)
			{
				aCache.m_szName = szSaveName;
			}
		}
	};

	for (int32_t i = 0; i < m_sMaxItems; i++)
	{
		SetGrafik(i, CDisplayContainer::eDisplayItem::eFeederDisplay);
		SetGrafik(i, CDisplayContainer::eDisplayItem::eDoseBaseNominalGrafik);
		SetGrafik(i, CDisplayContainer::eDisplayItem::eDoseBaseActualGrafik);
		SetGrafik(i, CDisplayContainer::eDisplayItem::eDoseButtonGrafik);
#ifdef _DEBUG
		{
			auto& aV = dataimage::getDoseDataImage(i);
			auto& rI = aV.m_Container.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseProductNameGrafik);
			auto aA = rI.GetGrafikContext();
			if (m_bProductNameEnable)
			{
				ASSERT((m_bProductNameEnable)?aA:!aA);
			}
		}
#endif
		if (m_bProductNameEnable) // Ist eigentlich nicht notwendig, weil der GrafikContext == nullptr ist -> s. Layout
		{
			SetProductName(i, (!m_ButtonAliveCounter), CDisplayContainer::eDisplayItem::eDoseProductNameGrafik);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::DrawRefillButtons(CDC* pDC)
{
	if (m_bDrawRefillButtonsEnable && m_ButtonAliveCounter)
	{
		auto SetGrafik = [pDC](const int32_t item, const CDisplayContainer::eDisplayItem index, const int32_t offset)
		{
			auto& aV = dataimage::getDoseDataImage(item);
			auto& aCache = aV.m_Cache;
			auto& rI	 = aV.m_Container.GetDisplayItem(index);
			auto aA		 = rI.GetGrafikContext();
			if (aA)
			{
				ASSERT(aA->GetDoseType() == aCache.m_DoseType);
				const auto& linestatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
				aA->SetData(aCache, linestatus);
				if (aA->IsVisible())
				{
					CRect aRect{ rI.GetSaveDisplayRect() };
					const BOOL bRefillRelease = aCache.m_ProcessStatus.flags.refillRelease;
					if (bRefillRelease)
					{
						aRect.OffsetRect(offset, 0);
					}
					rI.SetDisplayRect(aRect);
					aA->ShowGrafik(pDC, rI.GetDisplayRect());
				}
			}
		};

		for (int32_t i = 0; i < m_sMaxItems; i++)
		{
			SetGrafik(i, CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik, -30);
			SetGrafik(i, CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik, 30);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::DrawBarChart(CDC* pDC)
{
	if (m_bBarChartEnable)
	{
		const auto& aV = dataimage::getLineDataImage();
		const auto& cache = aV.m_Cache;
		const BOOL bServiceMode = cache.m_ProcessStatus.flags.servicemode;
		if ( ! bServiceMode)
		{
			m_BarChart.Rescale(cache.m_fMaxLeistung);
			m_BarChart.Update(pDC, cache.m_fLineSetpoint, cache.m_fLineMassflow);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::DrawLineTotalizerBox(CDC* pDC)
{
	if (m_bLineTotalizerBoxEnable)
	{
		const auto& aV = dataimage::getLineDataImage();
		const auto& cache = aV.m_Cache;
		const BOOL bServiceMode = cache.m_ProcessStatus.flags.servicemode;
		if ( ! bServiceMode )
		{
			m_LineTotalizerBox.Update(pDC, cache.m_fTotalizer[_S32(base::eTotalizerType::TOT_ALL)], cache.m_ulProductionTime);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::DrawServiceMode(CDC* pDC)
{
	static uint32_t toggle = 0;
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;
	const BOOL bServiceMode = cache.m_ProcessStatus.flags.servicemode;
	if ( bServiceMode )
	{
		toggle ^= 1U;
		if (toggle)
		{
			auto OldFont = pDC->SelectObject(&m_ServiceModeFont);
			auto oldcolor = pDC->SetTextColor(COLORRED);
			pDC->TextOut(m_ServiceBoxPos.x, m_ServiceBoxPos.y, m_szServiceMode);
			pDC->SelectObject(OldFont);
			pDC->SetTextColor(oldcolor);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnDraw(CDC* pDC)
{
	if ( ! GetSafeHwnd() )
		return;
	
	auto& dc = m_LinieDC.GetDC();
	if (dc.GetSafeHdc())
	{
		CRect aRect;
		GetClientRect(aRect);
		DrawItems(&dc);
		DrawBarChart(&dc);
		DrawLineTotalizerBox(&dc);
		pDC->BitBlt(0, 0, aRect.Width(), aRect.Height(), &dc, 0, 0, SRCCOPY);
		DrawServiceMode(pDC);
		DrawRefillButtons(pDC);
	}
}
//***********************************************************************************************
//***********************************************************************************************
#ifdef _DEBUG
void CEasyControlView::AssertValid() const
{
	CView::AssertValid();
}

void CEasyControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEasyControlDoc* CEasyControlView::GetDocument() const // Nicht-Debugversion ist inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyControlDoc)));
	return (CEasyControlDoc*)m_pDocument;
}
#endif //_DEBUG
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::CreateMessageBox()
{
	if (m_aInfoBox.GetSafeHwnd() == NULL)
	{
		m_aInfoBox.Create(this, m_InfoBoxPos);
	}
	else
	{
		m_aInfoBox.Move(m_InfoBoxPos);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnInitialUpdate()
{
	if (m_bBarChartEnable || m_bLineTotalizerBoxEnable)
	{
		CClientDC dc(this);
		if (m_bBarChartEnable)
		{
			m_BarChart.Create(&dc, m_BarChartRect);
		}
		if (m_bLineTotalizerBoxEnable)
		{
			m_LineTotalizerBox.Create(&dc, m_LineTotalizerBoxPos);
		}
	}

	Invalidate(FALSE);
	UpdateWindow();
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView :: FlashLightOff (void)
{
	OnAlarm1Clear();
}

//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::StartDose(const int32_t index)
{
	auto& rItem = GetDisplayItem(index, CDisplayContainer::eDisplayItem::eDoseButtonGrafik);
	ASSERT(rItem.GetGrafikContext());
	auto aB = dynamic_cast<CDoseButtonGrafik* >(rItem.GetGrafikContext());
	if ( aB && aB->IsVisible() )
	{
		{
			CClientDC clientDC(this);
			aB->FlashButton(&clientDC);
		}
		auto result = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSE_START);
		if (!result)
		{
			ECMessageBox(IDS_ERROR_DOSESTART, MB_ICONEXCLAMATION | MB_OK);
		}
		else
		{
			if (aB->IsButtonRun() || aB->IsButtonStart())
			{
				REMOTEREF.setDosePBStop(index);
			}
			else
			{
				if (aB->IsButtonReady())
				{
					const auto& aV = dataimage::getDoseDataImage(index);
					const auto& cache = aV.m_Cache;
					const auto& status = cache.m_ProcessStatus;
					if ( base::__ISBATCHERTYPE(cache.m_DoseType) && (status.flags.batchStarted == true) )
					{
						if (ECMessageBox(IDS_BATCHSTARTED, MB_YESNO | MB_ICONQUESTION) == IDNO)
						{
							REMOTEREF.setDosePBClearTotalizer(index);
							ECMessageBox(IDS_BATCHNEWSTARTED, MB_OK | MB_ICONEXCLAMATION);
						}
					}
					REMOTEREF.setDosePBStart(index);
				}
			}
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::StartDoseRefillRelease(const int32_t index)
{
	auto& rItem = GetDisplayItem(index, CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik);
	auto aB = dynamic_cast<CDoseButtonGrafikContext*>(rItem.GetGrafikContext());
	if (aB && aB->IsVisible())
	{
		{
			CClientDC clientDC(this);
			aB->FlashButton(&clientDC);
		}
		auto& aV = dataimage::getDoseDataImage(index);
		BOOL value = !aV.m_Cache.m_ProcessStatus.flags.refillRelease;
		REMOTEREF.setDoseRefillRelease(index, value);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::StartDoseRefill(const int32_t index)
{
	auto& rItem = GetDisplayItem(index, CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik);
	auto aB = dynamic_cast<CDoseButtonGrafikContext*>(rItem.GetGrafikContext());
	if (aB && aB->IsVisible())
	{
		{
			CClientDC clientDC(this);
			aB->FlashButton(&clientDC);
		}
		auto& aV = dataimage::getDoseDataImage(index);
		BOOL refillactive = aV.m_Cache.m_ProcessStatus.flags.refillActive;
		if (refillactive)
		{
			REMOTEREF.setDosePBAlarmClear(index);
			REMOTEREF.setDosePBRefillStop(index);
		}
		else
		{
			REMOTEREF.setDosePBRefillStart(index);
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView :: AlarmClear (int32_t id)
{
	ASSERT(id >= 0);
    if (id >= 0)
    {
		auto& logitem = LOGREF.GetItem(id);
		if (!logitem.GetChecked())
		{
			FlashLightOff();
			//OnAlarm03Clear();
			auto item = logitem.GetItem();
			ASSERT(item >= 0);
			if (item >= 0)
			{
				REMOTEREF.setDosePBAlarmClear(item);
			}
		}
    }
}
//***********************************************************************************************
//***********************************************************************************************
int32_t  CEasyControlView::IsValidClick(const CPoint& point, CDisplayContainer::eDisplayItem& hit)
{
	int32_t iPoint = -1;
	for (int32_t index = 0; index < m_sMaxItems; index++)
	{
		const auto& aC = dataimage::getDoseDataImage(index).m_Container;
		if (aC.SearchPoint(point, hit))
		{
			iPoint = index;
			break;
		}
	}
	return iPoint;
}
//***********************************************************************************************
//***********************************************************************************************
inline CDoseDisplayItem& CEasyControlView::GetDisplayItem(const int32_t index, const CDisplayContainer::eDisplayItem eItem)
{
	auto& aV = dataimage::getDoseDataImage(index);
	return aV.m_Container.GetDisplayItem(eItem);
}
//***********************************************************************************************
//***********************************************************************************************
int32_t  CEasyControlView::CheckClick (const CPoint& point)
{
	CDisplayContainer::eDisplayItem hit = CDisplayContainer::eDisplayItem::eDoseDisplayMax;
	auto index = IsValidClick(point, hit);
	if (index >= 0)
	{
		switch (hit)
		{
		case CDisplayContainer::eDisplayItem::eDoseButtonGrafik:
		{
			StartDose(index);
		}
		break;

		case CDisplayContainer::eDisplayItem::eFeederDisplay:
		{
			int32_t iDosierer = -1;
			base::eDoseType lTyp;
			REMOTEREF.getDoseType(index, lTyp);
			if ( ! base::__ISNONETYPE(lTyp))
			{
				iDosierer = index;
			}
			SelectDosierer(iDosierer);
		}
		break;

		case CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik:
		{
			ASSERT(m_bDrawRefillButtonsEnable);
			if ( ! m_ButtonAliveCounter)
			{
				index = -1;
			}
			else
			{
				StartDoseRefillRelease(index);
				m_ButtonAliveCounter = MAXBUTTONALIVECOUNTER;
			}
		}
		break;

		case CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik:
		{
			ASSERT(m_bDrawRefillButtonsEnable);
			if ( ! m_ButtonAliveCounter)
			{
				index = -1;
			}
			else
			{
				StartDoseRefill(index);
				m_ButtonAliveCounter = MAXBUTTONALIVECOUNTER;
			}
		}
		break;

		default:
			break;
		}
	}
	return index;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int32_t index = CheckClick(point);
	if (index < 0)
	{
		ClickedButtonAlive();
		SelectDosierer(-1);
		OnLineStatus();
	}
	CView::OnLButtonDblClk(nFlags, point);
}
#ifdef __SIM_WEIGHT_GUI
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnRButtonDown(UINT , CPoint )
{
	if (!CWeightDemoDlg::g_WeightDemoDlg)
	{
		CWeightDemoDlg::CreateBox(this);
	}
}
#endif
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnLButtonDown(UINT nFlags, CPoint point)
{
	int32_t index = CheckClick (point);
	if (index < 0)
	{
		ClickedButtonAlive();
		SelectDosierer(-1);
	}
	CView::OnLButtonDown(nFlags, point);
}
//***********************************************************************************************
//***********************************************************************************************
inline void CEasyControlView::ClickedButtonAlive()
{
	BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSE_ACTIVATE);
	m_ButtonAliveCounter = ((!bLogin) || m_ButtonAliveCounter || (!m_bDrawRefillButtonsEnable)) ? 0 : MAXBUTTONALIVECOUNTER;
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::SelectDosierer (int32_t iDosierer)
{
	BOOL bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSE_ACTIVATE);
	if (!bLogin)
	{
		iDosierer = -1;
	}
	if (m_DoseItem != iDosierer)
	{
		SetNewDoseItem(iDosierer);
		if ( iDosierer >= 0)
		{
			if ( ! DOSEPROPERTYREF)
			{
				ASSERT(m_DoseItem >= 0);
				DOSEPROPERTYREF = new CDosePropertySheet(this, _S32(m_DoseItem));
				DOSEPROPERTYREF->Create(this);
			}
		}
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEasyControlView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		LOGDEBUG("Activate CEasyControlView !");
		m_BarChart.ForceRepaint();
		AfxGetMainWnd()->SendMessage(WM_ACTIVATE_VIEW, WPARAM(eViewSelect::eDefaultView), 0);
	}
	else
	{
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEasyControlView::OnEraseBkgnd(CDC* pDC)
{
	CRect aRect;
	auto rSize = m_LinieDC.GetBitmapDimensions();
	GetClientRect(&aRect);
	aRect.right--;
	aRect.bottom--;
	if (rSize.cx < aRect.right)
	{
		CRect right{ rSize.cx, 0, aRect.right, aRect.bottom };
		pDC->FillSolidRect(right, WHITECOLOR);
	}
	if (rSize.cy > 0 && rSize.cy < aRect.bottom)
	{
		CRect bottom{ 0, rSize.cy, aRect.right, aRect.bottom };
		pDC->FillSolidRect(bottom, WHITECOLOR);
	}
	return TRUE;
}
