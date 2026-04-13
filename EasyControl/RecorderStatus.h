//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecorderStatus
///
/// @file   RecorderStatus.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------

#pragma once


enum class eRecorderStatus : int32_t
{
    eRecorderDeactive = 0,
    eRecorderActive,
    eRecorderError,
};

