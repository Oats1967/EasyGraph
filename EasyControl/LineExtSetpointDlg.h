//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineExtSetpointDlg
///
/// @file   LineExtSetpointDlg.h
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
#include "ButtonSwitchCtrl.h"
#include "InfoButton.h"


class CLineExtSetpointDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineExtSetpointDlg)

	enum { IDD = IDD_LINE_EXTSETPOINT };

private:
	const CBrush		c_EditBrush;
	const CBrush		c_GreenBrush;

	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CButtonSwitchCtrl	m_chkUserInvertSetpoint;
	CButtonSwitchCtrl	m_chkUserExtSetpoint;
	CInfoButton			m_ScaleInfoButton;
	CInfoButton			m_OffsetInfoButton;
	CInfoButton			m_ActExtSetpointInfoButton;
	float32_t			m_Scale;
	float32_t			m_Offset;
	float32_t			m_ActExtSetpoint;
	uint32_t			m_lUpdateSwitches;

	struct
	{
		BOOL bLineExternalSetpointEnable = FALSE;
		BOOL bLineExternalSetpointInverseEnable = FALSE;
		float32_t Scale = 0.0F;
		float32_t Offset = 0.0F;
	} backupParam;

private:
	void OnBnClickedLineExtSetpointInversUser();
	void OnBnClickedLineExtSetpointUser();
	void UpdateControl();
	void UpdateSwitches();
	void SaveParams();
	void RestoreParams();


protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void OnCancel() override;
	
public:
	CLineExtSetpointDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineExtSetpointDlg() override = default;

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
	LRESULT OnGaugeClick(WPARAM wp, LPARAM);
	BOOL OnNotifyEditScale();
	BOOL OnNotifyEditOffset();

	afx_msg void OnStnClickedLineExtSetpointScaleEdit();
	afx_msg void OnStnClickedLineExtSetpointOffsetEdit();
	afx_msg void OnBnClickedLineExtSetpointScaleInfo();
	afx_msg void OnBnClickedLineExtSetpointOffsetInfo();
	afx_msg void OnBnClickedLineExtSetpointActSetpointInfo();

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};
