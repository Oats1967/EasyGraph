//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module ToolTipDlg
///
/// @file   ToolTipDlg.h
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
class CToolTipDlg : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CToolTipDlg)

	enum { IDD = IDD_TOOLTIP };

public:
	static CToolTipDlg* g_ToolTipDlg;

private:
	CInfoBoxCtrl m_wndToolTipDlg;

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

public:
	CToolTipDlg();
	~CToolTipDlg() override
	{	g_ToolTipDlg = NULL;	}

	void Create(CWnd*pParent, const CPoint& top, const CString& szCaption);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static void CreateInfo(CWnd*pWnd,  const CPoint& top, const CString& szCaption);
	static void CreateInfo(CWnd* pWnd, const CPoint& top, const UINT caption);
	static void DestroyInfo();

	DECLARE_MESSAGE_MAP()
};

