//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsProductListPage.h
///
/// @file   SettingsProductListPage.h
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

#include <functional>
#include <map>
#include "afxcmn.h"
#include "BASE/include/AlarmTypes.h"
#include "ProductListCtrl.h"



class CSettingsProductListPage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsProductListPage)

	enum { IDD = IDD_SETTINGS_PRODUCTLIST };

private:
	CProductListCtrl	m_aListCtrl;
	CString			    m_szProduct;
	BOOL				m_bModified;
	uint64_t			m_QMNumber;
	int32_t				m_SelectedItemID;
	BOOL				m_QMNumberModified;
	BOOL				m_szProductModified;

	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

private:
	//------------------------------------------------------------------------------------
	///  @brief   Init
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void Init();
	void GetValue();
	void SetValue();
	void SaveItems();
	BOOL OnNotifyProductName();
	BOOL OnNotifyProductKey();
	BOOL CheckValidItem(const uint64_t& key, const std::string& szProduct);


protected:
	//------------------------------------------------------------------------------------
	///  @brief   DoDataExchange
	///
	///           Called by the framework to exchange data
	///
	///  @param[in] pDX , CDataExchange*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoDataExchange(CDataExchange* pDX) override;

	//------------------------------------------------------------------------------------
	///  @brief   OnInitDialog
	///
	///           This method is called by the framework in response to WM_INITDIALOG message
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnInitDialog() override;

	//------------------------------------------------------------------------------------
	///  @brief   OnSetActive
	///
	///           This member function is called by the framework when the page is chosen
	///           by the user and becomes an active page.
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnSetActive() override;

	//------------------------------------------------------------------------------------
	///  @brief   OnOK
	///
	///           This method is called when the user clicks OK button 
	///           ( the button with ID of IDOK )
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void OnOK() override;

	//------------------------------------------------------------------------------------
	///  @brief   OnApply
	///
	///           This method is called by the framework when the user chooses the OK or 
	///			  Apply now button. When the framework calls this function, changes on all
	///			  property pages in th eproperty sheet are accepted, the property sheet 
	///			  retains focus, and OnApply returns TRUE.
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnApply() override;



public:
	CSettingsProductListPage();
	~CSettingsProductListPage() override = default;

public:
	//------------------------------------------------------------------------------------
	///  @brief   OnPageChanged
	///
	///           This method ....
	///
	///  @param[in] wParam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnPageChanged	(WPARAM wParam, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnLvnItemActivateAlarmList
	///
	///           This method ....
	///
	///  @param[in]  pResult , LRESULT *
	///  @param[in]  pNMHDR , NMHDR *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult);
	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsAccept
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void  OnBnClickedSettingsAccept();
	afx_msg void  OnBnClickedSettingsDelete();
	afx_msg void  OnBnClickedSettingsModify();

	afx_msg void  OnStnClickedProductName();
	afx_msg void  OnStnClickedKey();


	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	DECLARE_MESSAGE_MAP()
};

