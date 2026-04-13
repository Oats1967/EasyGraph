//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module UserPage.h
///
/// @file   UserPage.h
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

class CUserPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CUserPage)

public:
	CUserPage();
	virtual ~CUserPage();

// Dialogfelddaten
	enum { IDD = IDD_PASSWORD_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
};

