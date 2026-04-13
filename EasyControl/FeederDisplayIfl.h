//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayIfl.h
///
/// @file   FeederDisplayIfl.h
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

#include "FeederDisplayLcl.h"
/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayIfl command target

class CFeederDisplayIfl : public CFeederDisplayLcl
{
    using CBaseClass = CFeederDisplayLcl;

private:
    CFeederDisplayIfl() = delete;

protected:
    void DrawBitmap(CDC* pDC) override;

public:
    CFeederDisplayIfl(int32_t sID, const CRect& rRect);
    CFeederDisplayIfl(const CFeederDisplayIfl&) = delete;
    CFeederDisplayIfl& operator = (const CFeederDisplayIfl&) = delete;
    ~CFeederDisplayIfl() override = default;
};



