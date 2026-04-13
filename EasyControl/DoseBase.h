//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DosierBase
///
/// @file   DosierBase.h
///
///
/// @coypright(c)  Ing.b�ro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/include/WbmStates.h"
#include "BASE/include/DoseDataCache.h"


#define __IS_P_CHANGED(__o, __a) ( __o.flags.##__a)
class CDoseBase
{
// Attributes
private:
	base::CDoseDataCache	m_Cache;
	base::LineProcessStatus m_LineStatus;

	base::LineProcessStatus m_LineStatusChanged;
	base::ProcessStatus		m_ProcessStatusChanged;
	base::RefillStatus		m_RefillStatusChanged;

	bool checkFloatDiff(const float32_t& f1, const float32_t& f2) const
	{ 
		static const float32_t c_epsilon = 1e-04f;
		return (_F32(fabs(f1 - f2)) > c_epsilon);
	}

protected:
	virtual void OnSetNewData(const base::CDoseDataCache& )
	{}

	virtual base::eFeedingType GetFeedingType() const
	{ return m_Cache.m_eFeedingType;}
	virtual float32_t GetRecipeValue() const
	{ return m_Cache.m_fAnteil;	}
	virtual int32_t GetID() const
	{	return m_Cache.m_id;	}
	virtual const std::string& GetName() const
	{	return m_Cache.m_szName;	}
	virtual float32_t GetSetpoint() const
	{	return m_Cache.m_fSetpoint;	}
	virtual float32_t GetMassflow() const
	{	return m_Cache.m_fMassflow;	}
	virtual float32_t GetTotalizer() const
	{	return m_Cache.m_fTotalizer;	}
	virtual float32_t GetWeight() const
	{	return m_Cache.m_fActWeight;	}
	virtual float32_t GetRefillMin() const
	{	return m_Cache.m_fRefillMin;	}
	virtual float32_t GetRefillMax() const
	{	return m_Cache.m_fRefillMax;}
	virtual float32_t GetRefillMinMin() const
	{	return m_Cache.m_fRefillMinMin;	}
	virtual float32_t GetDriveCommand() const
	{	return m_Cache.m_fActualDriveCommand;	}
	virtual float32_t GetBeltLoadMassflow() const
	{	return m_Cache.m_fBandlastMassflow;	}
	virtual float32_t GetLclWeightMinLevel() const
	{	return m_Cache.m_LclWeightMinLevel;	}
	virtual float32_t GetLclWeightMaxLevel() const
	{	return m_Cache.m_LclWeightMaxLevel;	}

	virtual bool GetAlarm() const
	{	return m_Cache.m_ulDoseAlarm.IsAlarm();	}
	virtual bool GetWarning() const
	{	return m_Cache.m_ulDoseWarning.IsAlarm();	}

	virtual bool GetStart() const
	{	return m_Cache.m_ProcessStatus.flags.started;	}
	virtual bool GetRunning() const
	{	return m_Cache.m_ProcessStatus.flags.running;	}
	virtual bool GetLocalMode() const
	{	return bool(m_Cache.m_ProcessStatus.flags.lineMode == 0);	}
	virtual bool GetLineMode() const
	{	return m_Cache.m_ProcessStatus.flags.lineMode;	}
	virtual bool GetIOStartInput() const
	{	return m_Cache.m_ProcessStatus.flags.startext;	}
	virtual bool GetIOLogoStart() const
	{	return m_Cache.m_ProcessStatus.flags.logostartext;	}
	virtual bool GetCalibration() const
	{	return m_Cache.m_ProcessStatus.flags.calibrateActive;	}
	virtual bool GetEmptyFeeder() const
	{	return m_Cache.m_ProcessStatus.flags.emptyfeederActive;	}
	virtual bool GetTaring() const
	{	return m_Cache.m_ProcessStatus.flags.tareActive;	}
	virtual bool GetIOReleaseInput() const
	{	return m_Cache.m_ProcessStatus.flags.releaseext;	}
	virtual bool GetRefillAlarm() const
	{	return m_Cache.m_ProcessStatus.flags.refillTimeOut;	}
	virtual bool GetRefillActive() const
	{	return m_Cache.m_ProcessStatus.flags.refillActive;	}
	virtual bool GetRefillEnable() const
	{	return m_Cache.m_ProcessStatus.flags.refillRelease;	}
	virtual bool GetRefillControlEnable() const
	{	return m_Cache.m_ProcessStatus.flags.refillControlEnable;	}
	virtual bool GetLCEnable() const
	{	return m_Cache.m_ProcessStatus.flags.LCEnable;	}
	virtual bool GetLCActive() const
	{	return m_Cache.m_ProcessStatus.flags.LCActive;	}
	virtual bool GetGravimetric() const
	{	return m_Cache.m_ProcessStatus.flags.gravimetric;	}
	virtual bool GetGravVolState() const
	{	return m_Cache.m_ProcessStatus.flags.gravvol;	}
	virtual bool GetMinBeltLoadAlarm() const
	{	return m_Cache.m_ProcessStatus.flags.wbfminbeltloadreached;	}
	virtual bool GetIfsSwitchMax() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchMax;	}
	virtual bool GetIfsSwitchMaxEnable() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchMaxEnable;	}
	virtual bool GetIfsSwitchMin() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchMin;	}
	virtual bool GetIfsSwitchMinEnable() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchMinEnable;	}
	virtual bool GetIfsSwitchHigh() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchHigh;	}
	virtual bool GetIfsSwitchHighEnable() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchHighEnable;	}
	virtual bool GetIfsSwitchLow() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchLow;	}
	virtual bool GetIfsSwitchLowEnable() const
	{	return m_Cache.m_ProcessStatus.flags.IfsSwitchLowEnable;	}
	virtual bool GetIfsEmptyFeeder() const
	{	return m_Cache.m_ProcessStatus.flags.IfsEmptyFeeder;	}
	virtual bool GetIfsOverflowFeeder() const
	{	return m_Cache.m_ProcessStatus.flags.IfsOverflowFeeder;	}

	virtual bool IsWeightBelowMinSensor() const
	{	return (m_Cache.m_RefillStatus.flags.refillWeightBelowMin);	}
	virtual bool IsWeightAboveMaxSensor() const
	{	return (m_Cache.m_RefillStatus.flags.refillWeightAboveMax);	}
	virtual bool IsEmptyFeederSensor() const
	{	return m_Cache.m_RefillStatus.flags.refillFeederEmptyStarted;	}

	virtual bool GetIOLineStartInput() const
	{	return m_LineStatus.flags.IOlinestart;	}


	bool IsTaringChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, tareActive);	}
	bool IsEmptyFeederChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, emptyfeederActive);	}
	bool IsCalibrationChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, calibrateActive);	}
	bool IsRunningChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, running);	}
	bool IsStartedChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, started);	}
	bool IsLocalModeChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, lineMode); }
	bool IsReleaseChanged()  const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, release);	}
	bool IsIOStartInputChanged()   const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, startext);	}
	bool IsIOLogoStartChanged()   const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, logostartext);	}
	virtual bool IsIOReleaseInputChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, releaseext);	}
	virtual bool IsRefillActiveChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, refillActive);	}
	virtual bool IsRefillAlarmChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, refillTimeOut);	}
	virtual bool IsRefillEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, refillRelease);	}
	virtual bool IsGravimetricChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, gravimetric);	}
	virtual bool IsRefillControlEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, refillControlEnable);	}
	virtual bool IsGravVolStateChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, gravvol);	}
	virtual bool IsLCEnableChanged() const
	{ return __IS_P_CHANGED(m_ProcessStatusChanged, LCEnable);	}
	virtual bool IsLCActiveChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, LCActive);	}
	virtual bool IsMinBeltLoadAlarmChanged() const
	{return __IS_P_CHANGED(m_ProcessStatusChanged, wbfminbeltloadreached);	}

	virtual bool IsIfsSwitchMaxChanged() const
	{ return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchMax);	}
	virtual bool IsIfsSwitchMaxEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchMaxEnable);	}
	virtual bool IsIfsSwitchMinChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchMin);	}
	virtual bool IsIfsSwitchMinEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchMinEnable);	}
	virtual bool IsIfsSwitchHighChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchHigh);	}
	virtual bool IsIfsSwitchHighEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchHighEnable);	}
	virtual bool IsIfsSwitchLowChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchLow);	}
	virtual bool IsIfsSwitchLowEnableChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsSwitchLowEnable);	}
	virtual bool IsIfsEmptyFeederChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsEmptyFeeder);	}
	virtual bool IsIfsOverflowFeederChanged() const
	{	return __IS_P_CHANGED(m_ProcessStatusChanged, IfsOverflowFeeder);	}

	virtual bool IsWeightBelowMinSensorChanged() const
	{	return m_RefillStatusChanged.flags.refillWeightBelowMin; }
	virtual bool IsWeightAboveMaxSensorChanged() const
	{	return m_RefillStatusChanged.flags.refillWeightAboveMax;	}
	virtual bool IsEmptyFeederSensorChanged() const
	{	return m_RefillStatusChanged.flags.refillFeederEmptyStarted;	}


	bool IsLclWeightMinLevelChanged(const float32_t& rValue) const
	{	return  checkFloatDiff(rValue, m_Cache.m_LclWeightMinLevel);	}
	bool IsLclWeightMaxLevelChanged(const float32_t& rValue) const
	{	return  checkFloatDiff(rValue, m_Cache.m_LclWeightMaxLevel);	}
	bool IsBeltloadMassflowChanged(const float32_t& rValue) const
	{	return  checkFloatDiff(rValue, m_Cache.m_fBandlastMassflow);}
	bool IsSetpointChanged(const float32_t& rSetpoint) const
	{	return  checkFloatDiff(rSetpoint, m_Cache.m_fSetpoint);	}
	bool IsMassflowChanged(const float32_t& rMassflow) const
	{	return  checkFloatDiff(rMassflow, m_Cache.m_fMassflow);	}
	bool IsWeightChanged(const float32_t& rWeight) const
	{	return  checkFloatDiff(rWeight, m_Cache.m_fActWeight);	}
	bool IsTotalizerChanged(const float32_t& rTotalizer) const
	{	return  checkFloatDiff(rTotalizer,m_Cache.m_fTotalizer);	}
	bool IsRefillMinChanged(const float32_t& rRefillMin) const
	{	return  checkFloatDiff(rRefillMin, m_Cache.m_fRefillMin);	}
	bool IsRefillMaxChanged(const float32_t& rRefillMax) const
	{	return  checkFloatDiff(rRefillMax, m_Cache.m_fRefillMax);	}
	bool IsRefillMinMinChanged(const float32_t& rRefillMinMin) const
	{	return  checkFloatDiff(rRefillMinMin, m_Cache.m_fRefillMinMin);	}
	bool IsDriveCommandChanged(const float32_t& rValue) const
	{	return  checkFloatDiff(rValue, m_Cache.m_fActualDriveCommand);	}

	bool IsIdChanged(const int32_t& id) const
	{	return  (id != m_Cache.m_id);	}
	bool IsNameChanged(const std::string& szName) const
	{	return  (szName != m_Cache.m_szName);	}
	bool IsFeedingTypeChanged(const base::eFeedingType& rData) const
	{ return  (rData != m_Cache.m_eFeedingType);	}
	bool IsRecipeValueChanged(const float32_t& rValue) const
	{	return  (rValue != m_Cache.m_fAnteil);	}
	bool IsDoseTypeChanged(const base::eDoseType& rType) const
	{	return  (rType != m_Cache.m_DoseType);	}
	bool IsAlarmChanged(const base::eAlarmErrorBits& rAlarm) const
	{	return (rAlarm.allflags ^ m_Cache.m_ulDoseAlarm.allflags) != 0;	}
	bool IsWarningChanged(const base::eAlarmErrorBits& rWarning) const
	{	return (rWarning.allflags ^ m_Cache.m_ulDoseWarning.allflags) != 0;	}
	bool IsMinBeltLoadStatusChanged(const BOOL& rStatus) const
	{	return (rStatus != m_Cache.m_sMinBeltLoadStatus);	}



	bool IsIOLineStartInputChanged() const
	{ return __IS_P_CHANGED(m_LineStatusChanged, IOlinestart);	}


	public:
	CDoseBase(int32_t sID = -1)
	{	m_Cache.m_id = sID;	}

	CDoseBase(const CDoseBase&) = default;
	CDoseBase& operator = (const CDoseBase&) = default;
	virtual ~CDoseBase(void) = default;

	void SetData(const base::CDoseDataCache& rData, const base::LineProcessStatus& rLineData);

	base::eDoseType GetDoseType() const
	{	return m_Cache.m_DoseType;	}

	void SetDoseType(const base::eDoseType eType)
	{	m_Cache.m_DoseType = eType;	}
	//---------------------------------------------------------------------------------------
	BOOL IsModified(void) const
	{	return m_ProcessStatusChanged.allflags != 0;	}

	void SetAllFlags(void)
	{	m_ProcessStatusChanged.allflags = (~0U);	}

};
//***********************************************************************************************************
//***********************************************************************************************************
inline void CDoseBase::SetData(const base::CDoseDataCache& rData, const base::LineProcessStatus& rLineData)
{
	m_LineStatusChanged.allflags = m_LineStatus.allflags ^ rLineData.allflags;
	m_ProcessStatusChanged.allflags = m_Cache.m_ProcessStatus.allflags ^ rData.m_ProcessStatus.allflags;
	m_RefillStatusChanged.allflags = m_Cache.m_RefillStatus.allflags ^ rData.m_RefillStatus.allflags;

	OnSetNewData(rData);
	m_Cache		  = rData;
	m_LineStatus = rLineData;
}
