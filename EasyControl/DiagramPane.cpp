#include "stdafx.h"
#include "EasyControl.h"
#include "DiagramPane.h"


// CDiagramPane

IMPLEMENT_DYNAMIC(CDiagramPane, CDockablePane)

CDiagramPane::CDiagramPane()
{

}

CDiagramPane::~CDiagramPane()
{
}


BEGIN_MESSAGE_MAP(CDiagramPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDiagramPane message handlers



int CDiagramPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_DiagramBar.Create(this, IDD_DIAGRAMBAR,
		CBRS_TOP| CBRS_TOOLTIPS | CBRS_FLYBY, IDD_DIAGRAMBAR))
	{
		TRACE0("Symbolleiste konnte nicht erstellt werden\n");
		return -1;      // Fehler bei Erstellung
	}

	return 0;
}

void CDiagramPane::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDockablePane::OnPaint() for painting messages


	CRect rectTree;
	m_DiagramBar.GetWindowRect(rectTree);
	ScreenToClient(rectTree);


}

void CDiagramPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_DiagramBar.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);

}


