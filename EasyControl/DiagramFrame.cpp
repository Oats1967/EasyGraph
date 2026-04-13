//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramFrame.cpp
///
/// @file   DiagramFrame.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "MainFrm.h"
#include "RemoteControl.h"
#include "DateTimePicker.h"
#include "EasyControl.h"
#include "RecView.h"
#include "DiagramView.h"
#include "DiagramFrame.h"
#include "DiagramScaleDlg.h"



enum { eRECORDERSELECT = 0, eDIAGRAMSELECT };


IMPLEMENT_DYNCREATE(CDiagramFrame, CMDIChildWndEx)



CDiagramFrame :: CDiagramFrame() : 
                      m_pDiagramView(NULL)
                    , m_pRecView(NULL)
{
}
//***************************************************************************************************
//***************************************************************************************************
BEGIN_MESSAGE_MAP(CDiagramFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
    ON_COMMAND(ID_TB_DIAGRAMBAR_3DLINE, OnDiagramFrame3DLine)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_3DLINE, OnDiagramFrame3DLineUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_TABLE, OnDiagramFrameTable)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_TABLE, OnDiagramFrameTableUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_GRAPHIC, OnDiagramFrameGraphic)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_GRAPHIC, OnDiagramFrameGraphicUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_DIAGRAMSELECT, OnDiagramFrameDiagramSelectMenu)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_DIAGRAMSELECT, OnDiagramFrameDiagramSelectMenuUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_DOSEITEM, OnDiagramFrameDoseItemMenu)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_DOSEITEM, OnDiagramFrameDoseItemMenuUpdateUI)
    ON_COMMAND_RANGE(ID_MN_DIAGRAMSELECT_DOSEPERFORMANCE, ID_MN_DIAGRAMSELECT_WEIGHT, OnDiagramFrameDiagramSelect)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MN_DIAGRAMSELECT_DOSEPERFORMANCE, ID_MN_DIAGRAMSELECT_WEIGHT, OnDiagramFrameDiagramSelectUpdateUI)
    ON_COMMAND_RANGE(ID_MN_DOSEITEM00, ID_MN_DOSEITEM32, OnDiagramFrameDoseItem)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MN_DOSEITEM00, ID_MN_DOSEITEM32, OnDiagramFrameDoseItemUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_TIMESELECT, OnDiagramFrameTimeSelectMenu)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_TIMESELECT, OnDiagramFrameTimeSelectMenuUpdateUI)
    ON_COMMAND_RANGE(ID_MN_TIMESELECT_DATE, ID_MN_TIMESELECT_TODAY, OnDiagramFrameTimeSelect)
    ON_UPDATE_COMMAND_UI_RANGE(ID_MN_TIMESELECT_DATE, ID_MN_TIMESELECT_TODAY, OnDiagramFrameTimeSelectUpdateUI)
    ON_COMMAND(ID_TB_DIAGRAMBAR_SCALE, OnDiagramFrameScaleSelectMenu)
    ON_UPDATE_COMMAND_UI(ID_TB_DIAGRAMBAR_SCALE, OnDiagramFrameScaleSelectMenuUpdateUI)
END_MESSAGE_MAP()

//***************************************************************************************************
//***************************************************************************************************
BOOL CDiagramFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    ASSERT(cs.style & WS_CHILD);
	//cs.style &= ~WS_SYSMENU;
    //cs.style |= WS_MAXIMIZE | WS_VISIBLE;
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
//***************************************************************************************************
//***************************************************************************************************
int CDiagramFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CString szFormat;

	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


    VERIFY(szFormat.LoadString(IDS_DIAGRAM));
    SetTitle(szFormat);
	return 0;
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDestroy()
{
    if (GetActiveView() == m_pDiagramView)
    {
        delete m_pRecView;
        m_pRecView = NULL;
    }
    else
    {
        delete m_pDiagramView;
        m_pDiagramView = NULL;
    }

	CMDIChildWndEx::OnDestroy();
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::ActivateFrame(int nCmdShow)
{
    nCmdShow = SW_SHOWMAXIMIZED;
    m_pDiagramView = GetActiveView();
    CMDIChildWndEx::ActivateFrame(nCmdShow);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::AddViewToDocument(CView* pOldView, CView* pNewView)
{
    if (pOldView != pNewView)
    {
        CDocument* pDoc = NULL;

        if (pOldView != NULL)
        {
            pDoc = pOldView->GetDocument();
        }
        else if (pNewView != NULL)
        {
            pDoc = pNewView->GetDocument();
        }
        if (pDoc)
        {
            // Show the newly active view and hide the inactive view.
            if (pOldView)
            {
                BOOL bAutoDelete = pDoc->m_bAutoDelete;
                pOldView->ShowWindow(SW_HIDE);
                pDoc->m_bAutoDelete = FALSE;
                // Dettach existing view
                pDoc->RemoveView(pOldView);
                pDoc->m_bAutoDelete = bAutoDelete;
            }
            if (pNewView)
            {
                // Attach new view
                pDoc->AddView(pNewView);

                pNewView->ShowWindow(SW_SHOW);
            }
        }
    }
}
//***************************************************************************************************
//***************************************************************************************************
CView* CDiagramFrame::CreateView(base::eDiagramType diagramtype)
{
    CView* pNewView = NULL;

    if (diagramtype == base::eDiagramType::eTable)
    {
        if (! m_pRecView)
        {
            if (!m_pDiagramView)
            {
                m_pDiagramView = GetActiveView();
            }
            // create the new view
            m_pRecView = new CRecView;
            m_pRecView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
        }
        pNewView = m_pRecView;
    }
    else
    {
        if ( ! m_pDiagramView)
        {
            if (!m_pRecView)
            {
                m_pRecView = GetActiveView();
            }
            // create the new view
            m_pDiagramView = new CDiagramView;
            m_pDiagramView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL);
        }
        pNewView = m_pDiagramView;
    }
    return pNewView;
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameGraphic()
{
    auto oldView = GetActiveView();
    auto newview = CreateView(base::eDiagramType::e2DLine);
    if (newview != oldView)
    {
        AddViewToDocument(oldView, newview);
        SetActiveView(newview);
        RecalcLayout();
    }
    if (newview)
    {
        newview->SendMessage(WM_DIAGRAMBAR_DIAGRAMTYPE, WPARAM(base::eDiagramType::e2DLine));
    }
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameGraphicUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetDiagramType() == base::eDiagramType::e2DLine);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrame3DLine()
{
    auto oldView = GetActiveView();
    auto newview = CreateView(base::eDiagramType::e3DLine);
    if (newview != oldView)
    {
        AddViewToDocument(oldView, newview);
        SetActiveView(newview);
        RecalcLayout();
    }
    if (newview)
    {
        newview->SendMessage(WM_DIAGRAMBAR_DIAGRAMTYPE, WPARAM(base::eDiagramType::e3DLine));
    }
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrame3DLineUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetDiagramType() == base::eDiagramType::e3DLine);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameScaleSelectMenuUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameScaleSelectMenu()
{
    auto& rDiagramSettings = GetDiagramConfig();
    CDiagramScaleDlg m_dlg(this, rDiagramSettings.GetAxisScale());
    m_dlg.DoModal();
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameTable()
{
    auto oldView = GetActiveView();
    auto newview = CreateView(base::eDiagramType::eTable);
    if (newview != oldView)
    {
        AddViewToDocument(oldView, newview);
        SetActiveView(newview);
        RecalcLayout();
    }
    if (newview)
    {
        newview->SendMessage(WM_DIAGRAMBAR_DIAGRAMTYPE, WPARAM(base::eDiagramType::eTable));
    }
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameTableUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(GetDiagramType() == base::eDiagramType::eTable);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDoseItemMenu()
{
    auto& toolbar = ((CMainFrame*)AfxGetMainWnd())->GetToolBar(eViewSelect::eDiagramView);
    auto* bt = (CMFCToolBarMenuButton*)toolbar.GetButton(4);
    bt->OnClick(&toolbar, 1);

}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameDoseItemMenuUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDoseItem(UINT id)
{
    int32_t newItem = -1;
    if (id != ID_MN_DOSEITEM00)
    {
        newItem = id - ID_MN_DOSEITEM01;
    }
    CView* pActiveView = GetActiveView();
    ASSERT(pActiveView);
    pActiveView->SendMessage(WM_DIAGRAMBAR_DOSEITEM, WPARAM(newItem));

}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameDoseItemUpdateUI(CCmdUI* pCmdUI)
{
    BOOL result = FALSE;
    const auto& tr = GetDiagramConfig();
    if (tr.GetID() < 0)
    {
        result = pCmdUI->m_nIndex == 0;
    }
    else
    {
        result = _S32(tr.GetID() + 1) == _S32(pCmdUI->m_nIndex);
    }
    if (result)
    {
        pCmdUI->SetCheck(result);
    }
    pCmdUI->Enable(TRUE);
}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameTimeSelectMenu()
{
    auto& toolbar = ((CMainFrame*)AfxGetMainWnd())->GetToolBar(eViewSelect::eDiagramView);
    auto* bt = (CMFCToolBarMenuButton*)toolbar.GetButton(5);
    bt->OnClick(&toolbar, 1);
}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameTimeSelectMenuUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameTimeSelect(UINT id)
{
    base::eTimeMode newTime = base::eTimeMode::TG_NONE;
    time_t date = 0;

    switch (id)
    {
        case ID_MN_TIMESELECT_DATE:
            {
                CDateTimePickerDlg m_wndDateTime;
                m_wndDateTime.DoModal();
                date = m_wndDateTime.GetDate();
                newTime = base::eTimeMode::TG_DATE;
            }
            break;
        case ID_MN_TIMESELECT_TODAY:
            newTime = base::eTimeMode::TG_TODAY;
            break;
        default:
            newTime = base::eTimeMode::TG_SECONDS;
            break;
    }
    CView* pActiveView = GetActiveView();
    ASSERT(pActiveView);
    pActiveView->SendMessage(WM_DIAGRAMBAR_TIME, WPARAM(newTime), date);
}
//***************************************************************************************************
//***************************************************************************************************
void  CDiagramFrame::OnDiagramFrameTimeSelectUpdateUI(CCmdUI* pCmdUI)
{
    const auto& tr = GetDiagramConfig();
    BOOL bChecked = FALSE;
    BOOL bEnable = FALSE;
    switch (pCmdUI->m_nID)
    {
    case ID_MN_TIMESELECT_DATE:
        bChecked = tr.GetTimeInterval() == base::eTimeMode::TG_DATE;
        bEnable = tr.GetDiagramType() == base::eDiagramType::eTable;
        break;
    case ID_MN_TIMESELECT_TODAY:
        bChecked = tr.GetTimeInterval() == base::eTimeMode::TG_TODAY;
        bEnable = tr.GetDiagramType() == base::eDiagramType::eTable;
        break;
    case ID_MN_TIMESELECT_SECONDS:
        bChecked = tr.GetTimeInterval() == base::eTimeMode::TG_SECONDS;
        bEnable = ( tr.GetDiagramType() == base::eDiagramType::e2DLine ) || (tr.GetDiagramType() == base::eDiagramType::e3DLine);
        break;
    default:
        ASSERT(FALSE);
        break;
    }
    if (bChecked)
    {
        pCmdUI->SetCheck(bChecked);
    }
    pCmdUI->Enable(bEnable);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDiagramSelectMenu()
{
    auto& toolbar = ((CMainFrame*)AfxGetMainWnd())->GetToolBar(eViewSelect::eDiagramView);
    auto* bt = (CMFCToolBarMenuButton*)toolbar.GetButton(3);
    bt->OnClick(&toolbar, 1);
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDiagramSelectMenuUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDiagramSelectUpdateUI(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
    const auto rSelect = GetDiagramConfig().GetDiagramSelect();
    BOOL result = FALSE;

    switch(pCmdUI->m_nID)
    {
		case ID_MN_DIAGRAMSELECT_MASSFLOW:
            result = rSelect.flags.bitVIEWMASSFLOW;
            break;
        case ID_MN_DIAGRAMSELECT_SETPOINT:
            result = rSelect.flags.bitVIEWSETPOINT;
            break;
        case ID_MN_DIAGRAMSELECT_DRIVECOMMAND:
            result = rSelect.flags.bitVIEWDRIVECOMMAND;
            break;
        case ID_MN_DIAGRAMSELECT_WEIGHT:
            result = rSelect.flags.bitVIEWWEIGHT;
            break;
        case ID_MN_DIAGRAMSELECT_DOSEPERFORMANCE:
            result = rSelect.flags.bitVIEWDOSEPERFORMANCE;
            break;
        case ID_MN_DIAGRAMSELECT_TOTALIZER:
            result = rSelect.flags.bitVIEWTOTALIZER;
            break;
        case ID_MN_DIAGRAMSELECT_ROTSPEED:
            result = rSelect.flags.bitVIEWROTSPEED;
            break;
        default:
            break;
    }
    if (result)
    {
        pCmdUI->SetCheck(result);
    }
}
//***************************************************************************************************
//***************************************************************************************************
void CDiagramFrame::OnDiagramFrameDiagramSelect(UINT id)
{
    auto iSelect = GetDiagramConfig().GetDiagramSelect();
    switch (id)
	{
		case ID_MN_DIAGRAMSELECT_MASSFLOW:
                iSelect.flags.bitVIEWMASSFLOW = !iSelect.flags.bitVIEWMASSFLOW;
				break;
		case ID_MN_DIAGRAMSELECT_SETPOINT:
                iSelect.flags.bitVIEWSETPOINT = ! iSelect.flags.bitVIEWSETPOINT;
                break;
		case ID_MN_DIAGRAMSELECT_DRIVECOMMAND:
                iSelect.flags.bitVIEWDRIVECOMMAND = !iSelect.flags.bitVIEWDRIVECOMMAND;
                break;
		case ID_MN_DIAGRAMSELECT_WEIGHT:
                iSelect.flags.bitVIEWWEIGHT = ! iSelect.flags.bitVIEWWEIGHT;
                break;
		case ID_MN_DIAGRAMSELECT_DOSEPERFORMANCE:
                iSelect.flags.bitVIEWDOSEPERFORMANCE = !iSelect.flags.bitVIEWDOSEPERFORMANCE;
				break;
		case ID_MN_DIAGRAMSELECT_TOTALIZER:
                iSelect.flags.bitVIEWTOTALIZER = !iSelect.flags.bitVIEWTOTALIZER;
                break;
        case ID_MN_DIAGRAMSELECT_ROTSPEED:
                iSelect.flags.bitVIEWROTSPEED = !iSelect.flags.bitVIEWROTSPEED;
                break;
        default:
				ASSERT(FALSE);
				break;
	}
    CView* pActiveView = GetActiveView();
    ASSERT(pActiveView);
    pActiveView->SendMessage(WM_DIAGRAMBAR_SELECT, WPARAM(iSelect.allflags));
}







