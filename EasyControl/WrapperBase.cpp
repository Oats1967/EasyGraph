//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WrapperBase
///
/// @file   WrapperBase.cpp
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
#include "WrapperBase.h"


static base::CEasyControlConfig					m_Config;
static base::utils::CUserRightsList				m_LoginItemList;
static base::utils::CMassflowMemory				m_MassflowMemory;
static base::utils::CRecItemList				m_Recorder;
static base::utils::CLogItemList				m_LogItemList;
static base::utils::CProductItemList            m_ProductDatabase;
static base::CSystemConfig				        m_SystemConfig;
static base::CLayoutConfig				        m_LayoutConfig;
static base::CRecorderConfig				    m_RecorderConfig;
static base::CRecorderDescription				m_RecorderDescription;
static base::CDiagramConfig				        m_DiagramConfig;



base::utils::CUserRightsList* g_UserRightsList    = &m_LoginItemList;;
base::CEasyControlConfig* g_EasyControlConfig     = &m_Config;
base::utils::CMassflowMemory* g_MassflowMemory    = &m_MassflowMemory;
base::utils::CRecItemList* g_RecItemList          = &m_Recorder;
base::utils::CLogItemList* g_LogItemList          = &m_LogItemList;
base::CSystemConfig* g_SystemConfig               = &m_SystemConfig;
base::CLayoutConfig* g_LayoutConfig               = &m_LayoutConfig;
base::CRecorderConfig* g_RecorderConfig           = &m_RecorderConfig;
base::CRecorderDescription* g_RecorderDescription = &m_RecorderDescription;
base::CDiagramConfig* g_DiagramConfig             = &m_DiagramConfig;
base::utils::CProductItemList* g_ProductDatabase  = &m_ProductDatabase;


remote::IRemoteServerInterface* g_RemoteServerInterface = nullptr;
remote::IRemoteInterface* g_RemoteInterface = nullptr;
IDoseServerInterface* g_DoseServerInterface = nullptr;
ITKBoardInterface* g_TKBoard = nullptr;





