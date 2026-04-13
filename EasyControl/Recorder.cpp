//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module Recorder
///
/// @file   Recorder.cpp
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
#include "BASE/Base.h"
#include "BASE/EasyTime/public/EasyTime.h"
#include "EasyControl.h"
#include "RemoteControl.h"
#include "DataImage.h"
#include "Recorder.h"

//***********************************************************************************************
//***********************************************************************************************
void CRecorder::CreateRecItem(base::utils::CRecItem& aRecItem) const
{
    for (uint32_t i = 0; i < m_sMaxItems; i++)
    {
        const auto& aV = (dataimage::getDoseDataImage(i));
        const auto& cache = aV.m_Cache;
        aRecItem.SetQMNumber(i, cache.m_QMNumber);
        aRecItem.Set(base::eMassflowSelect::eVIEWSETPOINT, i, cache.m_fSetpoint);
        aRecItem.Set(base::eMassflowSelect::eVIEWMASSFLOW, i, cache.m_fMassflow);
        aRecItem.Set(base::eMassflowSelect::eVIEWTOTALIZER, i, cache.m_fTotalizer);
        aRecItem.Set(base::eMassflowSelect::eVIEWDRIVECOMMAND, i, cache.m_fActualDriveCommand);
        aRecItem.Set(base::eMassflowSelect::eVIEWWEIGHT, i, cache.m_fActWeight);
        aRecItem.Set(base::eMassflowSelect::eVIEWDOSEPERFORMANCE, i, cache.m_fQMaxFaktor);
        aRecItem.Set(base::eMassflowSelect::eVIEWROTSPEED, i, cache.m_fIstRotationalSpeed);
    }
    aRecItem.SetMaxItems(m_sMaxItems);
    aRecItem.SetANNumber(toStdString((dataimage::getLineDataImage()).m_szANNumber));
}
//***********************************************************************************************
//***********************************************************************************************
void CRecorder::CreateRecItemZero(base::utils::CRecItem& aRecItem)
{
    for (uint32_t i = 0; i < m_sMaxItems; i++)
    {
        const auto& cache = dataimage::getDoseDataImage(i).m_Cache;
        aRecItem.SetQMNumber(i, cache.m_QMNumber);
        aRecItem.Set(base::eMassflowSelect::eVIEWSETPOINT, i, cache.m_fSetpoint);
        aRecItem.Set(base::eMassflowSelect::eVIEWMASSFLOW, i, 0.0F);
        aRecItem.Set(base::eMassflowSelect::eVIEWTOTALIZER, i, cache.m_fTotalizer);
        aRecItem.Set(base::eMassflowSelect::eVIEWDRIVECOMMAND, i, cache.m_fActualDriveCommand);
        aRecItem.Set(base::eMassflowSelect::eVIEWWEIGHT, i, cache.m_fActWeight);
        aRecItem.Set(base::eMassflowSelect::eVIEWDOSEPERFORMANCE, i, cache.m_fQMaxFaktor);
        aRecItem.Set(base::eMassflowSelect::eVIEWROTSPEED, i, 0.0F);
    }
    aRecItem.SetMaxItems(m_sMaxItems);
    aRecItem.SetANNumber(toStdString((dataimage::getLineDataImage()).m_szANNumber));
}
//***********************************************************************************************
//***********************************************************************************************
void CRecorder::AppendRecItem(const base::utils::CRecItem& aRecItem)
{
    uint32_t count = 0;
    REMOTEREF.getMaxDoseCount(count);
    SetMaxItems(count);
    auto oldfilename = RECORDERREF.GetFilename();
    RECORDERREF.CreateFilename();
    auto newfilename = RECORDERREF.GetFilename();
    if (oldfilename != newfilename)
    {
        RECORDERREF.Clear();
    }
    RECORDERREF.SetPath(m_Config.m_RecorderPath);
    RECORDERREF.AddItem(aRecItem);
    RECORDERREF.SetMaxItems(count);
    auto result = RECORDERREF.SaveLastItem();
    if (!m_bRunning)
    {
        m_RecorderState = eRecorderStatus::eRecorderDeactive;
    }
    else
    {
        if ( ! result)
        {
            assert(!m_TempFolder.empty());
            if ( ! m_TempFolder.empty())
            {
                RECORDERREF.SetPath(m_TempFolder);
                RECORDERREF.CreateFilename();
                RECORDERREF.SaveLastItem();
                RECORDERREF.SetPath(m_Config.m_RecorderPath);
            }
            m_RecorderState = eRecorderStatus::eRecorderError;
        }
        else
        {
            m_RecorderState = eRecorderStatus::eRecorderActive;
        }
    }
}
//***********************************************************************************************
//***********************************************************************************************
void CRecorder::AddRecItem(void)
{
    uint32_t count = 0;
    REMOTEREF.getMaxDoseCount(count);
    SetMaxItems(count);
    base::utils::CRecItem     aRecItem;
    CreateRecItem(aRecItem);
    AppendRecItem(aRecItem);
}
//***********************************************************************************************
//***********************************************************************************************
void CRecorder::StartRecItem(void)
{
    uint32_t count = 0;
    REMOTEREF.getMaxDoseCount(count);
    SetMaxItems(count);
    base::utils::CRecItem aRecItem;
    CreateRecItemZero(aRecItem);
    AppendRecItem(aRecItem);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CRecorder::WriteRecorder(void)
{
    BOOL bAddItem = FALSE;
    BOOL bNewItem = FALSE;

    // Pruefen, ob irgendein Dosierer aktiviert wurde.
    auto dTime = base::time::GetTimeU64() / 1000;
    base::LineProcessStatus status;
    REMOTEREF.getLineProcessStatus(status);
    const BOOL bFreigabe = status.flags.anydosestarted;
    if ( ! bFreigabe)
    {
        if (m_bRunning)
        {
            m_bRunning = FALSE;
            bAddItem = TRUE;
        }
    }
    else
    {
        if (m_Config.m_RecorderActive)
        {
            if (m_bRunning)
            {
                if (dTime >= m_dRecTime)
                {
                    bAddItem = TRUE;
                }
            }
            else
            {
                bAddItem = TRUE;
                bNewItem = TRUE;
            }
        }
        else
        {
            if (m_bRunning)
            {
                bAddItem = TRUE;
            }
        }
        m_bRunning = m_Config.m_RecorderActive;
    }
    if (bAddItem)
    {
        if (bNewItem)
        {
            StartRecItem();
            m_dRecTime = dTime + 1;
        }
        else
        {
            AddRecItem();
            m_dRecTime = dTime + m_Config.m_RecorderSampeTime;
        }
    }
    return bAddItem;
}
