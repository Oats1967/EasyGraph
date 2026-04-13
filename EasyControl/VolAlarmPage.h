//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VolAlarmPage
///
/// @file   VolAlarmPage.h
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

#include <map>
#include <functional>
#include "DosePage.h"
#include "InfoButton.h"

class CVolAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CVolAlarmPage)

// Dialogfelddaten
	enum { IDD = IDD_VOL_ALARM };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	CInfoButton		m_AlarmDriveCommandHighInfoButton;
	CInfoButton		m_AlarmDriveCommandLowInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;

	uint32_t	m_DriveCommandHigh;
	uint32_t	m_DriveCommandLow;
	uint32_t	m_AlarmReactionDelay;
	uint32_t	m_lAlarmStartReactionDelay;

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

private:

	BOOL OnNotifyAlarmDriveCommandLow();
	BOOL OnNotifyAlarmDriveCommandHigh();
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmReactionDelay();


public:
	CVolAlarmPage();
	~CVolAlarmPage() override = default;

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
	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedVolAlarmDriveCommandHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedVolAlarmDriveCommandHigh();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedVolAlarmDriveCommandLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedVolAlarmDriveCommandLow();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedVolAlarmReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedVolAlarmReactionDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedVolAlarmStartReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedVolAlarmStartReactionDelay();

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

	//********************************************************************************************************
	//********************************************************************************************************
	afx_msg void OnClickedVolAlarmDriveCommandHighInfo();
	afx_msg void OnClickedVolAlarmDriveCommandLowInfo();
	afx_msg void OnClickedVolAlarmReactionDelayInfo();
	afx_msg void OnClickedVolAlarmStartReactionDelayInfo();


	DECLARE_MESSAGE_MAP()
};

