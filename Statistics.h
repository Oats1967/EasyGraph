#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "QualityMap.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/Utils/public/LogItemList.h"
#include "BASE/Utils/public/ProductItemList.h"
#include "BASE/include/LineGraphConfig.h"
#include "BASE/include/EasyGraphSettings.h"


struct DateToShow
{
	DATE dateStart;
	DATE dateEnd;
};


using CTotalizerMap	 = CQualityMap<uint64_t, float32_t>;

class CStatistics
{
	base::utils::CRecItemList	  m_RecDaysList;
	base::utils::CLogItemList	  m_LogDaysList;
	base::utils::CProductItemList m_ProductDatabase;
	CTotalizerMap				  m_FeederTotalizerMap;
	CTotalizerMap				  m_QMTotalizerMap;
	base::CLineGraphConfig		  m_LineGraphConfig;
	base::CEasyGraphSettings	  m_Settings;
	std::vector<int32_t>		  m_LogRecMapping;
	std::vector<std::string>	  m_ANNumberList;
	uint32_t				      m_FeederCount;
	base::eMassflowSelect		  m_DoseSelect;
	std::string					  m_ANNumber;
	BOOL						  m_LogMessages;

private:
	//void GetQMNUmbers(void);
	void GetANNumbers(void);
	void CalcTotalizerFeeder(void);
	void CalcTotalizerQMNUmber(void);
	void CalcFeederCount(void);
	void CalcLogRecMapping(void);
	void Init();
	BOOL LoadLogItemList();

	static time_t ConvertTime(const DATE& rD)
	{
		COleDateTime rT(rD);
		SYSTEMTIME st;
		rT.GetAsSystemTime(st);
		CTime tme(st);
		return tme.GetTime();
	}

	static DATE ConvertTime(const time_t& rD)
	{
		COleDateTime rT(rD);
		return rT.m_dt;
	}

public:
    BOOL LoadRectItemList();

	SETGET(const base::CLineGraphConfig&, LineGraphConfig);
	SETGET(const base::utils::CProductItemList&, ProductDatabase);
	SETGET(const CTotalizerMap&, QMTotalizerMap);
	SETGET(const CTotalizerMap&, FeederTotalizerMap);
	SETGET(const base::eMassflowSelect, DoseSelect)
	SETGET(const base::utils::CRecItemList&, RecDaysList);
	SETGET(const base::utils::CLogItemList&, LogDaysList);
	SETGET(const uint32_t, FeederCount);
	SETGET(const std::vector<int32_t>&, LogRecMapping);
	SETGET(const BOOL, LogMessages);
	SETGET(const base::CEasyGraphSettings&, Settings);
	SETGET(const std::string&, ANNumber);

	int32_t GetActiveFeeder(void) const
	{	return m_Settings.m_ActiveFeeder;	}
	void SetActiveFeeder(const int32_t activefeeder)
	{	m_Settings.m_ActiveFeeder = activefeeder;	}

	std::vector<std::string>& GetANNumberList(void)
	{	return m_ANNumberList;	}

	const std::vector<std::string>& GetANNumberList(void) const
	{	return m_ANNumberList;	}


	int32_t GetActiveLine(void) const
	{ return m_Settings.m_ActiveLine;	}

	void SetActiveLine(const int32_t activefeeder)
	{	m_Settings.m_ActiveLine = activefeeder;	}

	const base::LineAttribute& GetLineAttribute(const base::eMassflowSelect select)
	{	return m_Settings.m_Attribues[_S32(select)];	}

	void SetLineAttribute(const base::eMassflowSelect select, const base::LineAttribute& lineAttribue)
	{	m_Settings.m_Attribues[_S32(select)] = lineAttribue;	}

	void SetDateToShow(const DateToShow& rDate)
	{	
		m_Settings.m_StartTime = ConvertTime(rDate.dateStart);
		m_Settings.m_EndTime = ConvertTime(rDate.dateEnd);
	}

	const DateToShow GetDateToShow(void) const
	{
		DateToShow rDate;
		rDate.dateStart = ConvertTime(m_Settings.m_StartTime);
		rDate.dateEnd   = ConvertTime(m_Settings.m_EndTime);
		return rDate;
	}

	CString GetHeaderDateTime() const;
	CString GetHeaderLine() const;

	CStatistics()
	{
		Init();
	}
};


extern CStatistics g_Statistics;

