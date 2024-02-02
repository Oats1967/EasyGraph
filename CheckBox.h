

#pragma once
#include <string>

using namespace std;


class CCheckBoxProp : public CMFCPropertyGridProperty
{
public:
	CCheckBoxProp(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
	{
		m_bGroup = TRUE;
	}

	virtual ~CCheckBoxProp() {}

	virtual BOOL HasButton() const
	{	return TRUE; 	}
};
