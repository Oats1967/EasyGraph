//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfRecipePage.cpp
///
/// @file   LwfRecipePage.cpp
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
#include "LwfRecipePage.h"
#include "FeederScaleBoxDlg.h"
#include "RefillGravBoxDlg.h"
#include "DataImage.h"
#include "GlobalConst.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLwfRecipePage, _func)

//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CLwfRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, &CLwfRecipePage::OnNotifyEdit)
	ON_MESSAGE(WM_DESTROYSCALEBOX, &CLwfRecipePage::OnDestroyScaleBox)
	ON_BN_CLICKED(IDC_LWF_RECIPE_VOLUMETRIC, &CLwfRecipePage::OnBnClickedDosiererVolumetric)
	ON_BN_CLICKED(IDC_LWF_RECIPE_TOTALIZER_RESET, &CLwfRecipePage::OnBnClickedDosiererTotalizerReset)
	ON_STN_CLICKED(IDC_LWF_RECIPE_NAME, &CLwfRecipePage::OnStnClickedDosiererName)
	ON_STN_CLICKED(IDC_LWF_RECIPE_QMNUMBER, &CLwfRecipePage::OnStnClickedDosiererQmnummer)
	ON_STN_CLICKED(IDC_LWF_RECIPE_ANNUMBER, &CLwfRecipePage::OnStnClickedDosiererANnummer)
	ON_STN_CLICKED(IDC_LWF_RECIPE_SETPOINT, &CLwfRecipePage::OnStnClickedDosiererSetpoint)
	ON_STN_CLICKED(IDC_LWF_RECIPE_ACTWEIGHT_STATIC, &CLwfRecipePage::OnStnClickedDosiererScale)
	ON_STN_CLICKED(IDC_LWF_RECIPE_BITMAP, &CLwfRecipePage::OnStnClickedDosiererRefill)
	ON_BN_CLICKED(IDC_LWF_RECIPE_LINE, &CLwfRecipePage::OnBnClickedDosiererLinie)
	ON_BN_CLICKED(IDC_LWF_RECIPE_MASSFLOWCTRL, &CLwfRecipePage::OnClickToolTipMassflow)
	ON_BN_CLICKED(IDC_LWF_RECIPE_ROTSPEEDCTRL, &CLwfRecipePage::OnClickToolTipRotation)
	ON_BN_CLICKED(IDC_LWF_RECIPE_DRIVECOMMANDCTRL, &CLwfRecipePage::OnClickToolTipDriveCommand)
	ON_BN_CLICKED(IDC_LWF_RECIPE_REFILL_RELEASE, &CLwfRecipePage::OnBnClickedRefillRelease)
	ON_BN_CLICKED(IDC_LWF_RECIPE_AGITATOR, &CLwfRecipePage::OnClickAgitator)
	ON_BN_CLICKED(IDC_LWF_RECIPE_NAME_BT, &CLwfRecipePage::OnClickNameBt)
	ON_BN_CLICKED(IDC_LWF_RECIPE_KEY_BT, &CLwfRecipePage::OnClickKeyBt)
	ON_BN_CLICKED(IDC_LWF_RECIPE_FEEDERTYPE, &CLwfRecipePage::OnClickedFeederTypeInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CLwfRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CLwfRecipePage::CLwfRecipePage(): CDosePage(CLwfRecipePage::IDD)
	, m_GaugeMassflow(this)
	, m_GaugeDriveCommand(this)
	, m_GaugeRotSpeed(this)
	, m_RefillRelease()
	, m_aVolGrav()
	, m_aLocalMode()
	, m_Agitator()
	, m_ProductListName()
	, m_ProductListKey()
	, c_RegeneratBrush(REGENERATBKCOLOR)
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, m_szActRotSpeed(_T(""))
	, m_fPercentage(0.0f)
	, m_fSetpoint(0.0f)
	, m_fMassflow(0.0f)
	, m_fDriveCommand(0)
	, m_fActRotSpeed(0)
	, m_fActWeight(0.0f)
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
	, m_fActTotalizer(0.0f)
	, m_fActDoseperformance(0.0f)
    , m_bRegenerat(FALSE)
	, m_EditMap({
			EDITITEM(IDC_LWF_RECIPE_SETPOINT,	OnNotifyEditSetpoint),
			EDITITEM(IDC_LWF_RECIPE_NAME, OnNotifyEditName),
			EDITITEM(IDC_LWF_RECIPE_QMNUMBER,	OnNotifyEditQMNumber),
			EDITITEM(IDC_LWF_RECIPE_ANNUMBER,	OnNotifyEditANNumber),
		})
{
}
//******************************************************************************************************
//******************************************************************************************************
void CLwfRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_LWF_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_LWF_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_LWF_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_LWF_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_LWF_RECIPE_ANNUMBER, m_szANNumber);
	DDX_Text(pDX, IDC_LWF_RECIPE_ROTATIONALSPEED, m_szActRotSpeed);
	DDX_Text(pDX, IDC_LWF_RECIPE_FEEDERTYPE, m_szFeederType);
	DDX_Float(pDX, IDC_LWF_RECIPE_SETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_LWF_RECIPE_PERCENTAGE, m_fPercentage);
	DDX_Float(pDX, IDC_LWF_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Float(pDX, IDC_LWF_RECIPE_DOSEPERFORMANCE, m_fActDoseperformance);
	DDX_FloatHR(pDX, IDC_LWF_RECIPE_ACTWEIGHT, m_fActWeight);
	DDX_Control(pDX, IDC_LWF_RECIPE_REFILL_RELEASE, m_RefillRelease);
	DDX_Control(pDX, IDC_LWF_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_LWF_RECIPE_DRIVECOMMANDCTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_LWF_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_LWF_RECIPE_ROTSPEEDCTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_LWF_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_LWF_RECIPE_VOLUMETRIC, m_aVolGrav);
	DDX_Control(pDX, IDC_LWF_RECIPE_AGITATOR, m_Agitator);
	DDX_Control(pDX, IDC_LWF_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_LWF_RECIPE_KEY_BT, m_ProductListKey);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLwfRecipePage::OnNotifyEditSetpoint(void)
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
BOOL CLwfRecipePage::OnNotifyEditANNumber(void)
{
	auto bModified = CEditCtrlEx::GetString(m_szANNumber);
	if (bModified)
	{
		REMOTEREF.setLineANNumber(toStdString(m_szANNumber));
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CLwfRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CLwfRecipePage::SetControlStyle (void)
{
	BOOL bLogin					= TRUE;

	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL bRefillState = status.flags.refillactive;
	const BOOL bRefillIOReleaseInputern = status.flags.refillreleaseextI;
	const BOOL bRefillRelease = status.flags.refillrelease;

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bVolumetric				    = ! processstatus.flags.gravimetric;
	const BOOL bLocalMode					= ! processstatus.flags.lineMode;
	const BOOL bStarted						= (processstatus.flags.started== true) || (processstatus.flags.running == true);
	const BOOL bLCAktiv						= processstatus.flags.LCActive;
	const BOOL bLCEnable					= processstatus.flags.LCEnable;
	const BOOL bSlaveMode					= processstatus.flags.slaveMode;
	const BOOL bRefillControlEnable			= processstatus.flags.refillControlEnable;
	const BOOL bFeederEmpty					= processstatus.flags.emptyfeederActive;
	const BOOL bTaring						= processstatus.flags.tareActive;

	ENABLE_ID(IDC_LWF_RECIPE_BITMAP, bLCAktiv);

	m_RefillRelease.SetCheck(bRefillRelease);
	m_RefillRelease.SetInputExt(bRefillIOReleaseInputern);
	m_RefillRelease.Enable(bRefillControlEnable);
	m_RefillRelease.EnableWindow((!bFeederEmpty) && (!bTaring));
	m_RefillRelease.Show();

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_LWF_RECIPE_SETPOINT,		bLogin && bLocalMode);
    ENABLE_ID(IDC_LWF_RECIPE_NAME,			bLogin);
    ENABLE_ID(IDC_LWF_RECIPE_QMNUMBER,		bLogin);
	ENABLE_ID(IDC_LWF_RECIPE_ANNUMBER,		bLogin);
	ENABLE_ID(IDC_LWF_RECIPE_NAME_BT,		bLogin);
	ENABLE_ID(IDC_LWF_RECIPE_KEY_BT,		bLogin);
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);

	auto bTotalizerReset = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET);
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_TOTALIZER_RESET, bLocalMode, bLocalMode && bTotalizerReset);

	BOOL bShowPercentage = (!bSlaveMode) && (!bLocalMode);
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_PERCENTAGE, bLogin && bShowPercentage, bShowPercentage);
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_PERCENTAGE_STATIC, bLogin && bShowPercentage, bShowPercentage);

	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bAgitatorEnable = deviceconfig.flags.agitatorEnable;
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_AGITATOR, bAgitatorEnable, bAgitatorEnable);


	base::eDoseType eType = base::eDoseType::DOSETYPE_NONE;
	REMOTEREF.getDoseType(m_sItem, eType);
	const BOOL bScaleShow   = __ISLOADCELLTYPE(eType);
	const BOOL bScaleEnable = bLCAktiv;
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_ACTWEIGHT_STATIC, bScaleShow, bScaleShow);
	ENABLE_SHOW_ID(IDC_LWF_RECIPE_ACTWEIGHT, bScaleShow, bScaleShow);
	if (bScaleEnable)
	{
		SHOWW_ID(IDC_LWF_RECIPE_VOLUMETRIC, TRUE);
		const BOOL bVolumetricExtern = processstatus.flags.volumetricext;
		m_aVolGrav.SetInputExt(bVolumetricExtern);
		m_aVolGrav.Enable(bLogin && (!bVolumetricExtern));
		m_aVolGrav.EnableWindow((!bVolumetricExtern));
		m_aVolGrav.SetCheck(bVolumetric);
		m_aVolGrav.Show();
	}
	else
	{
		//ASSERT(bVolumetric);
		SHOWW_ID(IDC_LWF_RECIPE_VOLUMETRIC, FALSE);
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::CheckQMaxfaktor (void)
{
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bLocalMode = ! processstatus.flags.lineMode;

	if ( bLocalMode  )
	{
		if ( m_fSetpoint  > m_fActDoseperformance )
		{
			ECMessageBox(IDS_SETPOINT_GREATER_DOSEPERFORMANCE, MB_OK | MB_ICONEXCLAMATION);
		}
		float32_t fTemp = 0.0f;
		REMOTEREF.getDoseMaxSetpoint(m_sItem, fTemp);
		if ( m_fSetpoint > fTemp )
		{
			ECMessageBox(IDS_SETPOINT_GREATER_MAXSETPOINT, MB_OK | MB_ICONEXCLAMATION);
		}
	}
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwfRecipePage::OnUpdateControls (void)
{
	ASSERT(__ISLOADCELLTYPE(m_lDoseType));
	if ((m_sItem < 0) || (!__ISLOADCELLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	std::string ANNumber;
	REMOTEREF.getLineANNumber(ANNumber);
	m_szANNumber = toCString(ANNumber);


	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0, 100));
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);
	float32_t fMaxRotationalSpeed = 0.0F;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);
	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	m_fActTotalizer = _F32(ROUND2(m_fActTotalizer));
	REMOTEREF.getDoseActualDoseperformance(m_sItem, m_fActDoseperformance);
	m_fActDoseperformance = _F32(ROUND2(m_fActDoseperformance));
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
	ASSERT(__ISLWFTYPE(m_lDoseType));

	m_GaugeDriveCommand.SetDriveCommand(m_fDriveCommand);
	m_GaugeDriveCommand.Update();

	m_GaugeRotSpeed.SetRotSpeed(m_fActRotSpeed);
	m_GaugeRotSpeed.SetMaxRotSpeed(fMaxRotationalSpeed);
	m_GaugeRotSpeed.Update();

	m_GaugeMassflow.SetMassflow(m_fMassflow);
	m_GaugeMassflow.SetSetpoint(m_fSetpoint);
	m_GaugeMassflow.SetMaxSetpoint(fMaxSetpoint);
	m_GaugeMassflow.Update();

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
BOOL CLwfRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_RefillRelease);

		BOOL bProductListAvail = (PRODUCTDATABASEREF.GetCount() != 0);
		SHOWW_ID(IDC_LWF_RECIPE_NAME_BT, bProductListAvail);
		SHOWW_ID(IDC_LWF_RECIPE_KEY_BT, bProductListAvail);

		SetNumberFont(IDC_LWF_RECIPE_NR);
		SetValue();
	}
	return result; 
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwfRecipePage::OnSetActive()
{
	m_aVolGrav.Redraw();
	m_aLocalMode.Redraw();
	m_Agitator.Redraw();
	m_ProductListName.Redraw();
	m_ProductListKey.Redraw();
	m_RefillRelease.Redraw();

	return CDosePage::OnSetActive();
}
//***************************************************************************************
//***************************************************************************************
LRESULT CLwfRecipePage::OnDestroyScaleBox(WPARAM , LPARAM )
{
	SetFocus();
	return 0L;
}
//***************************************************************************************
//***************************************************************************************
BOOL CLwfRecipePage::OnKillActive()
{
	CFeederScaleBoxDlg::DestroyScaleBox();
	CRefillGravBoxDlg::DestroyRefillBox();
	return CDosePage::OnKillActive();
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnBnClickedDosiererVolumetric()
{
	if (m_aVolGrav.IsEnable())
	{
		ToggleGravimetricMode();
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnBnClickedDosiererLinie()
{
	ASSERT(m_aLocalMode.IsEnable());
	ToggleLineMode();
}
//*************************************************************************************
//*************************************************************************************
void CLwfRecipePage::OnBnClickedRefillRelease()
{
	if (m_RefillRelease.IsEnable())
	{
		ToggleReleaseMode();
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnBnClickedDosiererTotalizerReset()
{
	ASSERT( USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET));
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LWF_RECIPE_NAME);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LWF_RECIPE_SETPOINT);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererQmnummer()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_UINT64CTRL, IDC_LWF_RECIPE_QMNUMBER);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererANnummer()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_LWF_RECIPE_ANNUMBER);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererScale()
{
	auto pWnd = GetDlgItem(IDC_LWF_RECIPE_BITMAP);
	ASSERT(pWnd);
	CRect aRect;
	pWnd->GetWindowRect(aRect);
	CFeederScaleBoxDlg::CreateScaleBox(this, m_sItem, CPoint{ aRect.right, aRect.top });
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnStnClickedDosiererRefill()
{
	base::DeviceConfig cfg;
	REMOTEREF.getDoseDeviceConfig(m_sItem, cfg);
	if (cfg.flags.moduleRefillEnable)
	{
		auto pWnd = GetDlgItem(IDC_LWF_RECIPE_BITMAP);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(aRect);
		CRefillGravBoxDlg::CreateRefillBox(this, m_sItem, CPoint{ aRect.right, aRect.top }, IDS_REFILLHEADER);
	}
}
//***************************************************************************************
//***************************************************************************************
HBRUSH CLwfRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_LWF_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_LWF_RECIPE_REGENERAT :
                            {
                                if ( m_bRegenerat )
                                {
                                    // Set the background mode for text to transparent 
                                    // so background will show thru.
                                    pDC->SetBkColor(REGENERATBKCOLOR);
                                    pDC->SetTextColor(REGENERATTEXTCOLOR);
                                    pDC->SetBkMode(OPAQUE);
                                    hbr = c_RegeneratBrush;
                                }
                            }
                            break;

        case IDC_LWF_RECIPE_ROTATIONALSPEED:
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

		case IDC_LWF_RECIPE_FEEDERTYPE:
							{
								pDC->SetTextColor(EDITTEXTCOLOR);
								pDC->SetBkMode(TRANSPARENT);
								//hbr = (HBRUSH)c_EditBrush;
							}
							break;

		case IDC_LWF_RECIPE_BITMAP:
                            {
								m_aGrafikContainer.Update(pDC);
							}
                            break;

		case IDC_LWF_RECIPE_ACTWEIGHT_STATIC:
		case IDC_LWF_RECIPE_NAME:
        case IDC_LWF_RECIPE_QMNUMBER:
        case IDC_LWF_RECIPE_SETPOINT:
		case IDC_LWF_RECIPE_ANNUMBER:
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
void CLwfRecipePage::OnClickToolTipMassflow()
{
	CreateToolTip(IDC_LWF_RECIPE_MASSFLOWCTRL, IDS_MASSFLOW_KGH);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnClickToolTipDriveCommand()
{
	CreateToolTip(IDC_LWF_RECIPE_DRIVECOMMANDCTRL, IDS_DRIVECOMMAND_PERCENTAGE);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnClickToolTipRotation()
{
	CreateToolTip(IDC_LWF_RECIPE_ROTSPEEDCTRL, IDS_ROTSPEED_UPM);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnClickAgitator()
{
	CreateAgitatorBox(IDC_LWF_RECIPE_AGITATOR, IDS_AGITATOR_SPEED);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnClickNameBt()
{
	CreateProductDatabaseBox(IDC_LWF_RECIPE_NAME_BT, TRUE);
}
//***************************************************************************************
//***************************************************************************************
void CLwfRecipePage::OnClickKeyBt()
{
	CreateProductDatabaseBox(IDC_LWF_RECIPE_KEY_BT, FALSE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CLwfRecipePage::OnClickedFeederTypeInfo()
{
	CreateHelpInfoBox(IDC_LWF_RECIPE_FEEDERTYPE, IDS_FEEDERTYPE, IDS_INFO_FEEDERTYPE);
}




