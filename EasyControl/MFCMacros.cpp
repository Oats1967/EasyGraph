//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MFCMacros
///
/// @file   MFCMacros.cpp
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
#include "MFCMacros.h"

namespace mfcmacros
{
    inline void EnableID(CWnd* pWnd, const BOOL bEnable)
    {
        ASSERT(pWnd);
        pWnd->EnableWindow(bEnable);
    }
    //---------------------------------------------------------------------
    inline void ShowID(CWnd* pWnd, const BOOL bShow)
    {
        ASSERT(pWnd);
        pWnd->ShowWindow((bShow) ? SW_SHOW : SW_HIDE);
    }
    //---------------------------------------------------------------------
    void EnableID(CWnd* parent, int32_t id, const BOOL bEnable)
    {
        ASSERT(parent);
        EnableID((CWnd*)parent->GetDlgItem(id), bEnable);
    }
    //---------------------------------------------------------------------
    void ShowID(CWnd* parent, int32_t id, const BOOL bShow)
    {
        ASSERT(parent);
        ShowID((CWnd*)parent->GetDlgItem(id), bShow);
    }
    //---------------------------------------------------------------------
    void CheckID(CButton* pButton, const BOOL bChecked)
    {
        ASSERT(pButton);
        BOOL change = pButton->GetCheck() != bChecked;
        if (change)
        {
            pButton->SetCheck((bChecked) ? BST_CHECKED : BST_UNCHECKED);

        }
    }
    //---------------------------------------------------------------------
    void CheckID(CWnd* parent, int32_t id, const BOOL bChecked)
    {
        ASSERT(parent);
        auto pButton = (CButton*)parent->GetDlgItem(id);
        ASSERT(pButton);
        CheckID(pButton, bChecked);
    }
    //---------------------------------------------------------------------
    void EnableShowID(CWnd* parent, int32_t id, const BOOL bEnable, const BOOL bShow)
    {
        ASSERT(parent);
        auto pWnd = (CWnd*)parent->GetDlgItem(id);
        ASSERT(pWnd);
        EnableID(pWnd, bEnable);
        ShowID(pWnd, bShow);
    }
    //---------------------------------------------------------------------
    void EnableShowCheckID(CWnd* parent, int32_t id, const BOOL bEnable, const BOOL bShow, const BOOL bChecked)
    {
        ASSERT(parent);
        auto pWnd = (CWnd*)parent->GetDlgItem(id);
        ASSERT(pWnd);
        EnableID(pWnd, bEnable);
        ShowID(pWnd, bShow);
        CheckID((CButton*)pWnd, bChecked);
    }
    //---------------------------------------------------------------------
    void SunkenID(CWnd* parent, int32_t id, const BOOL bEnable)
    {
        ASSERT(parent);
        auto pWnd = (CButton*)parent->GetDlgItem(id);
        ASSERT(pWnd);
        if (bEnable)
        {
            pWnd->ModifyStyle(0, SS_ETCHEDFRAME);
        }
        else
        {
            pWnd->ModifyStyle(SS_ETCHEDFRAME, 0);
        }
        pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void SetPosID(CWnd* parent, int32_t id, const int32_t x, const int32_t y)
    {
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        pWnd->SetWindowPos(NULL, x, y, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void SetPosSizeID(CWnd* parent, int32_t id, const int32_t x, const int32_t y, const int32_t cx, const int32_t cy )
    {
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        pWnd->SetWindowPos(NULL, x, y, cx, cy, SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void CenterPosID(CWnd* parent, int32_t id, const int32_t x, const int32_t y)
    {
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        CRect aRect;
        pWnd->GetWindowRect(aRect);
        pWnd->SetWindowPos(NULL, x - aRect.Width()/2, y - aRect.Height()/2, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void CenterPosYID(CWnd* parent, int32_t id, const int32_t x, const int32_t y)
    {
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        CRect aRect;
        pWnd->GetWindowRect(aRect);
        pWnd->SetWindowPos(NULL, x, y - aRect.Height() / 2, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void CenterPosNeighbourYID(CWnd* parent, int32_t id1, int32_t id2, const int32_t x, const int32_t y)
    {
        CWnd* pWnd1 = parent->GetDlgItem(id1);
        ASSERT(pWnd1);
        CRect aRect1;
        pWnd1->GetWindowRect(aRect1);

        CWnd* pWnd2 = parent->GetDlgItem(id2);
        ASSERT(pWnd2);
        CRect aRect2;
        pWnd2->GetWindowRect(aRect2);

        CRect aRect{ aRect1.TopLeft(), CSize(aRect1.Width() + aRect2.Width(), aRect1.Height()) };
        int32_t l1 = - aRect.Width() / 2;
        int32_t t1 = - aRect.Height() / 2;
        pWnd1->SetWindowPos(NULL, x  + l1 , y + t1, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
        pWnd2->SetWindowPos(NULL, x + l1 + aRect1.Width(), y + t1, -1, -1, SWP_NOSIZE | SWP_NOZORDER);
    }
    //---------------------------------------------------------------------
    void CenterTextID(CWnd* parent, int32_t id)
    {
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        pWnd->ModifyStyle(0, SS_CENTER|SS_CENTERIMAGE|ES_CENTER);
    }
    //---------------------------------------------------------------------
    void SetFontID(CWnd* parent, int32_t id,  CFont* pFont)
    {
        ASSERT(pFont);
        CWnd* pWnd = parent->GetDlgItem(id);
        ASSERT(pWnd);
        pWnd->SetFont(pFont);
    }
    //---------------------------------------------------------------------
    void SetBitmapID(CWnd* parent, int32_t id, CBitmap* pBitmap)
    {
        ASSERT(pBitmap);
        auto pWnd = (CStatic*)parent->GetDlgItem(id);
        ASSERT(pWnd);
        pWnd->SetBitmap(*pBitmap);
    }

    void SendMessage(CWnd* parent, UINT m, WPARAM w, LPARAM l)
    {
        ASSERT(parent);
        if (parent && parent->GetSafeHwnd())
        {
            parent->SendMessage(m, w, l);
        }
    }


};



