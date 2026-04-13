//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IcpRecipePage
///
/// @file   IcpRecipePage.cpp
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
#include "IcpRecipePage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIcpRecipePage, _func)

//****************************************************************************************
//****************************************************************************************
BEGIN_MESSAGE_MAP(CIcpRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_ICP_RECIPE_TOTALIZERRESET, &CIcpRecipePage::OnBnClickedIcpTotalizerReset)
	ON_STN_CLICKED(IDC_ICP_RECIPE_NAME, &CIcpRecipePage::OnStnClickedIcpName)
	ON_STN_CLICKED(IDC_ICP_RECIPE_ANNUMBER, &CIcpRecipePage::OnStnClickedDosiererANnummer)
	ON_STN_CLICKED(IDC_ICP_RECIPE_QMNUMBER, &CIcpRecipePage::OnStnClickedIcpQmnummer)
	ON_STN_CLICKED(IDC_ICP_RECIPE_SETPOINT, &CIcpRecipePage::OnStnClickedIcpSetpoint)
	ON_BN_CLICKED(IDC_ICP_RECIPE_LINE, &CIcpRecipePage::OnBnClickedIcpLine)
	ON_BN_CLICKED(IDC_ICP_RECIPE_MASSFLOWCTRL, &CIcpRecipePage::OnClickToolTipMassflow)
	ON_BN_CLICKED(IDC_ICP_RECIPE_DRIVECOMMANDCTRL, &CIcpRecipePage::OnClickToolTipDriveCommand)
	ON_BN_CLICKED(IDC_ICP_RECIPE_NAME_BT, &CIcpRecipePage::OnClickNameBt)
	ON_BN_CLICKED(IDC_ICP_RECIPE_KEY_BT, &CIcpRecipePage::OnClickKeyBt)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//****************************************************************************************
//****************************************************************************************
IMPLEMENT_DYNAMIC(CIcpRecipePage, CDosePage);
//****************************************************************************************
//****************************************************************************************
 CIcpRecipePage::CIcpRecipePage(): CDosePage(CIcpRecipePage::IDD)
	 , m_GaugeMassflow(this)
	 , m_GaugeDriveCommand(this)
	 , m_aLocalMode()
	 , m_ProductListName()
	 , m_ProductListKey()
	 , m_fSetpoint(0)
	 , m_fMassflow(0)
	 , m_fDriveCommand(0)
     , m_bRegenerat(FALSE)
	 , m_fPercentage(0.0f)
	 , m_fActTotalizer(0.0f)
     , m_bLocalMode(FALSE)
	 , m_aRegeneratBrush{ REGENERATBKCOLOR }
	 , m_EditMap({
					 EDITITEM(IDC_ICP_RECIPE_SETPOINT, OnNotifySetpoint),
					 EDITITEM(IDC_ICP_RECIPE_NAME,  OnNotifyEditName),
					 EDITITEM(IDC_ICP_RECIPE_QMNUMBER,	OnNotifyEditQMNumber),
					 EDITITEM(IDC_ICP_RECIPE_ANNUMBER,	OnNotifyEditANNumber)
		 })
 {
 }
 //****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ICP_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_ICP_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_ICP_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_ICP_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_ICP_RECIPE_ANNUMBER, m_szANNumber);
	DDX_Text(pDX, IDC_ICP_RECIPE_FEEDERTYPE, m_szFeederType);
	DDX_Float(pDX, IDC_ICP_RECIPE_SETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_ICP_RECIPE_PERCENTAGE, m_fPercentage);
	DDX_Float(pDX, IDC_ICP_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Control(pDX, IDC_ICP_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_ICP_RECIPE_DRIVECOMMANDCTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_ICP_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_ICP_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_ICP_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_ICP_RECIPE_KEY_BT, m_ProductListKey);
}
//****************************************************************************************
//****************************************************************************************
BOOL CIcpRecipePage::OnNotifySetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseSetpoint(m_sItem, m_fSetpoint);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIcpRecipePage::OnNotifyEditANNumber(void)
{
	auto bModified = CEditCtrlEx::GetString(m_szANNumber);
	if (bModified)
	{
		REMOTEREF.setLineANNumber(toStdString(m_szANNumber));
	}
	return bModified;
}
//****************************************************************************************
//****************************************************************************************
LRESULT CIcpRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::SetControlStyle (void)
{
	BOOL bLogin					    = TRUE;

	base::ProcessStatus pulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, pulStatus);
	const BOOL bLocalMode = pulStatus.flags.lineMode == false;
	const BOOL bStarted = pulStatus.flags.started;
	const BOOL bSlaveMode = pulStatus.flags.slaveMode;

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_ICP_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_ICP_RECIPE_NAME, (bLogin) && (bLocalMode ));
    ENABLE_ID(IDC_ICP_RECIPE_QMNUMBER, (bLogin) && (bLocalMode ));
	ENABLE_ID(IDC_ICP_RECIPE_ANNUMBER, bLogin);
    ENABLE_ID(IDC_ICP_RECIPE_SETPOINT, (bLogin) && (bLocalMode ));
	ENABLE_ID(IDC_ICP_RECIPE_NAME_BT, bLogin);
	ENABLE_ID(IDC_ICP_RECIPE_KEY_BT, bLogin);
    ENABLE_SHOW_ID(IDC_ICP_RECIPE_TOTALIZERRESET, bLocalMode , bLocalMode );
    ENABLE_SHOW_ID(IDC_ICP_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);

	BOOL bShowPercentage = (!bSlaveMode) && (!bLocalMode);
	ENABLE_SHOW_ID(IDC_ICP_RECIPE_PERCENTAGE, bLogin && bShowPercentage, bShowPercentage);
	ENABLE_SHOW_ID(IDC_ICP_RECIPE_PERCENTAGE_STATIC, bLogin && bShowPercentage, bShowPercentage);

}
//****************************************************************************************
//****************************************************************************************
BOOL CIcpRecipePage::OnUpdateControls(void)
{
	if ((m_sItem < 0) || (!__ISICPTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	std::string ANNumber;
	REMOTEREF.getLineANNumber(ANNumber);
	m_szANNumber = toCString(ANNumber);

	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0, 100));
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	REMOTEREF.getDosePercentage(m_sItem, m_fPercentage);
	REMOTEREF.getDoseRegenerat(m_sItem, m_bRegenerat);
	float32_t fMaxSetpoint = 0.0F;
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
//****************************************************************************************
//****************************************************************************************
BOOL CIcpRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		SetNumberFont(IDC_ICP_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CIcpRecipePage::OnSetActive()
{
	m_aLocalMode.Redraw();
	m_ProductListName.Redraw();
	m_ProductListKey.Redraw();
	return CDosePage::OnSetActive();
}

//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::OnBnClickedIcpTotalizerReset()
{
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION    | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::OnStnClickedIcpName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_ICP_RECIPE_NAME);
}
//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::OnStnClickedIcpQmnummer()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_ICP_RECIPE_QMNUMBER);
}
//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::OnStnClickedIcpSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_ICP_RECIPE_SETPOINT);
}
//***************************************************************************************
//***************************************************************************************
void CIcpRecipePage::OnStnClickedDosiererANnummer()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_ICP_RECIPE_ANNUMBER);
}
//****************************************************************************************
//****************************************************************************************
void CIcpRecipePage::OnBnClickedIcpLine()
{
	ToggleLineMode();
}
//****************************************************************************************
//****************************************************************************************
HBRUSH CIcpRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_ICP_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_ICP_RECIPE_REGENERAT :
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
                            hbr = m_aRegeneratBrush;
                        }
                    }
                    break;

		case IDC_ICP_RECIPE_BITMAP:
					{
						m_aGrafikContainer.Update(pDC);
					}
					break;


		case IDC_ICP_RECIPE_FEEDERTYPE:
					{
						pDC->SetTextColor(EDITTEXTCOLOR);
						pDC->SetBkMode(TRANSPARENT);
						hbr = (HBRUSH)c_EditBrush;
					}
					break;

        case IDC_ICP_RECIPE_NAME:
        case IDC_ICP_RECIPE_QMNUMBER:
		case IDC_ICP_RECIPE_ANNUMBER:
		case IDC_ICP_RECIPE_SETPOINT:
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
void CIcpRecipePage::OnClickToolTipMassflow()
{
	CreateToolTip(IDC_ICP_RECIPE_MASSFLOWCTRL, IDS_MASSFLOW_KGH);
}
//***************************************************************************************
//***************************************************************************************
void CIcpRecipePage::OnClickToolTipDriveCommand()
{
	CreateToolTip(IDC_ICP_RECIPE_DRIVECOMMANDCTRL, IDS_DRIVECOMMAND_PERCENTAGE);
}
//***************************************************************************************
//***************************************************************************************
void CIcpRecipePage::OnClickNameBt()
{
	CreateProductDatabaseBox(IDC_ICP_RECIPE_NAME_BT, TRUE);
}
//***************************************************************************************
//***************************************************************************************
void CIcpRecipePage::OnClickKeyBt()
{
	CreateProductDatabaseBox(IDC_ICP_RECIPE_KEY_BT, FALSE);
}

