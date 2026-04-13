//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RecView.h
///
/// @file   RecView.h
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

/////////////////////////////////////////////////////////////////////////////
// CRecView view
#include <afxcview.h>
#include "DiagramDoc.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/include/DiagramConfig.h"


class CRecView : public CListView
{
private:
    CImageList  m_aImageList;
    CString     m_szNr;
    CString     m_szTime;
    CString     m_szFeeder;
    CString     m_szMassflow;
    CString     m_szSetpoint;
    CString     m_szTotalizer;
    CString     m_szDriveCommand;
    CString     m_szWeight;
    CString     m_szDosePerformance;
    CString     m_szRotationalSpeed;

private:
    void BuildDate(const base::utils::CRecItemList& aR);
    void BuildItem(const base::MassflowBitfield& iSelect, const base::utils::CRecItemList& aR);
    void BuildGrid (void);
    void FormatString(const float f, CString& szTemp);

    void Draw(void);
    void DrawDate(void);
    void Init(void);
    void InitAll(void);
    void InitItem(const base::MassflowBitfield& iSelect);

    base::CDiagramConfig& GetDiagramConfig();
    void LoadRecItemList(const time_t newtime);

    BOOL IsValidTimeInterval(base::eTimeMode timemode) const;

protected:
	DECLARE_DYNCREATE(CRecView)

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

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext & dc) const;
#endif
    void OnActivateView(BOOL bActivate, CView * pActivateView, CView * pDeactiveView) override;
    void OnInitialUpdate() override;



    //------------------------------------------------------------------------------------
    ///  @brief   OnCreate
    ///
    ///           The framework calls this member function when the window is first created
    ///
    ///  @author  Detlef Hafer
    ///
    ///  @class        CRecView
    ///
    ///  @param[in] lpCreateStruct , LPCREATESTRUCT
    ///  @return int32_t
    ///
    //------------------------------------------------------------------------------------
    virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);


// Operations
public:
	CRecView();           // protected constructor used by dynamic creation
    ~CRecView() override = default;

	CDiagramDoc* GetDocument();

    LRESULT OnNewRecItem(WPARAM, LPARAM);
    LRESULT OnDiagramSelect(WPARAM wParam, LPARAM lParam);
    LRESULT OnDiagramBarDoseItem(WPARAM, LPARAM);
    LRESULT OnDiagramBarTime(WPARAM, LPARAM);
    LRESULT OnDiagramBarDiagramType(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline CDiagramDoc* CRecView::GetDocument()
   { return (CDiagramDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
inline base::CDiagramConfig& CRecView::GetDiagramConfig()
{
    auto* pDoc = (CDiagramDoc*)GetDocument();
    ASSERT(pDoc);
    return pDoc->GetDiagramConfig();
}
//***********************************************************************************************
//***********************************************************************************************
inline void CRecView::LoadRecItemList(const time_t newtime)
{
    auto* pDoc = (CDiagramDoc*)GetDocument();
    ASSERT(pDoc);
    pDoc->LoadRecItemList(newtime);
}
//***********************************************************************************************
//***********************************************************************************************
inline BOOL CRecView::IsValidTimeInterval(base::eTimeMode timemode) const
{
    return ( timemode == base::eTimeMode::TG_DATE ) ||
           ( timemode == base::eTimeMode::TG_TODAY );
}

