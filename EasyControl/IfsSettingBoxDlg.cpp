//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfsSettingBoxDlg
///
/// @file   IfsSettingBoxDlg.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "IfsSettingBoxDlg.h"
#include "RemoteControl.h"
#include "FormatMetric.h"
#include "Focus.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"




#define TIMERID 1

#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIfsSettingBoxDlg, _func)

CIfsSettingBoxDlg* CIfsSettingBoxDlg:: g_IfsSettingBoxDlg = NULL;

//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const UINT caption)
{
	CString szCaption;
	VERIFY(szCaption.LoadString(caption));
	CIfsSettingBoxDlg::CreateRefillBox(pWnd, index, top, szCaption);
}

//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::CreateRefillBox(CWnd* pWnd, const int32_t index, const CPoint& top, const CString& szCaption)
{
	if (!g_IfsSettingBoxDlg)
	{
		g_IfsSettingBoxDlg = new CIfsSettingBoxDlg;
		g_IfsSettingBoxDlg->Create(pWnd, index, top, szCaption);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::DestroyRefillBox(void)
{
	if (g_IfsSettingBoxDlg && g_IfsSettingBoxDlg->GetSafeHwnd())
	{
		g_IfsSettingBoxDlg->PostMessageW(WM_DESTROY);
	}
}

//***********************************************************************************************
//***********************************************************************************************
// CIfsSettingBoxDlg-Dialogfeld
IMPLEMENT_DYNAMIC(CIfsSettingBoxDlg, CBaseClass)
CIfsSettingBoxDlg::CIfsSettingBoxDlg()
	: CModelessDlg()
	, c_EditMap({
		EDITITEM(IDC_IFS_SETTING_DEBOUNCEMAX_EDIT,	OnNotifyEditDebounceMax),
		EDITITEM(IDC_IFS_SETTING_DEBOUNCEMIN_EDIT,	OnNotifyEditDebounceMin),
		})
	, c_EditBrush{ EDITBKCOLOR }
	, m_pParent{ nullptr }
	, m_szCaption { "" }
	, m_bInit{ FALSE }
	, m_MaxDebounceInfoButton()
	, m_MinDebounceInfoButton()
	, m_sItem{ 0 }
	, m_MaxDebounce{ 0 }
	, m_MinDebounce{ 0 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_IFS_SETTING_DEBOUNCEMAX_EDIT, m_MaxDebounce);
	DDX_Text(pDX, IDC_IFS_SETTING_DEBOUNCEMIN_EDIT, m_MinDebounce);

	DDX_Control(pDX, IDC_IFS_SETTING_DEBOUNCEMAX_INFO, m_MaxDebounceInfoButton);
	DDX_Control(pDX, IDC_IFS_SETTING_DEBOUNCEMIN_INFO, m_MinDebounceInfoButton);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CIfsSettingBoxDlg, CBaseClass)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_IFS_SETTING_DEBOUNCEMAX_EDIT, &CIfsSettingBoxDlg::OnStnDebounceMax)
	ON_STN_CLICKED(IDC_IFS_SETTING_DEBOUNCEMIN_EDIT, &CIfsSettingBoxDlg::OnStnDebounceMin)
	ON_BN_CLICKED(IDC_IFS_SETTING_DEBOUNCEMAX_INFO, &CIfsSettingBoxDlg::OnBnClickedDebounceMaxInfo)
	ON_BN_CLICKED(IDC_IFS_SETTING_DEBOUNCEMIN_INFO, &CIfsSettingBoxDlg::OnBnClickedDebounceMinInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::Create(CWnd* pParent, const int32_t id, const CPoint& top, const CString& szCaption)
{
	m_pParent = pParent;
	m_sItem = id;
	m_szCaption = szCaption;

	auto result = CBaseClass::Create(IDD, pParent);
	if (result)
	{
		LockWindow();
		ShowWindow(SW_HIDE);
		Move(top);
		ShowWindow(SW_SHOW);
		SetTimer(TIMERID, 1000, NULL);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		m_pParent->PostMessage(WM_IFSSETTINGBOXCLOSED, m_sItem);
	}
	CEditCtrl::Destroy();
	delete this;
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsSettingBoxDlg::OnTimer(UINT_PTR)
{
	if (!m_bInit)
	{
		m_bInit = TRUE;
		UnlockWindow();
	}
	SetValue();
}
//*************************************************************************************
//*************************************************************************************
void CIfsSettingBoxDlg::ShowHeader(const CString& value)
{
	SetWindowText(value);
}
//***************************************************************************************
//***************************************************************************************
LRESULT CIfsSettingBoxDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = c_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			SetValue();
		}
	}
	OnSetFocus();
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIfsSettingBoxDlg::OnNotifyEditDebounceMax()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_MaxDebounce);
	if (bModified)
	{
		REMOTEREF.setIfsDebounceMax(m_sItem, m_MaxDebounce);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIfsSettingBoxDlg::OnNotifyEditDebounceMin()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_MinDebounce);
	if (bModified)
	{
		REMOTEREF.setIfsDebounceMin(m_sItem, m_MinDebounce);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
void CIfsSettingBoxDlg::SetControlStyle(void)
{
	BOOL bLoginRefillgrenzen = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS);

	REMOTEREF.getIfsDebounceMax(m_sItem, m_MaxDebounce);
	REMOTEREF.getIfsDebounceMin(m_sItem, m_MinDebounce);

	ENABLE_ID(IDC_IFS_SETTING_DEBOUNCEMAX_EDIT, bLoginRefillgrenzen);
	ENABLE_ID(IDC_IFS_SETTING_DEBOUNCEMIN_EDIT, bLoginRefillgrenzen);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsSettingBoxDlg::SetValue(void)
{
	SetControlStyle();
	UpdateData(FALSE);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsSettingBoxDlg::GetValue(void)
{
	UpdateData(TRUE);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIfsSettingBoxDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		ShowHeader(m_szCaption);

		INITINFOBUTTON(m_MaxDebounceInfoButton);
		INITINFOBUTTON(m_MinDebounceInfoButton);

		SetValue();
	}
	return result;
}
//*************************************************************************************
//*************************************************************************************
void CIfsSettingBoxDlg::OnStnDebounceMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_SETTING_DEBOUNCEMAX_EDIT);
}
//*************************************************************************************
//*************************************************************************************
void CIfsSettingBoxDlg::OnStnDebounceMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_SETTING_DEBOUNCEMIN_EDIT);
}
//*************************************************************************************
//*************************************************************************************
HBRUSH CIfsSettingBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseClass::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_IFS_SETTING_DEBOUNCEMAX_EDIT:
		case IDC_IFS_SETTING_DEBOUNCEMIN_EDIT:
		{
			if (pWnd->IsWindowEnabled())
			{
				pDC->SetTextColor(EDITTEXTCOLOR);
				pDC->SetBkColor(EDITBKCOLOR);
				pDC->SetBkMode(OPAQUE);
				hbr = (HBRUSH)c_EditBrush;
			}
		}
		break;

	default:
		break;
	}
	return hbr;
}
//*************************************************************************************
//*************************************************************************************
void CIfsSettingBoxDlg::OnBnClickedDebounceMaxInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_IFS_SETTING_DEBOUNCEMAX_EDIT, IDS_DEBOUNCEMAX_S, IDS_INFO_DEBOUNCE);
}
//*************************************************************************************
//*************************************************************************************
void CIfsSettingBoxDlg::OnBnClickedDebounceMinInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_IFS_SETTING_DEBOUNCEMIN_EDIT, IDS_DEBOUNCEMIN_S, IDS_INFO_DEBOUNCE);
}
