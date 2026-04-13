//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ITKBoardInterface
///
/// @file   ITKBoardInterface.h
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

#include <string>
#include "BASE/types.h"
#include "BASE/include/Point.h"
#include "BASE/include/Size.h"
#include "BASE/include/Language.h"

class ITKBoardInterface
{
public:
    enum { TK_OK = 0, TK_NOTOPEN, TK_XMLOPENFAILED, TK_OPENFAILED, TK_SENDFAILED };

    enum class eLayout : uint16_t
    {
        TKL_NONE = 0, 
        TKL_NUMERIC,
        TKL_ALPHA_GERMAN,
        TKL_ALPHA_ENGLISH,
        TKL_ALPHA_DUTCH,
        TKL_MAX
    };

    using TKPoint = base::INTPOINT;
    using TKSize = base::INTSIZE;

public:
    virtual int32_t TK_Open(const std::string& filename) = 0;
    virtual int32_t TK_Close(void) = 0;
    virtual int32_t TK_LoadLayout(const eLayout nLayout) = 0;
    virtual int32_t TK_Show(const TKPoint& p) = 0;
    virtual int32_t TK_Show(void) = 0;
    virtual int32_t TK_ShowLayout(const eLayout layout, const TKPoint& p) = 0;
    virtual int32_t TK_GetSize(const eLayout layout, TKSize& aS) = 0;
    virtual int32_t TK_Hide(void) = 0;
    virtual BOOL    TK_IsHide(void) = 0;
    virtual int32_t TK_SetSize(const eLayout nLayout, const TKSize& aS) = 0;
    virtual void    TK_SetLanguage(const base::eLanguage nLayout) = 0;
    virtual eLayout TK_GetAlphaLayout(void) const = 0;
    virtual eLayout TK_GetNumericLayout(void) const = 0;
};

