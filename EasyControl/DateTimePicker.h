#pragma once

#include "EasyControl.h"


class CDateTimePickerDlg : public CDialog
{
	DECLARE_DYNAMIC(CDateTimePickerDlg)

	// Dialogfelddaten
	enum { IDD = IDD_DATETIMEPICKER };

private:
	CBCGPDateTimeCtrl m_wndDateTime;
	time_t m_Date;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   DoDataExchange
	///
	///           Called by the framework to exchange data
	///
	///  @param[in] pDX , CDataExchange*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung
	//------------------------------------------------------------------------------------
	///  @brief   OnInitDialog
	///
	///           This method is called by the framework in response to WM_INITDIALOG message
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnInitDialog() override;

public:
	CDateTimePickerDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	~CDateTimePickerDlg() override = default;

	const time_t& GetDate() const
	{ return m_Date;	}


	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
};
