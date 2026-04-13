//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineBase.h
///
/// @file   LineBase.h
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

#include <array>
#include "types.h"
#include "BASE/include/FeederType.h"


class CLineBase
{
	union LineBaseStatus
	{
		struct
		{
			uint32_t inputExtChanged : 1;
			uint32_t runChanged : 1;
			uint32_t totalizerChanged : 1;
			uint32_t actualValueChanged : 1;
			uint32_t nominalValueChanged : 1;
			uint32_t maxValueChanged : 1;
			uint32_t feederTypeChanged : 1;
			uint32_t productionTimeChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};

private:
	BOOL		m_inputExt;
	BOOL		m_run;
	float32_t	m_totalizer;
	float32_t	m_actualValue;
	float32_t	m_nominalValue;
	float32_t	m_maxValue;
	uint32_t	m_productionTime;
	base::eFeederType m_feederType;

protected:
	LineBaseStatus m_StatusFlags;

public:
	CLineBase() : m_inputExt(FALSE)
				, m_run(FALSE)
				, m_totalizer(0.0F)
				, m_actualValue(0.0F)
				, m_nominalValue(0.0F)
				, m_maxValue(0.0F)
				, m_productionTime(0)
				, m_feederType(base::eFeederType::FF_NONE)
	{
		m_StatusFlags.allflags = ~0U;
	}
	CLineBase(const CLineBase&) = delete;
	CLineBase& operator = (const CLineBase&) = default;
	virtual ~CLineBase(void) = default;

	void DeleteAllFlags()
	{
		m_StatusFlags.allflags = 0U;
	}
	void SetAllFlags()
	{
		m_StatusFlags.allflags = ~0U;
	}

	BOOL IsModified(void) const
	{
		return m_StatusFlags.allflags != 0;
	}

	void SetExtInput(const BOOL value)
	{
		m_StatusFlags.flags.inputExtChanged = (value != m_inputExt);
		m_inputExt = value;
	}
	BOOL GetExtInput(void) const
	{
		return m_inputExt;
	}
	void SetRun(const BOOL value)
	{
		m_StatusFlags.flags.runChanged = (value != m_run);
		m_run = value;
	}
	BOOL GetRun(void) const
	{
		return m_run;
	}
	void SetTotalizer(const float32_t value)
	{
		m_StatusFlags.flags.totalizerChanged = (value != m_totalizer);
		m_totalizer = value;
	}
	float32_t GetTotalizer(void) const
	{
		return m_totalizer;
	}
	void SetMassflow(const float32_t value)
	{
		m_StatusFlags.flags.actualValueChanged= (value != m_actualValue);
		m_actualValue = value;
	}
	float32_t GetMassflow(void) const
	{
		return m_actualValue;
	}
	void SetSetpoint(const float32_t value)
	{
		m_StatusFlags.flags.nominalValueChanged = (value != m_nominalValue);
		m_nominalValue = value;
	}
	float32_t GetSetpoint(void) const
	{
		return m_nominalValue;
	}
	void SetMaxValue(const float32_t value)
	{
		m_StatusFlags.flags.maxValueChanged = (value != m_maxValue);
		m_maxValue = value;
	}
	float32_t GetMaxValue(void) const
	{
		return m_maxValue;
	}
	void SetProductionTime(const uint32_t value)
	{
		m_StatusFlags.flags.productionTimeChanged = (value != m_productionTime);
		m_productionTime = value;
	}
	uint32_t GetProductionTime(void) const
	{
		return m_productionTime;
	}
	void SetFeederType(const base::eFeederType  value)
	{
		m_StatusFlags.flags.feederTypeChanged =  (value != m_feederType);
		m_feederType = value;
	}
	base::eFeederType  GetFeederType(void) const
	{
		return m_feederType;
	}

};









