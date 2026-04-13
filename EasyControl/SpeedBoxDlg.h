//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SpeedBoxDlg
///
/// @file   SpeedBoxDlg.h
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
#include "SpeedKnob.h"
#include "ModelessDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CSpeedBoxDlg :  public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CSpeedBoxDlg)

	enum { IDD = IDD_SPEEDBOX };

public:
	static CSpeedBoxDlg* g_SpeedBoxDlg;

private:
	CWnd* m_pParent;

protected:
	CSpeedKnob	m_wndKnob;
	CString		m_szCaption;
	BOOL		m_Tracking;
	int32_t		m_id;

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

	virtual void OnNewValue(const float32_t) {}
	virtual float32_t GetStartValue(void) const { return 0.0F; }
	void ShowHeader(const float32_t);

public:
	CSpeedBoxDlg();
	~CSpeedBoxDlg() override
	{	g_SpeedBoxDlg = NULL;	}

	void Create(CWnd*pParent, const int32_t id, const CPoint& top, const CString& szCaption);

	static void CreateSpeedBox(CWnd*pWnd, const int32_t id, const CPoint& top, const CString& szCaption);
	static void CreateSpeedBox(CWnd* pWnd, const int32_t id, const CPoint& top, const UINT caption);
	static void DestroySpeedBox();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	LRESULT OnTrackKnob(WPARAM, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

