//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module GenericRemoteConfig
///
/// @file   GenericRemoteConfig.h
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

#include <vector>
#include <array>
#include <algorithm>
#include <string>
#include "BASE/types.h"

namespace utils
{


struct LineItem
{
    std::string  m_szName;
    std::string  m_szPath;
};

using LineList = std::vector< LineItem >;

class CLineConfig
{
    LineList m_LineList;

public:
    CLineConfig() : m_LineList()
    {}
    ~CLineConfig() = default;

    LineList& GetLineList()
    {   return m_LineList;   }

    const LineItem* GetLineListItem(const std::string& szName)
    {
        auto ptr = std::find_if(m_LineList.cbegin(), m_LineList.cend(), [&szName](const LineItem& rItem)  { return rItem.m_szName == szName; });
        return (ptr == m_LineList.cend()) ? nullptr : &(*ptr);
    }

    const LineItem* operator[] ( const int32_t index) 
    {
        if (index >= 0 && index < m_LineList.size())
        {
            return &m_LineList[index];
        }
        return nullptr;
    }

};



};

