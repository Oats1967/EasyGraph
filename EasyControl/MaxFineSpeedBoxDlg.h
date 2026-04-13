//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module MaxFineSpeedBoxDlg
///
/// @file   MaxFineSpeedBoxDlg.h
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
class CMaxFineSpeedBoxDlg : public CSpeedBoxDlg
{
protected:
	void OnNewValue(const float32_t value) override;
	float32_t GetStartValue(void) const override;

public:
	CMaxFineSpeedBoxDlg() : CSpeedBoxDlg()
	{}

	~CMaxFineSpeedBoxDlg() override = default;

	static void CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
};



