//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module CProductDatabaseBox
///
/// @file   ProductDatabaseBox.h
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


#include "EasyControl.h"
#include "ProductListCtrl.h"
#include "ModelessDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CProductDatabaseBox : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CProductDatabaseBox)

	enum { IDD = IDD_PRODUCTDATABASE };

private:
	CProductListCtrl m_wndProductListCtrl;
	BOOL  m_bInvers;
	CWnd* m_pParent;

	static CProductDatabaseBox* g_ProductDatabaseBox;


private:
	void GetValue(void)
	{}

	void SetValue(void);

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	void OnLvnItemActivateList(NMHDR* pNMHDR, LRESULT* pResult);

public:
	CProductDatabaseBox();
	~CProductDatabaseBox() override
	{	g_ProductDatabaseBox = nullptr;	}

	void Create(CWnd* pParent, BOOL bInvers, const CPoint& top);

	static void CreateProductDatabaseBox(CWnd* pParent, BOOL bInvers, const CPoint& top);
	static void DestroyProductDatabaseBox(void);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
