//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineSettingsDlg
///
/// @file   LineSettingsDlg.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------

#include "stdafx.h"
#include "EasyControl.h"
#include "LineSettingsDlg.h"
#include "LineSetpointLineDlg.h"
#include "LineSetpointLocalDlg.h"
#include "LineRampDlg.h"
#include "LineExtSetpointDlg.h"
#include "LineTotalizerPulseDlg.h"
#include "DataImage.h"
#include "MFCMacros.h"



IMPLEMENT_DYNAMIC(CLineSettingsDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineSettingsDlg::CLineSettingsDlg(CWnd* pParent)
	: CDialog(CLineSettingsDlg::IDD, pParent)
{
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineSettingsDlg, CDialog)
	ON_MESSAGE(WM_PRESELECT, &CLineSettingsDlg::OnPreselect)
	ON_BN_CLICKED(IDC_LINE_SETTINGS_RAMP_BT, &CLineSettingsDlg::OnBnClickedLineSettingsRampBt)
	ON_BN_CLICKED(IDC_LINE_SETTINGS_EXTSETPOINT_BT, &CLineSettingsDlg::OnBnClickedLineSettingsExtSetpointBt)
	ON_BN_CLICKED(IDC_LINE_SETTINGS_LINESETPOINT_BT, &CLineSettingsDlg::OnBnClickedLineSettingsLineSetpointBt)
	ON_BN_CLICKED(IDC_LINE_SETTINGS_LOCALSETPOINT_BT, &CLineSettingsDlg::OnBnClickedLineSettingsLocalSetpointBt)
	ON_BN_CLICKED(IDC_LINE_SETTINGS_TOTIMPULSE_BT, &CLineSettingsDlg::OnBnClickedLineSettingsTotImpulseBt)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSettingsDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		const auto& aV = dataimage::getLineDataImage();
		const auto& cache = aV.m_Cache;
		const auto& processstatus = cache.m_ProcessStatus;

		m_ModuleBits.flags.ModuleSetpointRamp = processstatus.flags.ModuleSetpointRamp;
		m_ModuleBits.flags.ModuleExternalSetpoint = processstatus.flags.ModuleExternalSetpoint;
		m_ModuleBits.flags.ModuleLineSetpoint = processstatus.flags.ModuleLineSetpoint;
		m_ModuleBits.flags.ModuleLocalSetpoint = processstatus.flags.ModuleLocalSetpoint;
		m_ModuleBits.flags.ModuleTotalizerPulse = processstatus.flags.ModuleTotalizerPulse;

		SortButtons();
	}
	return result;
}
//**************************************************************************************************************
//**************************************************************************************************************
LRESULT CLineSettingsDlg::OnPreselect(WPARAM id, LPARAM)
{
	CWnd* pBtnB = GetDlgItem(_S32(id));
	ASSERT(pBtnB != NULL);  // You can use MFC 

	PostMessage(WM_COMMAND,
		MAKEWPARAM(_S32(id), BN_CLICKED),
		reinterpret_cast<LPARAM>(pBtnB->GetSafeHwnd()));
	return 0L;
}
//**************************************************************************************************************
//**************************************************************************************************************
inline void CLineSettingsDlg::ActivateDlg(CDialog& rDlg)
{
	rDlg.DoModal();
	EndDialog(IDOK);
}

//**************************************************************************************************************
//**************************************************************************************************************
void CLineSettingsDlg::OnBnClickedLineSettingsTotImpulseBt()
{
	CLineTotalizerPulseDlg dlg(this);
	ActivateDlg(dlg);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLineSettingsDlg::OnBnClickedLineSettingsRampBt()
{
	CLineRampDlg dlg(this);
	ActivateDlg(dlg);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLineSettingsDlg::OnBnClickedLineSettingsExtSetpointBt()
{
	CLineExtSetpointDlg dlg(this);
	ActivateDlg(dlg);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLineSettingsDlg::OnBnClickedLineSettingsLineSetpointBt()
{
	CLineSetpointLineDlg dlg(this);
	ActivateDlg(dlg);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLineSettingsDlg::OnBnClickedLineSettingsLocalSetpointBt()
{
	CLineSetpointLocalDlg dlg(this);
	ActivateDlg(dlg);
}
//*****************************************************************************************************************
//*****************************************************************************************************************
CSize CLineSettingsDlg::GetDlgItemSize(CWnd* pWnd) const
{
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	ScreenToClient(&aRect);
	return aRect.Size();
}
//*****************************************************************************************************************
//*****************************************************************************************************************
void CLineSettingsDlg::SortButtons()
{
	int32_t ids[10];
	int32_t count = 0;

	if (m_ModuleBits.flags.ModuleSetpointRamp)
	{
		ids[count++] = IDC_LINE_SETTINGS_RAMP_BT;
	}
	if (m_ModuleBits.flags.ModuleExternalSetpoint)
	{
		ids[count++] = IDC_LINE_SETTINGS_EXTSETPOINT_BT;
	}
	if (m_ModuleBits.flags.ModuleLineSetpoint)
	{
		ids[count++] = IDC_LINE_SETTINGS_LINESETPOINT_BT;
	}
	if (m_ModuleBits.flags.ModuleLocalSetpoint)
	{
		ids[count++] = IDC_LINE_SETTINGS_LOCALSETPOINT_BT;
	}
	if (m_ModuleBits.flags.ModuleTotalizerPulse)
	{
		ids[count++] = IDC_LINE_SETTINGS_TOTIMPULSE_BT;
	}
	ids[count++] = IDOK;
	ASSERT(count < _S32(__ARRAYSIZE(ids)));


	// Hide Apply and Help buttons
	HIDE_ID(IDC_LINE_SETTINGS_RAMP_BT);
	HIDE_ID(IDC_LINE_SETTINGS_EXTSETPOINT_BT);
	HIDE_ID(IDC_LINE_SETTINGS_LINESETPOINT_BT);
	HIDE_ID(IDC_LINE_SETTINGS_LOCALSETPOINT_BT);
	HIDE_ID(IDC_LINE_SETTINGS_TOTIMPULSE_BT);
	HIDE_ID(IDOK);

	if (count > 2)
	{
		const int32_t nSpacing = 10;        // space between two buttons...

		int32_t y = 0;
		int32_t x = 0;
		for (int32_t i = 0; i < count; i++)
		{
			CWnd* pWnd = GetDlgItem(ids[i]);
			ASSERT(pWnd);
			auto dlgItemsize = GetDlgItemSize(pWnd);

			if (i == 0)
			{
				y = nSpacing + dlgItemsize.cy / 2;
				x = nSpacing + dlgItemsize.cx / 2;
			}
			const int32_t nAddSpacing = (ids[i] == IDOK) ? 10 : 0;

			y	+= nAddSpacing;
			CRect aRect(CPoint(x - (dlgItemsize.cx / 2), y - (dlgItemsize.cy / 2) ), dlgItemsize);
			pWnd->MoveWindow(aRect); // Client coordinaten

			y = aRect.bottom + 3*nSpacing;
		}
		// Screen coordinates
		CRect aDlgRect;
		GetWindowRect(aDlgRect);
		SetWindowPos(NULL, aDlgRect.left, aDlgRect.top, aDlgRect.Width(), y - nSpacing, SWP_NOZORDER);
		for (int32_t i = 0; i < count; i++)
		{
			SHOWW_ID(ids[i], TRUE);
		}
	}
	else
	{
		PostMessage(WM_PRESELECT, WPARAM(ids[0]));
	}
}

