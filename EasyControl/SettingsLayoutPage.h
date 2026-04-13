//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SettingsLayoutPage.h
///
/// @file   SettingsLayoutPage.h
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
#include "EasyControl.h"
#include "BASE/include/SystemConfig.h"
#include "BASE/include/DoseTypeMap.h"
#include "BASE/include/UserClassMap.h"
#include "BASE/include/FeedingTypeMap.h"
#include "BASE/include/LCTypeDetectionMap.h"
#include "NextPrevButton.h"



class CTrashButton : public CBCGPButton
{
public:
	CTrashButton()
	{
		m_bRighImage = FALSE;
		m_bTopImage = TRUE;
		//m_bTransparent = TRUE;
		m_bDrawText = FALSE;
		m_nAlignStyle = CBCGPButton::ALIGN_CENTER;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
		m_bVisualManagerStyle = FALSE;
		SetImage(IDB_BTTRASHUP, IDB_BTTRASHUP, IDB_BTTRASHDISABLE, IDB_BTTRASHDOWN);
	}
};

class CAddButton : public CBCGPButton
{
public:
	CAddButton()
	{
		m_bRighImage = FALSE;
		m_bTopImage = TRUE;
		//m_bTransparent = TRUE;
		m_bDrawText = FALSE;
		m_nAlignStyle = CBCGPButton::ALIGN_CENTER;
		m_nFlatStyle = CBCGPButton::FlatStyle::BUTTONSTYLE_NOBORDERS;
		m_bVisualManagerStyle = FALSE;
		SetImage(IDB_BTADDUP, IDB_BTADDUP, IDB_BTADDDISABLE, IDB_BTADDDOWN);
	}
};


class CSettingsLayoutPage : public CBCGPPropertyPage
{
	DECLARE_DYNAMIC(CSettingsLayoutPage)

	// Dialogfelddaten
	enum { IDD = IDD_SETTINGS_LAYOUT };

private:
	const base::CDoseTypeMap m_DoseTypeMap;
	const base::CFeedingTypeMap m_FeedingTypeMap;
	const base::CLCTypeDetectionMap    m_LCTypeDetectionMap;

	const std::array < COLORREF, _S32(base::cMaxLCTypDetection) > c_WaageTextcolor;
	const std::array < COLORREF, _S32(base::cMaxLCTypDetection) > c_WaageBkcolor;
	std::array < CBrush, _S32(base::cMaxLCTypDetection) >  m_aBrushWaage;

	CTrashButton	 m_TrashButton;
	CAddButton		 m_AddButton;
	CNextButton		 m_NextButton;
	CPreviousButton	 m_PreviousButton;
	CFont            m_aFont;
	CFont            m_aDisplayFont;
	base::CSystemConfig m_AnlagenKonf;

	int32_t			 m_sAnzahl;
	BOOL			 m_bModified;
	int32_t			 m_StartPos;
private:
	void SetDefaultFont(const int32_t iStart, const int32_t iEnd);


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
	void SetValue(void);

	//------------------------------------------------------------------------------------
	///  @brief   GetValue
	///
	///           This method initiates a framework call to exchanges data with the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void GetValue(void);


	//------------------------------------------------------------------------------------
	///  @brief   Init
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void Init	 (void);

	//------------------------------------------------------------------------------------
	///  @brief   SaveItems
	///
	///           This method saves all items to a file
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SaveItems(void);
	//------------------------------------------------------------------------------------
	///  @brief   LoadItems
	///
	///           This method saves all items to a file
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void LoadItems(void);
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

public:
	CSettingsLayoutPage();
	~CSettingsLayoutPage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnPageChanged
	///
	///           This method is called by the framework when the page conent has been 
	///            changed
	///
	///  @param[in] wParam , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnPageChanged(WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedSystemLayoutDosiertyp
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedSystemLayoutDosiertyp(UINT nID);

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedSystemLayoutDosierWaage
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedSystemLayoutDosierWaage(UINT nID);


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedSystemLayoutFeederTyp
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedSystemLayoutFeederTyp(UINT nID);
	
	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedSystemLayoutNr
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] nID , UINT
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedSystemLayoutNr(UINT);


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

	afx_msg void OnClickedSettingsLayoutNext();
	afx_msg void OnClickedSettingsLayoutPrevious();
	afx_msg void OnClickedSettingsLayoutMinus();
	afx_msg void OnClickedSettingsLayoutPlus();

	DECLARE_MESSAGE_MAP()
};

