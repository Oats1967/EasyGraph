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

class CEasyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CEasyFileDialog)

	enum { ID_LISTVIEW_LST = lst2 };

protected:
	CListCtrl m_wndList;

	enum LISTVIEWCMD
	{
		ODM_VIEW_ICONS = 0x7029,
		ODM_VIEW_LIST = 0x702b,
		ODM_VIEW_DETAIL = 0x702c,
		ODM_VIEW_THUMBS = 0x702d,
		ODM_VIEW_TILES = 0x702e,
	};

protected:
	BOOL SetListView(LISTVIEWCMD cmd);

	CListCtrl m_wndList;          // list view showing files and folders

	BOOL OnInitDialog()
	{
		CFileDialog::OnInitDialog();
		PostMessage(MYWM_POSTINIT, 0, 0);
	}
	LRESULT OnPostInit(WPARAM wp, LPARAM lp);

public:
	CEasyFileDialog(BOOL openflag) :
		CFileDialog(openflag, 0, 0, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST,
			_T("Recipe files (*.rzp)|*.rzp|Xml files (*.xml)|*.xml||"),
			0, 0, true)
	{
		CString szRecipePath{ toCString(EASYCONTROLREF.m_RecipePath) };
		m_ofn.lpstrInitialDir = szRecipePath;
	}
	DECLARE_MESSAGE_MAP()
};
