//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabCtrl
///
/// @file   LineTabCtrl.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <vector>
#include "EasyControl.h"
#include "LineTabOverview.h"
#include "LineTabTotalizer.h"
#include "LineTabMassflow.h"
#include "LineTabExtern.h"



class CLineTabCtrl : public CBCGPTabCtrl
{
    DECLARE_DYNAMIC(CLineTabCtrl)

    enum eTabs
    {
        eTabOverview,
        eTabTotalizer,
        eTabExtern,
        eTabMax
    };

public:

    static CLineTabCtrl* g_pLineTabCtrl;

    CLineTabOverview    m_LineTabOverview;
    CLineTabMassflow    m_LineTabMassflow;
    CLineTabTotalizer   m_LineTabTotalizer;
    CLineTabExtern      m_LineTabExtern;
    CImageList          m_ImageList;


    std::vector < CDialog*> m_tabPages;
    int32_t m_tabCurrent;

private:
    void ShowDialog();
    void SetRectangle();

protected:
    void PostNcDestroy() override;

public:
    CLineTabCtrl();
    ~CLineTabCtrl() override;

public:
    void Create(CWnd* pParent, const CPoint& rectTab);


    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    LRESULT OnTimerRefresh(WPARAM, LPARAM);

    DECLARE_MESSAGE_MAP()
};


#define LINETABREF (CLineTabCtrl::g_pLineTabCtrl)

