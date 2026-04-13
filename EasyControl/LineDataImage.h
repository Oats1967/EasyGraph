//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineDataImage
///
/// @file   LineDataImage.h
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

#include "BASE/include/LineDataCache.h"


namespace dataimage
{
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	struct CLineDataImage
	{
		CString					m_szName;
		CString					m_szANNumber;
		BOOL					m_LayoutChanged;

		base::CLineDataCache    m_Cache;

		CLineDataImage();
		~CLineDataImage() = default;

		void Reset(void);
	};
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline void CLineDataImage::Reset(void)
	{
		m_Cache.Reset();
		m_szName = "";
		m_szANNumber = "";
		m_LayoutChanged = FALSE;
	}
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	inline CLineDataImage::CLineDataImage(void) :  
		 m_szName("")
		, m_szANNumber("")
		, m_LayoutChanged{ FALSE }
	{}
};



