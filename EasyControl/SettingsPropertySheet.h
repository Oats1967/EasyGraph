//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsPropertySheet.h
///
/// @file   SettingsPropertySheet.h
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

// CSettingsPropertySheet
#include "SettingsAlarmConfigPage.h"
#include "SettingsUserLoginPage.h"
#include "SettingsLayoutPage.h"
#include "SettingsRecipePage.h"
#include "SettingsPathPage.h"
#include "SettingsRemotePage.h"
#include "SettingsProductListPage.h"


class CSettingsPropertySheet : public CBCGPPropertySheet
{
	DECLARE_DYNAMIC(CSettingsPropertySheet)

	static const uint32_t c_NuberPages = 7;

	CSettingsAlarmConfigPage	m_aAlarmPage;
	CSettingsUserLoginPage		m_aUserLoginPage;
	CSettingsLayoutPage			m_aLayoutPage;
	CSettingsPathPage			m_aSystemPfad;
	CSettingsRecipePage			m_aRezeptPage;
	CSettingsRemotePage			m_aRemotePage;
	CSettingsProductListPage	m_aProductListPage;
	CBCGPToolBarImages			m_imageHeader[c_NuberPages];

	const uint32_t				c_PageTitle[c_NuberPages];
	CString						m_PageTitle[c_NuberPages];


	CBCGPPropertyPage* m_pPage;
	const uint32_t c_uiIconsResID;
	const uint32_t c_cxIcon;

private:
    void Init(void);
	//------------------------------------------------------------------------------------
	///  @brief   SortButtons
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void LoadHeaderImages();
	void LoadPageTitle();
	void SortButtons(void);

protected:
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------------------------------------------
	///  @brief   OnPageChanged
	///
	///           This method ....
	///
	///  @param[in] wParam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnPageChanged(WPARAM w, LPARAM);

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

	void OnDrawPageHeader(CDC* pDC, int nPage, CRect rectHeader) override;


public:
	CSettingsPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSettingsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CSettingsPropertySheet() override = default;

	afx_msg LRESULT OnDPIChanged(WPARAM wp, LPARAM lp);

};



