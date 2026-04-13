//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeView.cpp
///
/// @file   RecipeView.cpp
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

#include "EasyControl.h"

class CRecipeFileDialog : public CFileDialog
{
	CString m_szRecipePath;
	CString m_szFilename;


public:
	CRecipeFileDialog(BOOL openflag, LPCTSTR szfilename = NULL ) :
		CFileDialog(openflag, 
			_T(".rzp"), 
			szfilename,
			(openflag) ? _U32(OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR) : _U32(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT),
			_T("Recipe-XML files (*.xml)|*.xml|Recipe files (*.rzp)|*.rzp||")
			,0
			,0
			,TRUE)
		, m_szRecipePath{ toCString(EASYCONTROLREF.m_RecipePath) }
	{
		m_ofn.lpstrInitialDir = m_szRecipePath;
	}

	void SetFilename(CString szFilename)
	{ 
		m_ofn.lpstrFile = szFilename.GetBuffer(szFilename.GetLength());
	}
};
