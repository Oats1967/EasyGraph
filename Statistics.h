#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "QualityMap.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/Utils/public/LogItemList.h"
#include "BASE/include//ProductDatabaseMap.h"
#include "BASE/include/LineGraphConfig.h"


struct DateToShow
{
	DATE dateStart;
	DATE dateEnd;
};

struct LineAttribute
{
	int32_t			   m_LineWidth;
	BOOL			   m_Visible;
	BCGPChartCategory  m_Category;
	CBCGPColor		   m_Color;
};


using CTotalizerMap	 = CQualityMap<uint64_t, float32_t>;

class CStatistics
{
	base::utils::CRecItemList	  m_RecDaysList;
	base::utils::CLogItemList	  m_LogDaysList;
	CTotalizerMap				  m_FeederTotalizerMap;
	CTotalizerMap				  m_QMTotalizerMap;
	base::CLineGraphConfig		  m_LineGraphConfig;
	base::CProductDatabaseMap	  m_ProductDatabase;
	std::array< LineAttribute, base::cMassflowSelectMax> m_LineAttribues;

	DateToShow					  m_DateToShow;
	int32_t						  m_ActiveLine;
	int32_t						  m_ActiveFeeder;
	uint32_t				      m_FeederCount;
	base::eMassflowSelect		  m_DoseSelect;

private:
	void GetQMNUmbers(void);
	void CalcTotalizerFeeder(void);
	void CalcTotalizerQMNUmber(void);
	void CalcFeederCount(void);
	void Init();
	BOOL LoadLogItemList();


public:
    BOOL LoadRectItemList();

	SETGET(const DateToShow&, DateToShow);
	SETGET(const base::CLineGraphConfig&, LineGraphConfig);
	SETGET(const base::CProductDatabaseMap&, ProductDatabase);
	SETGET(const CTotalizerMap&, QMTotalizerMap);
	SETGET(const CTotalizerMap&, FeederTotalizerMap);
	SETGET(const int32_t, ActiveLine)
	SETGET(const int32_t, ActiveFeeder)
	SETGET(const base::eMassflowSelect, DoseSelect)
	SETGET(const base::utils::CRecItemList&, RecDaysList);
	SETGET(const uint32_t, FeederCount);


	const LineAttribute& GetLineAttribute(const base::eMassflowSelect select)
	{	return m_LineAttribues[_S32(select)];	}
	void SetLineAttribute(const base::eMassflowSelect select, const LineAttribute& lineAttribue)
	{	m_LineAttribues[_S32(select)] = lineAttribue;	}

	CString GetHeaderDateTime() const;
	CString GetHeaderLine() const;

	CStatistics()
	{
		Init();
	}
};


extern CStatistics g_Statistics;

