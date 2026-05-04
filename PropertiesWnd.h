#pragma once

#include <map>
#include "MassflowSelectMap.h"


class CUniqueProperty
{
protected:
	base::eMassflowSelect m_Select;
	int32_t m_Id;


	CUniqueProperty(const int32_t id = -1, const base::eMassflowSelect select = base::eMassflowSelect::eVIEWMAX) : 
		m_Id(id)
		,m_Select(select)
	{}

public:
	SETGET(const uint32_t, Id);
	SETGET(const base::eMassflowSelect, Select);
};

class CPropertySettingsGrid : public CMFCPropertyGridProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertySettingsGrid)

public:
	CPropertySettingsGrid(const int32_t id, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars), 
		CUniqueProperty(id)
	{}

	CPropertySettingsGrid(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
		CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList), CUniqueProperty()
	{}
};


class CPropertyGrid : public CMFCPropertyGridProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyGrid)

public:
	CPropertyGrid(const int32_t id, const base::eMassflowSelect select, const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL) :
		CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars),
		CUniqueProperty(id, select)
	{}

	CPropertyGrid(const int32_t id, const base::eMassflowSelect select, const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE) :
		CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList),
		CUniqueProperty(id, select)
	{}
};

class CPropertyColorGrid : public CMFCPropertyGridColorProperty, public CUniqueProperty
{
	DECLARE_DYNAMIC(CPropertyColorGrid)

public:
	CPropertyColorGrid(const int32_t id, const base::eMassflowSelect select, const CString& strName, const COLORREF& color, CPalette* pPalette = NULL, LPCTSTR lpszDescr = NULL) :
		CMFCPropertyGridColorProperty(strName, color, pPalette, lpszDescr),
		CUniqueProperty(id, select)
	{}
};


class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// Konstruktion
public:
	CPropertiesWnd() noexcept;

	void AdjustLayout();

// Attribute
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	//CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	int32_t m_LinienColorPos;
	CMassflowSelectMap c_MassflowSelectMap;

private:
	void InitPropList();
	void OnSetColor(CPropertyColorGrid* pGrid);
	void OnSetLineWidth(CPropertyGrid* pGrid);
	void OnSetCategory(CPropertyGrid* pGrid);
	void OnSetVisible(CPropertyGrid* pGrid);
	void OnSetRefreshtime(CPropertySettingsGrid* pGrid);
	void OnSetHistory(CPropertySettingsGrid* pGrid);

	CPropertyGrid* CreateProperty(const base::eMassflowSelect select);
	CPropertySettingsGrid* CreateRealTimeMonitoringProperty();



// Implementierung
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	LRESULT OnPropertyChanged(__in WPARAM wparam, __in LPARAM lParam);
	DECLARE_MESSAGE_MAP()


	void SetPropListFont();

	int m_nComboHeight;
};

