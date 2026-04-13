//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyControlDoc.cpp
///
/// @file   EasyControlDoc.cpp
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

#include "EasyControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyControlDoc

IMPLEMENT_DYNCREATE(CEasyControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CEasyControlDoc, CDocument)
END_MESSAGE_MAP()


// CEasyControlDoc Erstellung/Zerstörung

CEasyControlDoc::CEasyControlDoc()
{
	// TODO: Hier Code für One-Time-Konstruktion einfügen

}

CEasyControlDoc::~CEasyControlDoc()
{
}

BOOL CEasyControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CString szTitle;
	szTitle.LoadString(IDS_TITLEOVERVIEW);
	SetTitle(szTitle);
	return TRUE;
}




// CEasyControlDoc Serialisierung

void CEasyControlDoc::Serialize(CArchive& ar)
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


// CEasyControlDoc Diagnose

#ifdef _DEBUG
void CEasyControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEasyControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEasyControlDoc-Befehle

