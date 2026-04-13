//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayVol.h
///
/// @file   FeederDisplayVol.h
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

#include "FeederDisplay.h"
#include "HopperVol.h"

/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayVol command target

class CFeederDisplayVol : public CFeederDisplay
{
    using CBaseClass = CFeederDisplay;

    union DosierBaseStatus
    {
        struct
        {
            uint32_t bDriveCommandChanged : 1;
            uint32_t bRefillEnableChanged : 1;
            uint32_t bMinMaxSensorChanged : 1;
        } flags;
        uint32_t allflags = 0;
    };
    DosierBaseStatus	m_StatusFlags;

    const uint16_t BKX = 28U;
    const uint16_t BKY = 0;
    const uint16_t BKDX = 68U;
    const uint16_t BKDY = 100U;
    const RECT cBackgroundRect = { BKX, BKY, BKX + BKDX, BKY + BKDY };

protected:
    const POINT cCalibratePoint;
    const POINT cTarePoint;
    const POINT cEmptyFeederPoint;
    const POINT cReleasePoint;
    const POINT cRefillPoint;
    const POINT cDriveCommandPoint;
    const POINT cWeightAboveMaxPoint;
    const POINT cWeightBelowMinPoint;

    CHopperVol  m_Hopper;
    CBrush		m_brushBlueSolid;
    CBrush		m_brushRedSolid;
    CBrush		m_brushWhiteDashed;
    CPen		m_PenBlue;

protected:
    void OnUpdate(CDC* pDC) override;
    void OnRedraw(CDC* pDC) override;
    void DrawBitmap(CDC* pDC) override;

    BOOL IsTextUpdate(void) const override;
    BOOL IsBitmapUpdate() const override;
    void OnSetNewData(const base::CDoseDataCache& rNewData) override;

    const POINT& GetReleasePoint() const override
    {    return  cReleasePoint;    }
    const POINT& GetCalibratePoint() const override
    {   return  cCalibratePoint;   }
    const POINT& GetTarePoint() const override
    {    return  cTarePoint;   }
    const POINT& GetEmptyFeederPoint() const override
    {  return  cEmptyFeederPoint;   }
    const POINT& GetRefillPoint() const  override
    {  return  cRefillPoint;   }
    const RECT& GetBackgroundRect() const  override
    {   return cBackgroundRect;   }

    virtual const POINT& GetWeightBelowMinPoint() const
    {  return  cWeightBelowMinPoint;    }
    virtual const POINT& GetWeightAboveMaxPoint() const
    {  return  cWeightAboveMaxPoint;   }

    void DrawRefillState(CDC* pDC) override;

    virtual void DrawMinMaxSensor(CDC* pDC);
    virtual void DrawFilling(CDC*);

    virtual IGrafikHopper& GetGrafikHopper()
    {   return m_Hopper;    }

private:
    const POINT& GetDriveCommandPoint() const
    {  return cDriveCommandPoint;    }
    void DrawDriveCommand(CDC* pDC);


private:
    CFeederDisplayVol() = delete;

public:
    CFeederDisplayVol(const int32_t sID, const CRect& rRect);
    ~CFeederDisplayVol() override = default;
    CFeederDisplayVol(const CFeederDisplayVol&) = delete;
    CFeederDisplayVol& operator = (const CFeederDisplayVol&) = delete;

};


