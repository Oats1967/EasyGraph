//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbfRecipePage.cpp
///
/// @file   WbfRecipePage.cpp
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
#include "WbfRecipePage.h"
#include "GlobalConst.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbfRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_WBF_RECIPE_VOLUMETRISCH, &CWbfRecipePage::OnBnClickedWbfVolumetric)
	ON_BN_CLICKED(IDC_WBF_RECIPE_TOTALIZERRESET, &CWbfRecipePage::OnBnClickedWbfTotalizerReset)
	ON_STN_CLICKED(IDC_WBF_RECIPE_NAME, &CWbfRecipePage::OnStnClickedWbfRecipeName)
	ON_STN_CLICKED(IDC_WBF_RECIPE_QMNUMBER, &CWbfRecipePage::OnStnClickedWbfRecipeQmnumber)
	ON_STN_CLICKED(IDC_WBF_RECIPE_SETPOINT, &CWbfRecipePage::OnStnClickedWbfRecipeSetpoint)
	ON_BN_CLICKED(IDC_WBF_RECIPE_LINIE, &CWbfRecipePage::OnBnClickedWbfRecipeLine)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbfRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CWbfRecipePage::CWbfRecipePage(): CDosePage(CWbfRecipePage::IDD)
	, m_GaugeMassflow(this)
	, m_GaugeDriveCommand(this)
	, m_GaugeRotSpeed(this)
	, m_aVolGrav()
	, m_aLocalMode()
	, c_RegeneratBrush(REGENERATBKCOLOR)
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,  RGBCOLORBLACK }
	, m_fSetpoint(0)
	, m_fMassflow(0)
	, m_fDriveCommand(0)
	, m_fActRotSpeed(0)
	, m_fMischung(0)
	, m_fActTotalizer(0)
	, m_fAktVelocity(0)
	, m_fBandlastMassflow(0)
    , m_bRegenerat(FALSE)
	, m_szIstRotationalSpeed(_T(""))
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
{
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WBF_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_WBF_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_WBF_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_WBF_RECIPE_QMNUMBER, m_QMNumber);
	DDX_Text(pDX, IDC_WBF_RECIPE_ROTAIONALSPEED, m_szIstRotationalSpeed);
	DDX_Float(pDX, IDC_WBF_RECIPE_SETPOINT, m_fSetpoint);
	DDX_Float(pDX, IDC_WBF_RECIPE_PERCENTAGE, m_fMischung);
	DDX_Float(pDX, IDC_WBF_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Float(pDX, IDC_WBF_RECIPE_VELOCITY, m_fAktVelocity);
	DDX_Float(pDX, IDC_WBF_RECIPE_BELTLOADMASSFLOW, m_fBandlastMassflow);
	DDX_Control(pDX, IDC_WBF_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_WBF_RECIPE_DRIVECOMMANDCTRL, m_GaugeDriveCommand);
	DDX_Control(pDX, IDC_WBF_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_WBF_RECIPE_ROTSPEEDCTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_WBF_RECIPE_LINIE, m_aLocalMode);
	DDX_Control(pDX, IDC_WBF_RECIPE_VOLUMETRISCH, m_aVolGrav);

}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::SetControlStyle (void)
{
	BOOL bLogin = TRUE;

	base::ProcessStatus processstatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, processstatus);
	const BOOL bVolumetricExtern = processstatus.flags.volumetricext;
	const BOOL bVolumetric = !processstatus.flags.gravimetric;
	const BOOL bLocalMode = !processstatus.flags.lineMode;
	const BOOL bStarted = (processstatus.flags.started == true) || (processstatus.flags.running == true);
	const BOOL bLCAktiv = processstatus.flags.LCActive;
	const BOOL bSlaveMode = processstatus.flags.slaveMode;

	m_aVolGrav.SetInputExt(bVolumetricExtern);
	m_aVolGrav.Enable(bLogin && (!bVolumetricExtern) && bLCAktiv);
	m_aVolGrav.EnableWindow(bLCAktiv);
	m_aVolGrav.SetCheck(bVolumetric);
	m_aVolGrav.Show();

	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_WBF_RECIPE_LINIE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_WBF_RECIPE_SETPOINT, (bLogin) && (bLocalMode));
    ENABLE_ID(IDC_WBF_RECIPE_NAME, (bLogin) && (bLocalMode));
    ENABLE_ID(IDC_WBF_RECIPE_QMNUMBER, (bLogin) && (bLocalMode));
    ENABLE_SHOW_ID(IDC_WBF_RECIPE_TOTALIZERRESET, bLocalMode, bLocalMode);
    ENABLE_SHOW_ID(IDC_WBF_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfRecipePage::OnNotifySetpoint()
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
LRESULT CWbfRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;

		switch (id)
		{
		case IDC_WBF_RECIPE_SETPOINT:	bModified = OnNotifySetpoint();
			break;
		case IDC_WBF_RECIPE_NAME:	bModified = OnNotifyEditName();
			break;
		case IDC_WBF_RECIPE_QMNUMBER:	bModified = OnNotifyEditQMNumber();
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		if (bModified)
		{
			SetValue();
		}
	}
	return 0L;
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::CheckQMaxfaktor (void)
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bLocalMode = !ulStatus.flags.lineMode;

	if (bLocalMode)
	{
		float32_t	fMaxSetpoint = 0.0f;
		REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
		if ( m_fSetpoint > fMaxSetpoint)
		{
			ECMessageBox(IDS_SETPOINT_GREATER_MAXSETPOINT, MB_OK | MB_ICONEXCLAMATION);
		}
	}
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	float32_t	fMaxSetpoint = 0.0f;
	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
	float32_t	fMaxRotationalSpeed = 0.0f;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);

	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualDriveCommand(m_sItem, m_fDriveCommand);

	m_fDriveCommand = ROUND2(RANGE(m_fDriveCommand * 100.0f, 0.0F, 100.0F));
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);
	REMOTEREF.getDoseWbfBeltLoadMassflow(m_sItem, m_fBandlastMassflow);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseWbfBeltLoadActualVelocity(m_sItem, m_fAktVelocity);
	REMOTEREF.getDosePercentage(m_sItem, m_fMischung);

	REMOTEREF.getDoseRegenerat(m_sItem, m_bRegenerat);

	REMOTEREF.getDoseActualEncoderState(m_sItem, m_sEncoderState);
	switch (m_sEncoderState)
	{
		default:
		case base::eEncoderState::ENCODERSTATE_OFF:
			VERIFY(m_szIstRotationalSpeed.LoadString(IDS_AUS));
			break;

		case base::eEncoderState::ENCODERSTATE_RUN:
			m_szIstRotationalSpeed.Format(_T("%d"), NEXTLONG(m_fActRotSpeed));
			break;

		case base::eEncoderState::ENCODERSTATE_STOP:
			m_szIstRotationalSpeed.Format(_T("%d"), NEXTLONG(m_fActRotSpeed));
			break;

#if 0
		case base::eEncoderState::ENCODERSTATE_AUTO:
			VERIFY(m_szIstRotationalSpeed.LoadString(IDS_RUN));
			break;
#endif
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

	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CWbfRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	SetNumberFont(IDC_WBF_RECIPE_NR);
	SetValue();
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CWbfRecipePage::OnSetActive()
{
	m_aVolGrav.Redraw();
	m_aLocalMode.Redraw();
	return CDosePage::OnSetActive();
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnBnClickedWbfVolumetric()
{
	ToggleGravimetricMode();
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnBnClickedWbfRecipeLine()
{
	ToggleLineMode();
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnBnClickedWbfTotalizerReset()
{
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION    | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnStnClickedWbfRecipeName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_WBF_RECIPE_NAME);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnStnClickedWbfRecipeQmnumber()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_WBF_RECIPE_QMNUMBER);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbfRecipePage::OnStnClickedWbfRecipeSetpoint()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WBF_RECIPE_SETPOINT);
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CWbfRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBF_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_WBF_RECIPE_ROTAIONALSPEED:
                            {
								ASSERT((m_sEncoderState >= base::eEncoderState::ENCODERSTATE_OFF) && (m_sEncoderState < base::eEncoderState::ENCODERSTATE_MAX));
								// Set the text color to red
                                pDC->SetTextColor(c_EncoderTextcolor[_S32(m_sEncoderState)]);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkColor(c_EncoderBkcolor[_S32(m_sEncoderState)]);
                                pDC->SetBkMode(OPAQUE);
                                hbr = c_Brush[_S32(m_sEncoderState)];
                            }
                            break;
        case IDC_WBF_RECIPE_BITMAP:
                            {
                                m_aGrafikContainer.Update(pDC);
                            }
                            break;

        case IDC_WBF_RECIPE_REGENERAT :
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

        case IDC_WBF_RECIPE_NAME:
        case IDC_WBF_RECIPE_QMNUMBER:
        case IDC_WBF_RECIPE_SETPOINT:
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




