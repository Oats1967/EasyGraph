//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsRemotePage.cpp
///
/// @file   SettingsRemotePage.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include <cassert>
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "RemoteControl.h"
#include "SettingsRemotePage.h"
#include "BASE/Utils/public/xml/RemoteServerXml.h"
#include "BASE/Utils/public/xml/DoseServerXml.h"
#include "MFCMacros.h"



#define ICON_CB_SIZEX 80
#define ICON_CB_SIZEY 40
#define IMAGE_CB_MAX  5
#define MYID_CB_PROTOKOLLSELECT 1


#define IMAGE_NONE 0
#define IMAGE_MODBUS 1
#define IMAGE_SIEMENS 2
#define IMAGE_PROFIBUS 3
#define IMAGE_PROFINET 4



//********************************************************************************************
//********************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsRemotePage, CBCGPPropertyPage)
	ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, OnPageChanged)
	ON_CBN_SELCHANGE(MYID_CB_PROTOKOLLSELECT, OnCbnSelchangeSystemRemoteProtokoll)
	ON_WM_CREATE()
END_MESSAGE_MAP()
//********************************************************************************************
//********************************************************************************************
IMPLEMENT_DYNAMIC(CSettingsRemotePage, CBCGPPropertyPage)
//********************************************************************************************
//********************************************************************************************
CSettingsRemotePage::CSettingsRemotePage()
	: CBCGPPropertyPage(CSettingsRemotePage::IDD)
	, m_ProtocolMap{	std::make_tuple(IDS_PROTOCOL_NO,         IMAGE_NONE ),
						std::make_tuple(IDS_PROTOCOL_OPENMODBUS, IMAGE_MODBUS),
						std::make_tuple(IDS_PROTOCOL_PROFINET,   IMAGE_PROFINET),
						std::make_tuple(IDS_PROTOCOL_S7SNAP,     IMAGE_SIEMENS),
						std::make_tuple(IDS_PROTOCOL_SIMULATOR,	IMAGE_SIEMENS),
						std::make_tuple(IDS_PROTOCOL_PROFIBUS,	IMAGE_PROFIBUS)
					}
	, m_aProtokollCtrl()
	, m_aCBImageList()
    , c_EditBrush(EDITBKCOLOR)
	, m_bModified(FALSE)
	, m_lProtokoll(base::RemoteServerProtocol::REMOTE_PROTOCOL_NONE)
{
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage::LoadProtocol()
{
	m_lProtokoll = base::RemoteServerProtocol::REMOTE_PROTOCOL_NONE;
	base::xml::CDoseServerXml doseserverxml;
	BOOL result = doseserverxml.Load(EASYCONTROLREF.m_DoseServerFile);
	if ( ! result)
	{
		return;
	}
	auto doseserver = doseserverxml.Get();
	base::xml::CRemoteServerXml remoteserverxml;
	result = remoteserverxml.Load(doseserver.m_path2RemoteServer);
	if ( ! result)
	{
		return;
	}
	m_lProtokoll = remoteserverxml.Get().m_Protocol;
	m_bModified = FALSE;
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage::SaveProtocol()
{
	base::xml::CDoseServerXml doseserverxml;
	BOOL result = doseserverxml.Load(EASYCONTROLREF.m_DoseServerFile);
	if ( ! result)
	{
		return;
	}
	auto doseserver = doseserverxml.Get();
	base::xml::CRemoteServerXml remoteserverxml;
	result = remoteserverxml.Load(doseserver.m_path2RemoteServer);
	if ( ! result)
	{
		return;
	}
	auto& remoteserver = remoteserverxml.Get();
	remoteserver.m_Protocol = m_lProtokoll;
	remoteserverxml.Save(doseserver.m_path2RemoteServer);

	ECMessageBox(IDS_SYSTEM_REMOTEPROTOCOL_CHANGED, MB_ICONEXCLAMATION | MB_OK);

	m_bModified = FALSE;
}
//********************************************************************************************
//********************************************************************************************
LRESULT CSettingsRemotePage::OnPageChanged(WPARAM , LPARAM)
{
	if ( m_bModified )
	{
		if ( ECMessageBox(IDS_SYSTEM_QUERY_CHANGED, MB_ICONQUESTION  | MB_YESNO) == IDYES)
		{
			GetValue();
			SaveProtocol();
		}
		m_bModified = FALSE;
	}
	return 0L;
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage::SetModified(BOOL b)
{
	if ( b )
	{
		m_bModified = TRUE;
	}
	CBCGPPropertyPage :: SetModified(b);
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage ::  SetControlStyle()
{
	const BOOL bPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_REMOTECONNECTION);
	ENABLE_ID(MYID_CB_PROTOKOLLSELECT, bPermission);
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage::GetValue ()
{
	UpdateData(TRUE);
}
//********************************************************************************************
//********************************************************************************************
void CSettingsRemotePage::OnOK()
{
	if ( m_bModified )
	{
		GetValue();
		SaveProtocol();
		m_bModified = FALSE;
	}
	CBCGPPropertyPage::OnOK();
}
//********************************************************************************************
//********************************************************************************************
BOOL CSettingsRemotePage::OnApply()
{
	if ( m_bModified )
	{
		GetValue();
		SaveProtocol();
		m_bModified = FALSE;
	}
	return CBCGPPropertyPage::OnApply();
}
//**********************************************************************************************
//**********************************************************************************************
void CSettingsRemotePage::Init()
{
	CString szTemp;
	COMBOBOXEXITEM cbi;

	m_aProtokollCtrl.ResetContent();
	ASSERT(( _S32(m_lProtokoll) >= 0) && (_S32(m_lProtokoll) < _S32(m_ProtocolMap.size())));
	for (int32_t i = 0; i < _S32(m_ProtocolMap.size()); i++)
	{
        VERIFY(szTemp.LoadString(std::get<0>(m_ProtocolMap[i])));
        cbi.mask		    = CBEIF_IMAGE  | CBEIF_TEXT | CBEIF_SELECTEDIMAGE;
        cbi.iImage		    = std::get<1>(m_ProtocolMap[i]);
        cbi.iItem		    = i;
        cbi.pszText         = LPTSTR(LPCTSTR(szTemp));
        cbi.cchTextMax      = szTemp.GetLength();
        cbi.iSelectedImage	= std::get<1>(m_ProtocolMap[i]);
        m_aProtokollCtrl.InsertItem(&cbi);
	}
    m_aProtokollCtrl.SetCurSel(_S32(m_lProtokoll));
    m_aProtokollCtrl.ShowWindow(TRUE);
	UpdateData(FALSE);
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CSettingsRemotePage::OnInitDialog()
{
	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		CWnd* pWnd = GetDlgItem(IDC_SETTINGS_REMOTE_PROTOKOLL_TOUCHEDIT);
		ASSERT(pWnd);
		CRect aRect;
		pWnd->GetWindowRect(&aRect);
		ScreenToClient(aRect);
		m_aProtokollCtrl.MoveWindow(aRect);

		Init();
		SetControlStyle();
		m_bModified = FALSE;
	}
	return bResult;
}
//**********************************************************************************************
//**********************************************************************************************
BOOL CSettingsRemotePage::OnSetActive()
{
	GetParent()->SendMessage(WM_PROPERTYPAGE_CHANGED, WPARAM(this));
	Init ();
	return CBCGPPropertyPage::OnSetActive();
}
//**********************************************************************************************
//**********************************************************************************************
void CSettingsRemotePage::OnCbnSelchangeSystemRemoteProtokoll(void)
{
	auto pl = m_aProtokollCtrl.GetCurSel();
	if (pl >= 0 && pl < _S32(m_ProtocolMap.size()))
	{
		auto lP = static_cast<base::RemoteServerProtocol>(pl);
		if (lP != m_lProtokoll)
		{
			m_lProtokoll = lP;
			SetModified(TRUE);
			SetFocus();
		}
	}
}
//**********************************************************************************************
//**********************************************************************************************
int CSettingsRemotePage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRect aRect(0,0,ICON_CB_SIZEX,int(ICON_CB_SIZEY*5));

    if (CBCGPPropertyPage::OnCreate(lpCreateStruct) == -1)
        return -1;

	LoadProtocol();

	// IMAGE_NONE 0
	m_aCBImageList.Create(ICON_CB_SIZEX, ICON_CB_SIZEY, ILC_COLOR8, IMAGE_CB_MAX, IMAGE_CB_MAX); // 32, 32 for large icons
	HICON  aIcon = AfxGetApp()->LoadIcon(IDI_PROTOKOLL_NO);
    ASSERT(aIcon);
	VERIFY(m_aCBImageList.Add(aIcon) >= 0);

	// IMAGE_MODBUS 1
	aIcon = AfxGetApp()->LoadIcon(IDI_PROTOKOLL_MODBUS);
    ASSERT(aIcon);
	VERIFY(m_aCBImageList.Add(aIcon) >= 0);

	// IMAGE_SIEMENS 2
	aIcon = AfxGetApp()->LoadIcon(IDI_PROTOKOLL_SIEMENS);
	ASSERT(aIcon);
	VERIFY(m_aCBImageList.Add(aIcon) >= 0);

	// IMAGE_PROFIBUS 3
	aIcon = AfxGetApp()->LoadIcon(IDI_PROTOKOLL_PROFIBUS);
    ASSERT(aIcon);
	VERIFY(m_aCBImageList.Add(aIcon) >= 0);

	// IMAGE_PROFINET 4
	aIcon = AfxGetApp()->LoadIcon(IDI_PROTOKOLL_PROFINET);
	ASSERT(aIcon);
	VERIFY(m_aCBImageList.Add(aIcon) >= 0);

    m_aProtokollCtrl.Create(CBS_DROPDOWNLIST, aRect, this, MYID_CB_PROTOKOLLSELECT);
	m_aProtokollCtrl.SetImageList(&m_aCBImageList);
	return 0;
}




