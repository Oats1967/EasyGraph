//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramDoc.h
///
/// @file   DiagramDoc.h
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

#include "BASE/Utils/public/RecItemList.h"
#include "WrapperBase.h"


class CDiagramDoc : public CDocument
{
	DECLARE_DYNCREATE(CDiagramDoc)

private:
    base::utils::CRecItemList   m_aRecItemList;
    base::CDiagramConfig&       m_rDiagramConfig;


protected:
    BOOL OnNewDocument() override;
    void Serialize(CArchive& ar) override;

    // Operations
public:
    void LoadRecItemList(const time_t newtime);
    const base::utils::CRecItemList& GetRecItemList(void) const;
	base::eMassflowSelect GetSingleSelection() const;
	void SetSingleSelection(const base::eMassflowSelect iSelect);
	void ClearSelection();
	uint32_t GetSelectionCount() const;
	base::eSelectionType GetSelectionType() const;

    const  base::CDiagramConfig& GetDiagramConfig() const;
    base::CDiagramConfig& GetDiagramConfig();

public:
    CDiagramDoc() : m_rDiagramConfig{ DIAGRAMCONFIGREF }
    {}
    ~CDiagramDoc() override = default;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
inline const base::CDiagramConfig& CDiagramDoc::GetDiagramConfig() const
{ return m_rDiagramConfig; }

/////////////////////////////////////////////////////////////////////////////
inline base::CDiagramConfig& CDiagramDoc::GetDiagramConfig()
{
    return m_rDiagramConfig;
}
//-----------------------------------------------------------------------------------------
inline  const base::utils::CRecItemList& CDiagramDoc::GetRecItemList(void) const
{
    return (m_rDiagramConfig.GetTimeInterval() == base::eTimeMode::TG_TODAY) ? RECORDERREF : m_aRecItemList;
}

//***********************************************************************************************
//***********************************************************************************************
inline base::eMassflowSelect CDiagramDoc::GetSingleSelection() const
{
	return m_rDiagramConfig.GetSingleSelection();
}
//***********************************************************************************************
//***********************************************************************************************
inline void CDiagramDoc::SetSingleSelection(const base::eMassflowSelect iSelect)
{
	m_rDiagramConfig.SetSingleSelection(iSelect);
}
//***********************************************************************************************
//***********************************************************************************************
inline void CDiagramDoc::ClearSelection()
{
    m_rDiagramConfig.ClearSelection();
}

//***********************************************************************************************
//***********************************************************************************************
inline uint32_t CDiagramDoc::GetSelectionCount() const
{
    return m_rDiagramConfig.GetSelectionCount();
}
//***********************************************************************************************
//***********************************************************************************************
inline base::eSelectionType CDiagramDoc::GetSelectionType() const
{
    return m_rDiagramConfig.GetSelectionType();
}

 


