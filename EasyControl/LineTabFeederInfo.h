//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module LineTabFeederInfo
///
/// @file   LineTabFeederInfo.h
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


#include <array>
#include "EasyControl.h"


enum eTotFeedingType
{
	FT_MF,
	FT_SF1,
	FT_SF2,
	FT_MAX,
};

class CLineStatusIndicator : public CBCGPImageGaugeImpl
{
public:
	enum class eStatus : uint32_t
	{
		OFF = 0,
		ON,
		MAX
	};

private:
	eStatus m_Status;
	const std::array<UINT, _S32(eStatus::MAX)>  cBitmaps;

public:
	CLineStatusIndicator(CBCGPVisualContainer* pContainer = NULL) : CBCGPImageGaugeImpl(0, pContainer)
		, m_Status{ eStatus::MAX }
		, cBitmaps{ IDB_FEEDERSTATUSOFF_PNG, IDB_FEEDERSTATUSRUN_PNG }
	{}

	void SetLineStatus(const eStatus status)
	{
		if (status != m_Status)
		{
			m_Status = status;
			SetImage(cBitmaps[_S32(m_Status)], FALSE, TRUE);
		}
	}

	eStatus GetLineStatus() const
	{
		return m_Status;
	};
};
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
class CFeederStatusIndicator : public CBCGPImageGaugeImpl
{
public:
	enum class eStatus : uint32_t
	{
		OFF = 0,
		READY,
		ON,
		MAX
	};
private:
	eStatus m_Status;
	const std::array<UINT, _S32(eStatus::MAX)>  cBitmaps;

public:
	CFeederStatusIndicator(CBCGPVisualContainer* pContainer = NULL) : CBCGPImageGaugeImpl(0, pContainer)
		, m_Status(eStatus::MAX)
		, cBitmaps{ IDB_FEEDERMOTOROFF_PNG, IDB_FEEDERMOTORREADY_PNG, IDB_FEEDERMOTORRUN_PNG }
	{}

	void SetFeederStatus(const eStatus status)
	{
		if (status != m_Status)
		{
			m_Status = status;
			SetImage(cBitmaps[_S32(m_Status)], TRUE);
		}
	}

	eStatus GetFeederStatus() const
	{
		return m_Status;
	};

};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
struct CFeederInfo
{
	CBCGPVisualContainerCtrl m_wndLineStatus;
	CBCGPVisualContainerCtrl m_wndFeederStatus;
	CLineStatusIndicator*	 m_pLineStatusIndicator;
	CFeederStatusIndicator*  m_pFeederStatusIndicator;

	CString			  m_Title;
	float32_t		  m_Totalizer;
	float32_t		  m_Massflow;
	BOOL			  m_ExtInput;
	BOOL			  m_Run;
	BOOL			  m_Redraw;


	CFeederInfo() : m_wndLineStatus()
		, m_wndFeederStatus()
		, m_pLineStatusIndicator(nullptr)
		, m_pFeederStatusIndicator(nullptr)
		, m_Title(_T(""))
		, m_Totalizer(0.0F)
		, m_Massflow(0.0F)
		, m_ExtInput(FALSE)
		, m_Run(FALSE)
		, m_Redraw(FALSE)
	{}
};
