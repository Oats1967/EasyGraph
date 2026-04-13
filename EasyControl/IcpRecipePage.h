//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcpRecipePage.h
///
/// @file   IcpRecipePage.h
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


// CIcpRecipePage-Dialogfeld
#include <map>
#include <functional>
#include "GaugeMassflow.h"
#include "GaugeDriveCommand.h"
#include "ButtonLocalLine.h"
#include "ButtonProductList.h"
#include "DosePage.h"

class CIcpRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CIcpRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_ICP_RECIPE };

private:
	CGaugeMassflow			m_GaugeMassflow;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CButtonLocalLine		m_aLocalMode;
	CButtonProductList		m_ProductListName;
	CButtonProductList		m_ProductListKey;
	CBrush					m_aRegeneratBrush;
	CString					m_szANNumber;
	BOOL					m_bRegenerat;
    BOOL					m_bLocalMode;
	float32_t				m_fPercentage;
	float32_t				m_fSetpoint;
	float32_t				m_fMassflow;
	float32_t				m_fDriveCommand;
	float32_t				m_fActTotalizer;

	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

private:
	BOOL OnNotifySetpoint(void);
	BOOL OnNotifyEditANNumber(void);

	 
	//------------------------------------------------------------------------------------
	///  @brief   SetControlStyle
	///
	///           This method updates the state of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SetControlStyle (void);

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
	///  @brief   OnUpdateControls
	///
	///           This method initiates a framework call to update the content of the controls
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	BOOL OnUpdateControls(void) override;

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
	BOOL OnSetActive() override;

public:
	CIcpRecipePage();
	~CIcpRecipePage() override = default;

	//------------------------------------------------------------------------------------
	///  @brief   OnNotifyEdit
	///
	///
	///           This method is called by the framework when an editctrl has been changed
	///           by a keyboard input
	///
	///  @param[in] l , LPARAM
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT	OnNotifyEdit		(WPARAM w, LPARAM l);

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedDosiererTotalizerReset
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedIcpTotalizerReset();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedIcpName
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedIcpName();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedIcpQmnummer
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedIcpQmnummer();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedIcpSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedIcpSetpoint();
	afx_msg void OnStnClickedDosiererANnummer();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedIcpLine
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedIcpLine();
	afx_msg void OnClickToolTipMassflow();
	afx_msg void OnClickToolTipDriveCommand();
	afx_msg void OnClickNameBt();
	afx_msg void OnClickKeyBt();



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

