//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayIcp.h
///
/// @file   FeederDisplayIcp.h
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

class CFeederDisplayIcb : public CFeederDisplay
{
    const POINT cReleasePoint;
    const POINT cCalibratePoint;
    const POINT cTarePoint;
    const POINT cRefillPoint;
    const POINT cEmptyFeederPoint;
    const POINT cRefillStatePoint;
    const RECT	cBackgroundRect;

protected:
    //***********************************************************************************************************
    //***********************************************************************************************************
    const POINT& GetReleasePoint() const override
    {   return  cReleasePoint;   }

    const POINT& GetCalibratePoint() const override
    {  return  cCalibratePoint;    }

    const POINT& GetTarePoint() const override
    {   return  cTarePoint;   }

    const POINT& GetEmptyFeederPoint() const override
    {  return  cEmptyFeederPoint;   }

    virtual const POINT& GetRefillPoint() const  override
    {   return cRefillStatePoint;   }

    const RECT& GetBackgroundRect() const final
    {  return cBackgroundRect;   }


protected:
    void DrawBitmap(CDC* pDC) override;
    BOOL IsTextUpdate(void) const override
    {  return FALSE;   }

private:
    CFeederDisplayIcb() = delete;

public:
    CFeederDisplayIcb(int32_t sID, const CRect& rRect);
    CFeederDisplayIcb(const CFeederDisplayIcb&) = delete;
    CFeederDisplayIcb& operator = (const CFeederDisplayIcb&) = delete;
    ~CFeederDisplayIcb() override = default;
};




