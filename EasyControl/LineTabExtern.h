//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabExtern
///
/// @file   LineTabExtern.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "EasyControl.h"
#include "ButtonSwitchCtrl.h"


// CLineTabExtern-Dialog

class CLineTabExtern : public CDialog
{
	DECLARE_DYNAMIC(CLineTabExtern)

private:
	enum eInputs
	{
		eExtLineStart = 0,
		eExtAlarmShutdown,
		eExtSetpoint,
		eExtRampUp,
		eExtRampDown,
		eExtAlarmQuit0,
		eExtAlarmQuit13,
		eExtMainFeeder,
		eExtSideFeeder1,
		eExtSideFeeder2,
		eExtTemplateAlarm,
		eExtCustomerAlarm,
		eExtAutoOperate,
		eExtManualOperate,
		eExtMax
	};


private:
	CBCGPComboBox		m_wndCombo;
	CImageList			m_aImageList;
	CButtonSwitchCtrl	m_LineReleaseBt;
	int32_t				m_CurSel;
	CArray< CString >	m_ExtName;


private:
	void UpdateImage(int32_t pos, BOOL value);
	void OnUpdateControls();

	
protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;

public:
	enum { IDD = IDD_LINETAB_EXTERN };

public:
	CLineTabExtern(CWnd* pParent = NULL);   // Standardkonstruktor
	~CLineTabExtern() override = default;

	afx_msg void OnCbLineDigitalInput();
	afx_msg void OnSelchangeLinetabExternSignals();
	afx_msg void OnClickedLineTabExternLineRelease();

	LRESULT OnTimerRefresh(WPARAM, LPARAM);
	LRESULT OnGaugeClick(WPARAM wp, LPARAM);


	DECLARE_MESSAGE_MAP()
};
