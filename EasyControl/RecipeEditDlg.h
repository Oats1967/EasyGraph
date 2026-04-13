//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecipeEditDlg
///
/// @file   RecipeEditDlg.h
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
#include "ModelessDlg.h"
#include "EditCtrlEx.h"
#include "ButtonProductList.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CRecipeEditDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CRecipeEditDlg)

	enum { IDD = IDD_EDITDLG };

private:
	CButtonProductList m_Button;
	CString m_szText;
	CWnd* m_pParent;
	BOOL m_bProductName;
	UINT m_ID;

	static CRecipeEditDlg* g_RecipeEditDlg;

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickNameBt();
	afx_msg void OnClickName();

public:
	CRecipeEditDlg();
	~CRecipeEditDlg()
	{	g_RecipeEditDlg = nullptr;	}

	void Create(CWnd* pWnd, const CPoint& top, const CString& szName, const BOOL bProductName, UINT nID);
	static void CreateEditDlg(CWnd* pWnd, const CPoint& top, const CString& szName, UINT nID);
	static void CreateEditDlg(CWnd* pWnd, const CPoint& top, const uint64_t& QMNumber, UINT nID);

	LRESULT OnNewProductKey(WPARAM id, LPARAM);
	LRESULT OnNotifyEdit(WPARAM id, LPARAM);
	LRESULT OnGainFocus(WPARAM  w , LPARAM l);

	DECLARE_MESSAGE_MAP()
};

