//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module BitmapDC
///
/// @file   BitmapDC.h
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


class CBitmapDC
{
    CDC      m_DC;
    CBitmap  m_aBitmap;
    CBitmap* m_old;

private:
    void CreateImpl(CDC* pDC, HBITMAP pBitmap)
    {
        m_DC.CreateCompatibleDC(pDC);
        ASSERT(!m_aBitmap.GetSafeHandle());
        m_aBitmap.Attach(pBitmap);
        ASSERT(!m_old);
        m_old = (CBitmap*)m_DC.SelectObject(&m_aBitmap);
    }

public:
    CBitmapDC() : m_old{ nullptr }
    {}

    CBitmapDC (CDC* pDC, HBITMAP pBitmap): m_old{ nullptr }
    {   CreateImpl(pDC, pBitmap);   }

    CBitmapDC(CDC* pDC, CBitmap& aBitmap): m_old{ nullptr }
    {  CreateImpl(pDC, (HBITMAP)aBitmap.Detach());    }

    CBitmapDC(CDC* pDC, uint32_t nRessourceID): m_old{ nullptr }
    {
        CBitmap aB;
        VERIFY(aB.LoadBitmap(nRessourceID) != 0);
        CreateImpl(pDC, (HBITMAP)aB.Detach());
    }

    CBitmapDC(CDC* pDC, const CSize& size) : m_old{ nullptr }
    {
        ASSERT(size.cx * size.cy != 0);
        CBitmap aBitmap;
        aBitmap.CreateCompatibleBitmap(pDC, size.cx, size.cy);
        CreateImpl(pDC, (HBITMAP)aBitmap.Detach());
    }

    ~CBitmapDC()
    {
        Release();
    }

    void Create(CDC* pDC, HBITMAP pBitmap)
    {
        Release();
        CreateImpl(pDC, pBitmap);
    }

    void Create(CDC* pDC, CBitmap& aBitmap)
    {
        Release();
        CreateImpl(pDC, (HBITMAP)aBitmap.Detach());
    }

    void Create(CDC* pDC, uint32_t nRessourceID)
    {
        Release();
        CBitmap aB;
        VERIFY(aB.LoadBitmap(nRessourceID) != 0);
        CreateImpl(pDC, (HBITMAP)aB.Detach());
    }

    void Create(CDC* pDC, const CSize& size)
    {
        Release();
        ASSERT(size.cx * size.cy != 0);
        CBitmap aBitmap;
        aBitmap.CreateCompatibleBitmap(pDC, size.cx, size.cy);
        CreateImpl(pDC, (HBITMAP)aBitmap.Detach());
    }

    void Release()
    {
        if (m_aBitmap.GetSafeHandle())
        {
            m_aBitmap.DeleteObject();
            m_old = nullptr;
        }
        if (m_DC.GetSafeHdc())
        {
            if ((m_old != nullptr) && m_old->GetSafeHandle())
            {
                m_DC.SelectObject(m_old);
            }
            m_DC.DeleteDC();
        }
    }

    CDC& GetDC()
    {
        return m_DC;
    }

    BOOL IsBitmapDC() const
    {
        return m_DC.GetSafeHdc() != 0;
    }

    CBitmap& GetBitmap()
    {
        return m_aBitmap;
    }

    CSize GetBitmapDimensions()
    {
        CSize size{ 0, 0 };
        if (m_aBitmap.GetSafeHandle())
        {
            BITMAP aP;
            m_aBitmap.GetBitmap(&aP);
            size.cx = aP.bmWidth;
            size.cy = aP.bmHeight;
        }
        return size;
    }
};