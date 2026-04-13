//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserNameDlg.h
///
/// @file   UserNameDlg.h
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "EasyControl.h"


class CUserNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserNameDlg)

	enum { IDD = IDD_USERNAME };

	const uint8_t eBNSUPERVISOR = 0x01U;
	const uint8_t eBNADMINISTRATOR = 0x02U;
	const uint8_t  eBNSTANDARDUSER = 0x04U;
	const uint8_t eBNUSER = 0x08U;

public:
	CUserNameDlg(CWnd* pParent, base::utils::eUserClass currentUser);   // Standardkonstruktor

	~CUserNameDlg() override = default;;

	CString					m_szUserName;
	base::utils::eUserClass m_NewUserClass;

private:
	base::utils::eUserClass m_currentUserClass;
	BOOL	m_bUserNameAdmin;
	BOOL	m_bUserNameSupervisor;
	BOOL	m_bUserNameStandard;
	BOOL	m_bUserNameUser;

private:
	void SetUserLevel(const base::utils::eUserClass newUser);

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
	void DoDataExchange(CDataExchange* pDX) override;

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnNotifyEdit		(WPARAM w, LPARAM);

	DECLARE_MESSAGE_MAP()

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
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedBenutzername
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedBenutzername();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedUserNameAdmin
	///
	///           This method is called when the user clicks the control 
	///
	///  @param[in/out] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedUserNameAdmin();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedUserNameStandard
	///
	///           This method is called when the user clicks the control 
	///
	///  @param[in/out] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedUserNameStandard();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedUserNameUser
	///
	///           This method is called when the user clicks the control 
	///
	///  @param[in/out] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedUserNameUser();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedOk
	///
	///           This method is called when the user clicks OK button 
	///           ( the button with ID of IDOK )
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedOk();
};

