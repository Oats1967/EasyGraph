//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module PidRecipePage
///
/// @file   PidRecipePage.cpp
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
#include "PidRecipePage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CPidRecipePage, _func)

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CPidRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_PID_RECIPE_VOLUMETRIC, &CPidRecipePage::OnBnClickedPidVolumetric)
	ON_BN_CLICKED(IDC_PID_RECIPE_TOTALIZER_RESET, &CPidRecipePage::OnBnClickedPidTotalizerReset)
	ON_STN_CLICKED(IDC_PID_RECIPE_NAME, &CPidRecipePage::OnStnClickedPidName)
	ON_STN_CLICKED(IDC_PID_RECIPE_QMNUMBER, &CPidRecipePage::OnStnClickedPidQmNumber)
	ON_STN_CLICKED(IDC_PID_RECIPE_SETPOINT, &CPidRecipePage::OnStnClickedPidSetpoint)
	ON_STN_CLICKED(IDC_PID_RECIPE_ANNUMBER, &CPidRecipePage::OnStnClickedANnummer)
	ON_BN_CLICKED(IDC_PID_RECIPE_LINE, &CPidRecipePage::OnBnClickedPidLinie)
	ON_BN_CLICKED(IDC_PID_RECIPE_NAME_BT, &CPidRecipePage::OnClickNameBt)
	ON_BN_CLICKED(IDC_PID_RECIPE_KEY_BT, &CPidRecipePage::OnClickKeyBt)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CPidRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CPidRecipePage::CPidRecipePage(): CDosePage(CPidRecipePage::IDD)
	, m_GaugeMassflow(this)
	, m_GaugeDriveCommand(this)
	, m_aVolGrav()
	, m_aLocalMode()
	, m_fSetpoint(0)
	, m_fMassflow(0)
	, m_fDriveCommand(0)
    , m_bRegenerat(FALSE)
	, m_fMischung(0.0f)
	, m_fActTotalizer(0.0f)
	, m_fActDoseperformance(0.0f)
	, c_aRegeneratBrush(REGENERATBKCOLOR)
	, m_ProductListName()
	, m_ProductListKey()
	, m_EditMap({
		EDITITEM(IDC_PID_RECIPE_SETPOINT,	OnNotifyEditSetpoint),
		EDITITEM(IDC_PID_RECIPE_NAME,		OnNotifyEditName),
		EDITITEM(IDC_PID_RECIPE_QMNUMBER,	OnNotifyEditQMNumber),
		EDITITEM(IDC_PID_RECIPE_ANNUMBER,	OnNotifyEditANNumber),
		})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CPidRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PID_RECIPE_NR, m_lNr);
	DDX_Text(pDX, IDC_PID_RECIPE_NAME, m_szName);
	DDX_Text(pDX, IDC_PID_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_PID_RECIPE_ANNUMBER, m_szANNumber);
	DDX_Float(pDX, IDC_PID_RECIPE_SETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_PID_RECIPE_PERCENTAGE, m_fMischung);
	DDX_Float(pDX, IDC_PID_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Float(pDX, IDC_PID_RECIPE_DOSEPERFORMANCE, m_fActDoseperformance);
	DDX_Control(pDX, IDC_PID_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_PID_RECIPE_DRIVECOMMANDCTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_PID_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_PID_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_PID_RECIPE_VOLUMETRIC, m_aVolGrav);
	DDX_Control(pDX, IDC_PID_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_PID_RECIPE_KEY_BT, m_ProductListKey);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CPidRecipePage::OnNotifyEditSetpoint(void)
{
	BOOL bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseSetpoint(m_sItem, m_fSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CPidRecipePage::OnNotifyEditANNumber(void)
{
	BOOL bModified = CEditCtrlEx::GetString(m_szANNumber);
	if (bModified)
	{
		REMOTEREF.setLineANNumber(toStdString(m_szANNumber));
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CPidRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			SetValue();
		}
	}
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::CheckDosePerformance (void)
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bLocalMode = ! ulStatus.flags.lineMode;

	if ( bLocalMode )
	{
		float32_t fTemp = 0.0f;
		REMOTEREF.getDoseActualDoseperformance(m_sItem, fTemp);
		if ( m_fSetpoint > fTemp )
        {
			ECMessageBox(IDS_SETPOINT_GREATER_DOSEPERFORMANCE, MB_OK | MB_ICONEXCLAMATION);
        }
		REMOTEREF.getDoseMaxSetpoint(m_sItem, fTemp);
		if ( m_fSetpoint > fTemp )
        {
			ECMessageBox(IDS_SETPOINT_GREATER_MAXSETPOINT, MB_OK | MB_ICONEXCLAMATION);
        }
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::SetControlStyle (void)
{
	BOOL bLogin = TRUE;

	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bLocalMode		 = ! ulStatus.flags.lineMode;
	const BOOL bVolumetricExtern =   ulStatus.flags.volumetricext;
	const BOOL bVolumetric		 = ! ulStatus.flags.gravimetric;
	const BOOL bStarted		= ulStatus.flags.started;
	const BOOL bSlaveMode   = ulStatus.flags.slaveMode;


	m_aVolGrav.SetInputExt(bVolumetricExtern);
	m_aVolGrav.Enable(bLogin && (!bVolumetricExtern));
	m_aVolGrav.EnableWindow((!bVolumetricExtern));
	m_aVolGrav.SetCheck(bVolumetric);
	m_aVolGrav.Show();

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_PID_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_PID_RECIPE_SETPOINT, bLogin && bLocalMode);
    ENABLE_ID(IDC_PID_RECIPE_NAME, bLogin);
    ENABLE_ID(IDC_PID_RECIPE_QMNUMBER, bLogin);
	ENABLE_ID(IDC_PID_RECIPE_ANNUMBER, bLogin);
	ENABLE_ID(IDC_PID_RECIPE_NAME_BT, bLogin);
	ENABLE_ID(IDC_PID_RECIPE_KEY_BT, bLogin);

    ENABLE_SHOW_ID(IDC_PID_RECIPE_TOTALIZER_RESET,  TRUE, bLocalMode);
    ENABLE_SHOW_ID(IDC_PID_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);

	BOOL bShowPercentage = (!bSlaveMode) && (!bLocalMode);
	ENABLE_SHOW_ID(IDC_PID_RECIPE_PERCENTAGE, bLogin && bShowPercentage, bShowPercentage);
	ENABLE_SHOW_ID(IDC_PID_RECIPE_PERCENTAGE_STATIC, bLogin && bShowPercentage, bShowPercentage);

}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CPidRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISPIDTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	std::string ANNumber;
	REMOTEREF.getLineANNumber(ANNumber);
	m_szANNumber = toCString(ANNumber);

	float32_t fMaxSetpoint = 0.0F;
	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0, 100));
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseActualDoseperformance(m_sItem, m_fActDoseperformance);
	REMOTEREF.getDosePercentage(m_sItem, m_fMischung);
	REMOTEREF.getDoseRegenerat(m_sItem, m_bRegenerat);

	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);

	m_GaugeDriveCommand.SetDriveCommand(m_fDriveCommand);
	m_GaugeDriveCommand.Update();

	m_GaugeMassflow.SetMassflow(m_fMassflow);
	m_GaugeMassflow.SetSetpoint(m_fSetpoint);
	m_GaugeMassflow.SetMaxSetpoint(fMaxSetpoint);
	m_GaugeMassflow.Update();
	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CPidRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		SetNumberFont(IDC_PID_RECIPE_NR);

		BOOL bProductListAvail = !(PRODUCTDATABASEREF.GetCount() == 0);
		SHOWW_ID(IDC_PID_RECIPE_NAME_BT, bProductListAvail);
		SHOWW_ID(IDC_PID_RECIPE_KEY_BT, bProductListAvail);

		SetValue();
	}
	return result;;
}
//***************************************************************************************
//***************************************************************************************
BOOL CPidRecipePage::OnSetActive()
{
	m_aLocalMode.Redraw();
	m_aVolGrav.Redraw();
	m_ProductListName.Redraw();
	m_ProductListKey.Redraw();
	return CDosePage::OnSetActive();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnBnClickedPidVolumetric()
{
	ToggleGravimetricMode();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnBnClickedPidLinie()
{
	ToggleLineMode();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnBnClickedPidTotalizerReset()
{
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION    | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnStnClickedPidName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_PID_RECIPE_NAME);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnStnClickedPidQmNumber()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_UINT64CTRL, IDC_PID_RECIPE_QMNUMBER);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CPidRecipePage::OnStnClickedPidSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_PID_RECIPE_SETPOINT);
}
//***************************************************************************************
//***************************************************************************************
void CPidRecipePage::OnStnClickedANnummer()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_PID_RECIPE_ANNUMBER);
}

//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CPidRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_PID_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_PID_RECIPE_REGENERAT :
                            {
                                if ( m_bRegenerat )
                                {
                                    // Set the text color to red
                                    pDC->SetTextColor(INDEXCOLOR);

                                    // Set the background mode for text to transparent 
                                    // so background will show thru.
                                    pDC->SetBkColor(REGENERATBKCOLOR);
                                    pDC->SetTextColor(REGENERATTEXTCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = c_aRegeneratBrush;
                                }
                            }
                            break;

		case IDC_PID_RECIPE_BITMAP:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;


        case IDC_PID_RECIPE_NAME:
        case IDC_PID_RECIPE_QMNUMBER:
        case IDC_PID_RECIPE_SETPOINT:
		case IDC_PID_RECIPE_ANNUMBER:
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
        default:    
                            break;
    }
    return hbr;
}
//***************************************************************************************
//***************************************************************************************
void CPidRecipePage::OnClickNameBt()
{
	CreateProductDatabaseBox(IDC_PID_RECIPE_NAME_BT, TRUE);
}
//***************************************************************************************
//***************************************************************************************
void CPidRecipePage::OnClickKeyBt()
{
	CreateProductDatabaseBox(IDC_PID_RECIPE_KEY_BT, FALSE);
}




