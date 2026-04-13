//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillGravBoxDlg
///
/// @file   RefillGravBoxDlg.h
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
#include <functional>
#include "EasyControl.h"
#include "InfoButton.h"
#include "ButtonStartStop.h"
#include "ButtonEmptyFeederSpeed.h"
#include "ButtonRefillFeeder.h"
#include "ModelessDlg.h"


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CRefillGravBoxDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CRefillGravBoxDlg)

	enum { IDD = IDD_REFILLGRAV };


private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	const CBrush     c_EditBrush;
	CInfoButton		m_MinInfoButton;
	CInfoButton		m_MaxInfoButton;
	CInfoButton		m_MinMinInfoButton;
	CInfoButton		m_RefillTimeInfoButton;
	CInfoButton		m_RefillSwitchDelayInfoButton;
	CInfoButton		m_RefillStartInfoButton;
	CInfoButton		m_EmptyFeederInfoButton;

	CButtonStartStop	 m_RefillStartBt;
	CButtonStartStop	 m_EmptyFeederStartBt;
	CButtonEmptyFeederSpeed m_EmptyFeederSpeedBt;
	CButtonRefillFeeder	m_RefillFeederSpeedBt;

	int32_t			m_sItem;
	float32_t       m_fActWeight;
	float32_t	    m_fMin;
	float32_t	    m_fMax;
	float32_t	    m_fMinMin;
	uint32_t        m_lRefillSwitchDelay;
	uint32_t	    m_lRefillTime;
	int32_t			m_ShowScale;
	CWnd*			m_pParent;
	CString			m_szCaption;
	BOOL			m_bInit;
	BOOL			m_EmptyFeederSpeedBtEnable;

private:
	void SetControlStyle(void);
	void SetControlStyleScale(void);
	void ShowScaleItems(BOOL);

	BOOL OnNotifyEditRefillMin();
	BOOL OnNotifyEditRefillMax();
	BOOL OnNotifyEditRefillMinMin();
	BOOL OnNotifyEditRefillTime();
	BOOL OnNotifyEditRefillSwitchDelay();
	void SetValue(void);
	void GetValue(void);

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	void OnActivateWnd() override;
	void RemoveBottomControls(std::vector<int32_t>& ids) override;

public:
	CRefillGravBoxDlg();
	~CRefillGravBoxDlg() override
	{	g_RefillGravBoxDlg = NULL;	}

	void Create(CWnd*pParent, const int32_t m_sItem, const CPoint& top, const CString& szCaption);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnStnClickedRefillTime();
	afx_msg void OnBnClickedRefillStart();
	afx_msg void OnBnClickedRefillFeederEmptyStarted();
	afx_msg void OnStnClickedRefillSwitchDelay();
	afx_msg void OnStnClickedRefillMin();
	afx_msg void OnStnClickedRefillMax();
	afx_msg void OnStnClickedRefillMinMin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnClickedRefillMinInfo();
	afx_msg void OnClickedRefillMaxInfo();
	afx_msg void OnClickedRefillMinMinInfo();
	afx_msg void OnClickedRefillTimeInfo();
	afx_msg void OnClickedRefillSwitchDelayInfo();
	afx_msg void OnBnClickedRefillFeederEmptyStartedInfo();
	afx_msg void OnBnClickedRefillInfo();
	afx_msg void OnBnClickedRefillFeederEmptyStartedSpeed();
	afx_msg void OnBnClickRefillFeederSpeed();
	afx_msg void OnStnClickedRefillScale();

	LRESULT OnNotifyEdit(WPARAM id, LPARAM l);
	LRESULT OnScaleBoxClosed(WPARAM, LPARAM);
	LRESULT OnSpeedBoxClosed(WPARAM, LPARAM);

	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
	static void DestroyRefillBox(void);

	static CRefillGravBoxDlg* g_RefillGravBoxDlg;

	DECLARE_MESSAGE_MAP()
};

