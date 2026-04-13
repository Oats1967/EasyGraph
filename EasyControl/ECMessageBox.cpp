//------------------------------------------------------------------------------------
///
/// @brief  Implementation of module ECMessageBox
///
/// @file   ECMessageBox.cpp
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
#include "ECMessageBox.h"
                    

// als extern 
static HHOOK hhk;

//------------------------------------------------------------------------------------
///  @brief   CBTProc
///
///            This method ....
///
///  @author  Detlef Hafer
///
///
///  @param[in] nCode of type
///  @param[in] wParamof type
///  @param[in] lParamof type
///  @return LRESULT
///
//------------------------------------------------------------------------------------
LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
   HWND hChildWnd;    // msgbox is "child"
   CString s=_T("");
   // notification that a window is about to be activated
   // window handle is wParam
   if (nCode == HCBT_ACTIVATE)
   {
      // set window handles
      hChildWnd = (HWND)wParam;
      //to get the text of the Yes button
      uint32_t result;
      if (GetDlgItem(hChildWnd,IDYES)!=NULL)
      {
         VERIFY(s.LoadString(IDS_YES));
         result= SetDlgItemText(hChildWnd,IDYES,s);
      }
      if (GetDlgItem(hChildWnd,IDNO)!=NULL)
      {
         VERIFY(s.LoadString(IDS_NO));
         result= SetDlgItemText(hChildWnd,IDNO,s);
      }
      if (GetDlgItem(hChildWnd,IDOK)!=NULL)
      {
         VERIFY(s.LoadString(IDS_OK));
         result= SetDlgItemText(hChildWnd,IDOK,s);
      }
      if (GetDlgItem(hChildWnd,IDCANCEL)!=NULL)
      {
         VERIFY(s.LoadString(IDS_CANCEL));
         result= SetDlgItemText(hChildWnd,IDCANCEL,s);
      }
      //Continue changing other button captions

      // exit CBT hook
      UnhookWindowsHookEx(hhk);
    }
    // otherwise, continue with any possible chained hooks
    else 
    {
        CallNextHookEx(hhk, nCode, wParam, lParam);
    }
   return 0;
}
//********************************************************************************************************************************
//********************************************************************************************************************************
int32_t ECMessageBox(  uint32_t nIDPrompt, uint32_t nType)
{
    hhk = SetWindowsHookEx(WH_CBT, &CBTProc, 0, GetCurrentThreadId());
    return AfxMessageBox(  nIDPrompt, nType);
}
//********************************************************************************************************************************
//********************************************************************************************************************************
int32_t ECMessageBox( LPCTSTR lpszText, uint32_t nType)
{
   hhk = SetWindowsHookEx(WH_CBT, &CBTProc, 0, GetCurrentThreadId());
   return AfxMessageBox(lpszText, nType);
}


