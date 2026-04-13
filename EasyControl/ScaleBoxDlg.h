//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ScaleBoxDlg
///
/// @file   ScaleBoxDlg.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <map>
#include <array>
#include <vector>
#include <utility>
#include <functional>
#include "EasyControl.h"
#include "InfoButton.h"
#include "ButtonStartStop.h"
#include "BASE/include/LCSubTypeMap.h"
#include "ModelessDlg.h"



//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CScaleBoxDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CScaleBoxDlg)

	enum { IDD = IDD_SCALEBOX };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	const std::array<std::pair<COLORREF, COLORREF>, _S32(base::eGatefilter::GATEFILTER_MAX)> c_GateFilterState;
	const CBrush				c_EditBrush;
	CBrush						m_aBrush[_S32(base::eGatefilter::GATEFILTER_MAX)];
	CInfoButton					m_CorrectionFactorInfoButton;
	CInfoButton					m_NoiseInfoButton;
	CButtonStartStop			m_TareBt;
	const base::CLCSubTypeMap	c_LCSubTypeMap;
	CWnd*						m_pParent;
	BOOL						m_bInit;

private:
	float32_t			m_fRawWeight;
	float32_t			m_fWaagenbereich;
	float32_t			m_fNetWeight;
	float32_t			m_fCorrectionFactor;
	float32_t			m_fNoise;
	float32_t			m_fTaraWeight;
	BOOL				m_bAuthorized;
	BOOL				m_bLCClr;
	int32_t 			m_ShowScale;
	base::eLcSubType	m_eScaleSubType;
	base::eGatefilter   m_GateFilterState;
	int32_t				m_sItem;

private:
	void SetControlStyleScale(void);
	void ShowScaleItems(BOOL bShow);
	BOOL OnNotifyEditTaraWeight();
	BOOL OnNotifyEditCorrectionFactor();
	void LoadCellAuthorize();
	void LoadCellClrAddress();
	void LoadCellSetAddress();
	void SetValue(void);
	void GetValue(void);


protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	void RemoveBottomControls(std::vector<int32_t>& ids) override;

public:
	CScaleBoxDlg();
	virtual ~CScaleBoxDlg() = default;

	void Create(CWnd*pParent, const int32_t index, const CPoint& top);

	afx_msg void OnTimer(UINT_PTR nIDEvent);


	void OnAfterExpand() override;

	afx_msg void OnStnClickedScaleBoxTaragewicht();
	afx_msg void OnStnClickedScaleBoxKorrekturfaktor();
	afx_msg void OnBnClickedScaleBoxSensoradresseClr();
	afx_msg void OnBnClickedScaleBoxSensoradresseSet();
	afx_msg void OnBnClickedScaleBoxAuthorize();
	afx_msg void OnBnClickedScaleBoxTare();
	afx_msg void OnClickedScaleBoxCorrectionFactorInfo();
	afx_msg void OnClickedScaleBoxNoiseInfo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	LRESULT OnNotifyEdit(WPARAM id, LPARAM l);

	DECLARE_MESSAGE_MAP()
};

