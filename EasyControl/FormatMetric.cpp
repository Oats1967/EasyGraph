//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FormatMetric
///
/// @file   FormatMetric.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <cstdlib>
#include "EasyControl_DEU/resource.h"
#include "BASE/Base.h"
#include "FormatMetric.h"



//***********************************************************************************************************
//***********************************************************************************************************
static const CString& GetMetricKG()
{
	static bool bLoaded = false;
	static CString szMetric;

	if (! bLoaded)
	{
		bLoaded = true;
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_METRIC_KG));
		szMetric = _T(" ") + szTemp;
	}
	return szMetric;
}
//***********************************************************************************************************
//***********************************************************************************************************
static const CString& GetMetricKGH()
{
	static bool bLoaded = false;
	static CString szMetric;

	if ( ! bLoaded)
	{
		bLoaded = true;
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_METRIC_KGH));
		szMetric = _T(" ") + szTemp;
	}
	return szMetric;
}
//***********************************************************************************************************
//***********************************************************************************************************
static const CString& GetMetricKGM()
{
	static BOOL bLoaded = FALSE;
	static CString szMetric;

	if (!bLoaded)
	{
		bLoaded = TRUE;
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_METRIC_KGM));
		szMetric = _T(" ") + szTemp;
	}
	return szMetric;
}
//***********************************************************************************************************
//***********************************************************************************************************
static const CString& GetMetricPercent()
{
	static CString szMetric(_T("%"));
	return szMetric;
}
//***********************************************************************************************************
//***********************************************************************************************************
CString FormatMetric2(const float32_t value, const CString& add)
{
	auto sz = FormatMetric2(value);;
	sz += add;
	return std::move(sz);
}
//***********************************************************************************************************
//***********************************************************************************************************
CString FormatMetric2(const float32_t value)
{
	CString sz;

	if (value < 10.0f)
	{
		sz.Format(_T("%1.2f"), ROUND2(value));
	}
	else if (value < 100.0f)
	{
		sz.Format(_T("%2.1f"), ROUND1(value));
	}
	else if (value < 1000.0f)
	{
		sz.Format(_T("%3.0f"), ROUND0(value));
	}
	else
	{
		sz.Format(_T("%4.0f"), ROUND0(value));
	}
	return std::move(sz);
}
//***********************************************************************************************************
//***********************************************************************************************************
static CString FormatMetric3(const float32_t value, const CString& add)
{
	CString sz;

	if (value < 10.0f)
	{
		sz.Format(_T("%1.3f"), ROUND3(value));
	}
	else if (value < 100.0f)
	{
		sz.Format(_T("%2.2f"), ROUND2(value));
	}
	else if (value < 1000.0f)
	{
		sz.Format(_T("%3.1f"), ROUND1(value));
	}
	else if (value < 10000.0f)
	{
		sz.Format(_T("%4.0f"), ROUND0(value));
	}
	else
	{
		sz.Format(_T("%5.0f"), ROUND0(value));
	}
	sz += add;
	return std::move(sz);
}
//***********************************************************************************************************
//***********************************************************************************************************
static CString FormatMetric4(const float32_t value, const CString& add)
{
	CString sz;

	if (value < 100.0f)
	{
		sz.Format(_T("%2.3f"), ROUND3(value));
	}
	else if (value < 1000.0f)
	{
		sz.Format(_T("%3.2f"), ROUND2(value));
	}
	else if (value < 10000.0f)
	{
		sz.Format(_T("%4.1f"), ROUND1(value));
	}
	else
	{
		sz.Format(_T("%5.0f"), ROUND0(value));
	}
	sz += add;
	return std::move(sz);
}
//***********************************************************************************************************
//***********************************************************************************************************
static float32_t rounddisplay1(const float32_t value)
{
	const auto fValue = _F32(fabs(value));
	return (fValue < 1.0F) ? ROUND3(value) : ((fValue < 100.0F) ? ROUND2(value) : ROUND1(value));
}
//***********************************************************************************************************
//***********************************************************************************************************
static float32_t rounddisplay2(const float32_t value)
{
	const auto fValue = _F32(fabs(value));
	return (fValue < 1.0F) ? ROUND3(value) : ((fValue < 10.0F) ? ROUND2(value) : ROUND1(value));
}
//***********************************************************************************************************
//***********************************************************************************************************
static float32_t rounddisplay3(const float32_t value)
{
	const auto fValue = _F32(fabs(value));
	return (fValue < 1.0F) ? ROUND4(value) : ((fValue < 10.0F) ? ROUND3(value) : ROUND2(value));
}
//***********************************************************************************************
//***********************************************************************************************
void DDX_FloatKg(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		auto szValue = FormatMetric3(value, GetMetricKG());
		DDX_Text(pDX, nIDC, szValue);
	}
	else
	{
		DDX_Text(pDX, nIDC, value);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void DDX_ControlEnable(BOOL bEnable, CDataExchange* pDX, int nIDC, CWnd& rControl)
{
	if (bEnable)
	{
		DDX_Control(pDX, nIDC, rControl);
	}
}

//***********************************************************************************************
//***********************************************************************************************
void DDX_FloatKgHR(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	// High resolution
	if (!pDX->m_bSaveAndValidate)
	{
		auto szValue = FormatMetric4(value, GetMetricKG());
		DDX_Text(pDX, nIDC, szValue);
	}
	else
	{
		DDX_Text(pDX, nIDC, value);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void DDX_FloatKgh(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		auto szValue = FormatMetric3(value, GetMetricKGH());
		DDX_Text(pDX, nIDC, szValue);
	}
	else
	{
		DDX_Text(pDX, nIDC, value);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void DDX_Float(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		value = rounddisplay2(value);
	}
	DDX_Text(pDX, nIDC, value);
}
//******************************************************************************************************
//******************************************************************************************************
void DDX_Percentage(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		value = rounddisplay1(value);
	}
	DDX_Text(pDX, nIDC, value);
}
//***********************************************************************************************
//***********************************************************************************************
void DDX_TimeH(CDataExchange* pDX, int32_t nIDC, uint32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		auto szValue = FORMATTIME(value);
		DDX_Text(pDX, nIDC, szValue);
	}
	else
	{
		DDX_Text(pDX, nIDC, value);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void DDX_FloatHR(CDataExchange* pDX, int32_t nIDC, float32_t& value)
{
	if (!pDX->m_bSaveAndValidate)
	{
		value = rounddisplay3(value);
	}
	DDX_Text(pDX, nIDC, value);
}
//******************************************************************************************************
//******************************************************************************************************
void DDX_TextN(CDataExchange* pDX, int32_t nIDC, CString& value, const int32_t n)
{
	DDX_Text(pDX, nIDC, value);
	DDV_MaxChars(pDX, value, n);
}
//******************************************************************************************************
//******************************************************************************************************
CString FORMATKG(const float32_t value)
{
	return FormatMetric2(value, GetMetricKG());
}
//******************************************************************************************************
//******************************************************************************************************
CString FORMATKGH(const float32_t value)
{
	return FormatMetric2(value, GetMetricKGH());
}
//******************************************************************************************************
//******************************************************************************************************
CString FORMATKGM(const float32_t value)
{
	return FormatMetric2(value, GetMetricKGM());
}
//******************************************************************************************************
//******************************************************************************************************
CString FORMATPERCENT(const float32_t value)
{
	return FormatMetric2(value, GetMetricPercent());
}
//***********************************************************************************************
//***********************************************************************************************
CString FORMATTIME (const uint32_t value)
{
	// converts seconds to h:m:s 
	CString szValue;
	auto q = std::div(_S32(value), 60);
	int32_t s = q.rem;
	q = std::div(q.quot, 60);
	szValue.Format(_T("%02d:%02d:%02d h"), q.quot, q.rem, s);
	return szValue;
}

