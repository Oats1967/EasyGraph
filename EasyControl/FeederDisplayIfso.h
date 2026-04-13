//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayIfso
///
/// @file   FeederDisplayIfso.h
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

#include "FeederDisplayIfs.h"
/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayIfs command target

class CFeederDisplayIfso : public CFeederDisplayIfs
{
    using CBaseClass = CFeederDisplayIfs;

private:
    CFeederDisplayIfso() = delete;

protected:
    void DrawBitmap(CDC* pDC) override;

public:
    CFeederDisplayIfso(int32_t sID, const CRect& rRect);
    CFeederDisplayIfso(const CFeederDisplayIfs&) = delete;
    CFeederDisplayIfso& operator = (const CFeederDisplayIfso&) = delete;
    ~CFeederDisplayIfso() override = default;
};



