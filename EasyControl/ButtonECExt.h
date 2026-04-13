//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ButtonECExt
///
/// @file   ButtonECExt.h
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

#include "ButtonEC.h"

class CButtonECExt : public CButtonEC
{
protected:
	const tBitmaps c_Org;
	const tBitmaps c_Ext;
	BOOL m_bExtInput;

public:
	CButtonECExt(const CButtonEC::tBitmaps& rOrg, const CButtonEC::tBitmaps& rExt) :
		CButtonEC{ rOrg }
		, c_Org { rOrg }
		, c_Ext { rExt }
		, m_bExtInput{ FALSE }
	{}

	void SetInputExt(const BOOL bExtInput);
};
//*************************************************************************************************************
//*************************************************************************************************************
inline void CButtonECExt::SetInputExt(const BOOL bExtInput)
{
	if (bExtInput != m_bExtInput)
	{
		m_bExtInput = bExtInput;
		const tBitmaps& rBitmaps = (bExtInput) ? c_Ext : c_Org;
		SetImage(rBitmaps[0][0], rBitmaps[0][1], rBitmaps[0][2]);
		SetCheckedImage(rBitmaps[1][0], rBitmaps[1][1], rBitmaps[1][2]);
		m_bRedraw = TRUE;
	}
}

