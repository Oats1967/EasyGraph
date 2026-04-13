#pragma once


#include "Colors.def"


namespace mfcmacros
{
    void EnableID(CWnd* parent, int32_t id, const BOOL bEnable);
    void ShowID(CWnd* parent, int32_t id, const BOOL bShow);
    void CheckID(CWnd* parent, int32_t id, const BOOL bCheck);
    void EnableShowID(CWnd* parent, int32_t id, const BOOL bEnable, const BOOL bShow);
    void EnableShowCheckID(CWnd* parent, int32_t id, const BOOL bEnable, const BOOL bShow, const BOOL bChecked);
    void SunkenID(CWnd* parent, int32_t id, const BOOL bEnable);
    void SetPosID(CWnd* parent, int32_t id, const int32_t x, const int32_t y);
    void SetPosSizeID(CWnd* parent, int32_t id, const int32_t x, const int32_t y, const int32_t cx, const int32_t cy);
    void CenterPosID(CWnd* parent, int32_t id, const int32_t x, const int32_t y);
    void CenterPosYID(CWnd* parent, int32_t id, const int32_t x, const int32_t y);
    void CenterPosNeighbourYID(CWnd* parent, int32_t id1, int32_t id2, const int32_t x, const int32_t y);
    void CenterTextID(CWnd* parent, int32_t id);
    void SetFontID(CWnd* parent, int32_t id, CFont* pFont);
    void SetBitmapID(CWnd* parent, int32_t id, CBitmap* pFont);

    void SendMessage(CWnd* parent, UINT m, WPARAM w = 0, LPARAM l = 0);
};



#define SETCHECK(m_ch, b)       { if (m_ch.GetCheck()!=b) m_ch.SetCheck(b); }
#define ENABLE_ID(id,b)         {	mfcmacros::EnableID(this, id, b);  }
#define SHOWW_ID(id,b)          {   mfcmacros::ShowID(this, id, b);    }
#define CHECK_ID(id, b)         {   mfcmacros::CheckID(this, id, b);    }
#define ENABLE_SHOW_ID(id,b,c)  {   mfcmacros::EnableShowID(this, id, b, c);    }
#define ENABLE_SHOW_CHECK_ID(id, a, b, c) {   mfcmacros::EnableShowCheckID(this, id, a, b, c);    }
#define HIDE_ID(id)	        SHOWW_ID(id, FALSE)
#define SHOW_ID(id)	        SHOWW_ID(id, TRUE)
#define SUNKEN_ID(id,b)       {   mfcmacros::SunkenID(this,id, b);    }
#define SETPOS_ID(id, x, y)   {   mfcmacros::SetPosID(this,id, x,y);  }
#define SETPOSSIZE_ID(id, x, y,cx,cy)   {   mfcmacros::SetPosSizeID(this,id, x,y, cx, cy);  }
#define CENTERPOS_ID(id, x, y)   {   mfcmacros::CenterPosID(this,id, x,y);  }
#define CENTERPOSY_ID(id, x, y)   {   mfcmacros::CenterPosYID(this,id, x,y);  }
#define CENTERNPOSY_ID(id1, id2, x, y)   {   mfcmacros::CenterPosNeighbourYID(this,id1, id2 ,x,y);  }
#define CENTERTEXT_ID(id)  {   mfcmacros::CenterTextID(this,id);  }
#define SETFONT_ID(id, f)   {   mfcmacros::SetFontID(this,id, f);  }
#define SETBITMAP_ID(id, f)   {   mfcmacros::SetBitmapID(this,id, f);  }

#define INITINFOBUTTON(_a)      { (_a).SizeToContent();(_a).RedrawWindow(); }

#define BINDFUNC(_a, _b, _func) 	{ _a, std::bind(&_b::_func, this) }
#define DELETE_OBJECT(_a) { if ( _a) { delete (_a); (_a) = nullptr; } }


#define SETON(m_ch, b) { m_ch.SetOn(b); }


#define SZMARKED _T("X")
#define SZEMPTY  _T("")





