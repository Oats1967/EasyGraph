#include "pch.h"
#include "Statistics.h"
#include "StringConvert.h"


CStatistics g_Statistics;


void CStatistics::CalcFeederCount() 
{
	const uint32_t count = m_RecDaysList.GetCount();
	uint32_t maxItems = 0;
	for (uint32_t index = 0; index < count; index++)
	{
		const auto& rItem = m_RecDaysList.GetItem(index);
		const auto rMax = rItem.GetMaxItems();
		if (rMax > maxItems)
		{
			maxItems = rMax;
		}
	}
	m_FeederCount = maxItems;
}

BOOL CStatistics::LoadRectItemList()
{
	BOOL result = FALSE;
	if (m_ActiveLine >= 0 && m_ActiveLine < _S32(m_LineGraphConfig.m_field.size()))
	{
		m_RecDaysList.Clear();
		const auto& rLineItem = m_LineGraphConfig.m_field[m_ActiveLine];
		COleDateTime dSO(m_DateToShow.dateStart);
		COleDateTime dEO(m_DateToShow.dateEnd);

		COleDateTimeSpan difftime = dEO - dSO;
		int32_t days = difftime.GetDays();
		COleDateTimeSpan dayskip(1, 0, 0, 0);

		for (int32_t k = 0; k <= days; k++)
		{
			tm date_tm;
			memset(&date_tm, 0, sizeof(date_tm));
			date_tm.tm_year = dSO.GetYear() - 1900;
			date_tm.tm_mon = dSO.GetMonth() - 1;
			date_tm.tm_mday = dSO.GetDay();
			date_tm.tm_isdst = 0;
			time_t aTime = std::mktime(&date_tm);

			base::utils::CRecItemList tempList;
			tempList.SetPath(rLineItem.m_szPath);
			tempList.SetFilename(aTime);
			tempList.Load();
			m_RecDaysList += tempList;;
			dSO += dayskip;
		}
		CalcTotalizerQMNUmber();
		CalcTotalizerFeeder();
		CalcFeederCount();
		result = TRUE;
	}
	return result;
}

void CStatistics::CalcTotalizerQMNUmber(void)
{
	std::map < uint64_t, std::pair<COleDateTime, float64_t >> history;

	BOOL bInit = FALSE;
	const auto& rRecItemList = m_RecDaysList;
	for (uint32_t k = 0; k < rRecItemList.GetCount(); k++)
	{
		const auto& rRecItem = rRecItemList.GetItem(k);;
		const auto& rTime = COleDateTime(rRecItem.GetTime());
		const uint32_t rMax = rRecItem.GetMaxItems();
		for (uint32_t index = 0; index < rMax; index++)
		{
			const auto& qmNUmber = rRecItem.GetQMNumber(index);
			auto hit = history.find(qmNUmber);
			if (hit == history.cend())
			{
				history.insert({ qmNUmber, {  rTime, 0.0 } });
			}
			else
			{
				auto& rLasttime = std::get<0>(hit->second);
				COleDateTimeSpan difftime = rTime - rLasttime;
#if _DEBUG
				uint32_t seconds = difftime.GetSeconds();
				float32_t massflow = rRecItem.Get(base::eMassflowSelect::eVIEWMASSFLOW, index);
#endif
				float32_t fTot = rRecItem.Get(base::eMassflowSelect::eVIEWMASSFLOW, index) * _F32(difftime.GetSeconds());
				std::get<1>(hit->second) += fTot;
				rLasttime = rTime;
			}
		}
	}
	m_QMTotalizerMap.clear();
	for (const auto& rItem : history)
	{
		m_QMTotalizerMap.insert({ rItem.first, _F32(std::get<1>(rItem.second) / 3600.0F) });
	}
}



void CStatistics::CalcTotalizerFeeder(void)
{
	std::map< uint32_t, std::pair<COleDateTime, float64_t >> history;

	//auto count = m_RecList.count();
	const auto& rRecItemList = m_RecDaysList;
	for (uint32_t k = 0; k < rRecItemList.GetCount(); k++)
	{
		const auto& rRecItem = rRecItemList.GetItem(k);;
		const auto& rTime	 = COleDateTime(rRecItem.GetTime());
		uint32_t rMax		 = rRecItem.GetMaxItems();
		for (uint32_t index = 0; index < rMax; index++)
		{
			auto hit = history.find(index);
			if (hit == history.cend())
			{
				history.insert({ index, {  rTime, 0.0 } });
			}
			else
			{
				auto& rLasttime = std::get<0>(hit->second);
				COleDateTimeSpan difftime = rTime - rLasttime;
				float32_t fTot = rRecItem.Get(base::eMassflowSelect::eVIEWMASSFLOW, index) * _F32(difftime.GetSeconds());
				std::get<1>(hit->second) += fTot;
				rLasttime = rTime;
			}
		}
	}
	m_FeederTotalizerMap.clear();
	for (const auto& rItem : history)
	{
		m_FeederTotalizerMap.insert({ rItem.first, _F32(std::get<1>(rItem.second) / 3600.0F) });
	}
}




CString CStatistics::GetHeaderLine() const
{
	const base::CLineGraphConfig::VectorConfig& rIt = m_LineGraphConfig.m_field;
	return (m_ActiveLine < rIt.size()) ? CString("Linie : ") + toCString(rIt[m_ActiveLine].m_szName) : "";
}


CString CStatistics::GetHeaderDateTime() const
{
	CString szDate("Vom ");

	if (m_DateToShow.dateStart == m_DateToShow.dateEnd)
	{
		COleDateTime aDate(m_DateToShow.dateStart);
		auto szTemp = aDate.Format("%d.%m.%y");
		szDate += szTemp;
	}
	else
	{
		COleDateTime start(m_DateToShow.dateStart);
		COleDateTime end(m_DateToShow.dateEnd);
		auto szStart = start.Format("%d.%m.%y");
		auto szEnd = end.Format("%d.%m.%y");
		szDate += szStart + CString(" bis ") + szEnd;
	}
	return szDate;

}
