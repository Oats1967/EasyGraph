//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RemoteInterface
///
/// @file   RemoteInterface.h
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

#include <string>
#include "EasyControl.h"


class CLCStatus
{
	CString  m_LConline;
	CString  m_LCoffline;
	std::string m_srcLC;
	BOOL	 m_bLCoffline;
	BOOL	 m_bLConline;

private:
	void GetLCStatus();

public:
	CLCStatus() : m_LConline("")
				 , m_LCoffline("")
				 , m_srcLC("")
				 , m_bLCoffline(FALSE)
				 , m_bLConline(FALSE)
	{}

	void UpdateLCStatus();

	BOOL IsLConline(void)  const
	{
		return m_bLConline;
	}

	BOOL IsLCoffline(void)  const
	{
		return m_bLCoffline;
	}

	const CString& GetLConline(void) const
	{
		return m_LConline;
	}
	const CString& GetLCoffline(void) const
	{
		return m_LCoffline;
	}
};


CLCStatus& GetLCStatus();








