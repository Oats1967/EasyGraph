//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LoadResource.cpp
///
/// @file   LoadResource.cpp
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
#include <assert.h>


//------------------------------------------------------------------------------------
///  @brief   LoadResourceDLL
///
///           This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] sz , LPCSTR
///  @param[in] lID , LANGID
///  @return HINSTANCE
///
//------------------------------------------------------------------------------------
HINSTANCE LoadResourceDLL(LANGID lID, LPCTSTR sz) 
{
	TCHAR		EasyControlPath[MAX_PATH];

	_stprintf_s(EasyControlPath, _T("%s\\International\\%d\\EasyControl.dll"), sz, int(lID));
	HMODULE hDLL = LoadLibrary(EasyControlPath);
	return (HINSTANCE) hDLL;
}



