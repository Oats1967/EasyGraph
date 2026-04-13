//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module EasyControlLayout
///
/// @file   EasyControlLayout.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "EasyControlDoc.h"
#include "EasyControlView.h"
#include "DataImage.h"
#include "MFCMacros.h"


static const LOADLTYP<1> aLoadTyp1 =
{
#include "Dat\Layout1Data.dat"
};
static const LOADLTYP<2> aLoadTyp2 =
{
#include "Dat\Layout2Data.dat"
} ;
static const LOADLTYP<3> aLoadTyp3 =
{
#include "Dat\Layout3Data.dat"
} ;
static const LOADLTYP<4> aLoadTyp4 =
{
#include "Dat\Layout4Data.dat"
} ;

static const LOADLTYP<5> aLoadTyp5 =
{
#include "Dat\Layout5Data.dat"
};

static const LOADLTYP<6> aLoadTyp6 =
{
#include "Dat\Layout6Data.dat"
} ;

static const LOADLTYP<7> aLoadTyp7 =
{
#include "Dat\Layout7Data.dat"
} ;
static const LOADLTYP<8> aLoadTyp8 =
{
#include "Dat\Layout8Data.dat"
} ;
static const LOADLTYP<9> aLoadTyp9 =
{
#include "Dat\Layout9Data.dat"
} ;
static const LOADLTYP<10> aLoadTyp10 =
{
#include "Dat\Layout10Data.dat"
} ;
static const LOADLTYP<11> aLoadTyp11 =
{
#include "Dat\Layout11Data.dat"
} ;
static const LOADLTYP<12> aLoadTyp12 =
{
#include "Dat\Layout12Data.dat"
} ;
static const LOADLTYP<13> aLoadTyp13 =
{
#include "Dat\Layout13Data.dat"
} ;
static const LOADLTYP<14> aLoadTyp14 =
{
#include "Dat\Layout14Data.dat"
} ;
static const LOADLTYP<15> aLoadTyp15 =
{
#include "Dat\Layout15Data.dat"
} ;
static const LOADLTYP<16> aLoadTyp16 =
{
#include "Dat\Layout16Data.dat"
} ;
static const LOADLTYP<17> aLoadTyp17 =
{
#include "Dat\Layout17Data.dat"
} ;
static const LOADLTYP<18> aLoadTyp18 =
{
#include "Dat\Layout18Data.dat"
} ;
static const LOADLTYP<19> aLoadTyp19 =
{
#include "Dat\Layout19Data.dat"
} ;
static const LOADLTYP<20> aLoadTyp20 =
{
#include "Dat\Layout20Data.dat"
} ;
static const LOADLTYP<21> aLoadTyp21 =
{
#include "Dat\Layout21Data.dat"
} ;
static const LOADLTYP<22> aLoadTyp22 =
{
#include "Dat\Layout22Data.dat"
} ;
static const LOADLTYP<23> aLoadTyp23 =
{
#include "Dat\Layout23Data.dat"
} ;
static const LOADLTYP<24> aLoadTyp24 =
{
#include "Dat\Layout24Data.dat"
} ;
static const LOADLTYP<25> aLoadTyp25 =
{
#include "Dat\Layout25Data.dat"
};
static const LOADLTYP<26> aLoadTyp26 =
{
#include "Dat\Layout26Data.dat"
};
static const LOADLTYP<27> aLoadTyp27 =
{
#include "Dat\Layout27Data.dat"
};
static const LOADLTYP<28> aLoadTyp28 =
{
#include "Dat\Layout28Data.dat"
};
static const LOADLTYP<29> aLoadTyp29 =
{
#include "Dat\Layout29Data.dat"
};
static const LOADLTYP<30> aLoadTyp30 =
{
#include "Dat\Layout30Data.dat"
};
static const LOADLTYP<31> aLoadTyp31 =
{
#include "Dat\Layout31Data.dat"
};
static const LOADLTYP<32> aLoadTyp32 =
{
#include "Dat\Layout32Data.dat"
};


static const LOADLTYPBASE* c_PointArray[GLOBALDOSEMAXCOUNT] =
{
	(LOADLTYPBASE*)&aLoadTyp1,
	(LOADLTYPBASE*)&aLoadTyp2,
	(LOADLTYPBASE*)&aLoadTyp3,
	(LOADLTYPBASE*)&aLoadTyp4,
	(LOADLTYPBASE*)&aLoadTyp5,
	(LOADLTYPBASE*)&aLoadTyp6,
	(LOADLTYPBASE*)&aLoadTyp7,
	(LOADLTYPBASE*)&aLoadTyp8,
	(LOADLTYPBASE*)&aLoadTyp9,
	(LOADLTYPBASE*)&aLoadTyp10,
	(LOADLTYPBASE*)&aLoadTyp11,
	(LOADLTYPBASE*)&aLoadTyp12,
	(LOADLTYPBASE*)&aLoadTyp13,
	(LOADLTYPBASE*)&aLoadTyp14,
	(LOADLTYPBASE*)&aLoadTyp15,
	(LOADLTYPBASE*)&aLoadTyp16,
	(LOADLTYPBASE*)&aLoadTyp17,
	(LOADLTYPBASE*)&aLoadTyp18,
	(LOADLTYPBASE*)&aLoadTyp19,
	(LOADLTYPBASE*)&aLoadTyp20,
	(LOADLTYPBASE*)&aLoadTyp21,
	(LOADLTYPBASE*)&aLoadTyp22,
	(LOADLTYPBASE*)&aLoadTyp23,
	(LOADLTYPBASE*)&aLoadTyp24,
	(LOADLTYPBASE*)&aLoadTyp25,
	(LOADLTYPBASE*)&aLoadTyp26,
	(LOADLTYPBASE*)&aLoadTyp27,
	(LOADLTYPBASE*)&aLoadTyp28,
	(LOADLTYPBASE*)&aLoadTyp29,
	(LOADLTYPBASE*)&aLoadTyp30,
	(LOADLTYPBASE*)&aLoadTyp31,
	(LOADLTYPBASE*)&aLoadTyp32
};

//***********************************************************************************************************************
//***********************************************************************************************************************
void CEasyControlView::DestroyLayout ()
{
	for (int32_t i = 0; i < GLOBALDOSEMAXCOUNT; i++)
	{
		auto& aV	= dataimage::getDoseDataImage(i);
		aV.m_Container.Reset();
	}
}
//***********************************************************************************************************************
//***********************************************************************************************************************
void CEasyControlView:: InitLine ()
{
	auto& aV = dataimage::getLineDataImage();
	aV.m_Cache.m_lID = 1;
}
//***********************************************************************************************************************
//***********************************************************************************************************************
void CEasyControlView:: InitDose (int32_t i, const DOSEPOSITIONSTYP& aD)
{
	auto& aV = dataimage::getDoseDataImage(i);
	auto& rContainer = aV.m_Container;

	rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eFeederDisplay, aD._aRD);
	rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eFeederDisplay, aD._aRD);
	rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseBaseActualGrafik, aD._aAS);
	rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseBaseActualGrafik, aD._aAS);
	rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseBaseNominalGrafik, aD._aNS);
	rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseBaseNominalGrafik, aD._aNS);
	rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseButtonGrafik, aD._aBS);
	rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseButtonGrafik, aD._aBS);

	if (m_bDrawRefillButtonsEnable)
	{
		CRect rRefillRect(aD._aBS.left, aD._aRD.top - 50U, aD._aBS.right, aD._aRD.top - 10);
		rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik, rRefillRect);
		rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik, rRefillRect);

		rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik, rRefillRect);
		rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik, rRefillRect);
	}

	if (m_bProductNameEnable)
	{
		CRect rProductNameRect(aD._aBS.left, aD._aRD.top - 50U, aD._aBS.right, aD._aRD.top - 10);
		rProductNameRect.left -= 20;
		rProductNameRect.right += 20;
		rContainer.SetOrgRect(CDisplayContainer::eDisplayItem::eDoseProductNameGrafik, rProductNameRect);
		rContainer.SetDisplayRect(CDisplayContainer::eDisplayItem::eDoseProductNameGrafik, rProductNameRect);
	}

	aV.m_lID	= i+1;
}
//************************************************************************************************************************
//************************************************************************************************************************
void CEasyControlView::DrawLogo(CDC* pdc)
{
	CBitmapDC aLogo(pdc, IDB_LOGO);
	auto size = aLogo.GetBitmapDimensions();
	ASSERT(size.cx * size.cy > 0);
	pdc->BitBlt(m_LogoPos.x, m_LogoPos.y, size.cx, size.cy, &aLogo.GetDC(), 0, 0, SRCCOPY);
}
//************************************************************************************************************************
//************************************************************************************************************************
void CEasyControlView::DrawCustomerLogo(CDC* pdc)
{
	if ( m_CustomerLogoCfg.m_bInit )
	{
		ASSERT(! m_CustomerLogoCfg.m_CustomerLogoPNG.empty());
		ASSERT( m_CustomerLogoCfg.m_Location.x * m_CustomerLogoCfg.m_Location.y > 0);
		CImage image;
		auto result = image.Load(toCString(m_CustomerLogoCfg.m_CustomerLogoPNG)); // just change extension to load jpg
		if (result == S_OK)
		{
			CBitmapDC aCustomerLogo(pdc, image.Detach());
			auto size = aCustomerLogo.GetBitmapDimensions();
			ASSERT(size.cx * size.cy > 0);
			pdc->TransparentBlt(m_CustomerLogoCfg.m_Location.x, m_CustomerLogoCfg.m_Location.y, size.cx, size.cy, &aCustomerLogo.GetDC(), 0, 0, size.cx, size.cy, COLORBKWHITE);
		}
		else
		{
			LOGERROR("Customer Logo-Bitmap not found !" << m_CustomerLogoCfg.m_CustomerLogoPNG);
		}
	}
}
//************************************************************************************************************************
//************************************************************************************************************************
void CEasyControlView::DrawBackground(CBitmapDC& rDC)
{
	if (rDC.IsBitmapDC())
	{
		auto& dc = rDC.GetDC();
		auto size = rDC.GetBitmapDimensions();
		ASSERT(size.cx > 0 && size.cy > 0);
		dc.FillSolidRect(CRect(CPoint(0, 0), size), COLORBKWHITE);
		DrawLogo(&dc);
		DrawCustomerLogo(&dc);
	}
}
//************************************************************************************************************************
//************************************************************************************************************************
void CEasyControlView::CreateBitmapFromScratch(const LOADLTYPBASE& rLoad)
{
	CClientDC dc(this);

	if (m_LinieDC.IsBitmapDC())
	{
		m_LinieDC.Release();
	}
	m_LinieDC.Create(&dc, rLoad.aBitmapSize);
	DrawBackground(m_LinieDC);
}
//***********************************************************************************************************************
//***********************************************************************************************************************
void CEasyControlView::LoadLayout (const LOADLTYPBASE& rLoad)
{
	DestroyLayout ();

	const DOSEPOSITIONSTYP* pDose = &rLoad.aDose[0];
	for (int32_t i = 0; i < _S32(rLoad.uiAnzahl); i++)
	{
		InitDose (i, *pDose);
		pDose++;
	}
	InitLine ();
	CreateBitmapFromScratch(rLoad);
}
//***********************************************************************************************************************
//***********************************************************************************************************************
void CEasyControlView::LoadLayout (const int32_t n)
{
	ASSERT(n >= 0 && n <= GLOBALDOSEMAXCOUNT);
	LoadLayout(*c_PointArray[__max(n-1,0)]);
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CEasyControlView::CheckFeederDisplay(void)
{
	BOOL bNew = FALSE;

	for (int32_t i = 0; i < m_sMaxItems; i++)
	{
		auto& aV = dataimage::getDoseDataImage(i);
		base::eDoseType lDoseType = aV.m_Cache.m_DoseType;
		auto& rContainer = aV.m_Container;
		BOOL bCreate = rContainer.GetDoseType() != lDoseType;
		if (bCreate)
		{
			rContainer.SetDoseType(lDoseType);

			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eFeederDisplay);
				rItem.SetGrafikContext(CFeederDisplay::Create(i, lDoseType, rItem.GetOrgRect()));
			}
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseBaseActualGrafik);
				rItem.SetGrafikContext(CDoseBaseActualGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseBaseNominalGrafik);
				rItem.SetGrafikContext(CDoseBaseNominalGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseButtonGrafik);
				rItem.SetGrafikContext(CDoseButtonGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}

			if ( m_bDrawRefillButtonsEnable)
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik);
				rItem.SetGrafikContext(CDoseRefillReleaseButtonGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}
#if _DEBUG
			else
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseRefillReleaseButtonGrafik);
				ASSERT(rItem.GetGrafikContext() == nullptr);
			}
#endif

			if (m_bDrawRefillButtonsEnable)
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik);
				rItem.SetGrafikContext(CDoseRefillButtonGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}
#if _DEBUG
			else
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseRefillButtonGrafik);
				ASSERT(rItem.GetGrafikContext() == nullptr);
			}
#endif

			if (m_bProductNameEnable)
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseProductNameGrafik);
				rItem.SetGrafikContext(CDoseProductNameGrafik::Create(lDoseType, rItem.GetOrgRect()));
			}
#if _DEBUG
			else
			{
				auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem::eDoseProductNameGrafik);
				ASSERT(rItem.GetGrafikContext() == nullptr);
			}
#endif
			bNew = TRUE;
		}
	}
#if _DEBUG
	{
		for (int32_t i = m_sMaxItems; i < GLOBALDOSEMAXCOUNT; i++)
		{
			const auto& aV = dataimage::getDoseDataImage(i);
			const auto& rContainer = aV.m_Container;
			for (int32_t k = 0; k < _S32(CDisplayContainer::eDisplayItem::eDoseDisplayMax); k++)
			{
				const auto& rItem = rContainer.GetDisplayItem(CDisplayContainer::eDisplayItem(k));
				ASSERT(rItem.GetGrafikContext() == nullptr);
			}
		}
	}
#endif
	if ( bNew)
	{
		RecalcLayout();
	}
	return bNew;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CEasyControlView::RecalcLayout(void)
{
	auto ResetDisplay = [](const int32_t index)
	{
		auto& aC = dataimage::getDoseDataImage(index).m_Container;
		aC.ResetDisplay();
	};

	auto CalcDisplay = [zoomfactor = m_zoomfactor](const int32_t index)
	{
		auto& aC = dataimage::getDoseDataImage(index).m_Container;
		CRect boundingrect{ aC.GetBoundingRect() };
		aC.CalcDisplay(boundingrect.CenterPoint(), zoomfactor);
	};

	// GetBoundingRect
	if (m_zoomfactor <= 100)
	{
		ASSERT(m_zoomfactor == 100);
		m_zoomfactor = 100;
		for (int32_t i = 0; i < m_sMaxItems; i++)
		{
			ResetDisplay(i);
		}
	}
	else
	{
		for (int32_t i = 0; i < m_sMaxItems; i++)
		{
			CalcDisplay(i);
		}
		RepositionLayout();
	}
	DrawBackground(m_LinieDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CEasyControlView::RepositionLayout(void)
{
	auto GetSaveDisplayRect = [](const int32_t index)
	{
		auto& aV = dataimage::getDoseDataImage(index);
		return aV.m_Container.GetSaveDisplayBoundingRect();
	};
	auto GetOrgRect = [](const int32_t index)
	{
		auto& aV = dataimage::getDoseDataImage(index);
		return aV.m_Container.GetBoundingRect();
	};
	auto OffsetDisplayBoundingRect = [](const int32_t index, const int32_t x, const int32_t y)
	{
		auto& aV = dataimage::getDoseDataImage(index);
		aV.m_Container.OffsetDisplayBoundingRect(x, y);
	};

	// GetBoundingRect
	CRect orgboundingrect = GetOrgRect(0);

	CRect displayboundingrect = GetSaveDisplayRect(0);
	// Reposition x
	int32_t cx = displayboundingrect.Width();
	int32_t tx = cx * m_sMaxItems;
	auto size = m_LinieDC.GetBitmapDimensions();
	int32_t y = __max(orgboundingrect.top, 40);
	int32_t x = __max(_S32(size.cx - tx)/2, 0);
	for (int32_t i = 0; i < m_sMaxItems; i++)
	{
		displayboundingrect = GetSaveDisplayRect(i);
		OffsetDisplayBoundingRect(i, x - displayboundingrect.left, y - displayboundingrect.top);
		x += cx;
	}
}

