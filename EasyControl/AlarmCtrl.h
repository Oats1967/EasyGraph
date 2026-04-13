//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmCtrl
///
/// @file   AlarmCtrl.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include <array>
#include "BASE/include/AlarmTypes.h"
#include "EasyControl.h"



class CAlarmCtrl : public CListCtrl
{
public:
    struct AlarmChange
    {
        base::eAlarmError alarmerror;
        base::eAlarmCategory category;
        BOOL value;
    };

public:
    static const int32_t OFFSETCATAGORY = 2;

protected:
    int32_t GetColumnWidth();

protected:
    const std::array<CString, base::eAlarmCategoryMax> c_CateorySC;
    CImageList          m_ImageList;
    AlarmChange         m_Change;
    CWnd*               m_pParent;
    BOOL                m_PermissionModify;

    static BOOL         m_bLargeView;

protected:
    void CreateHelpInfoBox(const CPoint& cp, const base::eAlarmError alarmerror);
    void CreateHelpInfoBox(const CPoint& cp, const base::eAlarmCategory category);
    void CreateToolTip(const CPoint& cp, const base::eAlarmError alarmerror);
    void CreateToolTip(const CPoint& cp, const CString& szMessage);

public:
    CAlarmCtrl(CWnd* pParent);
    ~CAlarmCtrl() = default;

    virtual void BuildHeadLine();
    virtual void OnColumnclickAlarmList(NMHDR* pNMHDR);

    static void SetLargeView(const BOOL bLarge)
    {  m_bLargeView = bLarge;   }

    SETGET(BOOL, PermissionModify)

};


