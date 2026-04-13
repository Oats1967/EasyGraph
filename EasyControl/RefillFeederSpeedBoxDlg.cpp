//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module RefillFeederSpeedBoxDlg
///
/// @file   RefillFeederSpeedBoxDlg.cpp
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
#include "RefillFeederSpeedBoxDlg.h"


//***********************************************************************************************
//***********************************************************************************************
void CRefillFeederSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if (!g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CRefillFeederSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillFeederSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CRefillFeederSpeedBoxDlg::OnNewValue(const float32_t rValue)
{
	float32_t value = RANGE(rValue, 0.0f, 100.0f);
	REMOTEREF.setDoseRefillFeederSetpoint(m_id, value);
}
//***********************************************************************************************
//***********************************************************************************************
float32_t  CRefillFeederSpeedBoxDlg::GetStartValue(void) const
{
	float32_t value = 0.0F;
	REMOTEREF.getDoseRefillFeederSetpoint(m_id, value);
	return value;
}


