//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module IflMotorPage
///
/// @file   IflMotorPage.cpp
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
#include "IflMotorPage.h"
#include "GainFactorSpeedBoxDlg.h"
#include "ReduceFactorSpeedBoxDlg.h"
#include "FeederScaleBoxDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"


#define EDITITEM(_a, _func) 	BINDFUNC(_a, CIflMotorPage, _func)


//***************************************************************************************
//***************************************************************************************
BEGIN_MESSAGE_MAP(CIflMotorPage, CDosePage)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT, &CIflMotorPage::OnStnClickedSampleTime)
	ON_STN_CLICKED(IDC_IFL_MOTOR_PROPGAIN_EDIT, &CIflMotorPage::OnStnClickedPidGain)
	ON_BN_CLICKED(IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_INFO, &CIflMotorPage::OnBnClickedSampleTimeInfo)
	ON_BN_CLICKED(IDC_IFL_MOTOR_PROPGAIN_INFO, &CIflMotorPage::OnBnClickedPidGainInfo)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***************************************************************************************
//***************************************************************************************
IMPLEMENT_DYNAMIC(CIflMotorPage, CDosePage)
//***************************************************************************************
//***************************************************************************************
CIflMotorPage::CIflMotorPage(): CDosePage(CIflMotorPage::IDD) 
	, m_EditMap({
			EDITITEM(IDC_IFL_MOTOR_PROPGAIN_EDIT,	OnNotifyEditPidGain),
			EDITITEM(IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT,	OnNotifyEditSampleTime),

		})
		, m_SampleIntervalInfoButton()
		, m_PidGainInfoButton()
		, m_SampleInterval(0L)
		, m_fPidGain{ 0.0F }
{}
//***************************************************************************************
//***************************************************************************************
void CIflMotorPage::DoDataExchange(CDataExchange* pDX)
{
	CDosePage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_IFL_MOTOR_NR, m_lNr);
	DDX_Text(pDX, IDC_IFL_MOTOR_TITLE, m_szTitle);
	DDX_Float(pDX, IDC_IFL_MOTOR_PROPGAIN_EDIT, m_fPidGain);
	DDX_Text(pDX, IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT, m_SampleInterval);

	DDX_Control(pDX, IDC_IFL_MOTOR_IMAGE, m_aGrafikContainer);
	DDX_Control(pDX, IDC_IFL_MOTOR_PROPGAIN_INFO, m_PidGainInfoButton);
	DDX_Control(pDX, IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_INFO, m_SampleIntervalInfoButton);
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflMotorPage::OnNotifyEditPidGain(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbsRange(m_fPidGain, 0.0F, 10000.0F);
	if (bModified)
	{
		REMOTEREF.setDosePidPropGainGross(m_sItem, m_fPidGain);
	}
	return bModified;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CIflMotorPage::OnNotifyEditSampleTime(void)
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_SampleInterval, 0, 20U);
	if (bModified)
	{
		REMOTEREF.setPidSampleInterval(m_sItem, m_SampleInterval);
	}
	return bModified;
}
//*****************************************************************************************************
//*****************************************************************************************************
LRESULT CIflMotorPage::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
void CIflMotorPage::SetControlStyle (void)
{
	BOOL bLogin = USERRIGHTSREF.IsAktSupervisor();
	if (!bLogin)
	{
		bLogin = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_CONTROLSETTINGS));
	}
	ENABLE_ID(IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT, bLogin);
	ENABLE_ID(IDC_IFL_MOTOR_PROPGAIN_EDIT, bLogin);
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflMotorPage::OnUpdateControls (void)
{
	if ((m_sItem < 0) || (!__ISIFLTYPE(m_lDoseType)))
	{
		return FALSE;
	}
	REMOTEREF.getPidSampleInterval(m_sItem, m_SampleInterval);
	REMOTEREF.getDosePidPropGainGross(m_sItem, m_fPidGain);
	SetControlStyle();
	return CDosePage::OnUpdateControls();
}
//*****************************************************************************************************
//*****************************************************************************************************
BOOL CIflMotorPage::OnInitDialog()
{
	auto result = CDosePage::OnInitDialog();
	if (result)
	{
		INITINFOBUTTON(m_SampleIntervalInfoButton)
		INITINFOBUTTON(m_PidGainInfoButton)

		SetNumberFont(IDC_IFL_MOTOR_NR);
		SetValue();
	}
	return result;
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflMotorPage::OnStnClickedSampleTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
void CIflMotorPage::OnStnClickedPidGain()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_IFL_MOTOR_PROPGAIN_EDIT);
}
//*****************************************************************************************************
//*****************************************************************************************************
HBRUSH CIflMotorPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDosePage::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (pWnd->GetDlgCtrlID())
    {
        case IDC_IFL_MOTOR_NR :
                            {
                                // Set the text color to red
                                pDC->SetTextColor(INDEXCOLOR);

                                // Set the background mode for text to transparent 
                                // so background will show thru.
                                pDC->SetBkMode(TRANSPARENT);
                            }
                            break;


		case IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT:
		case IDC_IFL_MOTOR_PROPGAIN_EDIT:
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

		case IDC_IFL_MOTOR_IMAGE:
							{
								m_aGrafikContainer.Update(pDC);
							}
							break;

        default:    
                            break;
    }
    return hbr;
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflMotorPage::OnBnClickedPidGainInfo()
{
	CreateHelpInfoBox(IDC_IFL_MOTOR_PROPGAIN_EDIT, IDS_PROPGAIN_P, IDS_INFO_PROPGAIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CIflMotorPage::OnBnClickedSampleTimeInfo()
{
	CreateHelpInfoBox(IDC_IFL_MOTOR_PIDSAMPLEINTERVAL_EDIT, IDS_MEASURETIME_S, IDS_INFO_PIDSAMPLEINTERVAL);
}








