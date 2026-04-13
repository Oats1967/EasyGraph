//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayIfsu.h
///
/// @file   FeederDisplayIfsu.h
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
// CFeederDisplayIfsu command target

class CFeederDisplayIfsu : public CFeederDisplayIfs
{
    using CBaseClass = CFeederDisplayIfs;

private:
    CFeederDisplayIfsu() = delete;

protected:
    void DrawBitmap(CDC*) override;

public:
    CFeederDisplayIfsu(int32_t sID, const CRect& rRect);
    CFeederDisplayIfsu(const CFeederDisplayIfsu&) = delete;
    CFeederDisplayIfsu& operator = (const CFeederDisplayIfsu&) = delete;
    ~CFeederDisplayIfsu() override = default;
};



