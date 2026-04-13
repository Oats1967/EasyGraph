//------------------------------------------------------------------------------------
///
/// @brief  Implementation of module ECMessageBox
///
/// @file   ECMessageBox.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/types.h"
#include "EasyControl.h"
//------------------------------------------------------------------------------------
///  @brief   ECMessageBox
///
///           This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] nType , uint32_t
///  @param[in] nIDPrompt , uint32_t
///  @return int
///
//------------------------------------------------------------------------------------
int32_t ECMessageBox(uint32_t nIDPrompt, uint32_t nType);

//------------------------------------------------------------------------------------
///  @brief   ECMessageBox
///
///           This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] nType , uint32_t
///  @param[in] lpszText , LPCTSTR
///  @return int
///
//------------------------------------------------------------------------------------
int ECMessageBox(LPCTSTR lpszText, uint32_t nType);


