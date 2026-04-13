//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplay
///
/// @file   FeederDisplay.h
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

#include "DoseGrafikContext.h"



class CFeederDisplay : public CDoseGrafikContext
{
	using CBaseClass = CDoseGrafikContext;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bIdChanged : 1;
			uint32_t bLineModeChanged : 1;
			uint32_t bFeedingTypeChanged : 1;
			uint32_t bRunningChanged : 1;
			uint32_t bReleaseChanged : 1;
			uint32_t bIOStartInputChanged : 1;
			uint32_t bIOLineStartInputChanged : 1;
			uint32_t bIOLogoStartChanged : 1;
			uint32_t bSetpointChanged : 1;
			uint32_t bCalibrationChanged : 1;
			uint32_t bEmptyFeederChanged : 1;
			uint32_t bTaringChanged : 1;
			uint32_t bIOReleaseInputChanged : 1;
			uint32_t bDoseTypeChanged : 1;
			uint32_t bAlarmChanged : 1;
			uint32_t bWarningChanged : 1;
			uint32_t bRefillAlarmChanged : 1;
			uint32_t bRefillActiveChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};

private:
	static const COLORREF c_alarmfillcolor = RGB(251, 188, 179);
	static const COLORREF c_runfillcolor = RGB(184, 239, 184);
	static const COLORREF c_warningfillcolor = RGB(255, 237, 164);

private:
	CBrush			c_AlarmBrush;
	CBrush			c_WarningBrush;
	CBrush			c_RunBrush;
	CPen			c_AlarmPen;
	CPen			c_WarningPen;
	CPen			c_RunPen;
	DosierBaseStatus m_StatusFlags;

protected:
	void	OnUpdate(CDC* ) override
	{}

	void	OnRedraw(CDC* pDC) override;
	BOOL	IsBitmapUpdate(void) const override;
	void	OnSetNewData(const base::CDoseDataCache& rNew) override;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   DrawCalibration
	///
	///           This method ....
	///
	///  @param[in]  pDC , CDC *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual void	DrawCalibration(CDC*);
	virtual void	DrawEmptyFeeder(CDC* pDC);
	virtual void	DrawRefillState(CDC* pDC);
	virtual void	DrawTaring(CDC*);
	virtual void	DrawRelease(CDC* pDC);
	virtual void	DrawBackground(CDC* pDC);
	virtual void	DrawBitmap(CDC* ) {};

	virtual const POINT& GetReleasePoint() const = 0;
	virtual const POINT& GetCalibratePoint() const = 0;
	virtual const POINT& GetTarePoint() const = 0;
	virtual const POINT& GetEmptyFeederPoint() const = 0;
	virtual const POINT& GetRefillPoint() const = 0;
	virtual const RECT&  GetBackgroundRect() const = 0;

private:
	void	FillRoundRect(CDC* pDC, const CRect& aRect, CBrush* pBrush, CPen* pPen);

private:
	CFeederDisplay() = delete;

public:
	//------------------------------------------------------------------------------------
	///  @brief   Create
	///
	///           This method ....
	///
	///  @param[in] lTyp , long
	///  @param[in] sID , int32_t
	///  @return CFeederDisplay*
	///
	//------------------------------------------------------------------------------------
	static CFeederDisplay* Create(int32_t sID, base::eDoseType lTyp, const CRect& rRect);

	CFeederDisplay(int32_t sID, const CRect& rRect);
	CFeederDisplay(const CFeederDisplay&) = delete;
	CFeederDisplay& operator = (const CFeederDisplay&) = delete;
	~CFeederDisplay(void) override = default;

	void	Update(CDC* pDC, CStatic& aWnd);
};










