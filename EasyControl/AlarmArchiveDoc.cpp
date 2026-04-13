//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmArchiveDoc.cpp
///
/// @file   AlarmArchiveDoc.cpp
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
#include "AlarmArchiveDoc.h"


// CAlarmArchiveDoc

IMPLEMENT_DYNCREATE(CAlarmArchiveDoc, CDocument)

CAlarmArchiveDoc::CAlarmArchiveDoc(): 
	m_Zeitraster{ base::eTimeMode::TG_DATE }
	, m_ID{ -1 }
{
}

//------------------------------------------------------------------------------------
///  @brief   OnNewDocument
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAlarmArchiveDoc
///
///  @param[in] none
///  @return BOOL
///
//------------------------------------------------------------------------------------
BOOL CAlarmArchiveDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Zeitraster = base::eTimeMode::TG_DATE;
	m_ID = -1;

	CString szTitle;
	szTitle.LoadString(IDS_TITLEARCHIVE);
	SetTitle(szTitle);

	return TRUE;
}

CAlarmArchiveDoc::~CAlarmArchiveDoc()
{
}


BEGIN_MESSAGE_MAP(CAlarmArchiveDoc, CDocument)
END_MESSAGE_MAP()


//***********************************************************************************************************
//***********************************************************************************************************
void CAlarmArchiveDoc::LoadLogItemList(const time_t newtime)
{
	m_aLogItemList.SetPath(EASYCONTROLREF.m_AlarmLogPath);
	m_aLogItemList.SetFilename(newtime);
	auto result = m_aLogItemList.LoadAll();
	if (!result)
	{
		m_aLogItemList = base::utils::CLogItemList();
	}
}

#ifdef _DEBUG
//------------------------------------------------------------------------------------
///  @brief   AssertValid
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAlarmArchiveDoc
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
void CAlarmArchiveDoc::AssertValid() const
{
	CDocument::AssertValid();
}

//------------------------------------------------------------------------------------
///  @brief   Dump
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAlarmArchiveDoc
///
///  @param[in] dc , CDumpContext&
///  @return none
///
//------------------------------------------------------------------------------------
void CAlarmArchiveDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAlarmArchiveDoc-Serialisierung

//------------------------------------------------------------------------------------
///  @brief   Serialize
///
///           This method ....
///
///  @author  Detlef Hafer
///
///  @class        CAlarmArchiveDoc
///
///  @param[in] ar , CArchive&
///  @return none
///
//------------------------------------------------------------------------------------
void CAlarmArchiveDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: Hier Code zum Speichern einfügen
	}
	else
	{
		// TODO: Hier Code zum Laden einfügen
	}
}

void CAlarmArchiveDoc::SetTitle(const CString& szTitle)
{
	if (szTitle.IsEmpty())
	{
		CString szTemp;
		VERIFY(szTemp.LoadString(IDS_TITLEARCHIVE));
		CDocument::SetTitle(szTemp);
	}
	else
	{
		CDocument::SetTitle(szTitle);
	}
}




