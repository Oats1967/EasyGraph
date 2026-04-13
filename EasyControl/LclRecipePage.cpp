//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LclRecipePage
///
/// @file   LclRecipePage.cpp
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
#include "LclRecipePage.h"
#include "FeederScaleBoxDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLclRecipePage, _func)

//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CLclRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_LCL_RECIPE_NAME, &CLclRecipePage::OnBnClickedLclName)
	ON_STN_CLICKED(IDC_LCL_RECIPE_ACTWEIGHT_STATIC, &CLclRecipePage::OnStnClickedScale)
	ON_STN_CLICKED(IDC_LCL_RECIPE_MINLEVEL, &CLclRecipePage::OnStnClickedMinLevel)
	ON_STN_CLICKED(IDC_LCL_RECIPE_MAXLEVEL, &CLclRecipePage::OnStnClickedMaxLevel)
	ON_BN_CLICKED(IDC_LCL_RECIPE_MINLEVEL_INFO, &CLclRecipePage::OnBnClickedLclMinLevelInfo)
	ON_BN_CLICKED(IDC_LCL_RECIPE_MAXLEVEL_INFO, &CLclRecipePage::OnBnClickedLclMaxLevelInfo)
	ON_BN_CLICKED(IDC_LCL_RECIPE_NAME_BT, &CLclRecipePage::OnBnClickedLclNameBt)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CLclRecipePage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CLclRecipePage::CLclRecipePage(): CDosePage(CLclRecipePage::IDD) 
	, m_EditMap({
			EDITITEM(IDC_LCL_RECIPE_NAME, OnNotifyEditName),
			EDITITEM(IDC_LCL_RECIPE_MINLEVEL,	OnNotifyEditMinLevel),
			EDITITEM(IDC_LCL_RECIPE_MAXLEVEL,	OnNotifyEditMaxLevel),
		})
		, m_MinLevelInfoButton()
		, m_MaxLevelInfoButton()
		, m_ProductListName()
		, m_fActWeight{ 0.0F }
		, m_fMinLevel{ 0.0F }
		, m_fMaxLevel{ 0.0F }
{}
//***************************************************************************************
//***************************************************************************************
void CLclRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_TextN(pDX, IDC_LCL_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_LCL_RECIPE_NR, m_lNr);
	DDX_Text(pDX, IDC_LCL_RECIPE_TITLE, m_szTitle);

	DDX_FloatHR(pDX, IDC_LCL_RECIPE_ACTWEIGHT, m_fActWeight);
	DDX_Text(pDX, IDC_LCL_RECIPE_MINLEVEL, m_fMinLevel);
	DDX_Text(pDX, IDC_LCL_RECIPE_MAXLEVEL, m_fMaxLevel);
	DDX_Control(pDX, IDC_LCL_RECIPE_IMAGE, m_aGrafikContainer);
	DDX_Control(pDX, IDC_LCL_RECIPE_MINLEVEL_INFO, m_MinLevelInfoButton);
	DDX_Control(pDX, IDC_LCL_RECIPE_MAXLEVEL_INFO, m_MaxLevelInfoButton);
	DDX_Control(pDX, IDC_LCL_RECIPE_NAME_BT, m_ProductListName);

}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CLclRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CLclRecipePage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}

    ENABLE_ID(IDC_LCL_RECIPE_NAME,      bLogin);
	ENABLE_ID(IDC_LCL_RECIPE_NAME_BT,  bLogin);
	ENABLE_ID(IDC_LCL_RECIPE_MINLEVEL, bLogin);
	ENABLE_ID(IDC_LCL_RECIPE_MAXLEVEL, bLogin);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CLclRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISLCLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
	REMOTEREF.getDoseLclWeightMinLevel(m_sItem, m_fMinLevel);

	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fActWeight);
	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CLclRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_MinLevelInfoButton)
		INITINFOBUTTON(m_MaxLevelInfoButton)

		SetNumberFont(IDC_LCL_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CLclRecipePage::OnSetActive()
{
	m_ProductListName.Redraw();
	return CDosePage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CLclRecipePage::OnBnClickedLclName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LCL_RECIPE_NAME);
}
//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CLclRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LCL_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;


		case IDC_LCL_RECIPE_ACTWEIGHT_STATIC:
        case IDC_LCL_RECIPE_NAME:
		case IDC_LCL_RECIPE_MINLEVEL:
		case IDC_LCL_RECIPE_MAXLEVEL:
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

		case IDC_LCL_RECIPE_IMAGE:
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
void CLclRecipePage::OnStnClickedScale()
{
	auto pWnd = GetDlgItem(IDC_LCL_RECIPE_IMAGE);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CFeederScaleBoxDlg::CreateScaleBox(this, m_sItem, CPoint{ aRect.right, aRect.top });
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLclRecipePage::OnNotifyEditMinLevel(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMinLevel);
	if (bModified)
	{
		REMOTEREF.setDoseLclWeightMinLevel(m_sItem, m_fMinLevel);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLclRecipePage::OnNotifyEditMaxLevel(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fMaxLevel);
	if (bModified)
	{
		REMOTEREF.setDoseLclWeightMaxLevel(m_sItem, m_fMaxLevel);
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLclRecipePage::OnBnClickedLclMinLevelInfo()
{
	CreateHelpInfoBox(IDC_LCL_RECIPE_MINLEVEL, IDS_LCLMINLEVEL_KG, IDS_INFO_LCLMINLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLclRecipePage::OnBnClickedLclMaxLevelInfo()
{
	CreateHelpInfoBox(IDC_LCL_RECIPE_MAXLEVEL, IDS_LCLMAXLEVEL_KG, IDS_INFO_LCLMAXLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLclRecipePage::OnStnClickedMinLevel()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LCL_RECIPE_MINLEVEL);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLclRecipePage::OnStnClickedMaxLevel()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LCL_RECIPE_MAXLEVEL);
}
//***************************************************************************************
//***************************************************************************************
void CLclRecipePage::OnBnClickedLclNameBt()
{
	CreateProductDatabaseBox(IDC_LCL_RECIPE_NAME_BT, TRUE);
}







