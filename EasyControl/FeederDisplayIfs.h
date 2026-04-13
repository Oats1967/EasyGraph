//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayIf
///
/// @file   FeederDisplayIfs.h
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
#include "HopperIfs.h"
/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayIfs command target

class CFeederDisplayIfs : public CFeederDisplay
{
    using CBaseClass = CFeederDisplay;

private:

    union DosierBaseStatus
    {
        struct
        {
            uint32_t bIfsSwitchHighChanged       : 1;
            uint32_t bIfsSwitchHighEnableChanged : 1;
            uint32_t bIfsSwitchLowChanged        : 1;
            uint32_t bIfsSwitchLowEnableChanged  : 1;
            uint32_t bIfsSwitchMaxChanged        : 1;
            uint32_t bIfsSwitchMaxEnableChanged  : 1;
            uint32_t bIfsSwitchMinChanged        : 1;
            uint32_t bIfsSwitchMinEnableChanged  : 1;
            uint32_t bIfsSwitchEmptyFeeder       : 1;
            uint32_t bIfsSwitchOverflowFeeder    : 1;
        } flags;
        uint32_t allflags = 0;
    };
    DosierBaseStatus	m_StatusFlags;

    const uint16_t BKX = 10U;
    const uint16_t BKY = 0;
    const uint16_t BKDX = 80U;
    const uint16_t BKDY = 74U;
    const RECT cBackgroundRect = { BKX, BKY, BKX + BKDX, BKY + BKDY };

    const POINT cIfsSwitchHighPoint;
    const POINT cIfsSwitchLowPoint;
    const POINT cIfsSwitchMaxPoint;
    const POINT cIfsSwitchMinPoint;
    const POINT cCalibratePoint;
    const POINT cTarePoint;
    const POINT cEmptyFeederPoint;
    const POINT cReleasePoint;
    const POINT cRefillPoint;
    const POINT cTitlePoint;
    CHopperIfs  m_Hopper;
    CBrush		m_brushBlueSolid;
    CBrush		m_brushRedSolid;
    CBrush		m_brushWhiteDashed;
    CPen		m_PenBlue;
    CFont		m_Font;

private:
    void DrawFillingValue(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t value);;

protected:
    void OnRedraw(CDC* pDC) override;
    BOOL IsBitmapUpdate() const override;
    BOOL IsTextUpdate(void) const override
    {   return FALSE;    }
    void OnSetNewData(const base::CDoseDataCache& rNewData) override;

    virtual void DrawMinMaxSensor(CDC* pDC);
    virtual void DrawFilling(CDC*);
    virtual void DrawTitle(CDC* pDC);

    virtual const POINT& GetIfsSwitchHighPoint() const
    {  return  cIfsSwitchHighPoint;    }
    virtual const POINT& GetIfsSwitchLowPoint() const
    {   return  cIfsSwitchLowPoint;    }
    virtual const POINT& GetIfsSwitchMaxPoint() const
    {  return  cIfsSwitchMaxPoint;    }
    virtual const POINT& GetIfsSwitchMinPoint() const
    {  return  cIfsSwitchMinPoint;   }

    const POINT& GetReleasePoint() const override
    {  return  cReleasePoint;    }
    const POINT& GetCalibratePoint() const override
    {    return  cCalibratePoint;   }
    const POINT& GetTarePoint() const override
    {   return cTarePoint;    }
    const POINT& GetEmptyFeederPoint() const override
    {    return  cEmptyFeederPoint;    }
    const POINT& GetRefillPoint()  const override
    {  return  cRefillPoint;   }
    const RECT& GetBackgroundRect() const override
    {  return  cBackgroundRect;    }

    virtual IGrafikHopper& GetGrafikHopper()
    {  return m_Hopper;    }


private:
    CFeederDisplayIfs() = delete;

public:
    CFeederDisplayIfs(int32_t sID, const CRect& rRect);
    CFeederDisplayIfs(const CFeederDisplayIfs&) = delete;
    CFeederDisplayIfs& operator = (const CFeederDisplayIfs&) = delete;
    ~CFeederDisplayIfs() override = default;
};




