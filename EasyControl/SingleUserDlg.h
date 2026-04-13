//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module SingleUserDlg.h
///
/// @file   SingleUserDlg.h
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

class CSingleUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CSingleUserDlg)

	enum { IDD = IDD_SINGLEUSER };

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
	///  @brief   OnClose
	///
	///           This method ....
	///
	///  @param[in] none
	///  @return none
	///
	//------------------------------------------------------------------------------------
	afx_msg void OnClose();

public:
	CSingleUserDlg(CWnd* pParent = NULL);   // Standardkonstruktor
	~CSingleUserDlg() override = default;

public:
	CString m_szUserName;
	DECLARE_MESSAGE_MAP()
};

