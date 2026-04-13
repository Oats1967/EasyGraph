//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabOverview
///
/// @file   LineTabOverview.h
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
#include <map>
#include <functional>
#include "EasyControl.h"
#include "OverviewGaugeCtrl.h"
#include "ButtonStartStop.h"
#include "ButtonSwitchCtrl.h"
#include "ButtonRamp.h"


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CLineTabOverview : public CDialog
{
	DECLARE_DYNAMIC(CLineTabOverview)

	enum { IDD = IDD_LINETAB_OVERVIEW };

private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	const CBrush			c_EditBrush;
	const CBrush			c_GreenBrush;
	const CBrush			c_YellowBrush;
	const CBrush			c_RedBrush;
	CFont					m_aDescriptionFont;
	CFont					m_aStaticFont;
	COverviewGaugeCtrl		m_wndOverviewGauge;
	CRampUpButton			m_aRampUp;
	CRampDownButton			m_aRampDown;
	CButtonStartStop		m_StartBt;
	CBCGPStatic				m_StSetpoint;
	CBCGPStatic				m_StMassflow;
	float32_t				m_fSetpoint;
	float32_t				m_fMassflow;
	float32_t				m_fMaxSetpoint;
	BOOL					m_Redraw;
	BOOL					m_bLineStart;
	BOOL					m_bSetpointEditable;

private:
	void SetValue();
	void GetValue();
	const CBrush& GetMassflowBrush() const;
	BOOL IsSetpointEditable() const;
	BOOL IsRampEnabled() const;
	BOOL IsRampShow() const;
	BOOL IsLineStartEnabled() const;
	BOOL IsLineStartShow() const;

	BOOL OnNotifyEditSetpoint(void);


protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;

public:
	void SetMassflow(const float32_t);
	void SetSetpoint(const float32_t);
	void SetMaxSetpoint(const float32_t);

	CLineTabOverview(CWnd* pParent = NULL);
	~CLineTabOverview() override = default;

	void Redraw();

	afx_msg void OnDblclkLineTabOverviewGauge();
	afx_msg void OnBnClickedLineTabOverviewRampUp();
	afx_msg void OnBnClickedLineTabOverviewRampDown();
	afx_msg void OnClickedLineTabOverviewLineStart();
	afx_msg void OnStnClickedLineTabOverviewSetpoint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	LRESULT OnTimerRefresh(WPARAM, LPARAM);
	LRESULT OnNotifyEdit(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

//------------------------------------------------------------------------------
inline void CLineTabOverview::SetMassflow(const float32_t value)
{
	if (m_fMassflow != value)
	{
		m_fMassflow = value;
		m_Redraw = TRUE;
	}
}
//------------------------------------------------------------------------------
inline void CLineTabOverview::SetSetpoint(const float32_t value)
{
	if (m_fSetpoint != value)
	{
		m_fSetpoint = value;
		m_Redraw = TRUE;
	}
}
//------------------------------------------------------------------------------
inline void CLineTabOverview::SetMaxSetpoint(const float32_t value)
{
	if (m_fMaxSetpoint != value)
	{
		m_fMaxSetpoint = value;
		m_Redraw = TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabOverview::Redraw()
{
	if (IsWindowVisible())
	{
		if (m_Redraw)
		{
			m_Redraw = FALSE;
			SetValue();
		}
	}
}
