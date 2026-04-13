//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfAlarmConfigPage.h
///
/// @file   LwfAlarmConfigPage.h
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

#include "DosePage.h"
#include "AlarmConfigCtrl.h"

class CAlarmConfigPage : public CDosePage
{
	DECLARE_DYNAMIC(CAlarmConfigPage)

	enum { IDD = IDD_LWF_ALARMCONFIG };

private:
	CAlarmConfigCtrl	m_aListCtrl;
	base::CAlarmField	m_AlarmField;
	BOOL				m_bPermissionModify;
	int32_t				m_LastItem;
	base::eDoseType		m_LastDoseType;

private:
	void InitAlarmField();

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
	///  @brief   OnUpdateControls
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnUpdateControls(void) override;

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

public:
	CAlarmConfigPage();
	~CAlarmConfigPage() override = default;

	//------------------------------------------------------------------------------------
   ///  @brief   OnCtlColor
   ///
   ///           This method is called by the framework when a child control is about to
   ///           be drawn
   ///
   ///  @param[in] nCtlColor , UINT
   ///  @param[in] pWnd , CWnd*
   ///  @param[in] pDC , CDC*
   ///  @return HBRUSH
   ///
   //------------------------------------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
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
	afx_msg void OnColumnclickAlarmList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);

	//------------------------------------------------------------------------------------
   ///  @brief   OnCustomdraw
   ///
   ///           This method ....
   ///
   ///  @param[in] pResult , LRESULT*
   ///  @param[in] pNMHDR , NMHDR*
   ///  @return none
   ///
   //------------------------------------------------------------------------------------
	//afx_msg void OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult );

	afx_msg LRESULT OnAlarmError(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

