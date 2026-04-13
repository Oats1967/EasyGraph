//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IfsRecipePage
///
/// @file   IfsRecipePage.cpp
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
#include "IfsRecipePage.h"
#include "IfsSettingBoxDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIfsRecipePage, _func)


//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CIfsRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_MESSAGE(WM_IFSSETTINGBOXCLOSED, OnIfsSettingClosed)
	ON_STN_CLICKED(IDC_IFS_RECIPE_NAME, &CIfsRecipePage::OnStnClickedIfsName)
	ON_STN_CLICKED(IDC_IFS_RECIPE_REDUCEFACTOR_EDIT, &CIfsRecipePage::OnStnClickedReduceFactor)
	ON_STN_CLICKED(IDC_IFS_RECIPE_GAINFACTOR_EDIT, &CIfsRecipePage::OnStnClickedGainFactor)
	ON_STN_CLICKED(IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT, &CIfsRecipePage::OnStnClickedIfsStepTimeGain)
	ON_STN_CLICKED(IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT, &CIfsRecipePage::OnStnClickedIfsStepTimeReduce)
	ON_STN_CLICKED(IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT, &CIfsRecipePage::OnStnClickedIfsSetpointOverflow)

	ON_STN_CLICKED(IDC_IFS_RECIPE_BITMAP, &CIfsRecipePage::OnStnClickedIfsSetting)

	ON_BN_CLICKED(IDC_IFS_RECIPE_NAME_BT, &CIfsRecipePage::OnBnClickedIfsNameBt)
	ON_BN_CLICKED(IDC_IFS_RECIPE_REDUCEFACTOR_INFO, &CIfsRecipePage::OnBnClickedReduceFactorInfo)
	ON_BN_CLICKED(IDC_IFS_RECIPE_GAINFACTOR_INFO, &CIfsRecipePage::OnBnClickedGainFactorInfo)
	ON_BN_CLICKED(IDC_IFS_RECIPE_TIMESTEPGAIN_INFO, &CIfsRecipePage::OnBnClickedIfsStepTimeGainInfo)
	ON_BN_CLICKED(IDC_IFS_RECIPE_TIMESTEPREDUCE_INFO, &CIfsRecipePage::OnBnClickedIfsStepTimeReduceInfo)
	ON_BN_CLICKED(IDC_IFS_RECIPE_SETPOINTOVERFLOW_INFO, &CIfsRecipePage::OnBnClickedIfsSetpointOverflowInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CIfsRecipePage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CIfsRecipePage::CIfsRecipePage(): CDosePage(CIfsRecipePage::IDD) 
	, m_EditMap({
			EDITITEM(IDC_IFS_RECIPE_NAME, OnNotifyEditName),
			EDITITEM(IDC_IFS_RECIPE_GAINFACTOR_EDIT, OnNotifyEditGainFactor),
			EDITITEM(IDC_IFS_RECIPE_REDUCEFACTOR_EDIT, OnNotifyEditReduceFactor),
			EDITITEM(IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT, OnNotifyStepTimeGain),
			EDITITEM(IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT, OnNotifyStepTimeReduce),
			EDITITEM(IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT, OnNotifySetpointOverflowInfo)
		})
		, m_GainFactorInfoButton()
		, m_ReduceFactorInfoButton()
		, m_ProductListName()
		, m_GainFactor{ 0 }
		, m_ReduceFactor{ 0 }
		, m_StepTimeGain{ 0 }
		, m_StepTimeReduce{ 0 }
		, m_SetpointOverflow{ 0 }
{}
//***************************************************************************************
//***************************************************************************************
void CIfsRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_TextN(pDX, IDC_IFS_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_IFS_RECIPE_NR, m_lNr);
	DDX_Text(pDX, IDC_IFS_RECIPE_TITLE, m_szTitle);
	DDX_Text(pDX, IDC_IFS_RECIPE_GAINFACTOR_EDIT, m_GainFactor);
	DDX_Text(pDX, IDC_IFS_RECIPE_REDUCEFACTOR_EDIT, m_ReduceFactor);
	DDX_Text(pDX, IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT, m_StepTimeGain);
	DDX_Text(pDX, IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT, m_StepTimeReduce);
	DDX_Text(pDX, IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT, m_SetpointOverflow);

	DDX_Control(pDX, IDC_IFS_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_IFS_RECIPE_GAINFACTOR_INFO, m_GainFactorInfoButton);
	DDX_Control(pDX, IDC_IFS_RECIPE_REDUCEFACTOR_INFO, m_ReduceFactorInfoButton);
	DDX_Control(pDX, IDC_IFS_RECIPE_TIMESTEPGAIN_INFO, m_StepTimeGainInfoButton);
	DDX_Control(pDX, IDC_IFS_RECIPE_TIMESTEPREDUCE_INFO, m_StepTimeReduceInfoButton);
	DDX_Control(pDX, IDC_IFS_RECIPE_SETPOINTOVERFLOW_INFO, m_SetpointOverflowInfoButton);
	DDX_Control(pDX, IDC_IFS_RECIPE_NAME_BT, m_ProductListName);
}
//***************************************************************************************
//***************************************************************************************
LRESULT CIfsRecipePage::OnIfsSettingClosed(WPARAM, LPARAM)
{
	SetFocus();
	return 0L;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CIfsRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CIfsRecipePage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}

    ENABLE_ID(IDC_IFS_RECIPE_NAME,        bLogin);
	ENABLE_ID(IDC_IFS_RECIPE_GAINFACTOR_EDIT, bLogin && (m_StepTimeGain > 0));
	ENABLE_ID(IDC_IFS_RECIPE_REDUCEFACTOR_EDIT, bLogin && ( m_StepTimeReduce > 0));
	ENABLE_ID(IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT, bLogin);
	ENABLE_ID(IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT, bLogin);
	ENABLE_ID(IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT, bLogin);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIfsRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISIFSTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getIfsGainFactor(m_sItem, m_GainFactor);
	REMOTEREF.getIfsReduceFactor(m_sItem, m_ReduceFactor);
	REMOTEREF.getIfsStepTimeGain(m_sItem, m_StepTimeGain);
	REMOTEREF.getIfsStepTimeReduce(m_sItem, m_StepTimeReduce);
	REMOTEREF.getIfsSetpointOverflow(m_sItem, m_SetpointOverflow);

	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIfsRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_GainFactorInfoButton)
		INITINFOBUTTON(m_ReduceFactorInfoButton)
		INITINFOBUTTON(m_StepTimeGainInfoButton);
		INITINFOBUTTON(m_StepTimeReduceInfoButton);
		INITINFOBUTTON(m_SetpointOverflowInfoButton);

		SetNumberFont(IDC_IFS_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CIfsRecipePage::OnSetActive()
{
	m_ProductListName.Redraw();
	return CDosePage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsRecipePage::OnStnClickedIfsSetting()
{
	auto pWnd = GetDlgItem(IDC_IFS_RECIPE_BITMAP);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CIfsSettingBoxDlg::CreateRefillBox(this, m_sItem, CPoint{ aRect.right, aRect.top }, IDS_IFSSETTING);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsRecipePage::OnStnClickedIfsName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_IFS_RECIPE_NAME);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsRecipePage::OnStnClickedGainFactor()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFS_RECIPE_GAINFACTOR_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIfsRecipePage::OnStnClickedReduceFactor()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFS_RECIPE_REDUCEFACTOR_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnStnClickedIfsStepTimeGain()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnStnClickedIfsStepTimeReduce()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnStnClickedIfsSetpointOverflow()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CIfsRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_IFS_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;


        case IDC_IFS_RECIPE_NAME:
		case IDC_IFS_RECIPE_GAINFACTOR_EDIT:
		case IDC_IFS_RECIPE_REDUCEFACTOR_EDIT:
		case IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT:
		case IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT:
		case IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT:
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

		case IDC_IFS_RECIPE_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

        default:    
                            break;
    }
    return hbr;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIfsRecipePage::OnNotifyEditGainFactor(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_GainFactor);
	if (bModified)
	{
		m_GainFactor = ROUND1(m_GainFactor);
		REMOTEREF.setIfsGainFactor(m_sItem, m_GainFactor);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIfsRecipePage::OnNotifyEditReduceFactor(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_ReduceFactor);
	if (bModified)
	{
		m_ReduceFactor = ROUND1(m_ReduceFactor);
		REMOTEREF.setIfsReduceFactor(m_sItem, m_ReduceFactor);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CIfsRecipePage::OnNotifyStepTimeGain()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_StepTimeGain);
	if (bModified)
	{
		REMOTEREF.setIfsStepTimeGain(m_sItem, m_StepTimeGain);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CIfsRecipePage::OnNotifyStepTimeReduce()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_StepTimeReduce);
	if (bModified)
	{
		REMOTEREF.setIfsStepTimeReduce(m_sItem, m_StepTimeReduce);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIfsRecipePage::OnNotifySetpointOverflowInfo(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_SetpointOverflow);
	if (bModified)
	{
		m_SetpointOverflow = ROUND1(m_SetpointOverflow);
		REMOTEREF.setIfsSetpointOverflow(m_sItem, m_SetpointOverflow);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
void CIfsRecipePage::OnBnClickedIfsNameBt()
{
	CreateProductDatabaseBox(IDC_IFS_RECIPE_NAME_BT, TRUE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIfsRecipePage::OnBnClickedGainFactorInfo()
{
	CreateHelpInfoBox(IDC_IFS_RECIPE_GAINFACTOR_EDIT, IDS_IFS_GAINFACTOR_KGS, IDS_INFO_IFS_GAINFACTOR);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIfsRecipePage::OnBnClickedReduceFactorInfo()
{
	CreateHelpInfoBox(IDC_IFS_RECIPE_REDUCEFACTOR_EDIT, IDS_IFS_REDUCEFACTOR_KGS, IDS_INFO_IFS_REDUCEFACTOR);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnBnClickedIfsStepTimeGainInfo()
{
	CreateHelpInfoBox(IDC_IFS_RECIPE_TIMESTEPGAIN_EDIT, IDS_IFS_TIMESTEPGAIN_S, IDS_INFO_IFS_TIMESTEPGAIN);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnBnClickedIfsStepTimeReduceInfo()
{
	CreateHelpInfoBox(IDC_IFS_RECIPE_TIMESTEPREDUCE_EDIT, IDS_IFS_TIMESTEPREDUCE_S, IDS_INFO_IFS_TIMESTEPREDUCE);
}
//***********************************************************************************************
//***********************************************************************************************
void CIfsRecipePage::OnBnClickedIfsSetpointOverflowInfo()
{
	CreateHelpInfoBox(IDC_IFS_RECIPE_SETPOINTOVERFLOW_EDIT, IDS_IFS_SETPOINTOVERFLOW_KGH, IDS_INFO_IFS_SETPOINTOVERFLOW);
}











