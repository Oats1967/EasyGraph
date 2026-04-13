//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayLwf
///
/// @file   FeederDisplayLwf.h
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

#include "FeederDisplay.h"
#include "IGrafikHopper.h"

/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayLwf command target

class CFeederDisplayLwf : public CFeederDisplay
{
	using CBaseClass = CFeederDisplay;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bRefillEnableChanged			: 1;
			uint32_t bGravimetricChanged			: 1;
			uint32_t bRefillControlEnableChanged	: 1;
			uint32_t bGravVolStateChanged			: 1;
			uint32_t bLCEnableChanged				: 1;
			uint32_t bLCActiveChanged				: 1;
			uint32_t bWeightChanged					: 1;
			uint32_t bRefillMinChanged				: 1;
			uint32_t bRefillMaxChanged				: 1;
			uint32_t bRefillMinMinChanged			: 1;
			uint32_t bDriveCommandChanged			: 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus	m_StatusFlags;


	const uint16_t BKX = 28U;
	const uint16_t BKY = 0;
	const uint16_t BKDX = 68U;
	const uint16_t BKDY = 100U;
	const RECT cBackgroundRect = { BKX, BKY, BKX + BKDX, BKY + BKDY };

// Attributes
protected:
	CBrush		m_brushBlueSolid;
	CBrush		m_brushYellowSolid;
	CBrush		m_brushRedDashed;
	CBrush		m_brushWhiteDashed;
	CBrush		m_brushYellowDashed;
	CPen		m_PenBlue;

protected:
	//------------------------------------------------------------------------------------
	///  @brief   DrawFilling
	///
	///           This method ....
	///
	///  @param[in]  pDC , CDC *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual void	DrawFilling	(CDC*);
	virtual void	DrawFillingScale(CDC* pDC);
	virtual void	DrawFillingNoScale(CDC* pDC);
	//virtual void	DrawBitmap(CDC* pDC);

	virtual void	DrawDriveCommand(CDC*);
	virtual void	DrawRefillState	(CDC*);
	virtual void	DrawGravVol		(CDC *);

	virtual const POINT& GetFillPoint() const = 0;
	virtual const POINT& GetGravVolPoint() const = 0;
	virtual const POINT& GetWeightPoint() const = 0;
	virtual const POINT& GetDriveCommandPoint() const = 0;
	virtual const POINT& GetRefillStatePoint() const = 0;

	virtual IGrafikHopper& GetGrafikHopper() = 0;

protected:
	void OnRedraw(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

	const RECT& GetBackgroundRect() const override
	{	return cBackgroundRect;		}

	CFeederDisplayLwf() = delete;

public:
	CFeederDisplayLwf(int32_t sID, const CRect& rRect);
	~CFeederDisplayLwf()  override = default;
	CFeederDisplayLwf(const CFeederDisplayLwf&) = delete;
	CFeederDisplayLwf& operator = (const CFeederDisplayLwf&) = delete;
};




