//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmLog
///
/// @file   AlarmLog.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "AlarmLog.h"
#include "AlarmMap.h"


//***********************************************************************************************
//***********************************************************************************************
template <typename CallableType>
static void Iterate(const base::eAlarmErrorBits& deletestatus, CallableType func)
{
    if (deletestatus.allflags != 0)
    {
        DECLARE_ALARMERROR(field)
        std::for_each(field.cbegin(), field.cend(), func);
    }
}
//***********************************************************************************************
//***********************************************************************************************
void CAlarmLog::CheckLogItems(const int32_t index, const base::eAlarmErrorBits& deletestatus, const base::eAlarmLevel level)
{
    const base::eAlarmClass alarmclass = (level == base::eAlarmLevel::eAlarm) ? base::eAlarmClass::eALARMTYP : base::eAlarmClass::eWARNTYP;
    Iterate(deletestatus, [&index, &deletestatus, &alarmclass](const base::eAlarmError& item)
        {
            if (deletestatus.IsAlarmErrorBits(item))
            {
                LOGREF.CheckLogItems(index, item, alarmclass);
            }
        });
}
//***********************************************************************************************
//***********************************************************************************************
void CAlarmLog::AddLogItems(const int32_t index, const base::eAlarmError status, const base::eAlarmClass alarmclass)
{
    auto szErrorMessage = CAlarmMap::GetAlarmString(status);
    base::utils::CLogItem item;
    item.SetItem(index);
    item.SetAlarm(status);
    item.SetAlarmClass(alarmclass);
    item.SetMessage(toStdString(szErrorMessage));
    item.SetTime(CTimeToStdTime(CTime::GetCurrentTime()));
    auto oldfilename = LOGREF.GetFilename();
    LOGREF.SetFilename(); // Forces to create new filename
    auto newfilename = LOGREF.GetFilename();
    if (oldfilename != newfilename)
    {
        LOGREF.Clear();
    }
    LOGREF.AddItem(item);
    auto result = LOGREF.SaveLastItem();
    if (!result)
    {
        auto szSave = LOGREF.GetPath();
        LOGREF.SetPath(theApp.GetTempLogFolder());
        (void)LOGREF.SaveLastItem();
        LOGREF.SetPath(szSave);
    }
}
//***********************************************************************************************
//***********************************************************************************************
void CAlarmLog::CreateLogItems(const int32_t index, const base::eAlarmErrorBits& status, const base::eAlarmLevel alarmlevel)
{
    const base::eAlarmClass alarmclass = (alarmlevel == base::eAlarmLevel::eAlarm) ? base::eAlarmClass::eALARMTYP : base::eAlarmClass::eWARNTYP;

    Iterate(status, [&index, &status, &alarmclass](const base::eAlarmError& item)
        {
            if (status.IsAlarmErrorBits(item))
            {
                AddLogItems(index, item, alarmclass);
            }
        });
}
//***********************************************************************************************
//***********************************************************************************************
void CAlarmLog::AlarmCheck(const int32_t index, const base::eAlarmLevel level, const base::eAlarmErrorBits& alarmstatus, base::eAlarmErrorBits& lastalarm)
{
    if (alarmstatus.allflags != lastalarm.allflags)
    {
        base::eAlarmErrorBits newstatus;
        newstatus.allflags = alarmstatus.allflags & (~lastalarm.allflags);
        CreateLogItems(index, newstatus, level);

        base::eAlarmErrorBits deletestatus;
        deletestatus.allflags = lastalarm.allflags & (~alarmstatus.allflags);
        CheckLogItems(index, deletestatus, level);
        lastalarm = alarmstatus;
    }
}

//***********************************************************************************************
//***********************************************************************************************
void CAlarmLog::AlarmCheck(const int32_t index, base::eAlarmErrorBits& warnings, base::eAlarmErrorBits& lastwarnings, 
                                                base::eAlarmErrorBits& alarms, base::eAlarmErrorBits& lastalarms)
{
    base::eAlarmErrorBits transstatus;

    transstatus.allflags = lastwarnings.allflags & (~warnings.allflags);
    transstatus.allflags &= alarms.allflags;                            // transition from warning to alarm

    Iterate(transstatus, [&index, &transstatus](const base::eAlarmError& item)
        {
            if (transstatus.IsAlarmErrorBits(item))
            {
                LOGREF.TransitionAlarm(index, item, base::eAlarmClass::eWARNTYP, base::eAlarmClass::eALARMTYP);
            }
        });

    alarms.allflags     |= transstatus.allflags;
    lastalarms.allflags |= transstatus.allflags;

    warnings.allflags     &= (~transstatus.allflags);
    lastwarnings.allflags &= (~transstatus.allflags);

    AlarmCheck(index, base::eAlarmLevel::eWarning, warnings, lastwarnings);
    AlarmCheck(index, base::eAlarmLevel::eAlarm, alarms, lastalarms);
}




