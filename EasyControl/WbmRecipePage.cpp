//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module WbmRecipePage.cpp
///
/// @file   WbmRecipePage.cpp
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
#include "WbmRecipePage.h"
#include "GlobalConst.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


//******************************************************************************************************
//******************************************************************************************************
BEGIN_MESSAGE_MAP(CWbmRecipePage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_WBM_RECIPE_TOTALIZERRESET, OnBnClickedWbfRecipeDoseTotalizerReset)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_WBM_RECIPE_NAME, OnStnClickedWbfRecipeName)
	ON_STN_CLICKED(IDC_WBM_RECIPE_QMNUMMER, OnStnClickedWbfRecipeQmnummer)
	ON_BN_CLICKED(IDC_WBM_RECIPE_LINIE, OnBnClickedWbfRecipeLinie)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
IMPLEMENT_DYNAMIC(CWbmRecipePage, CDosePage)
//******************************************************************************************************
//******************************************************************************************************
CWbmRecipePage::CWbmRecipePage(): CDosePage(CWbmRecipePage::IDD)
	, m_GaugeMassflow(this)
	, m_GaugeRotSpeed(this)
	, m_aLocalMode()
	, c_RegeneratBrush{ REGENERATBKCOLOR }
	, c_EncoderTextcolor{ RGBCOLORWHITE, RGBCOLORWHITE, RGBCOLORWHITE,   RGBCOLORWHITE }
	, c_EncoderBkcolor	{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, c_Brush{ RGBCOLORBLACK, RGBCOLORRED,    RGBCOLORGREEN,   RGBCOLORBLACK }
	, m_szIstRotationalSpeed(_T(""))
	, m_bRegenerat(FALSE)
	, m_fMassflow(0)
	, m_fActRotSpeed(0)
	, m_fActTotalizer(0)
	, m_fAktVelocity(0)
	, m_fBandlastMassflow(0)
	, m_fMaxSetpoint(0)
	, m_fMaxRotationalSpeed(0)
	, m_sEncoderState(base::eEncoderState::ENCODERSTATE_UNDEFINED)
	, m_bLocalMode(FALSE)
	, m_fSetpoint(0.0F)
{}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WBM_RECIPE_TITLE, m_szTitle);
	DDX_TextN(pDX, IDC_WBM_RECIPE_NAME, m_szName, 30);
	DDX_Text(pDX, IDC_WBM_RECIPE_NR, m_lNr);

	DDX_Text(pDX, IDC_WBM_RECIPE_QMNUMMER, m_QMNumber);
	DDX_Text(pDX, IDC_WBM_RECIPE_ACTROTATIONSPEED, m_szIstRotationalSpeed);
	DDX_Float(pDX, IDC_WBM_RECIPE_TOTALIZER, m_fActTotalizer);
	DDX_Float(pDX, IDC_WBM_RECIPE_VELOCITY, m_fAktVelocity);
	DDX_Float(pDX, IDC_WBM_RECIPE_BELTLOADMASSFLOW, m_fBandlastMassflow);
	DDX_Control(pDX, IDC_WBM_RECIPE_BITMAP, m_aGrafikContainer);
	DDX_Control(pDX, IDC_WBM_RECIPE_MASSFLOWCTRL, m_GaugeMassflow);
	DDX_Control(pDX, IDC_WBM_RECIPE_ROTSPEEDCTRL, m_GaugeRotSpeed);
	DDX_Control(pDX, IDC_WBM_RECIPE_LINIE, m_aLocalMode);

}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::SetControlStyle (void)
{
	base::ProcessStatus ulStatus;
	REMOTEREF.getDoseProcessStatus(m_sItem, ulStatus);
	const BOOL bLocalMode = !ulStatus.flags.lineMode;
	const BOOL bStarted = ulStatus.flags.started;
	const BOOL bSlaveMode = ulStatus.flags.slaveMode;

	BOOL bLogin = TRUE;
	m_aLocalMode.SetCheck(!bLocalMode);
	m_aLocalMode.Enable((!bSlaveMode) && (!bStarted));
	m_aLocalMode.Show();
	ENABLE_SHOW_ID(IDC_WBM_RECIPE_LINIE, (!bSlaveMode) && (!bStarted), !bSlaveMode);

    ENABLE_ID(IDC_WBM_RECIPE_NAME,      (bLogin) && (bLocalMode));
    ENABLE_ID(IDC_WBM_RECIPE_QMNUMMER,  (bLogin) && (bLocalMode));
    ENABLE_SHOW_ID(IDC_WBM_RECIPE_TOTALIZERRESET, TRUE, bLocalMode);
    ENABLE_SHOW_ID(IDC_WBM_RECIPE_REGENERAT, m_bRegenerat, m_bRegenerat);
}
//******************************************************************************************************
//******************************************************************************************************
LRESULT CWbmRecipePage::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;

		switch (id)
		{
		case IDC_WBM_RECIPE_NAME:		bModified = OnNotifyEditName();
			break;
		case IDC_WBM_RECIPE_QMNUMMER:	bModified = OnNotifyEditQMNumber();
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
BOOL CWbmRecipePage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISBELTWEIGHERTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getDoseSetpoint(m_sItem, m_fSetpoint);
	float32_t	fMaxSetpoint = 0.0f;
	REMOTEREF.getDoseMaxSetpoint(m_sItem, fMaxSetpoint);
	float32_t	fMaxRotationalSpeed = 0.0f;
	REMOTEREF.getDoseMaxRotationalSpeed(m_sItem, fMaxRotationalSpeed);
	//---------------------------------------------------------------------------------------
	REMOTEREF.getDoseMassflow(m_sItem, m_fMassflow);
	REMOTEREF.getDoseActualRotationalSpeed(m_sItem, m_fActRotSpeed);

	REMOTEREF.getDoseWbfBeltLoadMassflow(m_sItem, m_fBandlastMassflow);

	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseTotalizer(m_sItem, m_fActTotalizer);
	REMOTEREF.getDoseWbfBeltLoadActualVelocity(m_sItem, m_fAktVelocity);

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
	ASSERT(__ISWBMTYPE(m_lDoseType));
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
BOOL CWbmRecipePage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();

	SetNumberFont(IDC_WBM_RECIPE_NR);
	SetValue();
	return result;
}
//***************************************************************************************
//***************************************************************************************
BOOL CWbmRecipePage::OnSetActive()
{
	m_aLocalMode.Redraw();
	return CDosePage::OnSetActive();
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::OnBnClickedWbfRecipeLinie()
{
	ToggleLineMode();
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::OnBnClickedWbfRecipeDoseTotalizerReset()
{
	if (ECMessageBox(IDS_QUERY_TOTALIZER_REALLY_RESET, MB_ICONQUESTION    | MB_YESNO) == IDYES)
	{
		REMOTEREF.setDosePBClearTotalizer(m_sItem);
	}
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::OnStnClickedWbfRecipeName()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_WBM_RECIPE_NAME);
}
//******************************************************************************************************
//******************************************************************************************************
void CWbmRecipePage::OnStnClickedWbfRecipeQmnummer()
{
	CEditCtrl :: GetInput(this, E_TYPCTRL::E_STRINGCTRL, IDC_WBM_RECIPE_QMNUMMER);
}
//******************************************************************************************************
//******************************************************************************************************
HBRUSH CWbmRecipePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_WBM_RECIPE_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;

        case IDC_WBM_RECIPE_ACTROTATIONSPEED:
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

        case IDC_WBM_RECIPE_BITMAP:
                            {
                                m_aGrafikContainer.Update(pDC);
                            }
                            break;

        case IDC_WBM_RECIPE_REGENERAT :
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

        case IDC_WBM_RECIPE_NAME:
        case IDC_WBM_RECIPE_QMNUMMER:
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


