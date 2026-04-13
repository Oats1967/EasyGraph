//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramFrame.h
///
/// @file   DiagramFrame.h
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


#include "BASE/include/DiagramConfig.h"
#include "DiagramDoc.h"

class CDiagramFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CDiagramFrame)



private:
    CView *             m_pDiagramView;
    CView *             m_pRecView;

private:
    base::eDiagramType GetDiagramType(void);

    void AddViewToDocument(CView* pOldView, CView* pNewView);
    CView* CreateView(base::eDiagramType diagramtype);

    base::CDiagramConfig& CDiagramFrame::GetDiagramConfig();

protected:
	CDiagramFrame();           // Dynamische Erstellung verwendet geschützten Konstruktor
    ~CDiagramFrame() override = default;

    //------------------------------------------------------------------------------------
    ///  @brief   ActivateFrame
    ///
    ///           This method ....
    ///
    ///  @param[in] nCmdShow , int
    ///  @return none
    ///
    //------------------------------------------------------------------------------------
    void ActivateFrame(int nCmdShow = -1) override;

protected:
	DECLARE_MESSAGE_MAP()
// Überschreibungen

    //------------------------------------------------------------------------------------
    ///  @brief   PreCreateWindow
    ///
    ///           This method is called by the framework before the creation of the Windows 
    ///		      window´attached to this CWnd object
    ///
    ///  @param[in] cs, CREATESTRUCT& 
    ///  @return BOOL
    ///
    //------------------------------------------------------------------------------------
    BOOL PreCreateWindow(CREATESTRUCT& cs) override;

    //------------------------------------------------------------------------------------
    ///  @brief   OnDiagramFrameTable
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return none
    ///
    //------------------------------------------------------------------------------------
    afx_msg void OnDiagramFrameTable();
    afx_msg void OnDiagramFrameTableUpdateUI(CCmdUI* pCmdUI);

    //------------------------------------------------------------------------------------
    ///  @brief   OnDiagramFrameTableGraphic
    ///
    ///           This method ....
    ///
    ///  @param[in] none
    ///  @return none
    ///
    //------------------------------------------------------------------------------------
    afx_msg void OnDiagramFrameGraphic();
    afx_msg void OnDiagramFrameGraphicUpdateUI(CCmdUI* pCmdUI);
    afx_msg void OnDiagramFrame3DLine();
    afx_msg void OnDiagramFrame3DLineUpdateUI(CCmdUI* pCmdUI);

    afx_msg void OnDiagramFrameTimeSelectMenu();
    afx_msg void OnDiagramFrameTimeSelectMenuUpdateUI(CCmdUI* pCmdUI);
    afx_msg void OnDiagramFrameTimeSelect(UINT);
    afx_msg void OnDiagramFrameTimeSelectUpdateUI(CCmdUI* pCmdUI);

    afx_msg void  OnDiagramFrameScaleSelectMenu();
    afx_msg void  OnDiagramFrameScaleSelectMenuUpdateUI(CCmdUI* pCmdUI);


    afx_msg void OnDiagramFrameDoseItemMenu(); 
    afx_msg void OnDiagramFrameDoseItemMenuUpdateUI(CCmdUI * pCmdUI);
    afx_msg void OnDiagramFrameDoseItem(UINT id);
    afx_msg void OnDiagramFrameDoseItemUpdateUI(CCmdUI * pCmdUI);

    afx_msg void OnDiagramFrameDiagramSelect(UINT id);

    afx_msg void OnDiagramFrameDiagramSelectUpdateUI(CCmdUI* pCmdUI);
    afx_msg void OnDiagramFrameDiagramSelectMenu();
    afx_msg void OnDiagramFrameDiagramSelectMenuUpdateUI(CCmdUI* pCmdUI);


public:

    //------------------------------------------------------------------------------------
    ///  @brief   OnCreate
    ///
    ///           The framework calls this member function when the window is first created
    ///
    ///  @param[in] lpCreateStruct , LPCREATESTRUCT
    ///  @return int
    ///
    //------------------------------------------------------------------------------------
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    //------------------------------------------------------------------------------------
    ///  @brief   OnDestroy
    ///
    ///           This method is called by the framework before the window is destroyed
    ///
    ///  @param[in] none
    ///  @return none
    ///
    //------------------------------------------------------------------------------------
    afx_msg void OnDestroy();
 };


 /////////////////////////////////////////////////////////////////////////////
 inline base::CDiagramConfig& CDiagramFrame::GetDiagramConfig()
 {
     CView* pActiveView = GetActiveView();
     ASSERT(pActiveView);
     CDiagramDoc* pDoc = (CDiagramDoc*)pActiveView->GetDocument();
     ASSERT(pDoc);
     return pDoc->GetDiagramConfig();
 }

//*******************************************************************************
//*******************************************************************************
inline base:: eDiagramType CDiagramFrame::GetDiagramType(void)
{
    return GetDiagramConfig().GetDiagramType();
}