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
		T2 fMin = numeric_limits<T2>::max();
		std::for_each(cbegin(), cend(), [&fMin](const auto& el) { fMin = (el < fMin) ? el : fMin; } );
	}

	const T2 GetMax() const
	{
		T2 fMax = numeric_limits<T2>::min();
		std::for_each(cbegin(), cend(), [&fMax](const auto& el) { fMax = (el > fMax) ? el : fMax; });
	}
};

