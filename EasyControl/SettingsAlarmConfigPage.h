//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsAlarmConfigPage.h
///
/// @file   SettingsAlarmConfigPage.h
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

#include "afxcmn.h"
#include "BASE/include/AlarmTypes.h"
#include "AlarmConfigCtrl.h"


class CSettingsAlarmConfigPage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsAlarmConfigPage)

	enum { IDD = IDD_SETTINGS_ALARMCONFIG };

private:
	CAlarmConfigCtrl	m_aListCtrl;
	base::CAlarmField	m_AlarmField;
	BOOL				m_bModified;

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
	void SaveItems();


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
	CSettingsAlarmConfigPage();
	~CSettingsAlarmConfigPage() override = default;

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
	afx_msg void OnLvnItemActivateAlarmList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnColumnclickAlarmList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnCustomdraw(NMHDR * pNMHDR, LRESULT * pResult);

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsAccept
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsAccept(void);

	DECLARE_MESSAGE_MAP()
};

