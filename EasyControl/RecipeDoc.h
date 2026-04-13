//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeDoc.h
///
/// @file   RecipeDoc.h
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


#include "BASE/Utils/public/RecipeItemList.h"


class CRecipeDoc : public CDocument
{
	DECLARE_DYNCREATE(CRecipeDoc)

	base::utils::CRecipeItemList m_aRecipeItemList;

public:
	CRecipeDoc() {};
	~CRecipeDoc() override = default;
	void Serialize(CArchive& ar) override;   // überladen für Dokument-E/A
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	const base::utils::CRecipeItemList& GetRecipeItemList() const
	{return m_aRecipeItemList; }

	base::utils::CRecipeItemList& GetRecipeItemList()
	{ return m_aRecipeItemList;	}

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};

