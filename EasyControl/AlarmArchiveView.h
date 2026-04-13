//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlarmArchiveView
///
/// @file   AlarmArchiveView.h
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


#include <array>
#include <afxcview.h>
#include "AlarmArchiveDoc.h"



class CAlarmArchiveView : public CListView
{
private:
	const std::array<int32_t, base::eAlarmClassMax> c_ImageArray;
	CImageList m_aImageList;

private:
	//------------------------------------------------------------------------------------
	///  @brief   SetValue
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetValue (void);
	//------------------------------------------------------------------------------------
	///  @brief   IsToday
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL IsToday  (void) const;

	void LoadLogItemList(const time_t newtime);

protected:
	DECLARE_DYNCREATE(CAlarmArchiveView)
	//------------------------------------------------------------------------------------
	///  @brief   OnInitialUpdate
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void OnInitialUpdate() override;
	void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	//------------------------------------------------------------------------------------
	///  @brief   OnUpdate
	///
	///           This method ....
	///
	///  @param[in] pHint , CObject*
	///  @param[in] lHint , LPARAM
	///  @param[in] pSender , CView*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;

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
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;;
#endif

// Operations
public:
	CAlarmArchiveView();           // protected constructor used by dynamic creation
	~CAlarmArchiveView() override = default;;

	//------------------------------------------------------------------------------------
	///  @brief   OnNewLogItem
	///
	///           This method ....
	///
	///  @param[in] lParam , LPARAM
	///  @param[in] wParam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	afx_msg LRESULT OnNewLogItem(WPARAM, LPARAM);

	afx_msg LRESULT OnLogTime(WPARAM wParam, LPARAM lParam);

	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	afx_msg int32_t OnCreate(LPCREATESTRUCT lpCreateStruct);


	DECLARE_MESSAGE_MAP()
};


inline void CAlarmArchiveView::LoadLogItemList(const time_t newtime)
{
	auto* pDoc = (CAlarmArchiveDoc*)GetDocument();
	ASSERT(pDoc);
	pDoc->LoadLogItemList(newtime);
}

/////////////////////////////////////////////////////////////////////////////


