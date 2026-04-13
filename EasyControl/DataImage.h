//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DataImage.h
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

#include "BASE/Base.def"
#include "LineDataImage.h"
#include "DoseDataImage.h"


namespace dataimage
{
	//**********************************************************************************************************************
	//**********************************************************************************************************************
	struct CDataImage
	{
		CDoseDataImage m_DoseItem[GLOBALDOSEMAXCOUNT];
		CLineDataImage m_Line;

		CDataImage(void);
		~CDataImage(void)
		{
			Reset();
		}
		void Reset(void);
	};

	//*****************************************************************************************
	//*****************************************************************************************

	inline CDataImage::CDataImage(void)
	{
		Reset();
	}

	//*****************************************************************************************
	//*****************************************************************************************
	inline void CDataImage::Reset(void)
	{
		for (int i = 0; i < GLOBALDOSEMAXCOUNT; i++)
			m_DoseItem[i].Reset();
		m_Line.Reset();
	}

	CDataImage& getDataImage();
	CDoseDataImage& getDoseDataImage(const int32_t index);
	CLineDataImage& getLineDataImage(void);
};



