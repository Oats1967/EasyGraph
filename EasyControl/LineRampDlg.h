//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineRampDlg
///
/// @file   LineRampDlg.h
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

#include "InfoButton.h"
#include <map>
#include <functional>
#include "BASE/Base.h"


class CLineRampDlg :  public CDialog
{
	DECLARE_DYNAMIC(CLineRampDlg)

	enum { IDD = IDD_LINE_RAMP };

private:
	const CBrush		c_EditBrush;
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CInfoButton m_RampStepInfoButton;
	CInfoButton m_RampDelayInfoButton;

	uint32_t  m_RampStep;
	uint32_t  m_RampDelay;

	struct
	{
		uint32_t  RampStep = 0;
		uint32_t  RampDelay = 0;
	} backupParam;


private:
	BOOL OnNotifyEditRampStep();
	BOOL OnNotifyEditRampDelay();
	void SaveParams();
	void RestoreParams();

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void OnCancel() override;

public:
	CLineRampDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineRampDlg() override = default;

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
	afx_msg void OnStnClickedRampStep();
	afx_msg void OnStnClickedRampDelay();

	afx_msg void OnBnClickedLineRampStepInfo();
	afx_msg void OnBnClickedLineRampDelayInfo();

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
