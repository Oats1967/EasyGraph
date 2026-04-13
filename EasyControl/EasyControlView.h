//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module EasyControlView.h
///
/// @file   EasyControlView.h
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


#include "DataImage.h"
#include "FeederDisplay.h"
#include "LoadLTyp.h"
#include "InfoBox.h"
#include "BitmapDC.h"
#include "BarChart.h"
#include "LineTotalizerBox.h"
#include "BASE/include/EasyControlConfig.h"
#include "BASE/Utils/public/LogItemList.h"
#include "BASE/Utils/public/RecItemList.h"



class CEasyControlView : public CView
{
	DECLARE_DYNCREATE(CEasyControlView)

private:
	CInfoBox					m_aInfoBox;
	CBitmapDC					m_LinieDC;
	CBarChart					m_BarChart;
	CLineTotalizerBox			m_LineTotalizerBox;
	CFont						m_ServiceModeFont;
	CFont						m_LineTotalizerBoxFont;
	CPoint						m_LogoPos;
	CPoint						m_InfoBoxPos;
	CPoint						m_LineBoxPos;
	CPoint						m_ServiceBoxPos;
	CPoint						m_LineTotalizerBoxPos;
	CRect						m_BarChartRect;
	CString						m_szServiceMode;
	base::CCustomerLogoConfig	m_CustomerLogoCfg;

	int32_t					m_DoseItem;
	int32_t					m_sMaxItems;
	int32_t					m_zoomfactor;
	BOOL					m_bAlarm1;
	BOOL					m_bAlarm03;
	uint32_t				m_ButtonAliveCounter;
	BOOL					m_bLineTotalizerBoxEnable;
	BOOL					m_bBarChartEnable;
	BOOL					m_bProductNameEnable;
	BOOL					m_bDrawRefillButtonsEnable;;

private:
	static CRect CalcDisplayRect(const CRect& rRect, const CPoint& cp, int32_t scale);

	void DestroyLayout	(void);
	void LoadLayout		(int32_t n);
	void LoadLayout		(const LOADLTYPBASE& rLoad);
	BOOL SetZoomFactor  (int32_t zoomfactor);

	void CreateBitmapFromScratch(const LOADLTYPBASE& rLoad);
	void DrawLogo(CDC* rdc);
	void DrawCustomerLogo(CDC* pdc);
	void DrawBackground(CBitmapDC& rDC);

	static void InitLine();
	void InitDose(int32_t i, const DOSEPOSITIONSTYP& aD);

	void DrawItems		(CDC* pDC);
	void SelectDosierer (int32_t);
	void ClickedButtonAlive();


	BOOL CheckFeederDisplay		(void);
	BOOL CheckLayout			(void);
	void RecalcLayout			(void);
	void RepositionLayout		(void);

	int32_t CheckClick(const CPoint& point);
	int32_t IsValidClick(const CPoint& point, CDisplayContainer::eDisplayItem&);
	CDoseDisplayItem&  GetDisplayItem(const int32_t index, const CDisplayContainer::eDisplayItem);

	void FlashLightOff(void);
	void AlarmClear(int32_t id);
	void StartDose(const int32_t index);
	void StartDoseRefillRelease(const int32_t index);
	void StartDoseRefill(const int32_t index);

	void CloseAllOpenDialogs();
	void SetNewDoseItem(int32_t item);
	void SetNewInfoMessage();
	void DrawServiceMode(CDC* pDC);
	void DrawLineTotalizerBox(CDC* pDC);
	void DrawBarChart(CDC* pDC);
	void DrawRefillButtons(CDC* pDC);
//	void DrawProductName(CDC* pDC);
	void CreateMessageBox();
	void OnAlarmBits();


protected:
	void OnDraw(CDC* pDC) override;
	void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) override;


// Operationen
public:
	CEasyControlView();
	~CEasyControlView() override = default;
	void OnInitialUpdate() override;

	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC*);
#ifdef __SIM_WEIGHT_GUI
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
#endif
    afx_msg void OnAlarm03Clear();
    afx_msg void OnUpdateAlarm03Clear(CCmdUI *pCmdUI);
	afx_msg void OnAlarm1Clear();
	afx_msg void OnUpdateAlarm1Clear(CCmdUI* pCmdUI);
	afx_msg void OnDosiererNext();
    afx_msg void OnUpdateDosiererNext(CCmdUI *pCmdUI);
    afx_msg void OnDosiererPrevious();
    afx_msg void OnUpdateDosiererPrevious(CCmdUI *pCmdUI);
	afx_msg void OnServiceMode();
	afx_msg void OnUpdateOnServiceMode(CCmdUI* pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnUpdateLineTotalizerReset
	///
	///           This method ....
	///
	///  @param[in]  pCmdUI , CCmdUI *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnLineStatus();
	afx_msg void OnUpdateLineStatus(CCmdUI* pCmdUI);

	//------------------------------------------------------------------------------------
	///  @brief   OnTimerRefresh
	///
	///           This method is called periodically by the framework and updates the 
	///            controls in the user interface.
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnTimerRefresh(WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnDoseEnd
	///
	///           This method is called by the framework when  a dosing unit has lost its 
	///			  focus
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnDoseEnd(WPARAM w, LPARAM);

	LRESULT OnLayoutChanged(WPARAM w, LPARAM);
	LRESULT OnNewLogItem(WPARAM w, LPARAM);
	LRESULT OnUserLogOut(WPARAM, LPARAM);
	LRESULT OnZoomFactor(WPARAM, LPARAM);
	LRESULT OnDoseSubDialogClosed(WPARAM w, LPARAM);
	LRESULT OnDeactivateView(WPARAM, LPARAM);
	LRESULT OnActivateView(WPARAM, LPARAM);
	LRESULT OnInfoBoxAlarmClear(WPARAM, LPARAM);
	LRESULT OnInfoBoxFlashlight(WPARAM, LPARAM);

	CEasyControlDoc* GetDocument() const;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext & dc) const;
#endif

	DECLARE_MESSAGE_MAP()

public:
};

#ifndef _DEBUG  // Debugversion in EasyControlView.cpp
inline CEasyControlDoc* CEasyControlView::GetDocument() const
   { return reinterpret_cast<CEasyControlDoc*>(m_pDocument); }
#endif


