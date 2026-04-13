//------------------------------------------------------------------------------------
///
/// @brief  Declaration of module LogSave
///
/// @file   LogSave.h
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

#include "EasyControl.h"


template <class T>
class CLogSave
{
	T m_class;

public:
	CLogSave() : m_class()
	{}

	const T& Get() const
	{	return m_class;	}

	T& Get()
	{	return m_class;	}

	BOOL Save(const CString& szFile)
	{
		auto dwAttrs = GetFileAttributes(szFile);
		VERIFY(SetFileAttributes(szFile, dwAttrs & ~FILE_ATTRIBUTE_READONLY));
		auto result = m_class.Save(toStdString(szFile));
		ASSERT(result);
		VERIFY(SetFileAttributes(szFile, dwAttrs | FILE_ATTRIBUTE_READONLY));
		return result;
	}
};
