#pragma once

#include <BCGPChartSeries.h>
#include "Statistics.h"




class CVirtualSeries : public CBCGPChartAreaSeries
{
public:
	CVirtualSeries(CBCGPChartVisualObject* pChart) : CBCGPChartAreaSeries(pChart)
	{
	}

	inline double Rand(double dblStart, double dblFinish)
	{
		double minVal = min(dblStart, dblFinish);
		double maxVal = max(dblStart, dblFinish);

		return (maxVal - minVal) * (double)rand() / (RAND_MAX + 1) + minVal;
	}


	/// Sample data creation
	void CreateData(const base::eMassflowSelect select, const int32_t index)
	{
		const auto& cTimeSpan = g_Statistics.GetDateToShow();
		COleDateTime dtStartTime{ cTimeSpan.dateStart };

		const auto& rRecDayList = g_Statistics.GetRecDaysList();
		int32_t nDataPointCount = rRecDayList.GetCount();
		m_arXValues.SetSize(nDataPointCount);
		m_arYValues.SetSize(nDataPointCount);
		m_arScreenPoints.SetSize(nDataPointCount);

#if 0
		COleDateTime dtCurrTime = COleDateTime::GetCurrentTime();
		COleDateTime dtStart(dtCurrTime.GetYear(), 1, 1, 0, 0, 0);
		COleDateTimeSpan dtInterval(0, 0, 0, 10);

		double dblVal = 0;
		for (int i = 0; i < nDataPointCount; i++)
		{
			double dblDelta = dblVal > 100 ? Rand(-100, 100) : Rand(0, 200);
			dblVal += dblDelta;

			m_arXValues[i] = dtStart.m_dt;
			m_arYValues[i] = dblVal;

			dtStart += dtInterval;
		}

#else
		for (int i = 0; i < nDataPointCount; i++)
		{
			const auto& rItem = rRecDayList.GetItem(i);

			COleDateTime dtTime{ rItem.GetTime() };
			m_arXValues[i] = dtTime.m_dt;
			m_arYValues[i] = rItem.Get(select, index);
		}
#endif

	}

	COleDateTime GetFirstDate() const
	{
		return m_arXValues.IsEmpty() ? COleDateTime::GetCurrentTime() : m_arXValues[0];
	}

	COleDateTime GetLastDate() const
	{
		return m_arXValues.IsEmpty() ? COleDateTime::GetCurrentTime() : m_arXValues[m_arXValues.GetSize() - 1];
	}

	/// Data point management - called by Chart
	virtual int GetDataPointCount() const
	{
		return (int)m_arXValues.GetSize();
	}

	virtual const CBCGPChartDataPoint* GetDataPointAt(int nIndex) const
	{
		// m_virtualPoint is defined in CBCGPChartSeries and can be used to return wrapped data,
		// but it has to be unconsted
		CBCGPChartDataPoint* pDP = (CBCGPChartDataPoint*)&m_virtualPoint;

		pDP->SetComponentValue(m_arYValues[nIndex]);
		pDP->SetComponentValue(m_arXValues[nIndex], CBCGPChartData::CI_X);

		return &m_virtualPoint;
	}

	virtual CBCGPChartValue GetDataPointValue(int nDataPointIndex, CBCGPChartData::ComponentIndex ci) const
	{
		if (ci == CBCGPChartData::CI_Y)
		{
			return m_arYValues[nDataPointIndex];
		}

		if (ci == CBCGPChartData::CI_X)
		{
			return m_arXValues[nDataPointIndex];
		}

		return CBCGPChartValue();
	}
	/// End Data point 

	/// Screen position management - called by Chart
	/// nScreenPointIndex is usually required for stacked charts
	virtual void SetDataPointScreenPoint(int nDataPointIndex, int /*nScreenPointIndex*/, CBCGPPoint pt)
	{
		m_arScreenPoints[nDataPointIndex] = pt;
	}

	virtual CBCGPPoint GetDataPointScreenPoint(int nDataPointIndex, int /*nScreenPointIndex*/) const
	{
		return m_arScreenPoints[nDataPointIndex];
	}

	virtual BOOL IsDataPointScreenPointsEmpty(int nDataPointIndex) const
	{
		return (m_arScreenPoints.GetSize() == 0 || nDataPointIndex >= m_arScreenPoints.GetSize());
	}
	/// End screen position management

public:
	CArray<double, double>	m_arYValues;
	CArray<double, double>	m_arXValues;
	CBCGPPointsArray		m_arScreenPoints;
};

