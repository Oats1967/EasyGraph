#pragma once

#include <algorithm>
#include <map>


template<class T1, class T2>
class CQualityMap : public std::map<T1, T2>
{
public:
	CQualityMap() = default;
	virtual ~CQualityMap() = default;

	CQualityMap(const CQualityMap& ar) : std::map<T1,T2>(ar)
	{}

	const T2 GetMin() const
	{
		T2 fMin = 0; 
		
		if (! empty() )
		{
			map<T1, T2>::const_iterator it = cbegin();
			fMin = it->second;
			std::for_each(++it, cend(), [&fMin](const auto& el) { (el.second < fMin) ? el.second : fMin; });
		}
		return fMin;
	}

	const T2 GetMax() const
	{
		T2 fMax = 0; //  numeric_limits<T2>::min();
		if (!empty())
		{
			map<T1, T2>::const_iterator it = cbegin();
			fMax = it->second;
			std::for_each(++it, cend(), [&fMax](const auto& el) { if (el.second > fMax) fMax = el.second; });
		}
		return fMax;
	}
};

