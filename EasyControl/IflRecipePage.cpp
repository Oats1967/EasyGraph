//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IflRecipePage
///
/// @file   IflRecipePage.cpp
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
#include <math.h>
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "RemoteControl.h"
#include "IflRecipePage.h"
#include "GainFactorSpeedBoxDlg.h"
#include "ReduceFactorSpeedBoxDlg.h"
#include "FeederScaleBoxDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIflRecipePage, _func)


BOOL CIflRecipePage::g_ShowLess = TRUE;
//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CIflRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_IFL_RECIPE_NAME, &CIflRecipePage::OnStnClickedIflName)
	ON_STN_CLICKED(IDC_IFL_RECIPE_ACTWEIGHT_STATIC, &CIflRecipePage::OnStnClickedScale)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MINLEVEL_EDIT, &CIflRecipePage::OnStnClickedMinLevel)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MAXLEVEL_EDIT, &CIflRecipePage::OnStnClickedMaxLevel)
	ON_STN_CLICKED(IDC_IFL_RECIPE_HYSTERESIS_EDIT, &CIflRecipePage::OnStnClickedHysteresis)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MINSETPOINT_EDIT, &CIflRecipePage::OnStnClickedMinLineSetpoint)
	ON_STN_CLICKED(IDC_IFL_RECIPE_MAXSETPOINT_EDIT, &CIflRecipePage::OnStnClickedMaxLineSetpoint)
	ON_STN_CLICKED(IDC_IFL_RECIPE_SETPOINT_EDIT, &CIflRecipePage::OnStnClickedLineSetpoint)

	ON_BN_CLICKED(IDC_IFL_RECIPE_NAME_BT, &CIflRecipePage::OnBnClickedIflNameBt)
	ON_BN_CLICKED(IDC_IFL_RECIPE_LINE, &CIflRecipePage::OnBnClickedIflLinie)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MINLEVEL_INFO, &CIflRecipePage::OnBnClickedMinLevelInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MAXLEVEL_INFO, &CIflRecipePage::OnBnClickedMaxLevelInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_HYSTERESIS_INFO, &CIflRecipePage::OnBnClickedHysteresisInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MINSETPOINT_INFO, &CIflRecipePage::OnBnClickedMinLineSetpointInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_MAXSETPOINT_INFO, &CIflRecipePage::OnBnClickedMaxLineSetpointInfo)
	ON_BN_CLICKED(IDC_IFL_RECIPE_SETPOINT_INFO, &CIflRecipePage::OnBnClickedLineSetpointInfo)

	ON_BN_CLICKED(IDC_IFL_RECIPE_CONTROLPARAMETER_MORELESS, &CIflRecipePage::OnBnClickedControlParamMoreLess)


	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CIflRecipePage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CIflRecipePage::CIflRecipePage(): CDosePage(CIflRecipePage::IDD) 
	, m_EditMap({
			EDITITEM(IDC_IFL_RECIPE_NAME, OnNotifyEditName),
			EDITITEM(IDC_IFL_RECIPE_MINLEVEL_EDIT,	OnNotifyEditMinLevel),
			EDITITEM(IDC_IFL_RECIPE_MAXLEVEL_EDIT,	OnNotifyEditMaxLevel),
			EDITITEM(IDC_IFL_RECIPE_HYSTERESIS_EDIT,	OnNotifyEditHysteresis),
			EDITITEM(IDC_IFL_RECIPE_MINSETPOINT_EDIT,	OnNotifyEditMinLineSetpoint),
			EDITITEM(IDC_IFL_RECIPE_MAXSETPOINT_EDIT,	OnNotifyEditMaxLineSetpoint),
			EDITITEM(IDC_IFL_RECIPE_SETPOINT_EDIT,	OnNotifyEditLineSetpoint),

		})
		, m_aLocalMode()
		, m_MinLevelInfoButton()
		, m_MaxLevelInfoButton()
		, m_HysteresisInfoButton()
		, m_ProductListName()
		, m_fActWeight{ 0.0F }
		, m_fMinLevel{ 0.0F }
		, m_fMaxLevel{ 0.0F }
		, m_fHysteresis{ 0.0F }
		, m_fMinLineSetpoint{ 0.0F }
		, m_fMaxLineSetpoint{ 0.0F }
		, m_fLineSetpoint{ 0.0F }
{}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_TextN(pDX, IDC_IFL_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_IFL_RECIPE_NR, m_lNr);
	DDX_Text(pDX, IDC_IFL_RECIPE_TITLE, m_szTitle);
	DDX_Float(pDX, IDC_IFL_RECIPE_MINLEVEL_EDIT, m_fMinLevel);
	DDX_Float(pDX, IDC_IFL_RECIPE_MAXLEVEL_EDIT, m_fMaxLevel);
	DDX_Float(pDX, IDC_IFL_RECIPE_HYSTERESIS_EDIT, m_fHysteresis);
	DDX_Float(pDX, IDC_IFL_RECIPE_MINSETPOINT_EDIT, m_fMinLineSetpoint);
	DDX_Float(pDX, IDC_IFL_RECIPE_MAXSETPOINT_EDIT, m_fMaxLineSetpoint);
	DDX_Float(pDX, IDC_IFL_RECIPE_SETPOINT_EDIT, m_fLineSetpoint);
	DDX_FloatHR(pDX, IDC_IFL_RECIPE_ACTWEIGHT, m_fActWeight);

	DDX_Control(pDX, IDC_IFL_RECIPE_IMAGE, m_aGrafikContainer);
	DDX_Control(pDX, IDC_IFL_RECIPE_MINLEVEL_INFO, m_MinLevelInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_MAXLEVEL_INFO, m_MaxLevelInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_HYSTERESIS_INFO, m_HysteresisInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_MINSETPOINT_INFO, m_MinLineSetpointInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_MAXSETPOINT_INFO, m_MaxLineSetpointInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_SETPOINT_INFO, m_LineSetpointInfoButton);
	DDX_Control(pDX, IDC_IFL_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_IFL_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_IFL_RECIPE_CONTROLPARAMETER_MORELESS, m_MoreLessButton);

}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMinLevel(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (value > m_fMaxLevel)
	{
		ECMessageBox(IDS_INPUTERROR_MINGREATERMAX, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(value != m_fMinLevel);
		if (bModified)
		{
			m_fMinLevel = value;
			REMOTEREF.setDoseLclWeightMinLevel(m_sItem, m_fMinLevel);
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMaxLevel(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (value < m_fMinLevel )
	{
		ECMessageBox(IDS_INPUTERROR_MINGREATERMAX, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fMaxLevel != value);
		if (bModified)
		{
			m_fMaxLevel = value;
			REMOTEREF.setDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditHysteresis(void)
{
	BOOL bModified = FALSE;

	auto value = CEditCtrl::GetFloatAbs();
	if (m_fMaxLevel - m_fMinLevel < 2 * value)
	{
		ECMessageBox(IDS_INPUTERROR_HYSTERESISHIGH, MB_OK | MB_ICONSTOP);
	}
	else
	{
		bModified = BOOL(m_fHysteresis != value);
		if (bModified)
		{
			m_fHysteresis = value;
			REMOTEREF.setDoseLclWeightHysteresis(m_sItem, m_fHysteresis);
		}
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMinLineSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMinLineSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseIflMinLineSetpoint(m_sItem, m_fMinLineSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditMaxLineSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMaxLineSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseIflMaxLineSetpoint(m_sItem, m_fMaxLineSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflRecipePage::OnNotifyEditLineSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fLineSetpoint);
	if (bModified)
	{
		REMOTEREF.setLineSetpoint(m_fLineSetpoint);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CIflRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = m_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			PostMessage(WM_TIMER_REFRESH);
		}
	}
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bLocalMode = !processstatus.flags.lineMode;
	const BOOL bSlaveMode = processstatus.flags.slaveMode;
	const BOOL bStarted = (processstatus.flags.started == true) || (processstatus.flags.running == true);

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

	ENABLE_ID(IDC_IFL_RECIPE_NAME, bLogin);
	ENABLE_ID(IDC_IFL_RECIPE_SETPOINT_EDIT, bLogin);

	const BOOL bShowControlParam = !g_ShowLess;
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MINLEVEL_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MAXLEVEL_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_HYSTERESIS_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MINSETPOINT_EDIT, bLogin, bShowControlParam);
	ENABLE_SHOW_ID(IDC_IFL_RECIPE_MAXSETPOINT_EDIT, bLogin, bShowControlParam);

	SHOWW_ID(IDC_IFL_RECIPE_MINLEVEL_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXLEVEL_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_HYSTERESIS_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MINSETPOINT_STATIC, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXSETPOINT_STATIC, bShowControlParam);

	SHOWW_ID(IDC_IFL_RECIPE_MINLEVEL_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXLEVEL_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_HYSTERESIS_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MINSETPOINT_INFO, bShowControlParam);
	SHOWW_ID(IDC_IFL_RECIPE_MAXSETPOINT_INFO, bShowControlParam);

}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISIFLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
	REMOTEREF.getDoseLclWeightMinLevel(m_sItem, m_fMinLevel);
	REMOTEREF.getDoseLclWeightHysteresis(m_sItem, m_fHysteresis);
	REMOTEREF.getDoseIflMinLineSetpoint(m_sItem, m_fMinLineSetpoint);
	REMOTEREF.getDoseIflMaxLineSetpoint(m_sItem, m_fMaxLineSetpoint);
	REMOTEREF.getLineSetpoint(m_fLineSetpoint);

	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fActWeight);
	SetControlStyle();
	m_MoreLessButton.ShowLess(g_ShowLess);
	m_MoreLessButton.Show();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_MinLevelInfoButton)
		INITINFOBUTTON(m_MaxLevelInfoButton)
		INITINFOBUTTON(m_HysteresisInfoButton)
		INITINFOBUTTON(m_MinLineSetpointInfoButton)
		INITINFOBUTTON(m_MaxLineSetpointInfoButton)
		INITINFOBUTTON(m_LineSetpointInfoButton)
		INITINFOBUTTON(m_MoreLessButton)

		SetNumberFont(IDC_IFL_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CIflRecipePage::OnSetActive()
{
	m_ProductListName.Redraw();
	m_aLocalMode.Redraw();
	m_MoreLessButton.Redraw();
	return CDosePage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedIflName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_IFL_RECIPE_NAME);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedScale()
{
	auto pWnd = GetDlgItem(IDC_IFL_RECIPE_IMAGE);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CFeederScaleBoxDlg::CreateScaleBox(this, m_sItem, CPoint{ aRect.right, aRect.top });
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMinLevel()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_MINLEVEL_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMaxLevel()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_MAXLEVEL_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedHysteresis()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_HYSTERESIS_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMinLineSetpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_MINSETPOINT_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedMaxLineSetpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_MAXSETPOINT_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflRecipePage::OnStnClickedLineSetpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_RECIPE_SETPOINT_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CIflRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_IFL_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;


        case IDC_IFL_RECIPE_NAME:
		case IDC_IFL_RECIPE_MINLEVEL_EDIT:
		case IDC_IFL_RECIPE_MAXLEVEL_EDIT:
		case IDC_IFL_RECIPE_HYSTERESIS_EDIT:
		case IDC_IFL_RECIPE_MINSETPOINT_EDIT:
		case IDC_IFL_RECIPE_MAXSETPOINT_EDIT:
		case IDC_IFL_RECIPE_SETPOINT_EDIT:
		case IDC_IFL_RECIPE_ACTWEIGHT_STATIC:
							{
                                if ( pWnd->IsWindowEnabled() )
                                {
                                    pDC->SetTextColor(EDITTEXTCOLOR);
                                    pDC->SetBkColor(EDITBKCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = (HBRUSH) c_EditBrush;
                                }
                            }
                            break;

		case IDC_IFL_RECIPE_IMAGE:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

        default:    
                            break;
    }
    return hbr;
}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::OnBnClickedIflNameBt()
{
	CreateProductDatabaseBox(IDC_IFL_RECIPE_NAME_BT, TRUE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMinLevelInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MINLEVEL_EDIT, IDS_LCLMINLEVEL_KG, IDS_INFO_LCLMINLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMaxLevelInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MAXLEVEL_EDIT, IDS_LCLMAXLEVEL_KG, IDS_INFO_LCLMAXLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedHysteresisInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_HYSTERESIS_EDIT, IDS_LCLHYSTERESIS_KG, IDS_INFO_LCHYSTERESIS);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMinLineSetpointInfo() 
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MINSETPOINT_EDIT, IDS_MINLINESETPOINT_KGH, IDS_INFO_MINLINESETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedMaxLineSetpointInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_MAXSETPOINT_EDIT, IDS_MAXLINESETPOINT_KGH, IDS_INFO_MAXLINESETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedLineSetpointInfo()
{
	CreateHelpInfoBox(IDC_IFL_RECIPE_SETPOINT_EDIT, IDS_LINESETPOINT_KGH, IDS_INFO_LINESETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflRecipePage::OnBnClickedControlParamMoreLess()
{
	g_ShowLess = !g_ShowLess;
	SetValue();
}
//***************************************************************************************
//***************************************************************************************
void CIflRecipePage::OnBnClickedIflLinie()
{
	ASSERT(m_aLocalMode.IsEnable());
	ToggleLineMode();
}










