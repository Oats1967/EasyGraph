//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayPid.h
///
/// @file   FeederDisplayPid.h
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

class CFeederDisplayPid : public CFeederDisplay
{
    const POINT cCalibratePoint;
    const POINT cTarePoint;
    const POINT cEmptyFeederPoint;
    const POINT cReleasePoint;
    const POINT cRefillPoint;
    const RECT	cBackgroundRect;


protected:
    const POINT& GetReleasePoint() const override
    {  return cReleasePoint;   }
    const POINT& GetTarePoint() const  override
    {   return cTarePoint;   }
    const POINT& GetCalibratePoint() const override
    {   return cCalibratePoint;   }
    const POINT& GetEmptyFeederPoint() const override
    {  return  cEmptyFeederPoint;   }
    const POINT& GetRefillPoint() const override
    {  return  cRefillPoint;    }
    const RECT& GetBackgroundRect() const override
    {  return cBackgroundRect;    }


protected:
    BOOL IsTextUpdate(void) const override
    {   return FALSE;   }

    void DrawBitmap(CDC* pDC) override;

private:
    CFeederDisplayPid() = delete;

public:
    CFeederDisplayPid(const int32_t sID, const CRect& rRect);
    CFeederDisplayPid(const CFeederDisplayPid&) = delete;
    CFeederDisplayPid& operator = (const CFeederDisplayPid&) = delete;
    ~CFeederDisplayPid() override = default;

};




