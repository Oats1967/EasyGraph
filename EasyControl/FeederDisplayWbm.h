//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module FeederDisplayWbm.h
///
/// @file   FeederDisplayWbm.h
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
#include "HopperWbm.h"
/////////////////////////////////////////////////////////////////////////////
// CFeederDisplayWbm command target

class CFeederDisplayWbm : public CFeederDisplay
{
	using CBaseClass = CFeederDisplay;

	union DosierBaseStatus
	{
		struct
		{
			uint32_t bGravimetricChanged : 1;
			uint32_t bMinBeltLoadAlarmChanged : 1;
			uint32_t bGravVolStateChanged : 1;
			uint32_t bMinBeltLoadStatusChanged : 1;
			uint32_t bWeightChanged : 1;
			uint32_t bRefillMaxChanged : 1;
			uint32_t bBeltLoadMassflowChanged : 1;
			uint32_t bDriveCommandChanged : 1;
		} flags;
		uint32_t allflags = 0;
	};
	DosierBaseStatus	m_StatusFlags;

private:
	const POINT cCalibratePoint;
	const POINT cTarePoint;
	const POINT cEmptyFeederPoint;
	const POINT cReleasePoint;
	const POINT cGravVolPoint;
	const POINT cWeightPoint;
	const POINT cBeltLoadPoint;
	const POINT cRefillStatePoint;
	const RECT	cBackgroundRect;

	CBrush		m_brushBlueSolid;
	CPen		m_PenBlue;
	CHopperWbm  m_Hopper;

protected:

	//------------------------------------------------------------------------------------
	///  @brief   DrawGravVol
	///
	///           This method ....
	///
	///  @param[in]  pDC , CDC *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual void	DrawGravVol(CDC*);

	//------------------------------------------------------------------------------------
	///  @brief   DrawBeltLoad
	///
	///           This method ....
	///
	///  @param[in]  pDC , CDC *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual	void	DrawBeltLoad(CDC*);

	//------------------------------------------------------------------------------------
	///  @brief   DrawFilling
	///
	///           This method ....
	///
	///  @param[in]  pDC , CDC *
	///  @return none
	///
	//------------------------------------------------------------------------------------
	virtual void	DrawFilling(CDC* pDC);

	//***************************
	// ********************************************************************************
	//***********************************************************************************************************
	const POINT& GetReleasePoint() const final
	{ return  cReleasePoint;	}
	const POINT& GetCalibratePoint() const final
	{ return  cCalibratePoint;	}
	const POINT& GetTarePoint() const final
	{ return  cTarePoint;	}
	const POINT& GetEmptyFeederPoint() const final
	{ return  cEmptyFeederPoint;	}
	const POINT& GetRefillPoint() const  final
	{	return cRefillStatePoint; 	}
	const RECT& GetBackgroundRect() const final
	{	return cBackgroundRect; 	}


private:
	const POINT& GetGravVolPoint() const
	{ return  cGravVolPoint; }


	//***********************************************************************************************************
	//***********************************************************************************************************
	virtual const POINT& GetWeightPoint() const
	{ return cWeightPoint;	}


protected:
	void OnRedraw(CDC* pDC) override;
	void DrawBitmap(CDC* pDC) override;
	void OnUpdate(CDC* pDC) override;
	BOOL IsTextUpdate(void) const override;
	BOOL IsBitmapUpdate(void) const override;
	void OnSetNewData(const base::CDoseDataCache& rNewData) override;

private:
	CFeederDisplayWbm() = delete;

public:
	CFeederDisplayWbm(const int32_t sID, const CRect& rRect);
	CFeederDisplayWbm(const CFeederDisplayWbm&) = delete;
	CFeederDisplayWbm& operator = (const CFeederDisplayWbm&) = delete;
	~CFeederDisplayWbm() override = default;
};




