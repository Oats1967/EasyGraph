//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IfsSettingBoxDlg
///
/// @file   IfsSettingBoxDlg.h
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
#include "ModelessDlg.h"


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CIfsSettingBoxDlg : public CModelessDlg

{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CIfsSettingBoxDlg)

	enum { IDD = IDD_IFS_SETTING };


private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;
	const CBrush     c_EditBrush;
	CInfoButton		 m_MaxDebounceInfoButton;
	CInfoButton		 m_MinDebounceInfoButton;

	CWnd*			m_pParent;
	CString			m_szCaption;
	BOOL			m_bInit;

	int32_t			m_sItem;
	uint32_t 	    m_MaxDebounce;
	uint32_t 	    m_MinDebounce;

private:
	void SetControlStyle(void);
	BOOL OnNotifyEditDebounceMax();
	BOOL OnNotifyEditDebounceMin();

	void SetValue(void);
	void GetValue(void);

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	void ShowHeader(const CString&);

public:
	CIfsSettingBoxDlg(void);
	~CIfsSettingBoxDlg() override
	{	g_IfsSettingBoxDlg = NULL;	}

	void Create(CWnd*pParent, const int32_t m_sItem, const CPoint& top, const CString& szCaption);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnStnDebounceMax();
	afx_msg void OnStnDebounceMin();

	afx_msg void OnBnClickedDebounceMaxInfo();
	afx_msg void OnBnClickedDebounceMinInfo();

	LRESULT OnNotifyEdit(WPARAM id, LPARAM l);

	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateRefillBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
	static void DestroyRefillBox(void);

	static CIfsSettingBoxDlg* g_IfsSettingBoxDlg;

	DECLARE_MESSAGE_MAP()
};

