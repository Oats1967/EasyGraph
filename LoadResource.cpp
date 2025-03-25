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
#include "pch.h"
#include <assert.h>


#define DEFAULTPATH _T("%s\\International\\%d\\EasyGraph.dll")

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
	TCHAR		_Path[MAX_PATH];

	_stprintf_s(_Path, DEFAULTPATH, sz, int(lID));
	HMODULE hDLL = LoadLibrary(_Path);
	return (HINSTANCE) hDLL;
}



