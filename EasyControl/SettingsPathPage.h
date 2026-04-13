//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsPathPage.h
///
/// @file   SettingsPathPage.h
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



class CSettingsPathPage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsPathPage)

	// Dialogfelddaten
	enum { IDD = IDD_SETTINGS_PATH };

private:
    const CBrush  c_EditBrush;
	CString m_szLogfilePath;
	CString m_szRecorderPath;
	CString	m_szRecipePath;
	CString m_szProductDatabaseFile;
	BOOL	m_bModified;
    BOOL    m_bRecorderAktiv;
    int32_t m_lRecorderInterval;

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
	void	Init                        (void);

	//------------------------------------------------------------------------------------
	///  @brief   SetControlStyle
	///
	///           This method updates the state of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void    SetControlStyle             (void);

	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method initiates a framework call to exchanges data with the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void	GetValue                    (void);

	BOOL OnNotifyRecorderInterval();

	//------------------------------------------------------------------------------------
	///  @brief   SetModified
	///
	///           This method is called to enable or disable the Apply Now button, based
	///			  on whether the settings in the property page should be applied to the
	///			  appropriate external object.
	///
	///  @param[in] b, BOOL
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetModified(BOOL);

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
	CSettingsPathPage();
	~CSettingsPathPage() override = default;

public:

	LRESULT OnPageChanged(WPARAM w, LPARAM);
	LRESULT OnNotifyEdit(WPARAM id, LPARAM);

	//------------------------------------------------------------------------------------
///  @brief   OnBnClickedSettingsPathLogfilepfadbutton
///
///           This method is called by the framework when the user clicks the control 
///
///  @param[in] none
///  @return none
///
//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathProductDatabaseFileButton();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsPathLogfilepfadbutton
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathLogfilePathButton();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsPathRecorderPathButton
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathRecorderPathButton();



	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsPathRecipePathButton
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathRecipePathButton();


	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsPathRecorderPathIntervalButton
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathRecorderPathIntervalButton();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedSettingsPathRecorderPathActiveButton
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedSettingsPathRecorderPathActiveButton();

	//------------------------------------------------------------------------------------
	///  @brief   OnCtlColor
	///
	///           This method is called by the framework when a child control is about to
	///           be drawn
	///
	///  @param[in] nCtlColor , UINT
	///  @param[in] pWnd , CWnd*
	///  @param[in] pDC , CDC*
	///  @return HBRUSH
	///
	//------------------------------------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

