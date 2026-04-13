//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmMap
///
/// @file   AlarmMap.h
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

#include "EasyControl.h"
#include <map>
#include "BASE/include/AlarmTypes.h"

class CAlarmMap
{
    using CategoryMap = std::map< const base::eAlarmCategory, std::pair<int32_t, int32_t>>;
    using AlarmMap = std::map< const base::eAlarmError, std::pair<int32_t, int32_t>>;

    static const  CategoryMap c_CategoryMap;
    static const  AlarmMap    c_AlarmMap;

    static CString GetString(const base::eAlarmError alarm, int32_t index);
    static CString GetString(const base::eAlarmCategory category, int32_t index);

public:
     static CString GetAlarmString(const base::eAlarmError alarm);
     static CString GetAlarmInfo(const base::eAlarmError alarm);
     static CString GetCategoryInfo(const base::eAlarmCategory category);
     static CString GetCategoryString(const base::eAlarmCategory category);
};


