//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WbfAlarmPage.h
///
/// @file   WbfAlarmPage.h
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


class CWbfAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CWbfAlarmPage)


// Dialogfelddaten
	enum { IDD = IDD_WBF_ALARM };

private:
	CInfoButton		m_AlarmMassflowHighInfoButton;
	CInfoButton		m_AlarmMassflowLowInfoButton;
	CInfoButton		m_AlarmDriveCommandHighInfoButton;
	CInfoButton		m_AlarmDriveCommandLowInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;
	CInfoButton		m_AlarmMinBeltLoadInfoButton;


	uint32_t	    m_MassflowHigh;
	uint32_t	    m_MassflowLow;
	uint32_t	    m_AlarmDriveCommandHigh;
	uint32_t	    m_AlarmDriveCommandLow;
	uint32_t	    m_AlarmReactionDelay;
	uint32_t	    m_lAlarmStartReactionDelay;
	float32_t		m_fMinBeltLoad;
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;


private:
	BOOL OnNotifyAlarmMassflowHigh();
	BOOL OnNotifyAlarmMassflowLow();
	BOOL OnNotifyAlarmDriveCommandHigh();
	BOOL OnNotifyAlarmDriveCommandLow();
	BOOL OnNotifyAlarmReactionDelay();
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmMinBeltLoad();

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
	CWbfAlarmPage();
	~CWbfAlarmPage() override = default;


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
	///  @brief   OnStnClickedWbfAlarmMassflowHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmMassflowHigh();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmMassflowLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmMassflowLow();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmDriveCommandHigh
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmDriveCommandHigh();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmDriveCommandLow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmDriveCommandLow();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmMinBeltLoad
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmMinBeltLoad();
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmReactionDelay();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedWbfAlarmStartReactionDelay
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedWbfAlarmStartReactionDelay();

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

	afx_msg void OnClickedWbfAlarmMassflowHighInfo();
	afx_msg void OnClickedWbfAlarmMassflowLowInfo();
	afx_msg void OnClickedWbfAlarmDriveCommandHighInfo();
	afx_msg void OnClickedWbfAlarmDriveCommandLowInfo();
	afx_msg void OnClickedWbfAlarmReactionDelayInfo();
	afx_msg void OnClickedWbfAlarmStartReactionDelayInfo();
	afx_msg void OnClickedWbfAlarmMinBeltLoadInfo();

	DECLARE_MESSAGE_MAP()


};

