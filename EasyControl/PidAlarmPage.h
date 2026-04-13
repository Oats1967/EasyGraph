//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module PidAlarmPage.h
///
/// @file   PidAlarmPage.h
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


class CPidAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CPidAlarmPage)

	enum { IDD = IDD_PID_ALARM };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	CInfoButton		m_AlarmMassflowHighInfoButton;
	CInfoButton		m_AlarmMassflowLowInfoButton;
	CInfoButton		m_AlarmDriveCommandHighInfoButton;
	CInfoButton		m_AlarmDriveCommandLowInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;
	
	uint32_t	m_MassflowHigh;
	uint32_t	m_MassflowLow;
	uint32_t	m_MaxDriveCommand;
	uint32_t	m_MinDriveCommand;
	uint32_t	m_AlarmReactionDelay;
	uint32_t	m_lAlarmStartReactionDelay;

private:
	BOOL OnNotifyAlarmHigh(void);
	BOOL OnNotifyAlarmLow(void);
	BOOL OnNotifyAlarmDriveCommandHigh(void);
	BOOL OnNotifyAlarmDriveCommandLow(void);
	BOOL OnNotifyAlarmReactionDelay(void);
	BOOL OnNotifyAlarmStartDelay(void);

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
	CPidAlarmPage();
	~CPidAlarmPage() override = default;


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
	///  @brief   OnStnClickedPidAlarmMassflowHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmMassflowHigh();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidAlarmMassflowLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmMassflowLow();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidAlarmDriveCommandHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmDriveCommandHigh();


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidAlarmDriveCommandLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmDriveCommandLow();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidAlarmReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmReactionDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedPidAlarmStartReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedPidAlarmStartReactionDelay();

	afx_msg void OnClickedPidAlarmMassflowHighInfo();
	afx_msg void OnClickedPidAlarmMassflowLowInfo();
	afx_msg void OnClickedPidAlarmDriveCommandHighInfo();
	afx_msg void OnClickedPidAlarmDriveCommandLowInfo();
	afx_msg void OnClickedPidAlarmReactionDelayInfo();
	afx_msg void OnClickedPidAlarmStartReactionDelayInfo();

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

	DECLARE_MESSAGE_MAP()
};

