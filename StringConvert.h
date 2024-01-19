//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module StringConvert
///
/// @file   StringConvert.h
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

#include <ctime>

inline CString toCString(const std::string&  str)
{
    return CString(str.c_str());
}

inline std::string toStdString(const CString& str)
{
    return (char_t*)CT2A(str);
}

inline CTime StdTimeToCTime(const std::time_t& time)
{
    CTime aTime((__time64_t)time);
    return aTime;
}

inline std::time_t CTimeToStdTime(const CTime& time)
{
    std::time_t aTime = time.GetTime();
    return aTime;
}






