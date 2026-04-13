//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfsAlarmPage
///
/// @file   IfsAlarmPage.h
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


class CIfsAlarmPage : public CDosePage
{
	DECLARE_DYNAMIC(CIfsAlarmPage)


// Dialogfelddaten
	enum { IDD = IDD_IFS_ALARM };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	CInfoButton		m_FeederEmptyTimeOutInfoButton;
	CInfoButton		m_FeederOverflowTimeoutInfoButton;
	CInfoButton		m_AlarmReactionDelayInfoButton;
	CInfoButton		m_AlarmStartReactionDelayInfoButton;

	uint32_t  m_FeederEmptyTimeOut;
	uint32_t  m_FeederOverflowTimeout;
	uint32_t  m_AlarmReactionDelay;
	uint32_t  m_lAlarmStartReactionDelay;

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;


private:
	BOOL OnNotifyAlarmStartReactionDelay();
	BOOL OnNotifyAlarmReactionDelay();
	BOOL OnNotifyFeederEmptyTimeOut();
	BOOL OnNotifyFeederOverflowTimeOut();

public:
	CIfsAlarmPage();
	~CIfsAlarmPage() override = default;

	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	afx_msg void OnStnClickedIfsFeederEmptyTimeOut();
	afx_msg void OnStnClickedIfsFeederOverflowTimeOut();
	afx_msg void OnStnClickedIfsAlarmReactionDelay();
	afx_msg void OnStnClickedIfsAlarmStartReactionDelay();

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

	afx_msg void OnBnClickedIfsFeederEmptyTimeOutInfo();
	afx_msg void OnBnClickedIfsFeederOverflowTimeOutInfo();
	afx_msg void OnClickedIfsAlarmReactionDelayInfo();
	afx_msg void OnClickedIfsAlarmStartReactionDelayInfo();

	DECLARE_MESSAGE_MAP()

};

