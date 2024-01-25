#pragma once

#include <vector>
#include <algorithm>
#include "QualityMap.h"
#include "BASE/Utils/public/RecItemList.h"
#include "Utils/LineConfig.h"


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
	DateToShow					  m_DateToShow;

	utils::CLineConfig			m_LineConfig;
	int32_t						m_ActiveLine;

	base::eMassflowSelect  m_DoseSelect;

private:
	void GetQMNUmbers(void);
	void CalcTotalizerFeeder(void);
	void CalcTotalizerQMNUmber(void);

public:
    BOOL LoadRectItemList(const DateToShow& rDate);

	BOOL InitLineConfig();
	const utils::CLineConfig& GetLineConfig() const
	{	return m_LineConfig;	}

	const CTotalizerMap& GetQMTotalizerMap() const
	{ return m_QMTotalizerMap;	}

	const DateToShow& GetDateToShow() const
	{ return m_DateToShow;	}

	CString GetHeaderDateTime() const;
	CString GetHeaderLine() const;

};