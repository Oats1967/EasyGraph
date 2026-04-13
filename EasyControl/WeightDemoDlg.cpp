#include "stdafx.h"
#ifdef __SIM_WEIGHT_GUI
#include "EasyControl.h"
#include "WeightDemoDlg.h"
#include "MFCMacros.h"



CWeightDemoDlg* CWeightDemoDlg::g_WeightDemoDlg = nullptr;

float32_t CWeightDemoDlg::m_fWeight = 0.0F;
float32_t CWeightDemoDlg::m_fStep   = 0.1F;
int32_t CWeightDemoDlg::m_sItem		= 0;
int32_t CWeightDemoDlg::m_lNr       = 1;




IMPLEMENT_DYNAMIC(CWeightDemoDlg, CDialog)

// CWeightDemoDlg-Dialog
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CWeightDemoDlg, CDialog)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_BN_CLICKED(IDC_WEIGHT_DEMO_PLUS, &CWeightDemoDlg::OnBnClickedRampUp)
	ON_BN_CLICKED(IDC_WEIGHT_DEMO_MINUS, &CWeightDemoDlg::OnBnClickedRampDown)
	ON_STN_DBLCLK(IDC_WEIGHT_DEMO_STEP_EDIT, &CWeightDemoDlg::OnStnClickedStepEdit)
	ON_STN_DBLCLK(IDC_WEIGHT_DEMO_WEIGHT_EDIT, &CWeightDemoDlg::OnStnClickedWeightEdit)
	ON_STN_DBLCLK(IDC_WEIGHT_DEMO_ITEM_EDIT, &CWeightDemoDlg::OnStnClickedItemEdit)
	ON_BN_CLICKED(IDC_WEIGHT_DEMO_OK, &CWeightDemoDlg::OnBnClickedEnd)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
CWeightDemoDlg::CWeightDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD, pParent)
	, c_EditBrush{ EDITBKCOLOR }
{

}
//***************************************************************************************
//***************************************************************************************
CWeightDemoDlg::~CWeightDemoDlg()
{
	g_WeightDemoDlg = nullptr;
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_WEIGHT_DEMO_STEP_EDIT, m_fStep);
	DDX_Text(pDX, IDC_WEIGHT_DEMO_ITEM_EDIT, m_lNr);
	DDX_Text(pDX, IDC_WEIGHT_DEMO_WEIGHT_EDIT, m_fWeight);
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::SetValue()
{
	UpdateData(FALSE);
}
//***************************************************************************************
//***************************************************************************************
BOOL CWeightDemoDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		SetValue();
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
void CWeightDemoDlg::PostNcDestroy()
{
	if (m_pParent && m_pParent->GetSafeHwnd())
	{
		//m_pParent->PostMessage(WM_SCALEBOXCLOSED, 0);
	}
	CEditCtrl::Destroy();
	delete this;
}
//***********************************************************************************************
//***********************************************************************************************
void CWeightDemoDlg::Create(CWnd* pParent)
{
	m_pParent = pParent;

	auto result = CDialog::Create(IDD, pParent);
	if (result)
	{
		ShowWindow(SW_SHOW);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CWeightDemoDlg::CreateBox(CWnd* pWnd)
{
	if (!g_WeightDemoDlg)
	{
		g_WeightDemoDlg = new CWeightDemoDlg(pWnd);
		g_WeightDemoDlg->Create(pWnd);
	}
}

//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnBnClickedRampUp()
{
	m_fWeight += m_fStep;
	REMOTEREF.setDoseLCWeight(m_sItem, m_fWeight);
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnBnClickedRampDown()
{
	m_fWeight -= m_fStep;
	REMOTEREF.setDoseLCWeight(m_sItem, m_fWeight);
}
//**************************************************************************************
//**************************************************************************************
LRESULT CWeightDemoDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		switch (id)
		{
		case IDC_WEIGHT_DEMO_WEIGHT_EDIT:	bModified = OnNotifyEditWeight();
			break;
		case IDC_WEIGHT_DEMO_STEP_EDIT:	bModified = OnNotifyEditStep();
			break;
		case IDC_WEIGHT_DEMO_ITEM_EDIT:	bModified = OnNotifyEditItem();
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

//**************************************************************************************
//**************************************************************************************
BOOL CWeightDemoDlg::OnNotifyEditStep()
{
	auto value = _F32(CEditCtrl::GetFloat());
	BOOL bModified = (value != m_fStep);
	if (bModified)
	{
		m_fStep = value;
	}
	return bModified;
}
//**************************************************************************************
//**************************************************************************************
BOOL CWeightDemoDlg::OnNotifyEditWeight()
{
	auto value = _F32(CEditCtrl::GetFloat());
	BOOL bModified = (value != m_fWeight);
	if (bModified)
	{
		m_fWeight = value;
		REMOTEREF.setDoseLCWeight(m_sItem, m_fWeight);
	}
	return bModified;
}
//**************************************************************************************
//**************************************************************************************
BOOL CWeightDemoDlg::OnNotifyEditItem()
{
	auto value = CEditCtrl::GetLongAbs();
	BOOL bModified = (value > 0) && (value != m_lNr);
	if (bModified)
	{
		m_lNr = value;
		m_sItem = value - 1;
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnStnClickedWeightEdit()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WEIGHT_DEMO_WEIGHT_EDIT);
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnStnClickedStepEdit()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_WEIGHT_DEMO_STEP_EDIT);
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnStnClickedItemEdit()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_WEIGHT_DEMO_ITEM_EDIT);
}
//***************************************************************************************
//***************************************************************************************
void CWeightDemoDlg::OnBnClickedEnd()
{
	DestroyWindow();
}
//*************************************************************************************
//*************************************************************************************
HBRUSH CWeightDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_WEIGHT_DEMO_STEP_EDIT:
	case IDC_WEIGHT_DEMO_WEIGHT_EDIT:
	case IDC_WEIGHT_DEMO_ITEM_EDIT:
	{
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetTextColor(EDITTEXTCOLOR);
			pDC->SetBkColor(EDITBKCOLOR);
			pDC->SetBkMode(OPAQUE);
			hbr = (HBRUSH)c_EditBrush;
		}
	}
	break;

	default:
		break;
	}
	return hbr;
}




// CWeightDemoDlg-Meldungshandler
#endif
