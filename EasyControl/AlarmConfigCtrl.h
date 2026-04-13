//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module AlarmConfigCtrl
///
/// @file   AlarmConfigCtrl.cpp
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
#include "AlarmCtrl.h"
#include "EasyControl.h"



class CAlarmConfigCtrl : public CAlarmCtrl
{
private:
    base::CAlarmField&  m_rAlarmField;

private:
    BOOL SetAlarm(const base::eAlarmError alarmerror, base::eAlarmCategory category, const BOOL value);
    void Set(const int32_t item, const base::eAlarmError alarmerror, const base::eAlarmCategory category, const BOOL value);

private:
    void CreateImageList();


public:
    CAlarmConfigCtrl(CWnd* pParent, base::CAlarmField& rAlarmField);
    ~CAlarmConfigCtrl() = default;

    void FillAlarmCtrl(const base::eAlarmErrorBits& rMask);
    BOOL OnLvnItemActivateAlarmList(NMHDR* pNMHDR);
    void OnCustomdraw(NMHDR* pNMHDR , LRESULT* pResult);

};


