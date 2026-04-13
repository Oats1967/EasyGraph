//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseButtonGrafik
///
/// @file   DoseButtonGrafik.h
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

#include "DoseButtonGrafikContext.h"


class CDoseButtonGrafik : public CDoseButtonGrafikContext
{
	using CBaseClass = CDoseButtonGrafikContext;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bIdChanged				  : 1;
			uint32_t bLineModeChanged		  : 1;
			uint32_t bFeedingTypeChanged	  : 1;
			uint32_t bRunningChanged		  : 1;
			uint32_t bStartedChanged		  : 1;
			uint32_t bReleaseChanged		  : 1;
			uint32_t bIOStartInputChanged     : 1;
			uint32_t bIOLineStartInputChanged : 1;
			uint32_t bIOLogoStartChanged	  : 1;
			uint32_t bSetpointChanged		  : 1;
			uint32_t bCalibrationChanged	  : 1;
			uint32_t bEmptyFeederChanged	  : 1;
			uint32_t bTaringChanged			  : 1;
		} flags;
		uint32_t allflags = 0;
	};

	enum class OpState : uint32_t
	{
		eOp_Off = 0,
		eOp_Ready,
		eOp_Run
	};
private:
	CFont				m_idFont;
	const POINT			cIDPoint;
	const POINT			cDoseButtonPoint;
	OpState				m_OpState;
	CPoint				m_OrgPoint;
	DosierBaseStatus	m_StatusFlags;

private:
	void CenterTextOut(CDC* pDC, const POINT& p, const CString& sz, COLORREF color = COLORBLACK, int32_t bkmode = OPAQUE);

protected:
	void OnRedraw(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNew) override;

	virtual BOOL GetReady() const { return FALSE; }

	virtual void DrawID(CDC* pDC, const POINT& p);
	virtual void DrawStatus(CDC* pDC);

private:
	CDoseButtonGrafik(void) = delete;

public:
	static CDoseButtonGrafik* Create(base::eDoseType lTyp, const CRect& rRect);

	CDoseButtonGrafik(const CRect& rRect);
	~CDoseButtonGrafik(void) override = default;
	CDoseButtonGrafik(const CDoseButtonGrafik&) = delete;
	CDoseButtonGrafik& operator = (const CDoseButtonGrafik&) = delete;

	void FlashButton(CDC* pDC) override;
	BOOL IsVisible() const override;

	virtual BOOL IsButtonRun() const;
	virtual BOOL IsButtonReady() const;
	virtual BOOL IsButtonStart() const;
};










