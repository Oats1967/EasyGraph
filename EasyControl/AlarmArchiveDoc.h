//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmArchiveDoc.h
///
/// @file   AlarmArchiveDoc.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once


#include "BASE/Utils/public/LogItemList.h"
#include "BASE/include/DiagramConfig.h"


class CAlarmArchiveDoc : public CDocument
{
	DECLARE_DYNCREATE(CAlarmArchiveDoc)

private:
	base::eTimeMode m_Zeitraster;
	int32_t m_ID;
	base::utils::CLogItemList m_aLogItemList;

protected:
	virtual BOOL OnNewDocument();

public:
	CAlarmArchiveDoc();
	virtual ~CAlarmArchiveDoc();
	virtual void Serialize(CArchive& ar);   // überladen für Dokument-E/A
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void SetTitle(const CString& szTitle);
	void LoadLogItemList(const time_t newtime);
	const base::utils::CLogItemList& GetLogItemList(void) const;

	SETGET(base::eTimeMode, Zeitraster)
	SETGET(int32_t, ID)

	DECLARE_MESSAGE_MAP()
};

//-----------------------------------------------------------------------------------------
inline  const base::utils::CLogItemList& CAlarmArchiveDoc::GetLogItemList(void) const
{
	return (m_Zeitraster == base::eTimeMode::TG_TODAY) ? LOGREF : m_aLogItemList;
}

