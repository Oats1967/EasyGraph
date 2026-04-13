//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbmAlarmPage.h
///
/// @file   WbmAlarmPage.h
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
#include "InfoButton.h"

class CWbmAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CWbmAlarmPage)

	enum { IDD = IDD_WBM_ALARM };

private:
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;
	CInfoButton		m_AlarmAlarmMinBeltLoadInfoButton;
	CInfoButton		m_AlarmAlarmMaxBeltLoadInfoButton;

	uint32_t	    m_AlarmReactionDelay;
	uint32_t	    m_lAlarmStartReactionDelay;
	float32_t	    m_fMinBeltLoad;
	float32_t	    m_fMaxBeltLoad;

private:
	BOOL OnNotifyAlarmReactionDelay();
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmWbfMinBeltLoad();
	BOOL OnNotifyAlarmWbfMaxBeltLoad();

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

public:
	CWbmAlarmPage();
	~CWbmAlarmPage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnNotifyEdit(WPARAM , LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMAlarmMinBeltLoad
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMAlarmMinBeltLoad();


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMAlarmMaxBeltLoad
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMAlarmMaxBeltLoad();


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMAlarmReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMAlarmReactionDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWBMAlarmStartReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWBMAlarmStartReactionDelay();

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

	afx_msg void OnClickedWBMAlarmMinBeltLoadInfo();
	afx_msg void OnClickedWBMAlarmMaxBeltLoadInfo();
	afx_msg void OnClickedWBMAlarmReactionDelayInfo();
	afx_msg void OnClickedWBMAlarmStartReactionDelayInfo();


	DECLARE_MESSAGE_MAP()

};

