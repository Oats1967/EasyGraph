//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EditCtrlEx
///
/// @file   EditCtrlEx.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "EasyControl.h"
#include "BASE/Base.h"
#include "EditCtrl.h"


class CEditCtrlEx : public CEditCtrl
{
public:
	static BOOL GetLongAbs(uint32_t& rValue);
	static BOOL GetLongAbsRange(uint32_t& rValue, const uint32_t Min, const uint32_t Max);
	static BOOL GetFloatAbs(float32_t& rValue);
	static BOOL GetFloat(float32_t& rValue);
	static BOOL GetFloatAbsRange(float32_t& rValue, const float32_t MIn, const float32_t fMax);
	static BOOL GetFloatRange(float32_t& rValue, const float32_t fMin, const float32_t fMax);

	static BOOL GetString(CString& rValue);
	static BOOL GetUint64(uint64_t& rValue);

	CEditCtrlEx() = default;
	virtual ~CEditCtrlEx() = default;
};


