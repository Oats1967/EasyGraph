//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsPropertySheet.cpp
///
/// @file   SettingsPropertySheet.cpp
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
#include "SettingsPropertySheet.h"
#include "MFCMacros.h"


// CSettingsPropertySheet
#define IMAGEMAX 6

#define HEADER_HEIGHT	globalUtils.ScaleByDPI(70, this)
#define HEADER_TEXT_COLOR	RGB(0, 11, 159)

IMPLEMENT_DYNAMIC(CSettingsPropertySheet, CBCGPPropertySheet)
//****************************************************************************************************************
//****************************************************************************************************************


CSettingsPropertySheet::CSettingsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBCGPPropertySheet(nIDCaption, pParentWnd, iSelectPage)
	, c_PageTitle{ IDS_SETTINGS_ALARMCONFIG, IDS_SETTINGS_LAYOUT, IDS_SETTINGS_PATH, IDS_SETTINGS_USER, IDS_SETTINGS_RECIPE, IDS_SETTINGS_REMOTE, IDS_SETTINGS_PRODUCTLIST }
	, m_pPage(NULL)
	, c_uiIconsResID { IDB_ICONS_LARGE_HC }
	, c_cxIcon{ 32 }
{
    Init();
}

//****************************************************************************************************************
//****************************************************************************************************************

CSettingsPropertySheet::CSettingsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CBCGPPropertySheet(pszCaption, pParentWnd, iSelectPage)
	, c_PageTitle{ IDS_SETTINGS_ALARMCONFIG, IDS_SETTINGS_LAYOUT, IDS_SETTINGS_PATH, IDS_SETTINGS_USER, IDS_SETTINGS_RECIPE, IDS_SETTINGS_REMOTE, IDS_SETTINGS_PRODUCTLIST } 
	, m_pPage(NULL)
	, c_uiIconsResID{ IDB_ICONS_LARGE_HC }
	, c_cxIcon{ 32 }
{
    Init();
}

//****************************************************************************************************************
//****************************************************************************************************************


BEGIN_MESSAGE_MAP(CSettingsPropertySheet, CBCGPPropertySheet)
ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, OnPageChanged)
ON_MESSAGE(WM_DPICHANGED, OnDPIChanged)
END_MESSAGE_MAP()



//****************************************************************************************************************
//****************************************************************************************************************
void CSettingsPropertySheet::LoadHeaderImages()
{
	for (int32_t i = 0; i < c_NuberPages; i++)
	{
		m_imageHeader[i].Clear();
		m_imageHeader[i].Load(IDB_SETTINGSHEADER1);
		m_imageHeader[i].SetSingleImage();

		globalUtils.ScaleByDPI(m_imageHeader[i], &CWnd::wndTop);
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
void CSettingsPropertySheet::LoadPageTitle()
{
	for (int32_t i = 0; i < c_NuberPages; i++)
	{
		VERIFY(m_PageTitle[i].LoadString(c_PageTitle[i]));
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
LRESULT CSettingsPropertySheet::OnDPIChanged(WPARAM wp, LPARAM lp)
{
	CBCGPWindowDpiState state(this);

	SetIconsList(c_uiIconsResID, c_cxIcon, (COLORREF)-1, TRUE /* Auto-scale */);
	LoadHeaderImages();
	return CBCGPPropertySheet::OnDPIChanged(wp, lp);
}
//****************************************************************************************************************
//****************************************************************************************************************
void CSettingsPropertySheet::OnDrawPageHeader(CDC* pDCPaint, int nPage, CRect rectHeader)
{
	CBCGPMemDC* pMemDC = NULL;
	CDC* pDC = NULL;

	if (IsGlassHeader())
	{
		pMemDC = new CBCGPMemDC(*pDCPaint, rectHeader);
		pDC = &pMemDC->GetDC();
		pDC->FillSolidRect(rectHeader, RGB(0, 0, 0));
	}
	else
	{
		pDC = pDCPaint;
	}

	CBCGPDrawManager dm(*pDC);

	m_imageHeader[nPage].DrawEx(pDC, rectHeader, 0);

	rectHeader.left += GetAeroBackButtonWidth();
	CRect rectText = rectHeader;
	rectText.left += globalUtils.ScaleByDPI(16, this);
	rectText.top += globalUtils.ScaleByDPI(10, this);

	// Draw caption:
	{
#ifdef _BCGSUITE_INC_
		CBCGPFontSelector fs(*pDC, &globalData.fontBold, HEADER_TEXT_COLOR, TRUE);
#else
		CBCGPFontSelector fs(*pDC, &globalUtils.GetFontCaption(this), HEADER_TEXT_COLOR, TRUE);
#endif

		CString strCaption{ m_PageTitle[nPage] };

		if (IsGlassHeader())
		{
			CBCGPVisualManager::GetInstance()->DrawTextOnGlass(pDC, strCaption, rectText, DT_SINGLELINE, 6, (COLORREF)-1);
		}
		else
		{
			pDC->DrawText(strCaption, rectText, DT_SINGLELINE);
		}

		rectText.OffsetRect(0, pDC->GetTextExtent(strCaption).cy + globalUtils.ScaleByDPI(4, this));
	}
	if (pMemDC != NULL)
	{
		delete pMemDC;
	}
}
//****************************************************************************************************************
//****************************************************************************************************************
void CSettingsPropertySheet::Init()
{
	const BOOL bAeroGlass = TRUE;
	const BOOL bWizardsSimplifiedBackButton = FALSE;
	const BOOL m_bHeader = TRUE;
	const BOOL m_bTexturedBackground = FALSE;
	const BOOL bAeroHeader = TRUE;
	const BOOL m_bDragClientArea = FALSE;
	const BOOL m_bUseVisualManagerStyle = TRUE;
	const CBCGPPageTransitionManager::BCGPPageTransitionEffect m_nPageTransitionEffect{ CBCGPPageTransitionManager::BCGPPageTransitionEffect::BCGPPageTransitionSlide };
	const CBCGPAnimationManager::BCGPAnimationType m_nAnimationType{ CBCGPAnimationManager::BCGPAnimationType::BCGPANIMATION_SmoothStop };
	const BOOL m_bFirstPageTransition = FALSE;

	CBCGPWindowDpiState state(AfxGetMainWnd());

	const PropSheetLook look = CBCGPPropertySheet::PropSheetLook_OutlookBar;

	SetLook(look, globalUtils.ScaleByDPI(60, AfxGetMainWnd()), bAeroGlass, bWizardsSimplifiedBackButton);

	SetIconsList(c_uiIconsResID, c_cxIcon, (COLORREF)-1, TRUE /* Auto-scale */);

	
	AddPage(&m_aAlarmPage);
	AddPage(&m_aLayoutPage);
	AddPage(&m_aSystemPfad);
	AddPage(&m_aUserLoginPage);
	AddPage(&m_aRezeptPage);
	AddPage(&m_aRemotePage);
	AddPage(&m_aProductListPage);

	//Crash !! EnableScrollBar(SB_VERT, ESB_DISABLE_BOTH);
	EnablePageHeader(m_bHeader ? HEADER_HEIGHT : 0, bAeroHeader, bAeroHeader);
	EnableDragClientArea(m_bDragClientArea);
	EnablePageSwipeGesture(m_bPageSwipeGesture);
	EnableVisualManagerStyle(m_bUseVisualManagerStyle, TRUE);
	EnablePageTransitionEffect((CBCGPPageTransitionManager::BCGPPageTransitionEffect)m_nPageTransitionEffect,
		m_nPageTransitionTime, (CBCGPAnimationManager::BCGPAnimationType)m_nAnimationType, NULL, m_bFirstPageTransition);

	 LoadPageTitle();
	 LoadHeaderImages();
}

//****************************************************************************************************************
//****************************************************************************************************************
LRESULT CSettingsPropertySheet::OnPageChanged(WPARAM wParam, LPARAM)
{
	CBCGPPropertyPage *pPage = (CBCGPPropertyPage *) wParam;
	if ( pPage != m_pPage )
	{
		if ( (m_pPage != NULL) && (m_pPage->GetSafeHwnd()) )
		{
			m_pPage->SendMessage(WM_PROPERTYPAGE_CHANGED);
		}
		m_pPage = pPage;
	}
	return 0L;
}
//****************************************************************************************************************
//****************************************************************************************************************
void CSettingsPropertySheet::SortButtons (void)
{
    const int32_t ids []   = {IDOK, IDCANCEL, ID_APPLY_NOW};
    const int32_t sids []  = {IDS_OK, IDS_CANCEL, IDS_APPLY_NOW}; 
    CRect       rectBtn;
    CRect       aRect;
    const int32_t nSpacing = 6;        // space between two buttons...
    CString szTemp;

  // Hide Apply and Help buttons
	SHOWW_ID(IDHELP, FALSE);

    CWnd *pWnd = GetDlgItem (IDHELP);
    ASSERT(pWnd);
    pWnd->GetWindowRect (rectBtn);
    ScreenToClient (&rectBtn);
    rectBtn.left = rectBtn.right;
    rectBtn.right= rectBtn.left + nSpacing;
    for( int32_t i = _S32(__ARRAYSIZE(ids))-1; i >= 0; i--)
    {
        VERIFY(szTemp.LoadString(sids[i]));
        pWnd = GetDlgItem (ids [i]);
        ASSERT(pWnd);
        pWnd->SetWindowText(szTemp);
        pWnd->GetWindowRect (aRect);
        ScreenToClient (&aRect);
        int32_t btnWidth = aRect.Width();
        CRect aR(rectBtn.left-nSpacing-btnWidth, aRect.top, rectBtn.left-nSpacing, aRect.bottom);
        pWnd->MoveWindow(aR);
        rectBtn = aR;
    }
}
//****************************************************************************************************************
//****************************************************************************************************************
BOOL CSettingsPropertySheet::OnInitDialog()
{
	BOOL bResult = CBCGPPropertySheet::OnInitDialog();
	// bResult ist immer FALSE
	SortButtons();
	SetActivePage(0);
	return bResult;
}

