//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwfAlarmPage.h
///
/// @file   LwfAlarmPage.h
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


class CLwfAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CLwfAlarmPage)

	enum { IDD = IDD_LWF_ALARM };

private:
	CInfoButton		m_AlarmMassflowHighInfoButton;
	CInfoButton		m_AlarmMassflowLowInfoButton;
	CInfoButton		m_AlarmDriveCommandHighInfoButton;
	CInfoButton		m_AlarmDriveCommandLowInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;
	CInfoButton		m_AlarmDosePerformanceInfoButton;
	CInfoButton		m_AlarmDoseMaxBatchTimeInfoButton;
	CInfoButton		m_AlarmNoiseLimitInfoButton;

	uint32_t	    m_AlarmMassflowHigh;
	uint32_t	    m_AlarmMassflowLow;
	uint32_t	    m_AlarmDriveCommandHigh;
	uint32_t	    m_AlarmDriveCommandLow;
	uint32_t	    m_AlarmReactionDelay;
	uint32_t	    m_lAlarmStartReactionDelay;
	uint32_t	    m_lAlarmDosePerformance;
	uint32_t	    m_lAlarmMaxBatchTime;
	float32_t	    m_AlarmNoiseLimit;

	const std::map <int32_t, std::function<BOOL()> > c_EditMap;


private:
	BOOL OnNotifyAlarmMassflowHigh();
	BOOL OnNotifyAlarmMassflowLow();
	BOOL OnNotifyAlarmDriveCommandHigh();
	BOOL OnNotifyAlarmDriveCommandLow();
	BOOL OnNotifyAlarmDosePerformance();
	BOOL OnNotifyAlarmReactionDelay();
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmNoiseLimit();
	BOOL OnNotifyAlarmMaxBatchTime();

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls() override;
	BOOL OnInitDialog() override;

public:
	CLwfAlarmPage();
	~CLwfAlarmPage() override = default;

	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmMassflowHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmMassflowHigh();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmMassflowLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmMassflowLow();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmDriveCommandHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmDriveCommandHigh();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmDriveCommandLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmDriveCommandLow();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @author  Detlef Hafer
	///
	///  @class        CLwfAlarmPage
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmReactionDelay();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmStartReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmStartReactionDelay();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmDosePerformance
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmDosePerformance();
	afx_msg void OnStnClickedDosiererAlarmMaxBatchTime();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererAlarmNoiseLimit
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererAlarmNoiseLimit();
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

	afx_msg void OnClickedDosiererAlarmMassflowHighInfo();
	afx_msg void OnClickedDosiererAlarmMassflowLowInfo();
	afx_msg void OnClickedDosiererAlarmDriveCommandHighInfo();
	afx_msg void OnClickedDosiererAlarmDriveCommandLowInfo();
	afx_msg void OnClickedDosiererAlarmReactionDelayInfo();
	afx_msg void OnClickedDosiererAlarmStartReactionDelayInfo();
	afx_msg void OnClickedDosiererAlarmDosePerformanceInfo();
	afx_msg void OnClickedDosiererAlarmMaxBatchTimeInfo();
	afx_msg void OnClickedDosiererAlarmNoiseLimitInfo();


	DECLARE_MESSAGE_MAP()
};


