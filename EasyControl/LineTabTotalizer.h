//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabTotalizer
///
/// @file   LineTabTotalizer.h
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
#include "ButtonStartStop.h"

class CTotalizerState : public CBCGPStatic
{
	BOOL m_State;
public:
	CTotalizerState() : m_State(-1)
	{
	}

	void SetState(BOOL bState)
	{
		if (bState != m_State)
		{
			m_State = bState;
			if (IsWindowEnabled())
			{
				SetPicture((bState)? IDB_BTGREENBIG : IDB_BTNONEBIG);
			}
		}
	}
};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
class CLineTabTotalizer : public CDialog
{
	DECLARE_DYNAMIC(CLineTabTotalizer)

	enum { IDD = IDD_LINETAB_TOTALIZER };

private:
	CTotalizerState			m_Impuls;
	CButtonStartStop		m_TotalizerResetBt;
	std::array<CFeederInfo, eTotFeedingType::FT_MAX> m_FeederInfo;
	float32_t				 m_TotalizerLine;
	float32_t				 m_TotalizerAll;
	uint32_t				 m_ProductionTime;

private:
	void SetValue();
	void SetValue(CFeederInfo& rInfo);
	void GetValue();

	void SetTitle(const eTotFeedingType feeder, const CString& title);
	void ReposLineStatus(CFeederInfo& rInfo);
	void InitLineStatus(CFeederInfo& rInfo);
	void InitFeedertatus(CFeederInfo& rInfo);
	void ReposFeederStatus(CFeederInfo& rInfo);
	void SetTotalizer(const eTotFeedingType, const float32_t);
	void SetExtInput(const eTotFeedingType, BOOL);
	void SetRun(const eTotFeedingType, BOOL);
	void Redraw(const eTotFeedingType);

	SETGET(float32_t, TotalizerLine)
	SETGET(float32_t, TotalizerAll)
	SETGET(uint32_t, ProductionTime)

protected:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;

public:
	CLineTabTotalizer(CWnd* pParent = NULL);
	~CLineTabTotalizer() override = default;

	LRESULT OnTimerRefresh(WPARAM, LPARAM);
	afx_msg void OnBnClickedLineTabTotalizerClear();

	DECLARE_MESSAGE_MAP()
};
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabTotalizer::SetTitle(const eTotFeedingType feeder, const CString& value)
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
inline void CLineTabTotalizer::SetTotalizer(const eTotFeedingType feeder, const float32_t value)
{
	ASSERT(feeder >= eTotFeedingType::FT_MF && feeder < eTotFeedingType::FT_MAX);
	CFeederInfo& rInfo = m_FeederInfo[feeder];
	if (rInfo.m_Totalizer != value)
	{
		rInfo.m_Totalizer = value;
		rInfo.m_Redraw = TRUE;
	}
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
inline void CLineTabTotalizer::SetExtInput(const eTotFeedingType feeder, const BOOL value)
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
inline void CLineTabTotalizer::SetRun(const eTotFeedingType feeder, const BOOL value)
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
inline void CLineTabTotalizer::Redraw(const eTotFeedingType feeder)
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
