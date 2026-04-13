//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DataImage
///
/// @file   DataImage.h
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

#include <array>
#include "FeederDisplay.h"
#include "DoseBaseActualGrafik.h"
#include "DoseBaseNominalGrafik.h"
#include "DoseButtonGrafik.h"
#include "DoseRefillButtonGrafik.h"
#include "DoseRefillReleaseButtonGrafik.h"
#include "DoseProductNameGrafik.h"
#include "BASE/include/DoseDataCache.h"
#include "DisplayContainer.h"


namespace dataimage
{
	//*****************************************************************************************
	//*****************************************************************************************
	struct CDoseDataImage
	{
		int32_t		 m_lID;

		CDisplayContainer			m_Container;
		base::CDoseDataCache		m_Cache;
		base::eAlarmErrorBits		m_ulLastDoseAlarm;
		base::eAlarmErrorBits		m_ulLastDoseWarning;

		void Reset(void);
		CDoseDataImage(void);
		~CDoseDataImage(void)
		{	Reset();	}
	};
	//*****************************************************************************************
	//*****************************************************************************************
	inline	CDoseDataImage::CDoseDataImage(void) : m_lID(-1)
		, m_Cache()
	{}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline	void CDoseDataImage::Reset(void)
	{
		m_lID = 0L;

		// Mauskontrolle
		m_Container.Reset();
		m_Cache.Reset();
	}
	
};



