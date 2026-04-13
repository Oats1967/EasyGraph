//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineSetpointLocalDlg
///
/// @file   LineSetpointLocalDlg.h
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

class CLineSetpointLocalDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineSetpointLocalDlg)

	enum { IDD = IDD_LINE_LOCALSETPOINT };

private:
	const CBrush		c_EditBrush;
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CInfoButton m_MaxSetpointInfoButton;
	float32_t m_MaxSetpoint;

	struct
	{
		float32_t MaxSetpoint = 0.0F;
	} backupParam;

private:
	BOOL OnNotifyEditMaxSetpoint();
	void SaveParams();
	void RestoreParams();

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void OnCancel() override;

public:
	CLineSetpointLocalDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineSetpointLocalDlg() override = default;


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

	//***********************************************************************************************
	//***********************************************************************************************
	afx_msg void OnStnClickedLineSetpointMaxSetpoint();
	afx_msg void OnBnClickedLineSetpointMaxSetpointInfo();


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
