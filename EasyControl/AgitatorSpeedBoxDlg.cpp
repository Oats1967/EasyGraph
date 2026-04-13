//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AgitatorSpeedBoxDlg
///
/// @file   AgitatorSpeedBoxDlg.cpp
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
#include "AgitatorSpeedBoxDlg.h"



//***********************************************************************************************
//***********************************************************************************************
void CAgitatorSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if (!g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CAgitatorSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CAgitatorSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CAgitatorSpeedBoxDlg::OnNewValue(const float32_t value)
{
	float32_t agitatorValue = RANGE(value, 0.0f, 100.0f);
	REMOTEREF.setDoseAgitatorSetpoint(m_id, agitatorValue);
}
//***********************************************************************************************
//***********************************************************************************************
float32_t  CAgitatorSpeedBoxDlg::GetStartValue(void) const
{
	float32_t agitatorValue = 0.0F;
	REMOTEREF.getDoseAgitatorSetpoint(m_id, agitatorValue);
	return agitatorValue;
}


