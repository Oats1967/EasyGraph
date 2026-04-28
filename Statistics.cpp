#include "pch.h"
#include "EasyGraph.h"
#include "Statistics.h"
#include "StringConvert.h"


#define SHRINKSIZE (500)

CStatistics g_Statistics;

//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::Init()
{
	const CBCGPColor::BCGP_COLOR c_Color[base::cMassflowSelectMax] = {
								CBCGPColor::BCGP_COLOR::Blue,
								CBCGPColor::BCGP_COLOR::Brown,
								CBCGPColor::BCGP_COLOR::Orange,
								CBCGPColor::BCGP_COLOR::Green,
								CBCGPColor::BCGP_COLOR::Cyan,
								CBCGPColor::BCGP_COLOR::Magenta,
								CBCGPColor::BCGP_COLOR::MediumPurple
	};
	// Durchsatz
	auto& rAttribut = m_Settings.m_Attribues;
	for (int32_t k = 0; k < _S32(rAttribut.size()); k++)
	{
		auto& rA = rAttribut[k];
		rA.m_Category = base::LineCategory::eLine;
		rA.m_Visible = TRUE;
		rA.m_LineWidth = 1;
		rA.m_Color = CBCGPColor(c_Color[k]);
	}
	rAttribut[_S32(base::eMassflowSelect::eVIEWTOTALIZER)].m_Visible = FALSE;
	m_Settings.m_ShowTime.now();;
	m_Settings.m_ActiveLine = 0;
	m_Settings.m_ActiveFeeder = 0;
	m_FeederCount = 0;
	m_DoseSelect = base::eMassflowSelect::eVIEWMAX;
	m_RealMonitoring = FALSE;
	m_ZoomType = BCGPChartMouseConfig::ZoomScrollOptions::ZSO_NONE;
	m_LogEnable = FALSE;

}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::CalcFeederCount() 
{
	m_FeederCount = m_RecDaysList.GetMaxItems();
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::CalcLogRecMapping(void)
{
	m_LogRecMapping.clear();
	const auto count = m_LogDaysList.GetCount();
	if ( count > 0 )
	{
		m_LogRecMapping.resize(count);
		for (uint32_t k = 0; k < count; k++)
		{
			m_LogRecMapping[k] = m_RecDaysList.FindRecItemTime(m_LogDaysList.GetItem(k).GetTime());
		}
	}
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::GetANNumbers(void)
{
	m_ANNumberList.clear();
	m_RecDaysList.GetANNumbers(m_ANNumberList);
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::LoadLogItemList(base::utils::CLogItemList& tempList, const time_t& aTime)
{
	tempList.SetFilename(aTime);
	tempList.LoadAll();
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
BOOL CStatistics::LoadLogItemList()
{
	BOOL result = FALSE;
	const auto& ActiveLine = m_Settings.m_ActiveLine;
	if (ActiveLine >= 0 && ActiveLine < _S32(m_LineGraphConfig.m_field.size()))
	{
		m_LogDaysList.Clear();
		const auto& rLineItem = m_LineGraphConfig.m_field[ActiveLine];
		auto dSO = m_Settings.m_ShowTime.m_dateStart;
		auto dEO = m_Settings.m_ShowTime.m_dateEnd;

		if (m_Settings.m_ShowTime.IsSameDay())
		{
			m_LogDaysList.SetPath(rLineItem.m_szLogPath);
			LoadLogItemList(m_LogDaysList, dSO);
		}
		else
		{
			int32_t days = m_Settings.m_ShowTime.GetDiffDays();
			base::utils::CLogItemList TempList;
			TempList.SetPath(rLineItem.m_szLogPath);
			for (int32_t k = 0; k <= days; k++)
			{
				auto newtime = COleDateTime(dSO) + COleDateTimeSpan(k, 0, 0, 0);
				LoadLogItemList(TempList, base::utils::OleDateTime2Time(newtime));
				m_LogDaysList += TempList;
			}
		}
		if ( ! IsZoom())
		{
			m_LogDaysList.Shrink(SHRINKSIZE);
		}
		m_LogDaysList.Extract(dSO, dEO);
		result = TRUE;
	}
	return result;
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
void CStatistics::LoadRectItemList(base::utils::CRecItemList& tempList, const time_t& aTime)
{
	tempList.SetFilename(aTime);
	tempList.LoadAll();
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
BOOL CStatistics::LoadRectItemList()
{
	BOOL result = FALSE;
	const auto& ActiveLine = m_Settings.m_ActiveLine;
	if (ActiveLine >= 0 && ActiveLine < _S32(m_LineGraphConfig.m_field.size()))
	{
		m_RecDaysList.Clear();
		m_RecDaysList.SetMaxItems(0);
		const auto& rLineItem = m_LineGraphConfig.m_field[ActiveLine];
		auto dSO = m_Settings.m_ShowTime.m_dateStart;
		auto dEO = m_Settings.m_ShowTime.m_dateEnd;

		if (m_Settings.m_ShowTime.IsSameDay())
		{
			m_RecDaysList.SetPath(rLineItem.m_szRecPath);
			LoadRectItemList(m_RecDaysList, dSO);
		}
		else
		{
			int32_t days = m_Settings.m_ShowTime.GetDiffDays();
			base::utils::CRecItemList TempList;
			TempList.SetPath(rLineItem.m_szRecPath);
			for (int32_t k = 0; k <= days; k++)
			{
				auto newtime = COleDateTime(dSO) + COleDateTimeSpan(k, 0, 0, 0);
				LoadRectItemList(TempList, base::utils::OleDateTime2Time(newtime));
				m_RecDaysList += TempList;
			}
		}
		if (! IsZoom() )
		{
			m_RecDaysList.Shrink(SHRINKSIZE);
		}
		m_RecDaysList.Extract(dSO, dEO);
		result = TRUE;
	}
	return result;
}

//*********************************************************************************************************************************
//*********************************************************************************************************************************
BOOL CStatistics::LoadData()
{
	auto result = LoadRectItemList();
	if (result)
	{
		GetANNumbers();
		if (!m_ANNumber.empty())
		{
			m_RecDaysList.Extract(m_ANNumber);
		}
		CalcTotalizerQMNUmber();
		CalcTotalizerFeeder();
		CalcFeederCount();
		if (m_LogEnable)
		{
			LoadLogItemList();
			CalcLogRecMapping();
		}
	}
	return result;
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
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
//*********************************************************************************************************************************
//*********************************************************************************************************************************
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
//*********************************************************************************************************************************
//*********************************************************************************************************************************
CString CStatistics::GetHeaderLine() const
{
	CString szTemp;

	const base::CLineGraphConfig::VectorConfig& rIt = m_LineGraphConfig.m_field;
	const auto& ActiveLine = m_Settings.m_ActiveLine;
	if (ActiveLine < rIt.size())
	{
		CString szLine;
		VERIFY(szLine.LoadString(IDS_PW_LINE));
		szTemp = szLine + _T(" : ") + toCString(rIt[ActiveLine].m_szName);
	}
	return szTemp;
}
//*********************************************************************************************************************************
//*********************************************************************************************************************************
CString CStatistics::GetHeaderDateTime() const
{
	CString szDate;

	CString szFrom;
	VERIFY(szFrom.LoadString(IDS_S_FROM));
	szFrom.Append(_T(": "));

	auto start = m_Settings.m_ShowTime.GetOleDateTimeStart();
	CString szStart = start.Format(_T("%d.%m.%y"));

	if (m_Settings.m_ShowTime.IsSameDay())
	{
		COleDateTime aDate(start);
		szDate = szFrom + szStart;
	}
	else
	{
		auto end = m_Settings.m_ShowTime.GetOleDateTimeEnd();
		CString szEnd = end.Format(_T("%d.%m.%y"));
		CString szTo;
		VERIFY(szTo.LoadString(IDS_S_TO));
		szTo.Append(_T(": "));
		szDate = szFrom + szStart + _T(" ") + szTo + szEnd;
	}
	return szDate;

}
