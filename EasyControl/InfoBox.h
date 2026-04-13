#pragma once

#include "EasyControl.h"
#include "InfoBoxCtrl.h"
#include "BASE/Utils/public/LogItem.h"
#include "ModelessDlg.h"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
class CInfoBox : public CModelessDlg
{
	using CBaseClass = CModelessDlg;

	DECLARE_DYNAMIC(CInfoBox)

	enum { IDD = IDD_INFOBOX };

private:
	CInfoBoxCtrl m_wndInfoBox;
	base::utils::CLogItem m_aLogItem;
	CRect m_orgrect;

private:
	void GetValue(void)
	{}

	void SetValue(void)
	{}

	void SetText();
	void ClearMessage();

protected:
	void DoDataExchange(CDataExchange* pDX)  override;
	BOOL OnInitDialog() override;

public:
	CInfoBox();
	~CInfoBox() override
	{}


	void SetErrorItem(const base::utils::CLogItem& aLogItem);
	void DeleteErrorItem();
	BOOL IsErrorItem() const
	{ return m_aLogItem.GetID() >= 0;	}

	void Create(CWnd* pParent, const CPoint& top);
	void Move(const CPoint& top) override;


	afx_msg void OnStnClickedInfoMessage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
