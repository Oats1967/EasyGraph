//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeTypen.h
///
/// @file   RecipeTypen.h
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

#include "BASE/types.h"


//--------------------------------------------------
typedef struct __DisplayColor
{
    COLORREF Textcolor;
    COLORREF Bkcolor;
} DisplayColor_s;

enum class eColorPair : uint16_t
{
    eColNormal = 0,
	eColModified,
	eColError,
    eColRegenerat,
    eColRecipeEdit,
    eColRecipeRegenerat,
    eColMax
};


enum class eRecipeState : uint16_t
{ 
	eRecipeStateNormal = 0,
	eRecipeStateComplete,
	eRecipeStateMax
};

//--------------------------------------------------
typedef struct __RecipeItem
{
	CString         m_szName;
	CString         m_szNr;
	CString         m_szDoseType;
	uint64_t        m_QMNumber = 0;
	float32_t	    m_fRecipePercentage		= 0.0F;
	float32_t	    m_fSetpoint				= 0.0F;
    float32_t       m_fDosePerformance		= 0.0F;
	float32_t	    m_fRefillAlarmLimit		= 0.0F;
	float32_t	    m_fRefillMin			= 0.0F;
	float32_t	    m_fRefillMax			= 0.0F;
	float32_t	    m_fTotalizer			= 0.0F;

	void Init()
	{
		m_szName = _T("");
		m_QMNumber = 0;
		m_szNr = _T("");
		m_szDoseType = _T("");
		m_fRecipePercentage = 0.0f;
		m_fSetpoint = 0.0f;
		m_fRefillAlarmLimit = 0.0f;
		m_fRefillMin = 0.0f;
		m_fRefillMax = 0.0f;
		m_fDosePerformance = 0.0f;
		m_fTotalizer = 0.0F;
	}
} RecipeItem_s, *lpRecipeItem_s;

#define MAXRECIPECOUNT 8


