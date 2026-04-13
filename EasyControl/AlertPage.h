//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlertPage.h
///
/// @file   AlertPage.h
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
#include "AlertCtrl.h"


class CAlertPage : public CDosePage
{
	DECLARE_DYNAMIC(CAlertPage)

	enum { IDD = IDD_LWF_ALERT };

protected:
    CAlertCtrl       m_aListCtrl;
    uint32_t         m_lLastCount;

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
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung

	BOOL OnUpdateControls() override;

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

    BOOL getListChanged() const;

public:
	CAlertPage();
    ~CAlertPage() override = default;

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
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    //------------------------------------------------------------------------------------
   ///  @brief   OnNewLogItem
   ///
   ///           This method ....
   ///
   ///  @param[in] w , WPARAM
   ///  @return LRESULT
   ///
   //------------------------------------------------------------------------------------
    LRESULT OnNewLogItem(WPARAM w, LPARAM);

	afx_msg void OnBnClickedDoseAlertClearAlarm();
	void OnColumnclickAlarmList(NMHDR* pNMHDR, LRESULT* pResult);
	void OnLvnItemActivateAlarmList(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};


