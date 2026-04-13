//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLwv
///
/// @file   FeederDisplayLwv.h
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

#include "FeederDisplayLwf.h"
#include "HopperLwv.h"

class CFeederDisplayLwv : public CFeederDisplayLwf
{
    using CBaseClass = CFeederDisplayLwf;

private:
    CHopperLwv   m_Hopper;
    const POINT cCalibratePoint;
    const POINT cTarePoint;
    const POINT cEmptyFeederPoint;
    const POINT cReleasePoint;
    const POINT cFillPoint;
    const POINT cRefillPoint;
    const POINT cGravVolPoint;
    const POINT cWeightPoint;
    const POINT cDriveCommandPoint;
    const POINT cRefillStatePoint;


protected:
    const POINT& GetReleasePoint() const override
    {    return  cReleasePoint;   }

    const POINT& GetCalibratePoint() const override
    {    return  cCalibratePoint;   }

    const POINT& GetTarePoint() const override
    {   return  cTarePoint;    }

    const POINT& GetEmptyFeederPoint() const override
    {   return  cEmptyFeederPoint;    }

    const POINT& GetFillPoint() const override
    {    return  cFillPoint;    }

    const POINT& GetRefillPoint() const override
    {    return  cRefillPoint;   }

    const POINT& GetGravVolPoint() const override
    {    return  cGravVolPoint;   }

    const POINT& GetWeightPoint() const override
    {   return  cWeightPoint;   }

    const POINT& GetDriveCommandPoint() const override
    {    return  cDriveCommandPoint;   }

    const POINT& GetRefillStatePoint() const override
    {   return  cRefillStatePoint;   }

    void DrawBitmap(CDC* pDC) override;

    IGrafikHopper& GetGrafikHopper() override
    {   return m_Hopper;   }

private:
    CFeederDisplayLwv() = delete;

public:
    CFeederDisplayLwv(const int32_t sID, const CRect& rRect);

    CFeederDisplayLwv(const CFeederDisplayLwv&) = delete;
    CFeederDisplayLwv& operator = (const CFeederDisplayLwv&) = delete;
    ~CFeederDisplayLwv() override = default;
};





