//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module FeederDisplayIfs
///
/// @file   FeederDisplayIfs.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl_DEU/resource.h"
#include "FeederDisplayIfs.h"




//***********************************************************************************************************
//***********************************************************************************************************
CFeederDisplayIfs::CFeederDisplayIfs(int32_t sID, const CRect& rRect) : CBaseClass(sID, rRect)
, cIfsSwitchHighPoint{ 83, 12 }
, cIfsSwitchMaxPoint{ 83, 22 }
, cIfsSwitchMinPoint{ 83, 42 }
, cIfsSwitchLowPoint{ 83, 52 }
, cCalibratePoint{ 70, 0 }
, cTarePoint{ 0, 0 }				// Not used
, cEmptyFeederPoint{ 0, 0 }			// Not used
, cReleasePoint{ 41, 86 }
, cRefillPoint{ 46, 0 }
, cTitlePoint{ 30, 36 }
, m_Hopper{}
, m_brushBlueSolid(COLORBLUE)
, m_brushRedSolid(COLORRED)
, m_brushWhiteDashed(HS_DIAGCROSS, COLORWHITE)
, m_PenBlue(PS_NULL, 0, COLORBLUE)
, m_Font{}
{
    m_Font.CreateFont(22, 0, 0, 0, FW_BOLD,
        FALSE, FALSE, 0, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfs::DrawTitle(CDC* pDC)
{
    auto iOldCR = pDC->SetTextColor(RGBCOLORWHITE);
    auto iOldBK = pDC->SetBkMode(TRANSPARENT);
    auto iOldTA = pDC->SetTextAlign(TA_LEFT | TA_TOP);
    auto oldfont = pDC->SelectObject(&m_Font);
    pDC->TextOut(cTitlePoint.x, cTitlePoint.y, L"IFU");
    pDC->SelectObject(oldfont);
    pDC->SetTextAlign(iOldTA);
    pDC->SetBkMode(iOldBK);
    pDC->SetTextColor(iOldCR);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfs::OnRedraw(CDC* pDC)
{
    CBaseClass::OnRedraw(pDC);
    DrawMinMaxSensor(pDC);
    DrawFilling(pDC);
    DrawTitle(pDC);
}
//***********************************************************************************************************
//***********************************************************************************************************
inline void CFeederDisplayIfs::DrawFillingValue(CDC* pDC, CBrush* pBrush, CPen* pPen, const float32_t cFillGrade)
{
    m_Hopper.DrawFilling(pDC, pBrush, pPen, cFillGrade);
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfs::DrawFilling(CDC* pDC)
{
    auto bEnable = GetIfsEmptyFeeder();
    if (bEnable)
    {
        DrawFillingValue(pDC, &m_brushRedSolid, &m_PenBlue, 0.15F);
    }
    if ( ! bEnable)
    {
        bEnable = GetIfsOverflowFeeder();
        if (bEnable)
        {
            DrawFillingValue(pDC, &m_brushRedSolid, &m_PenBlue, 1.0F);
        }
    }
    if ( ! bEnable)
    {
        bEnable = GetIfsSwitchHighEnable();
        if (bEnable)
        {
            bEnable &= GetIfsSwitchHigh();
            if (bEnable)
            {
                DrawFillingValue(pDC, &m_brushBlueSolid, &m_PenBlue, 1.0F);
            }
        }
    }
    if ( ! bEnable)
    {
        bEnable = GetIfsSwitchMaxEnable();
        if ( bEnable )
        {
            bEnable &= GetIfsSwitchMax();
            if ( bEnable )
            {
                DrawFillingValue(pDC, &m_brushBlueSolid, &m_PenBlue, 0.9F);
            }
        }
    }
    if ( ! bEnable)
    {
        bEnable = GetIfsSwitchMinEnable();
        if (bEnable)
        {
            bEnable &= GetIfsSwitchMin();
            if (bEnable)
            {
                DrawFillingValue(pDC, &m_brushBlueSolid, &m_PenBlue, 0.4F);
            }
        }
    }
    if ( ! bEnable)
    {
        bEnable = GetIfsSwitchLowEnable();
        if (bEnable)
        {
            bEnable &= GetIfsSwitchLow();
            if (bEnable)
            {
                DrawFillingValue(pDC, &m_brushBlueSolid, &m_PenBlue, 0.2F);
            }
        }
    }
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfs::DrawMinMaxSensor(CDC* pDC)
{
    {
        BOOL bEnable = GetIfsSwitchHighEnable();
        if (bEnable)
        {
            UINT nRessourceID = (GetIfsSwitchHigh()) ? IDB_BTGREEN : IDB_BTNONE;
            ShowBitmap(pDC, nRessourceID, GetIfsSwitchHighPoint());
        }
    }
    {
        BOOL bEnable = GetIfsSwitchMaxEnable();
        if (bEnable)
        {
            UINT nRessourceID = (GetIfsSwitchMax()) ? IDB_BTGREEN : IDB_BTNONE;
            ShowBitmap(pDC, nRessourceID, GetIfsSwitchMaxPoint());
        }
    }
    {
        BOOL bEnable = GetIfsSwitchMinEnable();
        if (bEnable)
        {
            UINT nRessourceID = 0;
            if (GetIfsEmptyFeeder())
            {
                nRessourceID = IDB_BTRED;
            }
            else if (GetIfsSwitchMin())
            {
                nRessourceID = IDB_BTGREEN;
            }
            else
            {
                nRessourceID = IDB_BTNONE;
            }
            ShowBitmap(pDC, nRessourceID, GetIfsSwitchMinPoint());
        }
    }
}
//***********************************************************************************************************
//***********************************************************************************************************
BOOL CFeederDisplayIfs::IsBitmapUpdate() const
{
    auto bUpdate = CBaseClass::IsBitmapUpdate();
    if (!bUpdate)
    {
        bUpdate = BOOL(m_StatusFlags.allflags != 0);
    }
    return bUpdate;
}
//***********************************************************************************************************
//***********************************************************************************************************
void CFeederDisplayIfs::OnSetNewData(const base::CDoseDataCache& rNewData)
{
    CBaseClass::OnSetNewData(rNewData);

    m_StatusFlags.flags.bIfsSwitchHighChanged       = IsIfsSwitchHighChanged();
    m_StatusFlags.flags.bIfsSwitchLowChanged        = IsIfsSwitchLowChanged();
    m_StatusFlags.flags.bIfsSwitchMaxChanged        = IsIfsSwitchMaxChanged();
    m_StatusFlags.flags.bIfsSwitchMinChanged        = IsIfsSwitchMinChanged();
    m_StatusFlags.flags.bIfsSwitchHighEnableChanged = IsIfsSwitchHighEnableChanged();
    m_StatusFlags.flags.bIfsSwitchLowEnableChanged  = IsIfsSwitchLowEnableChanged();
    m_StatusFlags.flags.bIfsSwitchMaxEnableChanged  = IsIfsSwitchMaxEnableChanged();
    m_StatusFlags.flags.bIfsSwitchMinEnableChanged  = IsIfsSwitchMinEnableChanged();
    m_StatusFlags.flags.bIfsSwitchEmptyFeeder       = IsIfsEmptyFeederChanged();
    m_StatusFlags.flags.bIfsSwitchOverflowFeeder    = IsIfsOverflowFeederChanged();
}




