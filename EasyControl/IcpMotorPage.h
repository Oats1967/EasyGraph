//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module IcpMotorPage.h
///
/// @file   IcpMotorPage.h
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
#include "InfoButton.h"
#include "DosePage.h"


class CIcpMotorPage : public CDosePage
{
	DECLARE_DYNAMIC(CIcpMotorPage)

	enum { IDD = IDD_ICP_MOTOR };

private:
	const std::map <int32_t, std::function<BOOL()> > m_EditMap;

	CInfoButton	m_MaxsetpointInfoButton;
	CInfoButton	m_SteepnessMassflowInfoButton;
	float32_t	m_fSteepnessMassflow;
	float32_t	m_fMaxSetpoint;

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
	void SetControlStyle	  ( void);

	BOOL OnNotifyMaxSetpoint();
	BOOL OnNotifySteepnessMassflow();


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
public:
	CIcpMotorPage();
	~CIcpMotorPage() override = default;

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
	LRESULT OnNotifyEdit(WPARAM id, LPARAM);


	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedIcpMaxSetpoint
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedIcpMaxSetpoint();
	afx_msg void OnClickedIcpMaxSetpointInfo();

	//------------------------------------------------------------------------------------
	///  @brief   OnStnClickedIcpMotorSteepnessMassflow
	///
	///           This method is called by the framework when the control was clicked
	///           to be changed by a new keyboard input.
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnStnClickedIcpMotorSteepnessMassflow();
	afx_msg void OnClickedIcpMotorSteilheitistwertInfo();

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

