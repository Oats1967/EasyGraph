//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwbRecipePage
///
/// @file   LwbRecipePage.cpp
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
#include "BASE/Base.h"
#include "FormatMetric.h"
#include "RemoteControl.h"
#include "LwbRecipePage.h"
#include "DataImage.h"
#include "GlobalConst.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLwbRecipePage, _func)

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CLwbRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CLwbRecipePage::OnNotifyEdit)
	ON_BN_CLICKED(IDC_LWB_RECIPE_VOLUMETRIC, &CLwbRecipePage::OnBnClickedDosiererVolumetric)
	ON_BN_CLICKED(IDC_LWB_RECIPE_TOTALIZER_RESET, &CLwbRecipePage::OnBnClickedDosiererTotalizerReset)
	ON_STN_CLICKED(IDC_LWB_RECIPE_NAME, &CLwbRecipePage::OnStnClickedDosiererName)
	ON_STN_CLICKED(IDC_LWB_RECIPE_QMNUMBER, &CLwbRecipePage::OnStnClickedDosiererQmnummer)
	ON_STN_CLICKED(IDC_LWB_RECIPE_ANNUMBER, &CLwbRecipePage::OnStnClickedDosiererANnummer)
	ON_STN_CLICKED(IDC_LWB_RECIPE_BATCHSETPOINT, &CLwbRecipePage::OnStnClickedDosiererSetpoint)
	ON_BN_CLICKED(IDC_LWB_RECIPE_LINE, &CLwbRecipePage::OnBnClickedDosiererLinie)
	ON_BN_CLICKED(IDC_LWB_RECIPE_TOTALIZER_CTRL, &CLwbRecipePage::OnClickToolTipTotalizer)
	ON_BN_CLICKED(IDC_LWB_RECIPE_ROTSPEED_CTRL, &CLwbRecipePage::OnClickToolTipRotation)
	ON_BN_CLICKED(IDC_LWB_RECIPE_DRIVECOMMAND_CTRL, &CLwbRecipePage::OnClickToolTipDriveCommand)
	ON_BN_CLICKED(IDC_LWB_RECIPE_AGITATOR, &CLwbRecipePage::OnClickAgitator)
	ON_BN_CLICKED(IDC_LWB_RECIPE_NAME_BT, &CLwbRecipePage::OnClickNameBt)
	ON_BN_CLICKED(IDC_LWB_RECIPE_KEY_BT, &CLwbRecipePage::OnClickKeyBt)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CLwbRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CLwbRecipePage::CLwbRecipePage(): CDosePage(CLwbRecipePage::IDD)
	, c_EditMap({
		EDITITEM(IDC_LWB_RECIPE_BATCHSETPOINT,OnNotifyEditSetpoint),
		EDITITEM(IDC_LWB_RECIPE_NAME, OnNotifyEditName),
		EDITITEM(IDC_LWB_RECIPE_QMNUMBER, OnNotifyEditQMNumber),
		EDITITEM(IDC_LWB_RECIPE_ANNUMBER, OnNotifyEditANNumber)
		})

	, m_GaugeTotalizer(this)
	, m_GaugeDriveCommand(this)
	, m_GaugeRotSpeed(this)
	, m_aVolGrav()
	, m_aLocalMode()
	, m_Agitator()
	, c_RegeneratBrush(REGENERATBKCOLOR)
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, m_szActRotSpeed(_T(""))
	, m_fPercentage(0.0f)
	, m_fSetpoint(0.0f)
	, m_fDriveCommand(0)
	, m_fActRotSpeed(0)
	, m_fActWeight(0.0f)
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
	, m_fActTotalizer(0.0f)
    , m_bRegenerat(FALSE)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CLwbRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX,  IDC_LWB_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_LWB_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX,  IDC_LWB_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_LWB_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_LWB_RECIPE_ANNUMBER, m_szANNumber);
	DDX_Text(pDX, IDC_LWB_RECIPE_ROTATIONALSPEED, m_szActRotSpeed);
	DDX_Text(pDX, IDC_LWB_RECIPE_FEEDERTYPE, m_szFeederType);
	DDX_Float(pDX, IDC_LWB_RECIPE_BATCHSETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_LWB_RECIPE_PERCENTAGE, m_fPercentage);
	DDX_Float(pDX, IDC_LWB_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_FloatKgHR(pDX, IDC_LWB_RECIPE_ACTWEIGHT, m_fActWeight);
	DDX_Control(pDX, IDC_LWB_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_LWB_RECIPE_DRIVECOMMAND_CTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_LWB_RECIPE_TOTALIZER_CTRL, m_GaugeTotalizer);
	DDX_Control(pDX, IDC_LWB_RECIPE_ROTSPEED_CTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_LWB_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_LWB_RECIPE_VOLUMETRIC, m_aVolGrav);
	DDX_Control(pDX, IDC_LWB_RECIPE_AGITATOR, m_Agitator);
	DDX_Control(pDX, IDC_LWB_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_LWB_RECIPE_KEY_BT, m_ProductListKey);

}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLwbRecipePage::OnNotifyEditSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{		
		REMOTEREF.setDoseSetpoint(m_sItem, m_fSetpoint);
		CheckRefillMin();
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLwbRecipePage::OnNotifyEditANNumber(void)
{
	auto bModified = CEditCtrlEx::GetString(m_szANNumber);
	if (bModified)
	{
		REMOTEREF.setLineANNumber(toStdString(m_szANNumber));
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
void CLwbRecipePage::CheckRefillMin(void)
{
	float32_t fMin = 0.0F;
	REMOTEREF.getDoseRefillLimitMin(m_sItem, fMin);
	if (fMin < m_fSetpoint)
	{
		ECMessageBox(IDS_BATCH_GREATER_MIN, MB_OK | MB_ICONEXCLAMATION);
		fMin = m_fSetpoint;
		float32_t fMax = _F32(NEXTLONG(fMin * 1.1));
		float32_t fMinMin = 0.5F;
		REMOTEREF.setDoseRefillLimit(m_sItem, fMin, fMax, fMinMin);
	}
}
//***************************************************************************************
//***************************************************************************************
LRESULT CLwbRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			PostMessage(WM_TIMER_REFRESH);
		}
	}
	return 0;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CLwbRecipePage::SetControlStyle (void)
{
	BOOL bLogin					= TRUE;

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bVolumetric				    = ! processstatus.flags.gravimetric;
	const BOOL bLocalMode					= ! processstatus.flags.lineMode;
	const BOOL bStarted						= (processstatus.flags.started== true) || (processstatus.flags.running == true);
	const BOOL bLCEnable					= processstatus.flags.LCEnable;
	const BOOL bSlaveMode					= processstatus.flags.slaveMode;

	if (bLCEnable)
	{
		SHOWW_ID(IDC_LWB_RECIPE_VOLUMETRIC, TRUE);
		const BOOL bVolumetricExtern = processstatus.flags.volumetricext;
		m_aVolGrav.SetInputExt(bVolumetricExtern);
		m_aVolGrav.Enable(bLogin && (!bVolumetricExtern));
		m_aVolGrav.EnableWindow((!bVolumetricExtern));
		m_aVolGrav.SetCheck(bVolumetric);
		m_aVolGrav.Show();
		SHOWW_ID(IDC_LWB_RECIPE_ACTWEIGHT, TRUE);
		SHOWW_ID(IDC_LWB_RECIPE_ACTWEIGHT_STATIC, TRUE);
	}
	else
	{
		ASSERT(bVolumetric);
		SHOWW_ID(IDC_LWB_RECIPE_VOLUMETRIC, FALSE);
		SHOWW_ID(IDC_LWB_RECIPE_ACTWEIGHT, FALSE);
		SHOWW_ID(IDC_LWB_RECIPE_ACTWEIGHT_STATIC, FALSE);
	}

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_LWB_RECIPE_BATCHSETPOINT,    bLogin && bLocalMode);
    ENABLE_ID(IDC_LWB_RECIPE_NAME,        bLogin && bLocalMode);
    ENABLE_ID(IDC_LWB_RECIPE_QMNUMBER,     bLogin && bLocalMode);
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);

	auto bTotalizerReset = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET);
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_TOTALIZER_RESET, bLocalMode, bLocalMode && bTotalizerReset);

	BOOL bShowPercentage = (!bSlaveMode) && (!bLocalMode);
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_PERCENTAGE, bLogin && bShowPercentage, bShowPercentage);
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_PERCENTAGE_STATIC, bLogin && bShowPercentage, bShowPercentage);

	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bAgitatorEnable = deviceconfig.flags.agitatorEnable;
	ENABLE_SHOW_ID(IDC_LWB_RECIPE_AGITATOR, bAgitatorEnable, bAgitatorEnable);
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwbRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBATCHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	std::string ANNumber;
	REMOTEREF.getLineANNumber(ANNumber);
	m_szANNumber = toCString(ANNumber);

	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0, 100));
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);
	float32_t fMaxRotationalSpeed = 0.0F;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);
	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	m_fActTotalizer = _F32(ROUND2(m_fActTotalizer));

	REMOTEREF.getDoseLCMeanWeight(m_sItem, m_fActWeight);
	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	float32_t fMaxSetpoint = 0.0F;
	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
	REMOTEREF.getDosePercentage(m_sItem, m_fPercentage);
	m_fPercentage = _F32(ROUND2(m_fPercentage));
	REMOTEREF.getDoseRegenerat(m_sItem, m_bRegenerat);

	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	switch (m_sEncoderState)
	{
	default:
	case base::eEncoderState::ENCODERSTATE_OFF:
		VERIFY(m_szActRotSpeed.LoadString(IDS_AUS));
		break;

	case base::eEncoderState::ENCODERSTATE_RUN:
		m_szActRotSpeed.Format(_T("%d"), NEXTLONG(m_fActRotSpeed));
		break;

	case base::eEncoderState::ENCODERSTATE_STOP:
		m_szActRotSpeed.Format(_T("%d"), NEXTLONG(m_fActRotSpeed));
		break;
	}
	m_GaugeDriveCommand.SetDriveCommand(m_fDriveCommand);
	m_GaugeDriveCommand.Update();

	m_GaugeRotSpeed.SetRotSpeed(m_fActRotSpeed);
	m_GaugeRotSpeed.SetMaxRotSpeed(fMaxRotationalSpeed);
	m_GaugeRotSpeed.Update();

	m_GaugeTotalizer.SetBatchMaxSetpoint(ROUND0(m_fSetpoint * 1.2F));
	m_GaugeTotalizer.SetSetpoint(m_fSetpoint);
	m_GaugeTotalizer.SetBatchTotalizer(m_fActTotalizer);
	m_GaugeTotalizer.Update();

	if (m_Agitator.IsWindowVisible())
	{
		float32_t agitatorValue = 0.0F;
		REMOTEREF.getDoseAgitatorSetpoint(m_sItem, agitatorValue);
		m_Agitator.SetSpeed(agitatorValue);
	}

	SetControlStyle();
	return  CDosePage::OnUpdateControls();
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwbRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		SetNumberFont(IDC_LWB_RECIPE_NR);
		SetValue();
	}
	return result; 
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwbRecipePage::OnSetActive()
{
	m_aVolGrav.Redraw();
	m_aLocalMode.Redraw();
	m_Agitator.Redraw();
	m_ProductListName.Redraw();
	m_ProductListKey.Redraw();
	return CDosePage::OnSetActive();
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnBnClickedDosiererVolumetric()
{
	if (m_aVolGrav.IsEnable())
	{
		ToggleGravimetricMode();
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnBnClickedDosiererLinie()
{
	ASSERT(m_aLocalMode.IsEnable());
	ToggleLineMode();
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnBnClickedDosiererTotalizerReset()
{
	ASSERT( USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET));
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnStnClickedDosiererName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LWB_RECIPE_NAME);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnStnClickedDosiererSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWB_RECIPE_BATCHSETPOINT);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnStnClickedDosiererQmnummer()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LWB_RECIPE_QMNUMBER);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnStnClickedDosiererANnummer()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LWB_RECIPE_ANNUMBER);
}
//***************************************************************************************
//***************************************************************************************

HBRUSH CLwbRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LWB_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_LWB_RECIPE_REGENERAT :
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
                                    hbr = c_RegeneratBrush;
                                }
                            }
                            break;

        case IDC_LWB_RECIPE_ROTATIONALSPEED:
                            {
	                            ASSERT((_S32(m_sEncoderState) >= _S32(base::eEncoderState::ENCODERSTATE_OFF)) && (_S32(m_sEncoderState) < _S32(base::eEncoderState::ENCODERSTATE_MAX) ));
                                // Set the text color to red
                                pDC->SetTextColor(c_EncoderTextcolor[_S32(m_sEncoderState)]);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkColor(c_EncoderBkcolor[_S32(m_sEncoderState)]);
                                pDC->SetBkMode(OPAQUE);
                                hbr = c_Brush[_S32(m_sEncoderState)];
                            }
                            break;

		case IDC_LWB_RECIPE_FEEDERTYPE:
							{
								pDC->SetTextColor(EDITTEXTCOLOR);
								pDC->SetBkMode(TRANSPARENT);
								hbr = (HBRUSH)c_EditBrush;
							}
							break;

		case IDC_LWB_RECIPE_BITMAP:
                            {
								m_aGrafikContainer.Update(pDC);
							}
                            break;

        case IDC_LWB_RECIPE_NAME:
        case IDC_LWB_RECIPE_QMNUMBER:
        case IDC_LWB_RECIPE_BATCHSETPOINT:
		case IDC_LWB_RECIPE_ANNUMBER:
                            {
                                if ( pWnd->IsWindowEnabled() )
                                {
                                    pDC->SetTextColor(EDITTEXTCOLOR);
                                    //pDC->SetBkColor(REGENERATBKCOLOR);
									pDC->SetBkMode(TRANSPARENT);
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
void CLwbRecipePage::OnClickToolTipTotalizer()
{
	CreateToolTip(IDC_LWB_RECIPE_TOTALIZER_CTRL, IDS_TOTALIZER_KG);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnClickToolTipDriveCommand()
{
	CreateToolTip(IDC_LWB_RECIPE_DRIVECOMMAND_CTRL, IDS_DRIVECOMMAND_PERCENTAGE);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnClickToolTipRotation()
{
	CreateToolTip(IDC_LWB_RECIPE_ROTSPEED_CTRL, IDS_ROTSPEED_UPM);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnClickAgitator()
{
	CreateAgitatorBox(IDC_LWB_RECIPE_AGITATOR, IDS_AGITATOR_SPEED);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnClickNameBt()
{
	CreateProductDatabaseBox(IDC_LWB_RECIPE_NAME_BT, TRUE);
}
//***************************************************************************************
//***************************************************************************************
void CLwbRecipePage::OnClickKeyBt()
{
	CreateProductDatabaseBox(IDC_LWB_RECIPE_KEY_BT, FALSE);
}









