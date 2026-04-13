//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LCStatus
///
/// @file   LCStatus.cpp
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
#include "RemoteControl.h"
#include "LCStatus.h"


static CLCStatus g_LCStatus;


CLCStatus& GetLCStatus()
{
	return g_LCStatus;
}


//**************************************************************************************************
//**************************************************************************************************
void CLCStatus::GetLCStatus()
{
	uint32_t count;
	REMOTEREF.getMaxDoseCount(count);
	ASSERT(count <= GLOBALDOSEMAXCOUNT);

	const uint32_t c_LClength = _U32(m_srcLC.length());
	//ASSERT(c_LClength >= count);

	if (c_LClength < count)
	{
		count = c_LClength;
	}

	m_bLConline = FALSE;
	m_LConline = _T("");
	CString temp;
	for (uint32_t index = 0; index < count; index++)
	{
		if (m_srcLC[index] == '0')
		{
			temp.Format(_T("%d "), index + 1);
			m_bLConline = TRUE;
		}
		else
		{
			temp.Format(_T("%c "), m_srcLC[index]);
		}
		m_LConline += temp;
	}

	m_LCoffline = _T("");
	m_bLCoffline = FALSE;
	for (uint32_t index = GLOBALDOSEMAXCOUNT; index < c_LClength; index++)
	{
		if (m_srcLC[index] == '0')
		{
			temp.Format(_T("%d "), (index % 8) + 1);
			m_bLCoffline = TRUE;
		}
		else
		{
			temp.Format(_T("%c "), m_srcLC[index]);
		}
		m_LCoffline += temp;
	}
}


//**************************************************************************************************
//**************************************************************************************************
void CLCStatus::UpdateLCStatus()
{
	std::string srcLC;
	REMOTEREF.getDoseLCStatus(srcLC);

	if (m_srcLC != srcLC)
	{
		m_srcLC = srcLC;
		GetLCStatus();
	}
}









