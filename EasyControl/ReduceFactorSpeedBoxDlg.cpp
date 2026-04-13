//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module ReduceFactorSpeedBoxDlg
///
/// @file   ReduceFactorSpeedBoxDlg.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "ReduceFactorSpeedBoxDlg.h"



//***********************************************************************************************
//***********************************************************************************************
void CReduceFactorSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if (!g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CReduceFactorSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CReduceFactorSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CReduceFactorSpeedBoxDlg::OnNewValue(const float32_t rValue)
{
	float32_t value = RANGE(rValue, 0.0f, 100.0f);
	REMOTEREF.setIfsReduceFactor(m_id, value);
}
//***********************************************************************************************
//***********************************************************************************************
float32_t  CReduceFactorSpeedBoxDlg::GetStartValue(void) const
{
	float32_t value = 0.0F;
	REMOTEREF.getIfsReduceFactor(m_id, value);
	return value;
}


