#include "stdafx.h"
#include "EasyControl.h"
#include "wmuser.h"
#include "DateTimePicker.h"

IMPLEMENT_DYNAMIC(CDateTimePickerDlg, CDialog)
CDateTimePickerDlg::CDateTimePickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDateTimePickerDlg::IDD, pParent)
{
}
//****************************************************************************************************
//****************************************************************************************************
BEGIN_MESSAGE_MAP(CDateTimePickerDlg, CDialog)
END_MESSAGE_MAP()
//****************************************************************************************************
//****************************************************************************************************
void CDateTimePickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKERCTRL, m_wndDateTime);
}
//****************************************************************************************************
//****************************************************************************************************
BOOL CDateTimePickerDlg::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		UINT nMask =
			CBCGPDateTimeCtrl::DTM_TIME |
			//CBCGPDateTimeCtrl::DTM_CHECKED |
			CBCGPDateTimeCtrl::DTM_SPIN;

		// | CBCGPDateTimeCtrl::DTM_DROPCALENDAR;

		UINT nFlags = CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_DROPCALENDAR;
		//-------------------
		// Setup date fields:
		//-------------------
		m_wndDateTime.SetState(nFlags, nMask);
		m_wndDateTime.SetTouchMode();

		//m_wndDateTime.SetReadOnly();
		UpdateData();
		::PostMessage(m_wndDateTime.GetSafeHwnd(), BM_CLICK, 0, 0);
	}
	return result;
}
//****************************************************************************************************
//****************************************************************************************************
void CDateTimePickerDlg::OnOK()
{
	// TODO: Fügen Sie hier Ihren spezialisierten Code ein, und/oder rufen Sie die Basisklasse auf.
	COleDateTime aTime(m_wndDateTime.GetDate());
	CTime cTime(aTime.GetYear(), aTime.GetMonth(), aTime.GetDay(), 0,0,0);
	m_Date = cTime.GetTime();
	CDialog::OnOK();
}
