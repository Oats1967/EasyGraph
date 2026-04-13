//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RecipeDoc.cpp
///
/// @file   RecipeDoc.cpp
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
#include "EasyControl.h"
#include "RecipeDoc.h"


// CRecipeDoc
IMPLEMENT_DYNCREATE(CRecipeDoc, CDocument)

BOOL CRecipeDoc::OnNewDocument()
{
	CString szTemp;

	if (!CDocument::OnNewDocument())
		return FALSE;

	CString szTitle;
	szTitle.LoadString(IDS_TITLERECIPE);
	SetTitle(szTitle);

	return TRUE;
}



BEGIN_MESSAGE_MAP(CRecipeDoc, CDocument)
END_MESSAGE_MAP()


// CRecipeDoc-Diagnose

#ifdef _DEBUG
void CRecipeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRecipeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRecipeDoc-Serialisierung

void CRecipeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}



