//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module AlertCtrl
///
/// @file   AlertCtrl.h
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

#include <string>
#include "BASE/Utils/public/LogItemList.h"
#include "AlarmCtrl.h"


class CAlertCtrl : public CAlarmCtrl
{
private:
    const base::utils::CLogItemList& c_rLoglist;

private:
    void CreateImageList();
    CString CreateAlarmString(const std::string& szMessage, const time_t& t0);


public:
    CAlertCtrl(CWnd* pParent, const base::utils::CLogItemList& rLoglist);
    ~CAlertCtrl() = default;

    void FillAlertCtrl(const int32_t sItem);
    BOOL OnLvnItemActivateAlarmList(NMHDR* pNMHDR);
};


