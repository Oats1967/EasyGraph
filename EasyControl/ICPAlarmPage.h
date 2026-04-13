//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcpAlarmPage
///
/// @file   IcpAlarmPage.h
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


class CIcpAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CIcpAlarmPage)


// Dialogfelddaten
	enum { IDD = IDD_ICP_ALARM };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	CInfoButton		m_AlarmDriveCommandHighInfoButton;
	CInfoButton		m_AlarmDriveCommandLowInfoButton;
	CInfoButton		m_AlarmMassflowHighInfoButton;
	CInfoButton		m_AlarmMassflowLowInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;

	uint32_t	m_DriveCommandHigh;
	uint32_t	m_DriveCommandLow;
	uint32_t	m_MassflowHigh;
	uint32_t	m_MassflowLow;
	uint32_t	m_AlarmReactionDelay;
	uint32_t	m_lAlarmStartReactionDelay;


protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;


private:
	BOOL OnNotifyAlarmDriveCommandLow();
	BOOL OnNotifyAlarmDriveCommandHigh();
	BOOL OnNotifyAlarmMassflowLow();
	BOOL OnNotifyAlarmMassflowHigh();
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmReactionDelay();

public:
	CIcpAlarmPage();
	~CIcpAlarmPage() override = default;

	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	afx_msg void OnStnClickedIcpAlarmDriveCommandHigh();
	afx_msg void OnStnClickedIcpAlarmMassflowHigh();
	afx_msg void OnStnClickedIcpAlarmDriveCommandLow();
	afx_msg void OnStnClickedIcpAlarmMassflowLow();
	afx_msg void OnStnClickedIcpAlarmReactionDelay();
	afx_msg void OnStnClickedIcpAlarmStartReactionDelay();

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

	afx_msg void OnClickedIcpAlarmDriveCommandHighInfo();
	afx_msg void OnClickedIcpAlarmDriveCommandLowInfo();
	afx_msg void OnClickedIcpAlarmReactionDelayInfo();
	afx_msg void OnClickedIcpAlarmStartReactionDelayInfo();
	afx_msg void OnClickedIcpAlarmMassflowHighInfo();
	afx_msg void OnClickedIcpAlarmMassflowLowInfo();


	DECLARE_MESSAGE_MAP()

};

