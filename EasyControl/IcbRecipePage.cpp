//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IcbRecipePage.cpp
///
/// @file   IcbRecipePage.cpp
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
#include "IcbRecipePage.h"
#include "DataImage.h"
#include "GlobalConst.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"




#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIcbRecipePage, _func)
//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CIcbRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CIcbRecipePage::OnNotifyEdit)
	ON_BN_CLICKED(IDC_ICB_RECIPE_TOTALIZER_RESET, &CIcbRecipePage::OnBnClickedDosiererTotalizerReset)
	ON_STN_CLICKED(IDC_ICB_RECIPE_NAME, &CIcbRecipePage::OnStnClickedDosiererName)
	ON_STN_CLICKED(IDC_ICB_RECIPE_QMNUMBER, &CIcbRecipePage::OnStnClickedDosiererQmnummer)
	ON_STN_CLICKED(IDC_ICB_RECIPE_BATCHSETPOINT, &CIcbRecipePage::OnStnClickedDosiererSetpoint)
	ON_BN_CLICKED(IDC_ICB_RECIPE_LINE, &CIcbRecipePage::OnBnClickedDosiererLinie)
	ON_BN_CLICKED(IDC_ICB_RECIPE_TOTALIZER_CTRL, &CIcbRecipePage::OnClickToolTipTotalizer)
	ON_BN_CLICKED(IDC_ICB_RECIPE_ROTSPEED_CTRL, &CIcbRecipePage::OnClickToolTipRotation)
	ON_BN_CLICKED(IDC_ICB_RECIPE_DRIVECOMMAND_CTRL, &CIcbRecipePage::OnClickToolTipDriveCommand)
	ON_BN_CLICKED(IDC_ICB_RECIPE_AGITATOR, &CIcbRecipePage::OnClickAgitator)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CIcbRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CIcbRecipePage::CIcbRecipePage(): CDosePage(CIcbRecipePage::IDD)
	, c_EditMap({
		EDITITEM(IDC_ICB_RECIPE_BATCHSETPOINT, OnNotifyEditSetpoint),
		EDITITEM(IDC_ICB_RECIPE_NAME, OnNotifyEditName),
		EDITITEM(IDC_ICB_RECIPE_QMNUMBER, OnNotifyEditQMNumber)
	})
	, m_GaugeTotalizer(this)
	, m_GaugeDriveCommand(this)
	, m_GaugeRotSpeed(this)
	, m_aLocalMode()
	, m_Agitator()
	, c_RegeneratBrush(REGENERATBKCOLOR)
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,    RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,  RGBCOLORBLACK }
	, m_szActRotSpeed(_T(""))
	, m_fPercentage(0.0f)
	, m_fSetpoint(0.0f)
	, m_fDriveCommand(0)
	, m_fActRotSpeed(0)
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
	, m_fActTotalizer(0.0f)
    , m_bRegenerat(FALSE)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CIcbRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ICB_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_ICB_RECIPE_NAME, m_szName,30);
	DDX_Text(pDX, IDC_ICB_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_ICB_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_ICB_RECIPE_ROTATIONALSPEED, m_szActRotSpeed);
	DDX_Text(pDX, IDC_ICB_RECIPE_FEEDERTYPE, m_szFeederType);
	DDX_Float(pDX, IDC_ICB_RECIPE_BATCHSETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_ICB_RECIPE_PERCENTAGE, m_fPercentage);
	DDX_Float(pDX, IDC_ICB_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Control(pDX, IDC_ICB_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_ICB_RECIPE_DRIVECOMMAND_CTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_ICB_RECIPE_TOTALIZER_CTRL, m_GaugeTotalizer);
	DDX_Control(pDX, IDC_ICB_RECIPE_ROTSPEED_CTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_ICB_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_ICB_RECIPE_AGITATOR, m_Agitator);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIcbRecipePage::OnNotifyEditSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{		
		REMOTEREF.setDoseSetpoint(m_sItem, m_fSetpoint);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CIcbRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
	return 0L;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIcbRecipePage::SetControlStyle (void)
{
	BOOL bLogin					= TRUE;

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bVolumetricExtern			= processstatus.flags.volumetricext;
	const BOOL bVolumetric				    = ! processstatus.flags.gravimetric;
	const BOOL bLocalMode					= ! processstatus.flags.lineMode;
	const BOOL bStarted						= (processstatus.flags.started== true) || (processstatus.flags.running == true);
	const BOOL bSlaveMode					= processstatus.flags.slaveMode;

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_ICB_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_ICB_RECIPE_BATCHSETPOINT,    bLogin && bLocalMode);
    ENABLE_ID(IDC_ICB_RECIPE_NAME,        bLogin && bLocalMode);
    ENABLE_ID(IDC_ICB_RECIPE_QMNUMBER,     bLogin && bLocalMode);
	ENABLE_SHOW_ID(IDC_ICB_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);

	auto bTotalizerReset = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET);
	ENABLE_SHOW_ID(IDC_ICB_RECIPE_TOTALIZER_RESET, bLocalMode, bLocalMode && bTotalizerReset);

	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bAgitatorEnable = deviceconfig.flags.agitatorEnable;
	ENABLE_SHOW_ID(IDC_ICB_RECIPE_AGITATOR, bAgitatorEnable, bAgitatorEnable);
}
//***************************************************************************************
//***************************************************************************************
BOOL CIcbRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISICBTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0, 100));
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);
	float32_t fMaxRotationalSpeed = 0.0F;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);
	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	m_fActTotalizer = _F32(ROUND2(m_fActTotalizer));
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

#if 0
	case base::eEncoderState::ENCODERSTATE_AUTO:
		VERIFY(m_szActRotSpeed.LoadString(IDS_RUN));
		break;
#endif
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

	SetControlStyle();
	return  CDosePage::OnUpdateControls();
}
//***************************************************************************************
//***************************************************************************************
BOOL CIcbRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		SetNumberFont(IDC_ICB_RECIPE_NR);
		SetValue();
	}
	return result; 
}
//***************************************************************************************
//***************************************************************************************
BOOL CIcbRecipePage::OnSetActive()
{
	m_aLocalMode.Redraw();
	m_Agitator.Redraw();
	return CDosePage::OnSetActive();
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnBnClickedDosiererLinie()
{
	ASSERT(m_aLocalMode.IsEnable());
	ToggleLineMode();
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnBnClickedDosiererTotalizerReset()
{
	ASSERT( USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET));
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnStnClickedDosiererName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_ICB_RECIPE_NAME);
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnStnClickedDosiererSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_ICB_RECIPE_BATCHSETPOINT);
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnStnClickedDosiererQmnummer()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_ICB_RECIPE_QMNUMBER);
}
//***************************************************************************************
//***************************************************************************************
HBRUSH CIcbRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_ICB_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_ICB_RECIPE_REGENERAT :
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

        case IDC_ICB_RECIPE_ROTATIONALSPEED:
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

		case IDC_ICB_RECIPE_FEEDERTYPE:
							{
								pDC->SetTextColor(EDITTEXTCOLOR);
								pDC->SetBkMode(TRANSPARENT);
								hbr = (HBRUSH)c_EditBrush;
							}
							break;

		case IDC_ICB_RECIPE_BITMAP:
                            {
								m_aGrafikContainer.Update(pDC);
							}
                            break;

        case IDC_ICB_RECIPE_NAME:
        case IDC_ICB_RECIPE_QMNUMBER:
        case IDC_ICB_RECIPE_BATCHSETPOINT:
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
void CIcbRecipePage::OnClickToolTipTotalizer()
{
	CreateToolTip(IDC_ICB_RECIPE_TOTALIZER_CTRL, IDS_TOTALIZER_KG);
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnClickToolTipDriveCommand()
{
	CreateToolTip(IDC_ICB_RECIPE_DRIVECOMMAND_CTRL, IDS_DRIVECOMMAND_PERCENTAGE);
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnClickToolTipRotation()
{
	CreateToolTip(IDC_ICB_RECIPE_ROTSPEED_CTRL, IDS_ROTSPEED_UPM);
}
//***************************************************************************************
//***************************************************************************************
void CIcbRecipePage::OnClickAgitator()
{
	CreateAgitatorBox(IDC_ICB_RECIPE_AGITATOR, IDS_AGITATOR_SPEED);
}








