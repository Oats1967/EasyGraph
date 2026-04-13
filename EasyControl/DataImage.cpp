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
#include "stdafx.h"
#include "DataImage.h"

namespace dataimage
{
    static CDataImage g_DataImage;
    CDataImage& getDataImage()
    {
        return g_DataImage;
    }

    CDoseDataImage& getDoseDataImage(const int32_t index)
    {
        return g_DataImage.m_DoseItem[index];
    }

    CLineDataImage& getLineDataImage(void)
    {
        return g_DataImage.m_Line;
    }
};

