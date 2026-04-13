//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module VolRecipePage.cpp
///
/// @file   VolRecipePage.cpp
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
#include "VolRecipePage.h"
#include "RefillVolBoxDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CVolRecipePage, _func)

//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CVolRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_VOL_RECIPE_TOTALIZERRESET, &CVolRecipePage::OnBnClickedVolTotalizerReset)
	ON_STN_CLICKED(IDC_VOL_RECIPE_NAME, &CVolRecipePage::OnBnClickedVolName)
	ON_STN_CLICKED(IDC_VOL_RECIPE_QMNUMBER, &CVolRecipePage::OnBnClickedVolQMNumber)
	ON_STN_CLICKED(IDC_VOL_RECIPE_ANNUMBER, &CVolRecipePage::OnStnClickedVolANnummer)
	ON_STN_CLICKED(IDC_VOL_RECIPE_SETPOINT, &CVolRecipePage::OnStnClickedVolSetpoint)
	ON_STN_CLICKED(IDC_VOL_RECIPE_BITMAP, &CVolRecipePage::OnStnClickedVolRefill)
	ON_BN_CLICKED(IDC_VOL_RECIPE_LINE, &CVolRecipePage::OnStnClickedVolLine)
	ON_BN_CLICKED(IDC_VOL_RECIPE_AGITATOR, &CVolRecipePage::OnClickAgitator)
	ON_BN_CLICKED(IDC_VOL_RECIPE_NAME_BT, &CVolRecipePage::OnClickNameBt)
	ON_BN_CLICKED(IDC_VOL_RECIPE_KEY_BT, &CVolRecipePage::OnClickKeyBt)
	ON_BN_CLICKED(IDC_VOL_RECIPE_MASSFLOWCTRL, &CVolRecipePage::OnClickToolTipMassflow)
	ON_BN_CLICKED(IDC_VOL_RECIPE_ROTSPEEDCTRL, &CVolRecipePage::OnClickToolTipRotation)
	ON_BN_CLICKED(IDC_VOL_RECIPE_DRIVECOMMANDCTRL, &CVolRecipePage::OnClickToolTipDriveCommand)
	ON_BN_CLICKED(IDC_VOL_RECIPE_REFILL_RELEASE, &CVolRecipePage::OnBnClickedRefillRelease)
	ON_BN_CLICKED(IDC_VOL_RECIPE_FEEDERTYPE, &CVolRecipePage::OnClickedFeederTypeInfo)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CVolRecipePage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CVolRecipePage::CVolRecipePage(): CDosePage(CVolRecipePage::IDD)
	, m_GaugeMassflow(this)
	, m_GaugeDriveCommand(this)
	, m_GaugeRotSpeed(this)
	, m_RefillRelease()
	, m_aLocalMode()
	, m_Agitator()
	, m_ProductListName()
	, m_ProductListKey()
	, c_RegeneratBrush(REGENERATBKCOLOR)
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, m_fSetpoint(0)
	, m_fMassflow(0)
	, m_fDriveCommand(0)
	, m_fActRotSpeed(0)
	, m_fPercentage(0.0f)
	, m_szActRotSpeed(_T(""))
	, m_fActTotalizer(0.0f)
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
	, m_bLocalMode(FALSE)
    , m_bRegenerat(FALSE)
	, m_EditMap({
			EDITITEM(IDC_VOL_RECIPE_SETPOINT,	OnNotifyEditSetpoint),
			EDITITEM(IDC_VOL_RECIPE_NAME,		OnNotifyEditName),
			EDITITEM(IDC_VOL_RECIPE_QMNUMBER,	OnNotifyEditQMNumber),
			EDITITEM(IDC_VOL_RECIPE_ANNUMBER,	OnNotifyEditANNumber)
	})

{}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_TextN(pDX, IDC_VOL_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_VOL_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_VOL_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_VOL_RECIPE_ANNUMBER, m_szANNumber);
	DDX_Text(pDX, IDC_VOL_RECIPE_ROTATIONALSPEED, m_szActRotSpeed);
	DDX_Text(pDX, IDC_VOL_RECIPE_FEEDERTYPE, m_szFeederType);
	DDX_Float(pDX, IDC_VOL_RECIPE_SETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_VOL_RECIPE_PERCENTAGE, m_fPercentage);
	DDX_Float(pDX, IDC_VOL_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Control(pDX, IDC_VOL_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_VOL_RECIPE_DRIVECOMMANDCTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_VOL_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_VOL_RECIPE_ROTSPEEDCTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_VOL_RECIPE_LINE, m_aLocalMode);
	DDX_Control(pDX, IDC_VOL_RECIPE_REFILL_RELEASE, m_RefillRelease);
	DDX_Control(pDX, IDC_VOL_RECIPE_AGITATOR, m_Agitator);
	DDX_Control(pDX, IDC_VOL_RECIPE_NAME_BT, m_ProductListName);
	DDX_Control(pDX, IDC_VOL_RECIPE_KEY_BT, m_ProductListKey);
}
//***************************************************************************************
//***************************************************************************************
BOOL CVolRecipePage::OnNotifyEditSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{
		REMOTEREF.setDoseSetpoint(m_sItem, m_fSetpoint);
		CheckQMaxfaktor();
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CVolRecipePage::OnNotifyEditANNumber(void)
{
	auto bModified = CEditCtrlEx::GetString(m_szANNumber);
	if (bModified)
	{
		REMOTEREF.setLineANNumber(toStdString(m_szANNumber));
	}
	return bModified;
}

//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CVolRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CVolRecipePage::SetControlStyle (void)
{
	base::RefillStatus status;
	REMOTEREF.getDoseRefillState(m_sItem, status);
	const BOOL bRefillState = status.flags.refillactive;
	const BOOL bRefillIOReleaseInputern = status.flags.refillreleaseextI;
	const BOOL bRefillRelease = status.flags.refillrelease;

	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bLocalMode = !ulStatus.flags.lineMode;
	const BOOL bStarted = ulStatus.flags.started;
	const BOOL bSlaveMode = ulStatus.flags.slaveMode;
	const BOOL bRefillControlEnable = ulStatus.flags.refillControlEnable;

	BOOL bLogin					= TRUE;

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_VOL_RECIPE_LINE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

	m_RefillRelease.SetCheck(bRefillRelease);
	m_RefillRelease.SetInputExt(bRefillIOReleaseInputern);
	m_RefillRelease.Enable(bRefillControlEnable);
	m_RefillRelease.EnableWindow(bRefillControlEnable);
	m_RefillRelease.Show();


    ENABLE_ID(IDC_VOL_RECIPE_SETPOINT,    bLogin && bLocalMode);
    ENABLE_ID(IDC_VOL_RECIPE_NAME,        bLogin);
    ENABLE_ID(IDC_VOL_RECIPE_QMNUMBER,    bLogin);
	ENABLE_ID(IDC_VOL_RECIPE_ANNUMBER,	  bLogin);
	ENABLE_ID(IDC_VOL_RECIPE_NAME_BT,	  bLogin);
	ENABLE_ID(IDC_VOL_RECIPE_KEY_BT,     bLogin);
	ENABLE_SHOW_ID(IDC_VOL_REGENERAT, m_bRegenerat, m_bRegenerat);

	auto bTotalizerReset = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_TOTALIZER_RESET);
	ENABLE_SHOW_ID(IDC_VOL_RECIPE_TOTALIZERRESET, bLocalMode, bLocalMode && bTotalizerReset);

	base::DeviceConfig deviceconfig;
	REMOTEREF.getDoseDeviceConfig(m_sItem, deviceconfig);
	const BOOL bAgitatorEnable = deviceconfig.flags.agitatorEnable;
	ENABLE_SHOW_ID(IDC_VOL_RECIPE_AGITATOR, bAgitatorEnable, bAgitatorEnable);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::CheckQMaxfaktor (void)
{
	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bLocalMode = !processstatus.flags.lineMode;

	if (bLocalMode)
	{
		float32_t fTemp = 0.0f;
		REMOTEREF.getDoseMaxSetpoint(m_sItem, fTemp);
		if ( m_fSetpoint > fTemp )
		{
			ECMessageBox(IDS_SETPOINT_GREATER_MAXSETPOINT, MB_OK | MB_ICONEXCLAMATION);
		}
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CVolRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISVOLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	std::string ANNumber;
	REMOTEREF.getLineANNumber(ANNumber);
	m_szANNumber = toCString(ANNumber);

	float32_t	fMaxSetpoint = 0.0f;
	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
	float32_t	fMaxRotationalSpeed = 0.0f;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);

	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);
	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0.0F, 100.0F));
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);
	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	REMOTEREF.getDosePercentage(m_sItem, m_fPercentage);
	REMOTEREF.getDoseRegenerat(m_sItem, m_bRegenerat);

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
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CVolRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_RefillRelease);

		BOOL bProductListAvail = !(PRODUCTDATABASEREF.GetCount() == 0);
		SHOWW_ID(IDC_VOL_RECIPE_NAME_BT, bProductListAvail);
		SHOWW_ID(IDC_VOL_RECIPE_KEY_BT, bProductListAvail);
		SetNumberFont(IDC_VOL_RECIPE_NR);
		SetValue();
	}
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CVolRecipePage::OnSetActive()
{
	m_aLocalMode.Redraw();
	m_RefillRelease.Redraw();
	m_Agitator.Redraw();
	m_ProductListName.Redraw();
	m_ProductListKey.Redraw();
	return CDosePage::OnSetActive();
}
//***************************************************************************************
//***************************************************************************************
BOOL CVolRecipePage::OnKillActive()
{
	CRefillVolBoxDlg::DestroyRefillBox();
	return CDosePage::OnKillActive();
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnStnClickedVolRefill()
{
	base::DeviceConfig cfg;
	REMOTEREF.getDoseDeviceConfig(m_sItem, cfg);
	if (cfg.flags.moduleRefillEnable)
	{
		auto pWnd = GetDlgItem(IDC_VOL_RECIPE_BITMAP);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(aRect);
		CRefillVolBoxDlg::CreateRefillBox(this, m_sItem, CPoint{ aRect.right, aRect.top }, IDS_REFILLHEADER);
	}
}

//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::OnStnClickedVolLine()
{
	ToggleLineMode();
}
//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::OnBnClickedVolTotalizerReset()
{
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION    | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::OnBnClickedRefillRelease()
{
	if (m_RefillRelease.IsEnable())
	{
		ToggleReleaseMode();
	}
}
//*************************************************************************************
//*************************************************************************************
void CVolRecipePage::OnBnClickedVolName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_VOL_RECIPE_NAME);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::OnStnClickedVolSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_VOL_RECIPE_SETPOINT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CVolRecipePage::OnBnClickedVolQMNumber()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_UINT64CTRL, IDC_VOL_RECIPE_QMNUMBER);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnStnClickedVolANnummer()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_VOL_RECIPE_ANNUMBER);
}

//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CVolRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_VOL_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_VOL_REGENERAT :
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

							case IDC_VOL_RECIPE_FEEDERTYPE:
							{
								pDC->SetTextColor(EDITTEXTCOLOR);
								pDC->SetBkMode(TRANSPARENT);
								//hbr = (HBRUSH)c_EditBrush;
							}
							break;

        case IDC_VOL_RECIPE_ROTATIONALSPEED:
                            {
	                            ASSERT((_S32(m_sEncoderState) >= _S32(base::eEncoderState::ENCODERSTATE_OFF)) && (_S32(m_sEncoderState) < _S32(base::eEncoderState::ENCODERSTATE_MAX)));
                                // Set the text color to red
                                pDC->SetTextColor(c_EncoderTextcolor[_S32(m_sEncoderState)]);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkColor(c_EncoderBkcolor[_S32(m_sEncoderState)]);
                                pDC->SetBkMode(OPAQUE);
                                hbr = c_Brush[_S32(m_sEncoderState)];
                            }
                            break;

        case IDC_VOL_RECIPE_NAME:
        case IDC_VOL_RECIPE_QMNUMBER:
		case IDC_VOL_RECIPE_ANNUMBER:
        case IDC_VOL_RECIPE_SETPOINT:
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

		case IDC_VOL_RECIPE_BITMAP:
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
void CVolRecipePage::OnClickAgitator()
{
	CreateAgitatorBox(IDC_VOL_RECIPE_AGITATOR, IDS_AGITATOR_SPEED);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnClickNameBt()
{
	CreateProductDatabaseBox(IDC_VOL_RECIPE_NAME_BT, TRUE);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnClickKeyBt()
{
	CreateProductDatabaseBox(IDC_VOL_RECIPE_KEY_BT, FALSE);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnClickToolTipMassflow()
{
	CreateToolTip(IDC_VOL_RECIPE_MASSFLOWCTRL, IDS_MASSFLOW_KGH);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnClickToolTipDriveCommand()
{
	CreateToolTip(IDC_VOL_RECIPE_DRIVECOMMANDCTRL, IDS_DRIVECOMMAND_PERCENTAGE);
}
//***************************************************************************************
//***************************************************************************************
void CVolRecipePage::OnClickToolTipRotation()
{
	CreateToolTip(IDC_VOL_RECIPE_ROTSPEEDCTRL, IDS_ROTSPEED_UPM);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CVolRecipePage::OnClickedFeederTypeInfo()
{
	CreateHelpInfoBox(IDC_VOL_RECIPE_FEEDERTYPE, IDS_FEEDERTYPE, IDS_INFO_FEEDERTYPE);
}







