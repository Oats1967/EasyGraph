//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabMassflow
///
/// @file   LineTabMassflow.h
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

#include "EasyControl.h"
#include "LineTabFeederInfo.h"

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CLineTabMassflow : public CDialog
{
	DECLARE_DYNAMIC(CLineTabMassflow)

	enum { IDD = IDD_LINETAB_MASSFLOW };

private:
	std::array<CFeederInfo, eTotFeedingType::FT_MAX> m_FeederInfo;

private:
	void SetValue();
	void SetValue(CFeederInfo& rInfo);
	void GetValue();

	void SetTitle(const eTotFeedingType feeder, const CString& title);
	void ReposLineStatus(CFeederInfo& rInfo);
	void InitLineStatus(CFeederInfo& rInfo);
	void InitFeedertatus(CFeederInfo& rInfo);
	void ReposFeederStatus(CFeederInfo& rInfo);
	void SetMassflow(const eTotFeedingType, const float32_t);
	void SetExtInput(const eTotFeedingType, BOOL);
	void SetRun(const eTotFeedingType, BOOL);
	void Redraw(const eTotFeedingType);

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;

public:
	CLineTabMassflow(CWnd* pParent = NULL);
	~CLineTabMassflow() override = default;

	LRESULT OnTimerRefresh(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabMassflow::SetTitle(const eTotFeedingType feeder, const CString& value)
{
	ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
	CFeederInfo& rInfo = m_FeederInfo[feeder];
	if (rInfo.m_Title != value)
	{
		rInfo.m_Title	= value;
		rInfo.m_Redraw	= TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabMassflow::SetMassflow(const eTotFeedingType feeder, const float32_t value)
{
	ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
	CFeederInfo& rInfo = m_FeederInfo[feeder];
	if (rInfo.m_Massflow != value)
	{
		rInfo.m_Massflow = value;
		rInfo.m_Redraw = TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabMassflow::SetExtInput(const eTotFeedingType feeder, const BOOL value)
{
	ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
	CFeederInfo& rInfo = m_FeederInfo[feeder];
	if (rInfo.m_ExtInput != value)
	{
		rInfo.m_ExtInput = value;
		rInfo.m_Redraw	 = TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabMassflow::SetRun(const eTotFeedingType feeder, const BOOL value)
{
	ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
	CFeederInfo& rInfo = m_FeederInfo[feeder];
	if (rInfo.m_Run != value)
	{
		rInfo.m_Run = value;
		rInfo.m_Redraw = TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabMassflow::Redraw(const eTotFeedingType feeder)
{
	if (IsWindowVisible())
	{
		ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
		CFeederInfo& rInfo = m_FeederInfo[feeder];
		if (rInfo.m_Redraw)
		{
			rInfo.m_Redraw = FALSE;
			SetValue(rInfo);
		}
	}
}
