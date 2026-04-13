//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module HelpInfoDlg
///
/// @file   HelpInfoDlg.h
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

#include "EasyControl.h"
#include "InfoBoxCtrl.h"
#include "ModelessDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CHelpInfoDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CHelpInfoDlg)

	enum { IDD = IDD_HELPINFO };

public:
	static CHelpInfoDlg* g_HelpInfoDlg;

private:
	CInfoBoxCtrl m_wndHelpInfoDlg;
	CWnd* m_pParent;

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;


public:
	CHelpInfoDlg();

	~CHelpInfoDlg() override
	{	g_HelpInfoDlg = NULL;	}

	void Create(CWnd*pParent, const CPoint& top, const CString& szCaption, const CString& szMessage);

	static void CreateInfo(CWnd*pWnd,  const CPoint& top, const CString& szCaption, const CString& szMessage = CString(""));
	static void CreateInfo(CWnd* pWnd, const CPoint& top, const UINT caption, const UINT id = _U32(-1));
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};

