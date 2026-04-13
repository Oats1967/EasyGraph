//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ProductListCtrl
///
/// @file   ProductListCtrl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "EasyControl.h"



class CProductListCtrl : public CListCtrl
{
private:
    base::utils::CProductItemList&  m_rProductList;

protected:
    CImageList          m_ImageList;
    CWnd*               m_pParent;
    BOOL                m_bInvers;

private:
    void CreateImageList();

public:
    CProductListCtrl(CWnd* pParent, base::utils::CProductItemList& rProductList);
    ~CProductListCtrl() = default;

    void Fill(const int32_t selectitem = -1);
    void BuildHeadLine(BOOL bInvers = FALSE);
};


