//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsRecipePage.h
///
/// @file   SettingsRecipePage.h
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
#include <map>
#include "BASE/include/EasyControlConfig.h"
#include "BASE/include/SystemBackup.h"
#include "afxwin.h"


class CSettingsRecipePage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsRecipePage)

	// Dialogfelddaten
	enum { IDD = IDD_SETTINGS_RECIPE };

private:
	const std::map < base::eLanguage, std::pair<int32_t, int32_t> > c_Lang;

	CComboBoxEx  m_aLanguageCtrl;
    CImageList   m_aImageList;
    CString      m_szVersion;
	base::eLanguage m_iNewLang;
	base::eLanguage m_iLang;
	BOOL         m_bModified;

private:
	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method initiates a framework call to exchanges data with the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void GetValue();

	//------------------------------------------------------------------------------------
	///  @brief   SetModified
	///
	///           This method is called to enable or disable the Apply Now button, based
	///			  on whether the settings in the property page should be applied to the
	///			  appropriate external object.
	///
	///  @param[in] b, BOOL
	///  @return BOOL
	///
	//------------------------------------------------------------------------------------
	void SetModified(BOOL);
	void ExportDataItem(base::CSystemBackup& field);
	void ImportDataItem(const base::CSystemBackup& field);

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
	CSettingsRecipePage();
	~CSettingsRecipePage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsRecipeTouch
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsRecipeTouch();

	//------------------------------------------------------------------------------------
	///  @brief   OnCbnSelchangeSettingsRecipeLanguageselect
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnCbnSelchangeSettingsRecipeLanguageselect();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsRecipeDate
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsRecipeDate();
	afx_msg void OnBnClickedSettingsRecipeImport();
	afx_msg void OnBnClickedSettingsRecipeExport();


	//------------------------------------------------------------------------------------
	///  @brief   OnCreate
	///
	///           The framework calls this member function when the window is first created
	///
	///  @param[in] lpCreateStruct , LPCREATESTRUCT
	///  @return int32_t
	///
	//------------------------------------------------------------------------------------
	afx_msg int32_t OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

