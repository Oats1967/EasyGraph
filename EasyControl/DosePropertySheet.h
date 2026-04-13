//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DosePropertySheet
///
/// @file   DosePropertySheet.h
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

#include <vector>
#include "BASE/include/DoseType.h"
#include "LclRecipePage.h"
#include "LwfRecipePage.h"
#include "LwfMotorPage.h"
#include "LwfAlarmPage.h"
#include "LwbRecipePage.h"
#include "LwbMotorPage.h"
#include "IcbRecipePage.h"
#include "AlertPage.h"
#include "AlarmConfigPage.h"
#include "WbfRecipePage.h"
#include "WbfMotorPage.h"
#include "WbfAlarmPage.h"
#include "PidRecipePage.h"
#include "PidMotorPage.h"
#include "PidAlarmPage.h"
#include "IcpRecipePage.h"
#include "IcpMotorPage.h"
#include "IcpAlarmPage.h"
#include "VolRecipePage.h"
#include "VolMotorPage.h"
#include "VolAlarmPage.h"
#include "WbmRecipePage.h"
#include "WbmMotorPage.h"
#include "WbmAlarmPage.h"
#include "IfsRecipePage.h"
#include "IfsAlarmPage.h"
#include "IflRecipePage.h"
#include "IflMotorPage.h"


// CDosePropertySheet

class CDosePropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CDosePropertySheet)

	enum ePages
	{
		eRecipePage = 0,
		eMotorPage,
		eAlarmPage,
		eAlarmConfigPage,
		eAlertPage,
		eMaxPages
	};

public:
	CDosePropertySheet(CWnd* pParentWnd, int32_t sID);
	~CDosePropertySheet() override;
	static CDosePropertySheet* m_pDoseProperty;

protected:
	DECLARE_MESSAGE_MAP()
private:
	CLclRecipePage		    m_aLclRecipePage;
	CLwfRecipePage		    m_aLwfRecipePage;
	CLwfMotorPage		    m_aLwfMotorPage;
	CLwfAlarmPage		    m_aLwfAlarmPage;
	CLwbRecipePage		    m_aLwbRecipePage;
	CLwbMotorPage		    m_aLwbMotorPage;
	CIcbRecipePage		    m_aIcbRecipePage;
	CAlertPage				m_aAlertPage;
	CAlarmConfigPage		m_aAlarmConfigPage;
	CWbfRecipePage		    m_aWbfRecipePage;
	CWbfMotorPage		    m_aWbfMotorPage;
	CWbfAlarmPage		    m_aWbfAlarmPage;
	CPidRecipePage		    m_aPidRecipePage;
	CPidMotorPage		    m_aPidMotorPage;
	CPidAlarmPage		    m_aPidAlarmPage;
	CIcpRecipePage		    m_aIcpRecipePage;
	CIcpMotorPage		    m_aIcpMotorPage;
	CIcpAlarmPage		    m_aIcpAlarmPage;
	CVolRecipePage		    m_aVolRecipePage;
	CVolMotorPage		    m_aVolMotorPage;
	CVolAlarmPage		    m_aVolAlarmPage;
    CWbmRecipePage          m_aWbmRecipePage;
    CWbmMotorPage           m_aWbmMotorPage;
    CWbmAlarmPage           m_aWbmAlarmPage;
	CIfsRecipePage          m_aIfsRecipePage;
	CIfsAlarmPage           m_aIfsAlarmPage;
	CIflRecipePage          m_aIflRecipePage;
	CIflMotorPage          m_aIflMotorPage;

	CImageList				m_imageTab;
	std::vector<CMFCPropertyPage*> m_Pages;
	int32_t					m_sItem;
	CWnd*					m_pParentWnd;
	std::array<int32_t, eMaxPages> m_ImageId;

private:
	void FillImageIds();

	//------------------------------------------------------------------------------------
	///  @brief   AddPages
	///
	///           This method adds property pages to the sheet
	///
	///  @param[in] newitem , int32_t
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void AddPages		(const int32_t);

	//------------------------------------------------------------------------------------
	///  @brief   RemovePages
	///
	///           This method ....
	///
	///  @param[in] lDoseType , long
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void RemovePages	(void);

	//------------------------------------------------------------------------------------
	///  @brief   MyTitle
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void MyTitle        (void);

	//------------------------------------------------------------------------------------
	///  @brief   OnDoseNewItem
	///
	///           This method is called by the framework and updates the 
	///           controls in the user interface.
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	void SetNewItem		(int32_t);

	//------------------------------------------------------------------------------------
	///  @brief   SortButtons
	///
	///           This method sorts buttons right aligned
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	void SortButtons(void);

protected:
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
	void PostNcDestroy() override;

public:
	//------------------------------------------------------------------------------------
	///  @brief   OnDoseNewItem
	///
	///           This method is called by the framework and updates the 
	///           controls in the user interface.
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnDoseNewItem    (WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnTimerRefresh
	///
	///           This method is called periodically by the framework and updates the 
	///            controls in the user interface.
	///
	///  @param[in] w , WPARAM
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
	LRESULT OnTimerRefresh (WPARAM w, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnNewLogItem
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return LRESULT
	///
	//------------------------------------------------------------------------------------
    LRESULT OnNewLogItem           (WPARAM, LPARAM);
	LRESULT OnDoseEnd(WPARAM wNewItem, LPARAM);

	//------------------------------------------------------------------------------------
	///  @brief   OnDestroy
	///
	///           This method is called by the framework before the window is destroyed
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnDestroy();
	//afx_msg void OnClose();
};



#define DOSEPROPERTYREF (CDosePropertySheet ::m_pDoseProperty)


