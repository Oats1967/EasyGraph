//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTabCtrl
///
/// @file   LineTabCtrl.cpp
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
#include "LineTabCtrl.h"
#include "LineTabOverview.h"
#include "LineTabTotalizer.h"
#include "LineTabExtern.h"
#include "DataImage.h"
#include "MFCMacros.h"


CLineTabCtrl* CLineTabCtrl::g_pLineTabCtrl = NULL;

IMPLEMENT_DYNAMIC(CLineTabCtrl, CBCGPTabCtrl)
CLineTabCtrl::CLineTabCtrl() : CBCGPTabCtrl()
    , m_LineTabOverview()
    , m_LineTabTotalizer()
    , m_LineTabExtern()
    , m_tabCurrent(0)
{
    m_ImageList.Create(30, 30, ILC_COLOR24, 0, 1);
}
//************************************************************************************************
//************************************************************************************************
BEGIN_MESSAGE_MAP(CLineTabCtrl, CBCGPTabCtrl)
    ON_WM_LBUTTONDOWN()
    ON_MESSAGE(WM_TIMER_REFRESH, &CLineTabCtrl::OnTimerRefresh)
END_MESSAGE_MAP()
//************************************************************************************************
//************************************************************************************************
CLineTabCtrl::~CLineTabCtrl()
{
    g_pLineTabCtrl = nullptr;
}
//************************************************************************************************************************
//************************************************************************************************************************
void CLineTabCtrl::ShowDialog()
{
    for (int32_t i = 0; i < m_tabPages.size(); i++)
    {
        if ( i == m_tabCurrent)
        {
            m_tabPages[i]->ShowWindow(SW_SHOW);
            m_tabPages[i]->SetFocus();
        }
        else
        {
            m_tabPages[i]->ShowWindow(SW_HIDE);
        }
    }
}
//************************************************************************************************************************
//************************************************************************************************************************
void CLineTabCtrl::Create(CWnd* pParent, const CPoint& rectPoint)
{
    auto loadImage = [this](int32_t& imageresid)
    {
        static const int32_t imageres[4] = { IDB_LINETAB_1 , IDB_LINETAB_2, IDB_LINETAB_3, IDB_LINETAB_4 };

        ASSERT(imageresid >= 0 && imageresid < 4);
        CBitmap aBitmap;
        VERIFY(aBitmap.LoadBitmap(imageres[imageresid]));
        m_ImageList.Add(&aBitmap, RGB(255, 0, 255));
        imageresid++;
    };

    CRect aRect; //  { 0, 0, 320, 220 };
    CBCGPTabCtrl::Create(WS_CHILD | WS_VISIBLE, aRect, pParent, 1);

    int32_t imageresid = 0;
    {
        m_LineTabOverview.Create(CLineTabOverview::IDD, this);
        m_tabPages.push_back(&m_LineTabOverview);
        loadImage(imageresid);
    }
    const auto& rL = dataimage::getLineDataImage();
    const auto& cache = rL.m_Cache;
    if (cache.m_ProcessStatus.flags.ModuleTotalizer)
    {
        m_LineTabTotalizer.Create(CLineTabTotalizer::IDD, this);
        m_tabPages.push_back(&m_LineTabTotalizer);
        loadImage(imageresid);
    }
    {
        m_LineTabMassflow.Create(CLineTabMassflow::IDD, this);
        m_tabPages.push_back(&m_LineTabMassflow);
        loadImage(imageresid);
    }
    {
        m_LineTabExtern.Create(CLineTabExtern::IDD, this);
        m_tabPages.push_back(&m_LineTabExtern);
        loadImage(imageresid);
    }

    m_LineTabOverview.GetClientRect(aRect);
    aRect.bottom += 50;
    SetWindowPos(NULL, rectPoint.x, rectPoint.y - 100, aRect.Width(), aRect.Height(), SWP_NOZORDER);


    SetImageList(&m_ImageList);
    for (int32_t i = 0; i < m_tabPages.size(); i++)
    {
        TCITEM aItem;
        aItem.mask = TCIF_IMAGE;
        aItem.iImage = i;
        InsertItem(i, &aItem);
    }
    SetRectangle();
    ShowWindow(SW_SHOW);
    ShowDialog();
}
//************************************************************************************************************************
//************************************************************************************************************************
void CLineTabCtrl::SetRectangle()
{
    CRect tabRect, itemRect;
    int32_t nX, nY, nXc, nYc;

    GetClientRect(&tabRect);
    GetItemRect(0, itemRect);

    nX = itemRect.left;
    nY = itemRect.bottom + 1;
    nXc = tabRect.right - itemRect.left - 1;
    nYc = tabRect.bottom - nY - 1;

    m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    for (int32_t index = 1; index < _S32(m_tabPages.size()); index++) 
    {
        m_tabPages[index]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CLineTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    CBCGPTabCtrl::OnLButtonDown(nFlags, point);

    if ((m_tabCurrent >= 0) && (m_tabCurrent < m_tabPages.size()) && (m_tabCurrent != GetCurFocus()))
    {
        m_tabCurrent = GetCurFocus();
        ShowDialog();
    }
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CLineTabCtrl::PostNcDestroy()
{
    delete this;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineTabCtrl::OnTimerRefresh(WPARAM, LPARAM)
{
    ASSERT(m_tabCurrent >= 0 && m_tabCurrent < m_tabPages.size());
    ASSERT(m_tabPages[m_tabCurrent]);
    m_tabPages[m_tabCurrent]->SendMessage(WM_TIMER_REFRESH);
    return 0;
}
