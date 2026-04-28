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
	BOOL						  m_RealMonitoring;
	BOOL						  m_LogEnable;
	BCGPChartMouseConfig::ZoomScrollOptions m_ZoomType;

private:
	//void GetQMNUmbers(void);
	void GetANNumbers(void);
	void CalcTotalizerFeeder(void);
	void CalcTotalizerQMNUmber(void);
	void CalcFeederCount(void);
	void CalcLogRecMapping(void);
	void Init();
	BOOL LoadLogItemList();
	void LoadLogItemList(base::utils::CLogItemList& tempList, const time_t& _tmStart);
	BOOL LoadRectItemList();
	void LoadRectItemList(base::utils::CRecItemList& tempList, const time_t& _tmStart);




public:
	BOOL LoadData();

	SETGET(const base::CLineGraphConfig&, LineGraphConfig);
	SETGET(const base::utils::CProductItemList&, ProductDatabase);
	SETGET(const CTotalizerMap&, QMTotalizerMap);
	SETGET(const CTotalizerMap&, FeederTotalizerMap);
	SETGET(const base::eMassflowSelect, DoseSelect)
	SETGET(const base::utils::CRecItemList&, RecDaysList);
	SETGET(const base::utils::CLogItemList&, LogDaysList);
	SETGET(const uint32_t, FeederCount);
	SETGET(const std::vector<int32_t>&, LogRecMapping);
	SETGET(const base::CEasyGraphSettings&, Settings);
	SETGET(const std::string&, ANNumber);
	SETGET(const BOOL, RealMonitoring);
	SETGET(const BOOL, LogEnable);
	SETGET(BCGPChartMouseConfig::ZoomScrollOptions, ZoomType)

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

	void SetDateToShow(const base::DateToShow& rDate)
	{	
		m_Settings.m_ShowTime = rDate;
	}

	const base::DateToShow& GetDateToShow(void) const
	{
		return m_Settings.m_ShowTime;
	}

	CString GetHeaderDateTime() const;
	CString GetHeaderLine() const;

	BOOL IsZoom() const
	{
		return BOOL(m_ZoomType != BCGPChartMouseConfig::ZoomScrollOptions::ZSO_NONE);
	}

	CStatistics()
	{
		Init();
	}
};


extern CStatistics g_Statistics;

