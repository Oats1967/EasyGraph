//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EditCtrlEx.cpp
///
/// @file   EditCtrlEx.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EditCtrlEx.h"
#include "MFCMacros.h"

//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetLongAbs(uint32_t& rValue)
{
	auto value = _U32(CEditCtrl::GetLongAbs());
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetLongAbsRange(uint32_t& rValue, const uint32_t Min, const uint32_t Max)
{
	ASSERT(Min < Max);
	auto value = _U32(CEditCtrl::GetLongAbs());
	value = RANGE(value, Min, Max);
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetUint64(uint64_t& rValue)
{
	auto value = CEditCtrl::GetUint64();
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetFloatAbs(float32_t& rValue)
{
	auto value = _F32(fabs(CEditCtrl::GetFloat()));
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetFloat(float32_t& rValue)
{
	auto value = CEditCtrl::GetFloat();
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetFloatAbsRange(float32_t& rValue, const float32_t fMin, const float32_t fMax)
{
	ASSERT(fMin < fMax);
	auto value = _F32(fabs(CEditCtrl::GetFloat()));
	value = RANGE(value, fMin, fMax);
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetFloatRange(float32_t& rValue, const float32_t fMin, const float32_t fMax)
{
	ASSERT(fMin < fMax);
	auto value = _F32(CEditCtrl::GetFloat());
	value = RANGE(value, fMin, fMax);
	BOOL bModified = (rValue != value);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CEditCtrlEx::GetString(CString& rValue)
{
	auto value = CEditCtrl::GetString();
	BOOL bModified = (value != rValue);
	if (bModified)
	{
		rValue = value;
	}
	return bModified;
}






