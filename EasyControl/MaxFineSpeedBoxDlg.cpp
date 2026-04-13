//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module MaxFineSpeedBoxDlg
///
/// @file   MaxFineSpeedBoxDlg.cpp
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
#include "MaxFineSpeedBoxDlg.h"



//***********************************************************************************************
//***********************************************************************************************
void CMaxFineSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption)
{
	if (!g_SpeedBoxDlg)
	{
		g_SpeedBoxDlg = new CMaxFineSpeedBoxDlg;
		g_SpeedBoxDlg->Create(pWnd, id, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CMaxFineSpeedBoxDlg::CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CreateSpeedBox(pWnd, id, top, szCaption);
}
//***********************************************************************************************
//***********************************************************************************************
void CMaxFineSpeedBoxDlg::OnNewValue(const float32_t _value)
{
	float32_t value = RANGE(_value, 0.0f, 100.0f);
	REMOTEREF.setDoseBatchFineStream(m_id, value);
}
//***********************************************************************************************
//***********************************************************************************************
float32_t  CMaxFineSpeedBoxDlg::GetStartValue(void) const
{
	float32_t value = 0.0F;
	REMOTEREF.getDoseBatchFineStream(m_id, value);
	return value;
}


