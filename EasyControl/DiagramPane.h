#pragma once

#include "DiagramBar.h"

// myPane.h, CDiagramPane class which is inherit from CDockablePane 

class CDiagramPane : public CDockablePane
{
	DECLARE_DYNAMIC(CDiagramPane)

public:
	CDiagramPane();
	virtual ~CDiagramPane();
	CDiagramBar m_DiagramBar;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CDiagramBar& GetDiagramBar()
	{
		return m_DiagramBar;
	}
};

