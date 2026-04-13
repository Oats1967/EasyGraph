//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DosePage
///
/// @file   DosePage.h
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

#include <memory>
#include "EasyControl.h"
#include "remote/include/RemoteInterface.h"
#include "GrafikContainer.h"
#include "BASE/include/DoseType.h"
#include "BASE/include/FeedingTypeMap.h"
#include "FormatMetric.h"



class CDosePage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDosePage)

private:
	const base::CFeedingTypeMap	 c_FeedingTypeMap;

protected:
	CGrafikContainer m_aGrafikContainer;
	CFont            m_aFont;
	const CBrush     c_EditBrush;
	CString          m_szName;
	uint64_t		 m_QMNumber;
	CString          m_szFeederType;
	CString          m_szTitle;
	int32_t     	 m_lNr;
	int32_t          m_sItem;
	base::eDoseType	 m_lDoseType;

protected:
	virtual BOOL OnUpdateControls();
	virtual void SetValue(void);
	virtual void GetValue(void);

	BOOL OnInitDialog() override;
	BOOL OnSetActive() override;
	BOOL OnKillActive() override;
	void OnOK() override;
	void BuildHeadLine(CListCtrl& _aListCtrl);
	void ToggleLineMode();
	void ToggleGravimetricMode();
	void ToggleReleaseMode();
	void SetNumberFont(int32_t nr);

	void CreateHelpInfoBox(const int32_t InfoId, const int32_t captionId, const int32_t textId);
	void CreateToolTip(UINT ctrl, UINT MessageID);
	void CreateAgitatorBox(UINT ctrl, UINT messageID);
	void CreateProductDatabaseBox(UINT ctrl, BOOL bName);

public:
	virtual BOOL OnNotifyEditName();
	virtual BOOL OnNotifyEditQMNumber();

public:
	CDosePage(UINT id);
	~CDosePage() override = default;

	LRESULT OnDoseNewItem(WPARAM w, LPARAM);
	LRESULT OnNewProductKey(WPARAM id, LPARAM qmnummer);
	LRESULT OnSpeedBoxClosed(WPARAM, LPARAM);
	LRESULT OnGainFocus(WPARAM, LPARAM);
	LRESULT OnTimerRefresh(WPARAM w, LPARAM);

	DECLARE_MESSAGE_MAP()
};


