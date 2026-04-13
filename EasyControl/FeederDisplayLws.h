//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLws
///
/// @file   FeederDisplayLws.h
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
#include "HopperLws.h"

/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayLws command target

class CFeederDisplayLws : public CFeederDisplayLwf
{
    using CBaseClass = CFeederDisplayLwf;

private:
    CHopperLws   m_Hopper;
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
    {   return  cReleasePoint;   }

    const POINT& GetCalibratePoint() const override
    {  return  cCalibratePoint;   }

    const POINT& GetTarePoint() const override
    {   return  cTarePoint;   }

    const POINT& GetEmptyFeederPoint() const override
    {   return  cEmptyFeederPoint;    }

    const POINT& GetFillPoint() const override
    {   return  cFillPoint;  }

    const POINT& GetRefillPoint() const override
    {   return  cRefillPoint;    }

    const POINT& GetGravVolPoint() const override
    {   return  cGravVolPoint;   }

    const POINT& GetWeightPoint() const override
    {    return  cWeightPoint;   }

    const POINT& GetDriveCommandPoint() const override
    {    return  cDriveCommandPoint;  }

    const POINT& GetRefillStatePoint() const override
    {  return  cRefillStatePoint;   }


    IGrafikHopper& GetGrafikHopper() override
    {  return m_Hopper;   }

    void DrawBitmap(CDC* pDC) override;

private:
    CFeederDisplayLws() = delete;

public:
    CFeederDisplayLws(int32_t sID, const CRect& rRect);
    CFeederDisplayLws(const CFeederDisplayLws&) = delete;
    CFeederDisplayLws& operator = (const CFeederDisplayLws&) = delete;
    ~CFeederDisplayLws() override = default;
};



