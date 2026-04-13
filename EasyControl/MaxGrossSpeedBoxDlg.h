//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MaxGrossSpeedBoxDlg
///
/// @file   MaxGrossSpeedBoxDlg.h
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

#include "SpeedBoxDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CMaxGrossSpeedBoxDlg : public CSpeedBoxDlg
{
protected:
	void OnNewValue(const float32_t value) override;
	float32_t GetStartValue(void) const override;

public:
	CMaxGrossSpeedBoxDlg() : CSpeedBoxDlg()
	{}
	~CMaxGrossSpeedBoxDlg() override = default;

	static void CreateSpeedBox(CWnd*pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
};

