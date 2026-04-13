//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsUserLoginPage.h
///
/// @file   SettingsUserLoginPage.h
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


#include <array>
#include <string>
#include <utility>
#include <map>
#include "RemoteControl.h"
#include "BASE/include/UserClassMap.h"


class CSettingsUserLoginPage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsUserLoginPage)

	enum { IDD = IDD_SETTINGS_USER };

private:
	const base::CUserClassMap m_UserClassMap;
	const std::map<const base::utils::eUserCategory, const int32_t> c_categorymaplist;

	CListCtrl		aListCtrl;
	CImageList		m_aImageList;
	CImageList		m_aCBImageList;
	CComboBoxEx	    m_aUserList;
    CFont           m_aFont;
	CString			m_szUserLevel;

	std::string							m_szEditUser;
	base::utils::CUserRightsList	m_aLoginList;
	BOOL							m_bModified;

private:
	//------------------------------------------------------------------------------------
	///  @brief   SetValue
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	SetValue    (void);

	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	GetValue    (void);

	//------------------------------------------------------------------------------------
	///  @brief   GetEnable
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL	GetEnable   (void) const;

    BOOL	GetShow     (void) const;

	BOOL	CanDelete(void) const;

	//------------------------------------------------------------------------------------
	///  @brief   Init
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	Init (void);

	//------------------------------------------------------------------------------------
	///  @brief   InitColumns
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	InitColumns (void);

	void	Accept (void);

	//------------------------------------------------------------------------------------
	///  @brief   MarkField
	///
	///           This method ....
	///
	///  @param[in] b , BOOL
	///  @param[in] i , int32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	MarkField(base::utils::eUserCategory category, BOOL b);

	//------------------------------------------------------------------------------------
	///  @brief   BuildPermissionList
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	BuildPermissionList(void);

	//------------------------------------------------------------------------------------
	///  @brief   BuildUserNameListBox
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	BuildUserNameListBox (void);

	//------------------------------------------------------------------------------------
	///  @brief   BuildPassword
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	BuildPassword		 (void);

	//------------------------------------------------------------------------------------
	///  @brief   BuildPasswordValid
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	BuildPasswordValid	 (void);

	//------------------------------------------------------------------------------------
	///  @brief   BuildUser
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	BuildUser			 (void);

	const base::utils::CUserRights* GetSafeItem() const;
	base::utils::CUserRights* GetSafeItem();
	const base::utils::CUserRights* GetEditItem() const;
	base::utils::CUserRights* GetEditItem();
	const std::string& GetSafeUser() const;
	const std::string& GetEditUser() const;
	BOOL IsMinorClass() const;


public:
	CSettingsUserLoginPage();
	~CSettingsUserLoginPage() override = default;

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
	///  @brief   SetModified
	///
	///           This method is called to enable or disable the Apply Now button, based
	///			  on whether the settings in the property page should be applied to the
	///			  appropriate external object.
	///
	///  @param[in] b , BOOL
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetModified(BOOL b);

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

	//------------------------------------------------------------------------------------
	///  @brief   OnApply
	///
	///           This method is called by the framework when the user chooses the OK or 
	///			  Apply now button. When the framework calls this function, changes on all
	///			  property pages in th eproperty sheet are accepted, the property sheet 
	///			  retains focus, and OnApply returns TRUE.
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnApply() override;

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

public:
	//------------------------------------------------------------------------------------
	///  @brief   OnLvnItemActivateSettingsUserUserproperty
	///
	///           This method ....
	///
	///  @param[in]  pResult , LRESULT *
	///  @param[in]  pNMHDR , NMHDR *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnLvnItemActivateSettingsUserUserproperty(NMHDR *pNMHDR, LRESULT *pResult);

	//------------------------------------------------------------------------------------
	///  @brief   OnCbnSelchangeSettingsUserUserlist
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnCbnSelchangeSettingsUserUserlist();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsUserNewUser
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsUserNewUser();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsUserDeleteUser
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsUserDeleteUser();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsUserPasswordReset
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsUserPasswordReset();

	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

};

