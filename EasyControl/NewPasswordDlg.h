//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module NewPasswordDlg.h
///
/// @file   NewPasswordDlg.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

class CNewPasswordDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewPasswordDlg)

public:
	CNewPasswordDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	~CNewPasswordDlg() override = default;

// Dialogfelddaten
	enum { IDD = IDD_NEWPASSWORD };

protected:
	//------------------------------------------------------------------------------------
	///  @brief   DoDataExchange
	///
	///           Called by the framework to exchange data
	///
	///  @author  Detlef Hafer
	///
	///  @class        CNewPasswordDlg
	///
	///  @param[in] pDX , CDataExchange*
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV-Unterstützung

protected:

	//------------------------------------------------------------------------------------
	///  @brief   OnOK
	///
	///           This method is called when the user clicks OK button 
	///           ( the button with ID of IDOK )
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void OnOK() override;

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
	CString m_szPassword;
	CString m_szPasswordConfirm;

	//------------------------------------------------------------------------------------
	///  @brief   OnEnChangeNewPasswordPassword
	///
	///           This method is called by the framework when the user has taken an action 
	///			  that may have altered text in an edit control. This notification is
	///			  sent after the system updates the screen.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnEnChangeNewPasswordPassword();

	//------------------------------------------------------------------------------------
	///  @brief   OnEnChangeNewPasswordPasswordConfirm
	///
	///           This method is called by the framework when the user has taken an action 
	///			  that may have altered text in an edit control. This notification is
	///			  sent after the system updates the screen.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnEnChangeNewPasswordPasswordConfirm();

	DECLARE_MESSAGE_MAP()

};

