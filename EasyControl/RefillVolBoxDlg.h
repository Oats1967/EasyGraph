//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module RefillVolBoxDlg
///
/// @file   RefillVolBoxDlg.h
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
#include "ButtonRefillFeeder.h"
#include "ButtonEmptyFeederSpeed.h"
#include "ModelessDlg.h"





//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CRefillVolBoxDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CRefillVolBoxDlg)

	enum { IDD = IDD_REFILLVOL };


private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	const CBrush     c_EditBrush;
	CInfoButton		 m_EmptyFeederInfoButton;
	CInfoButton		 m_FeederEmptyStartInfoButton;
	CInfoButton		 m_MaxDebounceInfoButton;
	CInfoButton		 m_MinDebounceInfoButton;
	CInfoButton		 m_RefillTimeInfoButton;
	CInfoButton		 m_RefillStartInfoButton;


	CButtonRefillFeeder	m_RefillFeederSpeedBt;
	CButtonStartStop	 m_RefillStartBt;
	CButtonStartStop	 m_EmptyFeederStartBt;
	CButtonEmptyFeederSpeed m_EmptyFeederSpeedBt;



	CWnd*			m_pParent;
	CString			m_szCaption;
	BOOL			m_bInit;

	int32_t			m_sItem;
	uint32_t 	    m_FeederEmptyStart;
	uint32_t 	    m_MaxDebounce;
	uint32_t 	    m_MinDebounce;
	uint32_t 	    m_lRefillTime;

private:
	void SetControlStyle(void);
	BOOL OnNotifyEditFeederEmptyStart();
	BOOL OnNotifyEditRefillTime();
	BOOL OnNotifyEditDebounceMax();
	BOOL OnNotifyEditDebounceMin();

	void SetValue(void);
	void GetValue(void);

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	void OnActivateWnd() override;

public:
	CRefillVolBoxDlg();
	~CRefillVolBoxDlg() override
	{	g_RefillVolBoxDlg = NULL;	}

	void Create(CWnd*pParent, const int32_t m_sItem, const CPoint& top, const CString& szCaption);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnStnClickedFeederEmptyStart();
	afx_msg void OnStnDebounceMax();
	afx_msg void OnStnDebounceMin();
	afx_msg void OnStnClickedRefillTime();


	afx_msg void OnBnClickedFeederEmptyStartInfo();
	afx_msg void OnBnClickedDebounceMaxInfo();
	afx_msg void OnBnClickedDebounceMinInfo();
	afx_msg void OnBnClickedRefillTimeInfo();
	afx_msg void OnBnClickedRefillStartInfo();
	afx_msg void OnBnClickedRefillStart();
	afx_msg void OnBnClickRefillFeederSpeed();
	afx_msg void OnBnClickedRefillFeederEmptyStartedInfo();
	afx_msg void OnBnClickedRefillFeederEmptyStarted();
	afx_msg void OnBnClickedRefillFeederEmptyStartedSpeed();


	LRESULT OnNotifyEdit(WPARAM id, LPARAM l);
	LRESULT OnSpeedBoxClosed(WPARAM, LPARAM);

	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
	static void DestroyRefillBox(void);


	static CRefillVolBoxDlg* g_RefillVolBoxDlg;

	DECLARE_MESSAGE_MAP()
};

