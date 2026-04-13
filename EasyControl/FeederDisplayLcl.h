//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLcl.h
///
/// @file   FeederDisplayLcl.h
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
#include "HopperLcl.h"
/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayLcl command target

class CFeederDisplayLcl : public CFeederDisplayLwf
{
    using CBaseClass = CFeederDisplayLwf;

private:
    union DosierBaseStatus
    {
        struct
        {
            uint32_t bLclMinLevelChanged : 1;
            uint32_t bLclMaxLevelChanged : 1;
        } flags;
        uint32_t allflags = 0;
    };
    DosierBaseStatus	m_StatusFlags;

private:
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
    CHopperLcl   m_Hopper;

protected:
    const POINT& GetReleasePoint() const override
    {   return  cReleasePoint;   }

    const POINT& GetCalibratePoint() const override
    {    return  cCalibratePoint;   }

    const POINT& GetTarePoint() const override
    {    return  cTarePoint;    }

    const POINT& GetEmptyFeederPoint() const override
    {    return  cEmptyFeederPoint;    }

    const POINT& GetFillPoint() const override
    {    return  cFillPoint;   }

    const POINT& GetRefillPoint() const override
    {     return  cRefillPoint;   }

    const POINT& GetGravVolPoint() const override
    {    return  cGravVolPoint;   }

    const POINT& GetWeightPoint() const override
    {   return  cWeightPoint;    }

    const POINT& GetDriveCommandPoint() const override
    {    return  cDriveCommandPoint;   }

    const POINT& GetRefillStatePoint() const override
    {   return  cRefillStatePoint;    }


    void	DrawDriveCommand(CDC*) override {}
    void	DrawRefillState(CDC*) override {}
    void	DrawGravVol(CDC*) override {}
    void    DrawRelease(CDC* ) override {}
    void    DrawCalibration(CDC* ) override {}
    void    DrawEmptyFeeder(CDC* ) override {}
    void    DrawBitmap(CDC*) override;
    void    DrawFilling(CDC* pDC) override;

    IGrafikHopper& GetGrafikHopper() override
    {  return m_Hopper;    }

    BOOL IsTextUpdate() const override;
    void OnSetNewData(const base::CDoseDataCache& rNewData) override;

private:
    CFeederDisplayLcl() = delete;

public:
    CFeederDisplayLcl(int32_t sID, const CRect& rRect);
    CFeederDisplayLcl(const CFeederDisplayLcl&) = delete;
    CFeederDisplayLcl& operator = (const CFeederDisplayLcl&) = delete;
    ~CFeederDisplayLcl() override = default;
};



