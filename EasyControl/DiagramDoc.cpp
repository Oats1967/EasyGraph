//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramDoc.cpp
///
/// @file   DiagramDoc.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "DiagramDoc.h"
#include "RemoteControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramDoc


IMPLEMENT_DYNCREATE(CDiagramDoc, CDocument)

BEGIN_MESSAGE_MAP(CDiagramDoc, CDocument)
	//{{AFX_MSG_MAP(CDiagramDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagramDoc construction/destruction
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CDiagramDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CString szTitle;
	VERIFY(szTitle.LoadString(IDS_TITLEDIAGRAM));
	SetTitle(szTitle);
	m_rDiagramConfig.SetID(-1);
	m_rDiagramConfig.SetSingleSelection(base::eMassflowSelect::eVIEWMASSFLOW);
	m_rDiagramConfig.SetDiagramType(base::eDiagramType::eUnknown);

    return TRUE;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CDiagramDoc :: LoadRecItemList(const time_t newtime)
{
    m_aRecItemList.SetPath(RECORDERCONFIGREF.m_RecorderPath);
    m_aRecItemList.SetFilename(newtime);
    auto result = m_aRecItemList.LoadAll();
    if (!result)
    {
        m_aRecItemList = base::utils::CRecItemList();
    }
}
//***********************************************************************************************************
//***********************************************************************************************************



/////////////////////////////////////////////////////////////////////////////
// CDiagramDoc serialization

void CDiagramDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDiagramDoc diagnostics

#ifdef _DEBUG
void CDiagramDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDiagramDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG






