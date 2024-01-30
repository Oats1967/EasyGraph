#pragma once

#include <BCGPChartSeries.h>
#include "Statistics.h"




class CVirtualSeries : public CBCGPChartLineSeries
{
public:
	CVirtualSeries(CBCGPChartVisualObject* pChart) : CBCGPChartLineSeries(pChart)
	{
		pChart->SetCurveType(BCGPChartFormatSeries::CCT_LINE);
	}

	/// Sample data creation
	void CreateData(const base::eMassflowSelect select, const int32_t index)
	{

		const auto& cTimeSpan = g_Statistics.GetDateToShow();
		COleDateTime dtStartTime{ cTimeSpan.dateStart };

		const auto& rRecDayList = g_Statistics.GetRecDaysList();
		int32_t nDataPointCount = rRecDayList.GetCount();
		for (int i = 0; i < nDataPointCount; i++)
		{
			const auto& rItem = rRecDayList.GetItem(i);

			COleDateTime dtTime{ rItem.GetTime() };
			auto szTime = dtTime.Format("%d.%m.%y %H:%M:%S");
			AddDataPoint(szTime, rItem.Get(select, index));
		}
	}


	COleDateTime GetFirstDate() const
	{
		const auto& cTimeSpan = g_Statistics.GetDateToShow();
		return COleDateTime{ cTimeSpan.dateStart };
	}

	COleDateTime GetLastDate() const
	{
		const auto& cTimeSpan = g_Statistics.GetDateToShow();
		return COleDateTime{ cTimeSpan.dateEnd };
	}

};

