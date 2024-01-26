#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "QualityMap.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/include//ProductDatabaseMap.h"
#include "BASE/include/LineGraphConfig.h"


struct DateToShow
{
	DATE dateStart;
	DATE dateEnd;
};

using CTotalizerMap	 = CQualityMap<uint64_t, float32_t>;

class CStatistics
{
	base::utils::CRecItemList	  m_RecList;
	CTotalizerMap				  m_FeederTotalizerMap;
	CTotalizerMap				  m_QMTotalizerMap;
	base::CLineGraphConfig		  m_LineGraphConfig;
	base::CProductDatabaseMap	  m_ProductDatabase;

	DateToShow					  m_DateToShow;
	int32_t						  m_ActiveLine;
	base::eMassflowSelect		  m_DoseSelect;

private:
	void GetQMNUmbers(void);
	void CalcTotalizerFeeder(void);
	void CalcTotalizerQMNUmber(void);

public:
    BOOL LoadRectItemList();

	SETGET(const DateToShow&, DateToShow);
	SETGET(const base::CLineGraphConfig&, LineGraphConfig);
	SETGET(const base::CProductDatabaseMap&, ProductDatabase);
	SETGET(const CTotalizerMap&, QMTotalizerMap);
	SETGET(const CTotalizerMap&, FeederTotalizerMap);
	SETGET(const int32_t, ActiveLine)
	SETGET(const base::eMassflowSelect, DoseSelect)

	CString GetHeaderDateTime() const;
	CString GetHeaderLine() const;
};


extern CStatistics g_Statistics;

