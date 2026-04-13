//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramScaleDlg
///
/// @file   DiagramScaleDlg.cpp
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

#include <map>
#include <functional>
#include "EasyControl.h"
#include "BASE/include/DiagramConfig.h"
#include "AutoButton.h"


// CDiagramScaleDlg-Dialog

class CDiagramScaleDlg : public CBCGPDialog
{
	using CBaseClass = CBCGPDialog;

	DECLARE_DYNAMIC(CDiagramScaleDlg)

	enum { IDD = IDD_DIAGRAM_SCALE };

private:
	const CBrush     c_EditBrush;
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;

private:
	base::CAxisScale& r_AxisScale;
	base::CAxisScale m_SaveAxisScale;
	CAutoButton m_AutoSetpoint;
	CAutoButton m_AutoWeight;
	CAutoButton m_AutoDriveCommand;
	CAutoButton m_AutoRotSpeed;
	CAutoButton m_AutoDosePerformance;
	CAutoButton m_AutoMassflow;

private:
	BOOL OnNotifyDosePerformanceMin();
	BOOL OnNotifyDosePerformanceMax();
	BOOL OnNotifySetpointMin();
	BOOL OnNotifySetpointMax();
	BOOL OnNotifyMassflowMin();
	BOOL OnNotifyMassflowMax();
	BOOL OnNotifyWeightMin();
	BOOL OnNotifyWeightMax();
	BOOL OnNotifyDriveCommandMin();
	BOOL OnNotifyDriveCommandMax();
	BOOL OnNotifyRotSpeedMin();
	BOOL OnNotifyRotSpeedMax();
	void SetValue(void);
	void GetValue(void);


	template <int32_t ix>
	BOOL OnNotifyValue(const base::eMassflowSelect sel);
	void ToggleAuto(const base::eMassflowSelect select);


protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	virtual void OnCancel() override;
	virtual BOOL OnInitDialog() override;


public:
	CDiagramScaleDlg(CWnd* pParent, base::CAxisScale& rScale);   // Standardkonstruktor
	virtual ~CDiagramScaleDlg();

// Dialogfelddaten
	SETGET(const base::CAxisScale&, SaveAxisScale);

	afx_msg void OnStnClickedDiagramScaleDosePerformanceMin();
	afx_msg void OnStnClickedDiagramScaleDosePerformanceMax();
	afx_msg void OnStnClickedDiagramScaleSetpointMin();
	afx_msg void OnStnClickedDiagramScaleSetpointMax();
	afx_msg void OnStnClickedDiagramScaleMassflowMin();
	afx_msg void OnStnClickedDiagramScaleMassflowMax();
	afx_msg void OnStnClickedDiagramScaleWeightMin();
	afx_msg void OnStnClickedDiagramScaleWeightMax();
	afx_msg void OnStnClickedDiagramScaleDriveCommandMin();
	afx_msg void OnStnClickedDiagramScaleDriveCommandMax();
	afx_msg void OnStnClickedDiagramScaleRotSpeedMin();
	afx_msg void OnStnClickedDiagramScaleRotSpeedMax();
	afx_msg void OnBnClickedDosePerformanceAuto();
	afx_msg void OnBnClickedSetpointAuto();
	afx_msg void OnBnClickedMassflowAuto();
	afx_msg void OnBnClickedWeightAuto();
	afx_msg void OnBnClickedDriveCommandAuto();
	afx_msg void OnBnClickedRotSpeedAuto();

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnNotifyEdit(WPARAM w, LPARAM);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	DECLARE_MESSAGE_MAP()
};
