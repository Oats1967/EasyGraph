//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsRemotePage.h
///
/// @file   SettingsRemotePage.h
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
#include <tuple>
#include "BASE/include/RemoteConfig.h"




class CSettingsRemotePage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsRemotePage)

	// CSettingsRemotePage-Dialogfeld
	using Protocols = std::tuple<int32_t, int32_t>;

// Dialogfelddaten
	enum { IDD = IDD_SETTINGS_REMOTE };

private:
	const std::array < Protocols, _S32(base::RemoteServerProtocol::REMOTE_PROTOCOL_MAX)> m_ProtocolMap;
	CComboBoxEx	    m_aProtokollCtrl;
    CImageList		m_aCBImageList;
    const CBrush    c_EditBrush;
	BOOL		    m_bModified;
	base::RemoteServerProtocol m_lProtokoll;

private:
	//------------------------------------------------------------------------------------
	///  @brief   Init
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------

	void	Init ( void);
	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method initiates a framework call to exchanges data with the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	GetValue (void);

	//------------------------------------------------------------------------------------
	///  @brief   SetControlStyle
	///
	///           This method updates the state of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void    SetControlStyle(void);
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
	void SetModified(BOOL);

	void LoadProtocol();

	void SaveProtocol();

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
	///  @brief   OnSetActive
	///
	///           This member function is called by the framework when the page is chosen
	///           by the user and becomes an active page.
	///
	///  @param[in] none
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	BOOL OnSetActive() override;

public:
	CSettingsRemotePage();
	~CSettingsRemotePage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnPageChanged
	///
	///           This method ....
	///
	///  @param[in] wParam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnPageChanged(WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnCbnSelchangeSystemRemoteProtokoll
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnCbnSelchangeSystemRemoteProtokoll(void);

	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int
	///
	//------------------------------------------------------------------------------------
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

