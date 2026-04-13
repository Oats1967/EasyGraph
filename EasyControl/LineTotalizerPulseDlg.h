//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTotalizerPulseDlg
///
/// @file   LineTotalizerPulseDlg.h
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

#include "InfoButton.h"
#include <map>
#include <functional>
#include "BASE/Base.h"


class CLineTotalizerPulseDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineTotalizerPulseDlg)

	enum { IDD = IDD_LINE_TOTALIZERIMPULSE };

private:
	const CBrush		c_EditBrush;
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CInfoButton m_StepInfoButton;
	CInfoButton m_ImpulseInfoButton;

	float32_t  m_Step;
	uint32_t   m_Impulse;

	struct
	{
		float32_t  Step = 0.0F;
		uint32_t   Impulse = 0;
	} backupParam;


private:
	BOOL OnNotifyEditStep();
	BOOL OnNotifyEditImpulse();
	void SaveParams();
	void RestoreParams();


protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	BOOL OnInitDialog() override;
	void OnCancel() override;


public:
	CLineTotalizerPulseDlg(CWnd* pParent = nullptr);   // Standardkonstruktor
	~CLineTotalizerPulseDlg() override = default;

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
	LRESULT OnNotifyEdit(WPARAM , LPARAM);

	//***********************************************************************************************
	//***********************************************************************************************
	afx_msg void OnStnClickedStep();
	afx_msg void OnStnClickedImpulse();

	afx_msg void OnBnClickedLineStepInfo();
	afx_msg void OnBnClickedLineImpulseInfo();

	//------------------------------------------------------------------------------------
   ///  @brief   OnCtlColor
   ///
   ///           This method is called by the framework when a child control is about to
   ///           be drawn
   ///
   ///  @param[in] nCtlColor , UINT
   ///  @param[in] pWnd , CWnd*
   ///  @param[in] pDC , CDC*
   ///  @return HBRUSH
   ///
   //------------------------------------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};
