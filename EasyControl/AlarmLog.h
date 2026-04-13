//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmLog
///
/// @file   AlarmLog.h
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

#include "BASE/include/AlarmTypes.h"


class CAlarmLog
{
    public:
        static void AlarmCheck(const int32_t index, const base::eAlarmLevel level, const base::eAlarmErrorBits& alarmstatus, base::eAlarmErrorBits& lastalarm);
        static void CheckLogItems(const int32_t id, const base::eAlarmErrorBits& deletestatus, const base::eAlarmLevel level);
        static void CreateLogItems(const int32_t index, const base::eAlarmErrorBits& status, const base::eAlarmLevel alarmlevel);
        static void AddLogItems(const int32_t index, const base::eAlarmError status, const base::eAlarmClass alarmclass);
        static void AlarmCheck(const int32_t index, base::eAlarmErrorBits& warnings, base::eAlarmErrorBits& lastwarnings,
                                                base::eAlarmErrorBits& alarms, base::eAlarmErrorBits& lastalarms);

};


