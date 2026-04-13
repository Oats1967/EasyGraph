//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module Recorder
///
/// @file   Recorder.h
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

#include "EasyControl.h"
#include "RemoteControl.h"
#include "RecorderStatus.h"
#include "BASE/include/RecorderConfig.h"

class CRecorder
{
    base::CRecorderConfig m_Config;
    uint64_t              m_dRecTime;
    uint32_t              m_sMaxItems;
    BOOL	              m_bRunning;
    eRecorderStatus       m_RecorderState;
    std::string           m_TempFolder;

private:
    void SetMaxItems(const uint32_t count)
    {
        m_sMaxItems = count;
    }

    void CreateRecItem(base::utils::CRecItem& aRecItem) const;
    void CreateRecItemZero(base::utils::CRecItem& aRecItem);
    void AppendRecItem(const base::utils::CRecItem& aRecItem);
    void AddRecItem(void);

    void StartRecItem(void);

public:
    CRecorder() : m_dRecTime(0), m_sMaxItems(0), m_bRunning(FALSE), m_RecorderState { eRecorderStatus::eRecorderDeactive }
    {}
    virtual ~CRecorder() = default;

    void SetConfig(const base::CRecorderConfig& config)
    {  m_Config = config;    }

    BOOL WriteRecorder(void);

    GET(const eRecorderStatus, RecorderState)
    SETGET(const std::string, TempFolder)
};
