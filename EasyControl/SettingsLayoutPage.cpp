
//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module SettingsLayoutPage
///
/// @file   SettingsLayoutPage.cpp
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
#include "EasyControl.h"
#include "ECMessageBox.h"
#include "SettingsLayoutPage.h"
#include "MFCMacros.h"



#define MAXANLAGENCOUNT 6



IMPLEMENT_DYNAMIC(CSettingsLayoutPage, CBCGPPropertyPage)
CSettingsLayoutPage::CSettingsLayoutPage()
	: CBCGPPropertyPage(CSettingsLayoutPage::IDD)
	, c_WaageTextcolor { RGB(0,100,0),  RGB(0,160,0),  RGB(160,0,0),  RGB(160,0,0), RGB(160,0,0), RGB(160,0,0) }
	, c_WaageBkcolor { WHITECOLOR, WHITECOLOR,  WHITECOLOR, WHITECOLOR, WHITECOLOR,  WHITECOLOR }
	, m_bModified(FALSE)
	, m_sAnzahl(0)
	, m_StartPos(0)
{
	VERIFY(m_aFont.CreateFont(
		40,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

   VERIFY(m_aDisplayFont.CreateFont(
	   12,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("Arial")));         // lpszFacename

	for (int32_t i = 0; i < _S32(base::cMaxLCTypDetection); i++)
	{
		VERIFY(m_aBrushWaage[i].CreateSolidBrush(c_WaageBkcolor[i]));
	}
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::DoDataExchange(CDataExchange* pDX)
{
	CBCGPPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SETTINGS_LAYOUT_COUNT, m_sAnzahl);
	DDX_Control(pDX, IDC_SETTINGS_LAYOUT_MINUS, m_TrashButton);
	DDX_Control(pDX, IDC_SETTINGS_LAYOUT_PLUS, m_AddButton);
	DDX_Control(pDX, IDC_SETTINGS_LAYOUT_NEXT, m_NextButton);
	DDX_Control(pDX, IDC_SETTINGS_LAYOUT_PREVIOUS, m_PreviousButton);
}
//*************************************************************************************************
//*************************************************************************************************
BEGIN_MESSAGE_MAP(CSettingsLayoutPage, CBCGPPropertyPage)
	ON_MESSAGE(WM_PROPERTYPAGE_CHANGED, OnPageChanged)
	ON_CONTROL_RANGE(STN_CLICKED,IDC_SETTINGS_LAYOUT_DOSETYPE1,		IDC_SETTINGS_LAYOUT_DOSETYPE6, &CSettingsLayoutPage::OnStnClickedSystemLayoutDosiertyp)
	ON_CONTROL_RANGE(STN_CLICKED,IDC_SETTINGS_LAYOUT_NUMBER1,				IDC_SETTINGS_LAYOUT_NUMBER6, &CSettingsLayoutPage::OnStnClickedSystemLayoutNr)
	ON_CONTROL_RANGE(STN_CLICKED,IDC_SETTINGS_LAYOUT_LOADCELL1,		IDC_SETTINGS_LAYOUT_LOADCELL6, &CSettingsLayoutPage::OnStnClickedSystemLayoutDosierWaage)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_SETTINGS_LAYOUT_FEEDERTYPE1, IDC_SETTINGS_LAYOUT_FEEDERTYPE6, &CSettingsLayoutPage::OnStnClickedSystemLayoutFeederTyp)
    ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETTINGS_LAYOUT_NEXT, &CSettingsLayoutPage::OnClickedSettingsLayoutNext)
	ON_BN_CLICKED(IDC_SETTINGS_LAYOUT_PREVIOUS, &CSettingsLayoutPage::OnClickedSettingsLayoutPrevious)
	ON_BN_CLICKED(IDC_SETTINGS_LAYOUT_MINUS, &CSettingsLayoutPage::OnClickedSettingsLayoutMinus)
	ON_BN_CLICKED(IDC_SETTINGS_LAYOUT_PLUS, &CSettingsLayoutPage::OnClickedSettingsLayoutPlus)
END_MESSAGE_MAP()
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::SetModified(BOOL b)
{
	if ( b )
	{
		m_AnlagenKonf.SetModified(TRUE);
	}
	CBCGPPropertyPage :: SetModified(b);
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::LoadItems()
{
	m_AnlagenKonf = SYSTEMCONFIGREF;
	m_AnlagenKonf.SetModified(FALSE);
#if _DEBUG
	uint32_t lCount = 0;
	REMOTEREF.getMaxDoseCount(lCount);
	ASSERT(lCount == m_AnlagenKonf.GetCount());
#endif
	m_sAnzahl = m_AnlagenKonf.GetCount();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::SaveItems()
{
	if (m_AnlagenKonf.IsModified())
	{
		SYSTEMCONFIGREF = m_AnlagenKonf;
		m_AnlagenKonf.SetModified( FALSE);
		REFDOSESERVER.SetSystemConfig(m_AnlagenKonf);
		AfxGetMainWnd()->PostMessage(WM_PREPARENEWLAYOUT);
	}
}
//*************************************************************************************************
//*************************************************************************************************
LRESULT CSettingsLayoutPage::OnPageChanged(WPARAM , LPARAM)
{
	if ( m_bModified )
	{
		if ( ECMessageBox(IDS_SYSTEM_QUERY_CHANGED, MB_ICONQUESTION  | MB_YESNO) == IDYES)
		{
			GetValue();
			SaveItems();
		}
		m_bModified = FALSE;
	}
	return 0L;
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::Init()
{
	m_StartPos = 0L;
	SetValue();
	SetModified(m_bModified = FALSE);
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage :: SetValue()
{
	CString szNr;
	VERIFY(szNr.LoadString(IDS_RECIPENR));

	BOOL bModify = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSIERTYPENEINSTELLUNGEN);
	for (int32_t i  = 0; i < MAXANLAGENCOUNT; i++)
	{
		if ( i >= m_sAnzahl)
		{
			HIDE_ID(IDC_SETTINGS_LAYOUT_DOSETYPE1+i);
			HIDE_ID(IDC_SETTINGS_LAYOUT_LOADCELL1+i);
			HIDE_ID(IDC_SETTINGS_LAYOUT_NUMBER1+i);
			HIDE_ID(IDC_SETTINGS_LAYOUT_FEEDERTYPE1 + i);
		}
		else
		{
			ASSERT(i + m_StartPos < m_sAnzahl);
			const int32_t id = i + m_StartPos;
			const auto& aDose = m_AnlagenKonf.GetItem(id);
			{
				SHOW_ID(IDC_SETTINGS_LAYOUT_DOSETYPE1 + i);
				CWnd* pWnd = (CWnd*)GetDlgItem(IDC_SETTINGS_LAYOUT_DOSETYPE1 + i);
				ASSERT(pWnd);
				pWnd->SetWindowText(LPCTSTR(toCString(m_DoseTypeMap.get(aDose.GetDoseType()))));
				pWnd->EnableWindow(bModify);
			}

			{
				if (__ISLOADCELLTYPE(aDose.GetDoseType()))
				{
					SHOW_ID(IDC_SETTINGS_LAYOUT_LOADCELL1 + i);
					CWnd* pWnd = (CWnd*)GetDlgItem(IDC_SETTINGS_LAYOUT_LOADCELL1 + i);
					ASSERT(pWnd);
					pWnd->SetWindowText(LPCTSTR(toCString(m_LCTypeDetectionMap.get(aDose.GetLCTypeDetection()))));
					pWnd->EnableWindow(bModify);
				}
				else
				{
					HIDE_ID(IDC_SETTINGS_LAYOUT_LOADCELL1 + i);
				}
			}

			{
				SHOW_ID(IDC_SETTINGS_LAYOUT_NUMBER1 + i);
				CWnd* pWnd = (CStatic*)GetDlgItem(IDC_SETTINGS_LAYOUT_NUMBER1 + i);
				ASSERT(pWnd);

				CString sz;
				sz.Format(_T("%s%d"), LPCTSTR(szNr), int32_t(id + 1));
				pWnd->SetWindowText(sz);
				pWnd->EnableWindow(bModify);
			}
			{
				if ( __ISVALIDDOSETYPE(aDose.GetDoseType()))
				{
					SHOW_ID(IDC_SETTINGS_LAYOUT_FEEDERTYPE1 + i);
					CWnd* pWnd = (CWnd*)GetDlgItem(IDC_SETTINGS_LAYOUT_FEEDERTYPE1 + i);
					ASSERT(pWnd);
					pWnd->SetWindowText(LPCTSTR(toCString(m_FeedingTypeMap.get(aDose.GetFeedingType()))));
					pWnd->EnableWindow(bModify);
				}
				else
				{
					HIDE_ID(IDC_SETTINGS_LAYOUT_FEEDERTYPE1 + i);
				}

			}
		}
	}
	ENABLE_SHOW_ID(IDC_SETTINGS_LAYOUT_PREVIOUS, BOOL(m_StartPos > 0), m_sAnzahl > MAXANLAGENCOUNT)
	ENABLE_SHOW_ID(IDC_SETTINGS_LAYOUT_NEXT, BOOL(m_StartPos + MAXANLAGENCOUNT < m_sAnzahl), m_sAnzahl > MAXANLAGENCOUNT);
	ENABLE_ID(IDC_SETTINGS_LAYOUT_PLUS, BOOL(m_sAnzahl < GLOBALDOSEMAXCOUNT));
	ENABLE_ID(IDC_SETTINGS_LAYOUT_MINUS, BOOL(m_sAnzahl > 1));

	UpdateData(FALSE);
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CSettingsLayoutPage::OnInitDialog()
{
	SetDefaultFont(IDC_SETTINGS_LAYOUT_LOADCELL1, IDC_SETTINGS_LAYOUT_LOADCELL6);
	SetDefaultFont(IDC_SETTINGS_LAYOUT_DOSETYPE1, IDC_SETTINGS_LAYOUT_DOSETYPE6);
	SetDefaultFont(IDC_SETTINGS_LAYOUT_LOADCELL1, IDC_SETTINGS_LAYOUT_LOADCELL6);
	SetDefaultFont(IDC_SETTINGS_LAYOUT_NUMBER1, IDC_SETTINGS_LAYOUT_NUMBER6);
	SetDefaultFont(IDC_SETTINGS_LAYOUT_FEEDERTYPE1, IDC_SETTINGS_LAYOUT_FEEDERTYPE6);

	auto bResult = CBCGPPropertyPage::OnInitDialog();
	ASSERT(bResult);
	if (bResult)
	{
		CWnd* pWnd = GetDlgItem(IDC_SETTINGS_LAYOUT_COUNT);
		ASSERT(pWnd);
		pWnd->SetFont(&m_aFont);

		m_TrashButton.SizeToContent();
		m_AddButton.SizeToContent();
		m_NextButton.SizeToContent();
		m_PreviousButton.SizeToContent();

		LoadItems();
		Init();
	}
	return bResult;
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::GetValue ()
{
	UpdateData(TRUE);
}
//*************************************************************************************************
//*************************************************************************************************
BOOL CSettingsLayoutPage::OnApply()
{
	if ( m_bModified )
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	return CBCGPPropertyPage::OnApply();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnOK()
{
	if ( m_bModified )
	{
		m_bModified = FALSE;
		GetValue();
		SaveItems();
	}
	CBCGPPropertyPage::OnOK();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnStnClickedSystemLayoutDosiertyp(UINT nID)
{
	GetValue();

	const int32_t activeNr	= _S32(nID-IDC_SETTINGS_LAYOUT_DOSETYPE1 + m_StartPos);
	ASSERT((activeNr >= 0) && (activeNr < GLOBALDOSEMAXCOUNT));
	auto& item = m_AnlagenKonf.GetItem(activeNr);
	int32_t eType = _S32(item.GetDoseType());
	eType = (eType + 1) % _S32(base::cDoseTypeMax);
	item.SetDoseType(static_cast<base::eDoseType>(eType));
	SetModified(m_bModified = TRUE);
	SetValue();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnStnClickedSystemLayoutNr(UINT nID)
{
	GetValue();
	const int32_t activeNr = _S32(nID-IDC_SETTINGS_LAYOUT_NUMBER1 + m_StartPos);
	ASSERT((activeNr >= 0) && (activeNr < GLOBALDOSEMAXCOUNT));
	SetValue();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnStnClickedSystemLayoutDosierWaage(UINT nID)
{
	GetValue();

	const int32_t activeNr	= (int32_t) (nID-IDC_SETTINGS_LAYOUT_LOADCELL1 + m_StartPos);
	ASSERT((activeNr >= 0) && (activeNr < GLOBALDOSEMAXCOUNT));
	auto& item = m_AnlagenKonf.GetItem(activeNr);
	int32_t eType = _S32(item.GetLCTypeDetection());
	eType = (eType + 1) % _S32(base::cMaxLCTypDetection);
	item.SetLCTypeDetection(static_cast<base::eLcTypeDetection>(eType));
	SetModified(m_bModified = TRUE);
	SetValue();
}

//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnStnClickedSystemLayoutFeederTyp(UINT nID)
{
	GetValue();

	const int32_t activeNr = _S32(nID - IDC_SETTINGS_LAYOUT_FEEDERTYPE1 + m_StartPos);
	ASSERT((activeNr >= 0) && (activeNr < GLOBALDOSEMAXCOUNT));
	auto& item = m_AnlagenKonf.GetItem(activeNr);
	int32_t eType = _S32(item.GetFeedingType());
	eType = (eType + 1) % _S32(base::cMaxFeedingType);
	item.SetFeedingType(static_cast<base::eFeedingType>(eType));
	SetModified(m_bModified = TRUE);
	SetValue();
}

//*************************************************************************************************
//*************************************************************************************************
BOOL CSettingsLayoutPage::OnSetActive()
{
	GetParent()->SendMessage(WM_PROPERTYPAGE_CHANGED, WPARAM(this));
	Init ();
	return CBCGPPropertyPage::OnSetActive();
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::SetDefaultFont(const int32_t iStart, const int32_t iEnd)
{
	for (int32_t index = iStart; index <= iEnd; index++)
	{
		GetDlgItem(index)->SetFont(&m_aDisplayFont);
	}
}
//*************************************************************************************************
//*************************************************************************************************
HBRUSH CSettingsLayoutPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CBCGPPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

    switch (nID)
    {
		case IDC_SETTINGS_LAYOUT_COUNT:
		{
			// Set the text color to red
			pDC->SetTextColor(INDEXCOLOR);

			// Set the background mode for text to transparent 
			// so background will show thru.
			pDC->SetBkMode(TRANSPARENT);
		}
		break;

        case IDC_SETTINGS_LAYOUT_LOADCELL1 :
        case IDC_SETTINGS_LAYOUT_LOADCELL2 :
        case IDC_SETTINGS_LAYOUT_LOADCELL3 :
        case IDC_SETTINGS_LAYOUT_LOADCELL4 :
        case IDC_SETTINGS_LAYOUT_LOADCELL5 :
        case IDC_SETTINGS_LAYOUT_LOADCELL6 :
                            {
								int32_t sAktivNr	= (int32_t) (nID-IDC_SETTINGS_LAYOUT_LOADCELL1 + m_StartPos);
								ASSERT(sAktivNr >= 0 && sAktivNr < GLOBALDOSEMAXCOUNT);
								int32_t lcindex = _S32(m_AnlagenKonf.GetItem(sAktivNr).GetLCTypeDetection());

								// Set the text color to red
								pDC->SetTextColor(c_WaageTextcolor[lcindex]);
								pDC->SetBkColor(c_WaageBkcolor[lcindex]);

								// Set the background mode for text to transparent 
								// so background will show thru.
								pDC->SetBkMode(OPAQUE);

								hbr = m_aBrushWaage[lcindex];
                            }
                            break;

        default:    
                            break;
    }
    return hbr;
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnClickedSettingsLayoutNext()
{
	auto save = m_StartPos;
	m_StartPos = __min(m_StartPos + 1, m_sAnzahl - MAXANLAGENCOUNT);
	if (save != m_StartPos)
	{
		SetValue();
	}
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnClickedSettingsLayoutPrevious()
{
	auto save = m_StartPos;
	m_StartPos = __max(m_StartPos - 1, 0);
	if (save != m_StartPos)
	{
		SetValue();
	}
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnClickedSettingsLayoutMinus()
{
	if (m_sAnzahl > 1)
	{
		m_AnlagenKonf.RemoveItem();
		m_sAnzahl = m_AnlagenKonf.GetCount();
		m_StartPos = __max(m_sAnzahl - MAXANLAGENCOUNT, 0);

		SetValue();
		SetModified(m_bModified = TRUE);
	}
}
//*************************************************************************************************
//*************************************************************************************************
void CSettingsLayoutPage::OnClickedSettingsLayoutPlus()
{
	if (m_sAnzahl < GLOBALDOSEMAXCOUNT)
	{
		m_AnlagenKonf.AppendItem();
		m_sAnzahl = m_AnlagenKonf.GetCount();
		m_StartPos = __max(m_sAnzahl - MAXANLAGENCOUNT, 0);

		SetValue();
		SetModified(m_bModified = TRUE);
	}
}


