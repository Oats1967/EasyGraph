//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LwbRecipePage
///
/// @file   LwbRecipePage.h
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
#include <functional>
#include <array>
#include "DosePage.h"
#include "GaugeTotalizer.h"
#include "GaugeDriveCommand.h"
#include "GaugeRotSpeed.h"
#include "ButtonLocalLine.h"
#include "ButtonVolGrav.h"
#include "ButtonAgitator.h"
#include "ButtonProductList.h"
#include "BASE/include/EncoderState.h"



class CLwbRecipePage : public CDosePage
{
	DECLARE_DYNAMIC(CLwbRecipePage)


// Dialogfelddaten
	enum { IDD = IDD_LWB_RECIPE };


	DECLARE_MESSAGE_MAP()
private:
	const std::map <int32_t, std::function<BOOL()> > c_EditMap;

	CGaugeTotalizer			m_GaugeTotalizer;
	CGaugeDriveCommand		m_GaugeDriveCommand;
	CGaugeRotSpeed			m_GaugeRotSpeed;

	CButtonVolGrav			m_aVolGrav;
	CButtonLocalLine		m_aLocalMode;
	CButtonAgitator			m_Agitator;
	CButtonProductList		m_ProductListName;
	CButtonProductList		m_ProductListKey;


	const CBrush          c_RegeneratBrush;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderTextcolor;
	const std::array<COLORREF, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_EncoderBkcolor;
	const std::array<CBrush, _S32(base::eEncoderState::ENCODERSTATE_MAX)>	c_Brush;
	CString			m_szActRotSpeed;		// kann auch aus/run sein
	CString          m_szANNumber;

	float32_t		m_fPercentage;
	float32_t		m_fSetpoint;
	float32_t		m_fDriveCommand;
	float32_t		m_fActRotSpeed;
	float32_t		m_fActWeight;
	base::eEncoderState m_sEncoderState;

	float32_t		m_fActTotalizer;
	BOOL            m_bRegenerat;

private:
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

	BOOL OnNotifyEditSetpoint(void);
	BOOL OnNotifyEditANNumber(void);

	void CheckRefillMin(void);

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
	BOOL OnUpdateControls() override;

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
	CLwbRecipePage();
	~CLwbRecipePage() override = default;


	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedDosiererVolumetric
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedDosiererVolumetric();

	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedDosiererTotalizerReset
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @author  Detlef Hafer
	///
	///  @class        CLwbRecipePage
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedDosiererTotalizerReset();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererName
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererName();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererQmnummer
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererQmnummer();
	afx_msg void OnStnClickedDosiererANnummer();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedDosiererSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedDosiererSetpoint();


	//------------------------------------------------------------------------------------
	///  @brief   OnBnClickedDosiererLinie
	///
	///           This method is called by the framework when the user clicks the control 
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnBnClickedDosiererLinie();

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
	afx_msg void OnClickToolTipTotalizer();
	afx_msg void OnClickToolTipDriveCommand();
	afx_msg void OnClickToolTipRotation();
	afx_msg void OnClickAgitator();
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
};

