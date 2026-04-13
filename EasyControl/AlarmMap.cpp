//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmMap
///
/// @file   AlarmMap.cpp
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
#include "AlarmMap.h"


const CAlarmMap::CategoryMap CAlarmMap::c_CategoryMap = {
            { base::eAlarmCategory::categoryAM, { IDS_CATEGORY_AM, IDS_INFO_CATEGORY_AM }},
            { base::eAlarmCategory::categoryFL, { IDS_CATEGORY_FL, IDS_INFO_CATEGORY_FL }},
            { base::eAlarmCategory::categoryS,  { IDS_CATEGORY_S, IDS_INFO_CATEGORY_S }},
            { base::eAlarmCategory::categorySD, { IDS_CATEGORY_SD, IDS_INFO_CATEGORY_HS }},
            { base::eAlarmCategory::categoryRA, { IDS_CATEGORY_RA, IDS_INFO_CATEGORY_RA }},
};



const CAlarmMap::AlarmMap CAlarmMap::c_AlarmMap = {
                    { base::eAlarmError::ERROR_DOSE_SYNCERROR,                  { IDS_ALARMERROR_DOSE_SYNCERROR, IDS_INFO_ALARMERROR_DOSE_SYNCERROR }},
                    { base::eAlarmError::ERROR_DOSE_DRIVECOMMAND_LOW,           { IDS_ALARMERROR_DOSE_DRIVECOMMAND_LOW, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_LOW } },
                    { base::eAlarmError::ERROR_DOSE_DRIVECOMMAND_HIGH,          { IDS_ALARMERROR_DOSE_DRIVECOMMAND_HIGH, IDS_INFO_ALARMERROR_DOSE_DRIVECOMMAND_HIGH }},
                    { base::eAlarmError::ERROR_DOSE_MASSFLOW_LOW,               { IDS_ALARMERROR_DOSE_MASSFLOW_LOW, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_LOW } },
                    { base::eAlarmError::ERROR_DOSE_MASSFLOW_HIGH,              { IDS_ALARMERROR_DOSE_MASSFLOW_HIGH, IDS_INFO_ALARMERROR_DOSE_MASSFLOW_HIGH }},
                    { base::eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_LOW,        { IDS_ALARMERROR_DOSE_DOSEPERFORMANCE_LOW, IDS_INFO_ALARMERROR_DOSE_DOSEPERFORMANCE_LOW }},
                    { base::eAlarmError::ERROR_DOSE_DOSEPERFORMANCE_HIGH,       { IDS_ALARMERROR_DOSE_DOSEPERFORMANCE_HIGH, IDS_INFO_ALARMERROR_DOSE_DOSEPERFORMANCE_HIGH }},
                    { base::eAlarmError::ERROR_WBF_BELTLOAD_LOW,	            { IDS_ALARMERROR_WBF_BELTLOAD_LOW, IDS_INFO_ALARMERROR_WBF_BELTLOAD_LOW }},
                    { base::eAlarmError::ERROR_WBF_BELTLOAD_HIGH,               { IDS_ALARMERROR_WBF_BELTLOAD_HIGH, IDS_INFO_ALARMERROR_WBF_BELTLOAD_HIGH }},
                    { base::eAlarmError::ERROR_DOSE_ENCODER,                    { IDS_ALARMERROR_DOSE_ENCODER, IDS_INFO_ALARMERROR_DOSE_ENCODER }},
                    { base::eAlarmError::ERROR_DOSE_ENCODER_SPEED_LOW,          { IDS_ALARMERROR_DOSE_ENCODER_SPEED_LOW, IDS_INFO_ALARMERROR_DOSE_ENCODER_SPEED_LOW }},
                    { base::eAlarmError::ERROR_DOSE_ENCODER_SPEED_HIGH,         { IDS_ALARMERROR_DOSE_ENCODER_SPEED_HIGH, IDS_INFO_ALARMERROR_DOSE_ENCODER_SPEED_HIGH }},
                    { base::eAlarmError::ERROR_DOSE_LC_INVALIDWEIGHT,           { IDS_ALARMERROR_DOSE_LC_INVALIDWEIGHT, IDS_INFO_ALARMERROR_DOSE_LC_INVALIDWEIGHT }},
                    { base::eAlarmError::ERROR_DOSE_LC_MINWEIGHT,               { IDS_ALARMERROR_DOSE_LC_MINWEIGHT, IDS_INFO_ALARMERROR_DOSE_LC_MINWEIGHT }},
                    { base::eAlarmError::ERROR_DOSE_LC_MAXWEIGHT,               { IDS_ALARMERROR_DOSE_LC_MAXWEIGHT, IDS_INFO_ALARMERROR_DOSE_LC_MAXWEIGHT }},
                    { base::eAlarmError::ERROR_DOSE_LC_OVERLOAD,                { IDS_ALARMERROR_DOSE_LC_OVERLOAD, IDS_INFO_ALARMERROR_DOSE_LC_OVERLOAD }},
                    { base::eAlarmError::ERROR_DOSE_LC_GENERALERROR,            { IDS_ALARMERROR_DOSE_LC_GENERALERROR, IDS_INFO_ALARMERROR_DOSE_LC_GENERALERROR }},
                    { base::eAlarmError::ERROR_DOSE_LC_NOISEERROR,              { IDS_ALARMERROR_DOSE_LC_NOISEERROR, IDS_INFO_ALARMERROR_DOSE_LC_NOISEERROR }},
                    { base::eAlarmError::ERROR_DOSE_LC_TIMEOUT,                 { IDS_ALARMERROR_DOSE_LC_TIMEOUT, IDS_INFO_ALARMERROR_DOSE_LC_TIMEOUT }},
                    { base::eAlarmError::ERROR_DOSE_SHUTDOWN,                   { IDS_ALARMERROR_DOSE_SHUTDOWN,IDS_INFO_ALARMERROR_DOSE_SHUTDOWN }},
                    { base::eAlarmError::ERROR_DOSE_ALARMSTOP,                  { IDS_ALARMERROR_DOSE_ALARMSTOP,IDS_INFO_ALARMERROR_DOSE_ALARMSTOP }},
                    { base::eAlarmError::ERROR_DOSE_EXTALARM,                   { IDS_ALARMERROR_DOSE_EXTALARM, IDS_INFO_ALARMERROR_DOSE_EXTALARM }},
                    { base::eAlarmError::ERROR_DOSE_EXTREFILLALARM,             { IDS_ALARMERROR_DOSE_EXTREFILLALARM, IDS_INFO_ALARMERROR_DOSE_EXTREFILLALARM }},
                    { base::eAlarmError::ERROR_DOSE_REFILL_TIMEOUT,             { IDS_ALARMERROR_DOSE_REFILL_TIMEOUT, IDS_INFO_ALARMERROR_DOSE_REFILL_TIMEOUT }},
                    { base::eAlarmError::ERROR_LWF_REFILLALARMLIMIT,            { IDS_ALARMERROR_LWF_REFILLALARMLIMIT, IDS_INFO_ALARMERROR_LWF_REFILLALARMLIMIT }},
                    { base::eAlarmError::ERROR_ICP_BETRIEB,	                    { IDS_ALARMERROR_ICP_BETRIEB, IDS_INFO_ALARMERROR_ICP_BETRIEB }},
                    { base::eAlarmError::ERROR_EMPTYFEEDER_LOADCELL,            { IDS_ALARMERROR_EMPTYFEEDER_LOADCEL, IDS_INFO_ALARMERROR_EMPTYFEEDER_LOADCEL }},
                    { base::eAlarmError::ERROR_EMPTYFEEDER_RELEASEFAILED,       { IDS_ALARMERROR_EMPTYFEEDER_RELEASEFAILED, IDS_INFO_ALARMERROR_EMPTYFEEDER_RELEASEFAILED }},
                    { base::eAlarmError::ERROR_EMPTYFEEDER_TIMEOUT,             { IDS_ALARMERROR_EMPTYFEEDER_TIMEOUT, IDS_INFO_ALARMERROR_EMPTYFEEDER_TIMEOUT }},
                    { base::eAlarmError::ERROR_EMPTYFEEDER_GENERAL,             { IDS_ALARMERROR_EMPTYFEEDER_GENERAL, IDS_INFO_ALARMERROR_EMPTYFEEDER_GENERAL }},
                    { base::eAlarmError::ERROR_CALIB_REFILLACTIVE,              { IDS_ALARMERROR_CALIB_REFILLACTIVE, IDS_INFO_ALARMERROR_CALIB_REFILLACTIVE }},
                    { base::eAlarmError::ERROR_CALIB_RELEASEFAILED,             { IDS_ALARMERROR_CALIB_RELEASEFAILED, IDS_INFO_ALARMERROR_CALIB_RELEASEFAILED }},
                    { base::eAlarmError::ERROR_CALIB_LOADCELL,                  { IDS_ALARMERROR_CALIB_LOADCELL, IDS_INFO_ALARMERROR_CALIB_LOADCELL }},
                    { base::eAlarmError::ERROR_CALIB_INVALIDSETUP,              { IDS_ALARMERROR_CALIB_INVALIDSETUP, IDS_INFO_ALARMERROR_CALIB_INVALIDSETUP }},
                    { base::eAlarmError::ERROR_TARE_RELEASEFAILED,              { IDS_ALARMERROR_TARE_RELEASEFAILED, IDS_INFO_ALARMERROR_TARE_RELEASEFAILED }},
                    { base::eAlarmError::ERROR_TARE_REFILLACTIVE,               { IDS_ALARMERROR_TARE_REFILLACTIVE, IDS_INFO_ALARMERROR_TARE_REFILLACTIVE }},
                    { base::eAlarmError::ERROR_TARE_LOADCELL,                   { IDS_ALARMERROR_TARE_LOADCELL, IDS_INFO_ALARMERROR_TARE_LOADCELL }},
                    { base::eAlarmError::ERROR_TARE_GENERAL,                    { IDS_ALARMERROR_TARE_GENERAL, IDS_INFO_ALARMERROR_TARE_GENERAL }},
                    { base::eAlarmError::ERROR_DOSE_BATCH_TIMEOUT,              { IDS_ALARMERROR_BATCH_TIMEOUT, IDS_INFO_ALARMERROR_BATCH_TIMEOUT }},
                    { base::eAlarmError::ERROR_IFS_INFEEDHOPPER_EMPTY,          { IDS_ALARMERROR_IFS_INFEEDHOPPER_EMPTY, IDS_INFO_ALARMERROR_IFS_INFEEDHOPPER_EMPTY }},
                    { base::eAlarmError::ERROR_IFS_INFEEDHOPPER_OVERFLOW,       { IDS_ALARMERROR_IFS_INFEEDHOPPER_OVERFLOW, IDS_INFO_ALARMERROR_IFS_INFEEDHOPPER_OVERFLOW }}
};
//***************************************************************************************************************
//***********************************************************************************************
CString CAlarmMap::GetString(const base::eAlarmError alarm, int32_t index)
{
    CString sz;
#if  _DEBUG
    assert(c_AlarmMap.size() == _S32(base::eAlarmError::ERROR_MAX));
    const auto it = c_AlarmMap.find(alarm);
    ASSERT(it != c_AlarmMap.cend());
    if (it == c_AlarmMap.cend())
    {
        LOGERROR("Cannot find AlarmString!, Alarm = " << _S32(alarm));
    }
    else
    {
        if (index == 0)
        {
            VERIFY(sz.LoadString(it->second.first));
        }
        else
        {
            VERIFY(sz.LoadString(it->second.second));
        }
    }
#else
    const auto& it = c_AlarmMap.at(alarm);
    if (index == 0)
    {
        VERIFY(sz.LoadString(it.first));
    }
    else
    {
        VERIFY(sz.LoadString(it.second));
    }
#endif
    return sz;
}
//***************************************************************************************************************
//***********************************************************************************************
CString CAlarmMap::GetString(const base::eAlarmCategory category, int32_t index)
{
    CString sz;
#if _DEBUG
    assert(c_CategoryMap.size() == _S32(base::eAlarmCategory::categoryMax));
    const auto it = c_CategoryMap.find(category);
    ASSERT(it != c_CategoryMap.cend());
    if (it == c_CategoryMap.cend())
    {
        LOGERROR("Cannot find Category-string!, Category = " << _S32(category));
    }
    else
    {
        if (index == 0)
        {
            VERIFY(sz.LoadString(it->second.first));
        }
        else
        {
            VERIFY(sz.LoadString(it->second.second));
        }
    }
#else
    const auto& it = c_CategoryMap.at(category);
    if (index == 0)
    {
        VERIFY(sz.LoadString(it.first));
    }
    else
    {
        VERIFY(sz.LoadString(it.second));
    }
#endif
    return sz;
}
//***************************************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
CString CAlarmMap :: GetAlarmString(const base::eAlarmError alarm )
{
    return GetString(alarm, 0);
}
//***********************************************************************************************
//***********************************************************************************************
CString CAlarmMap::GetAlarmInfo(const base::eAlarmError alarm)
{
    return GetString(alarm, 1);
}
//***************************************************************************************************************
//***********************************************************************************************
CString CAlarmMap::GetCategoryString(const base::eAlarmCategory category)
{
    return GetString(category, 0);
}
//***********************************************************************************************
//***********************************************************************************************
CString CAlarmMap::GetCategoryInfo(const base::eAlarmCategory category)
{
    return GetString(category, 1);
}
