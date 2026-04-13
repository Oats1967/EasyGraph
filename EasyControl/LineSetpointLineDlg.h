//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineSetpointLineDlg
///
/// @file   LineSetpointLineDlg.h
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
#include "BASE/Base.h"
#include "InfoButton.h"



class CLineSetpointLineDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineSetpointLineDlg)

	enum { IDD = IDD_LINE_LINESETPOINT };

private:
	const CBrush		c_EditBrush;
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CInfoButton m_MaxSetpointInfoButton;
	CInfoButton m_HysteresisInfoButton;
	CInfoButton m_MinTotBandInfoButton;
	CInfoButton m_FilterTimeInfoButton;

	float32_t m_MaxSetpoint;
	float32_t m_Hysteresis;
	float32_t m_MinTotBand;
	uint32_t  m_FilterTime;

	struct
	{
		float32_t MaxSetpoint = 0.0F;
		float32_t Hysteresis = 0.0F;
		float32_t MinTotBand = 0.0F;
		uint32_t  FilterTime = 0;
	} backupParam;


private:
	BOOL OnNotifyEditMaxSetpoint();
	BOOL OnNotifyEditMinTotband();
	BOOL OnNotifyEditHysteresis();
	BOOL OnNotifyEditNominalFilterTime();
	void SaveParams();
	void RestoreParams();


protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void OnCancel() override;

public:
	CLineSetpointLineDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineSetpointLineDlg() override = default;

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

	//***********************************************************************************************
	//***********************************************************************************************
	afx_msg void OnStnClickedLineSetpointMinTotband();
	afx_msg void OnStnClickedLineSetpointFilterTime();
	afx_msg void OnStnClickedLineSetpointHysteresis();
	afx_msg void OnStnClickedLineSetpointMaxSetpoint();

	afx_msg void OnBnClickedLineSetpointMaxSetpointInfo();
	afx_msg void OnBnClickedLineSetpointHysteresisInfo();
	afx_msg void OnBnClickedLineSetpointMinTotbandInfo();
	afx_msg void OnBnClickedLineSetpointFilterTimeInfo();


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
