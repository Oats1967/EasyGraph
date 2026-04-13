//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module VolMotorPage.h
///
/// @file   VolMotorPage.h
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
#include "InfoButton.h"
#include "DosePage.h"


// CVolMotorPage-Dialogfeld

class CVolMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CVolMotorPage)

	// Dialogfelddaten
	enum { IDD = IDD_VOL_MOTOR };

private:
	CInfoButton		m_MaxSetpointInfoButton;
	CInfoButton		m_MaxRotationalSpeedInfoButton;
	CInfoButton		m_DoseperformanceInfoButton;

	float32_t	m_fMaxSetpoint;
	float32_t	m_fMaxRotationalSpeed;
	float32_t	m_fDoseperformance;

	const std::map <int32_t, std::function<BOOL()> > m_EditMap;


private:
	void SetControlStyle	  ( void);
	BOOL OnNotifyMaxSetpoint(void);
	BOOL OnNotifyMaxRotationalSpeed(void);
	BOOL OnNotifyDoseperformance(void);

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnUpdateControls(void) override;
	BOOL OnInitDialog() override;

public:
	CVolMotorPage();
	~CVolMotorPage() override = default;


	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	afx_msg void OnStnClickedMaxSetpoint();
	afx_msg void OnClickedMaxSetpointInfo();
	afx_msg void OnStnClickedMotorRotationalSpeed();
	afx_msg void OnStnClickedDoseperformance();
	afx_msg void OnStnClickedMotorCalibrate();
	afx_msg void OnClickedMotorDoseperformanceInfo();
	afx_msg void OnClickedMotorRotationalSpeedInfo();



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

