//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EmptyFeederSpeedBoxDlg
///
/// @file   EmptyFeederSpeedBoxDlg.cpp
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
#include "EasyControl.h"
#include "EmptyFeederSpeedBoxDlg.h"



//***********************************************************************************************
//***********************************************************************************************
void CEmptyFeederSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if (!g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CEmptyFeederSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CEmptyFeederSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CEmptyFeederSpeedBoxDlg::OnNewValue(const float32_t _value)
{
	float32_t value = RANGE(_value, 0.0f, 100.0f);
	REMOTEREF.setDoseEmptyFeederSpeed(m_id, value);
}
//***********************************************************************************************
//***********************************************************************************************
float32_t  CEmptyFeederSpeedBoxDlg::GetStartValue(void) const
{
	float32_t value = 0.0F;
	REMOTEREF.getDoseEmptyFeederSpeed(m_id, value);
	return value;
}


