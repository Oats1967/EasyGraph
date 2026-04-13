//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module WrapperBase
///
/// @file   WrapperBase.h
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

#include "remote/include/RemoteInterface.h"
#include "DoseServerInterface.h"
#include "TKBoardInterface.h"
#include "BASE/Utils/public/UserRightsList.h"
#include "BASE/Utils/public/LogItemList.h"
#include "BASE/Utils/public/ProductItemList.h"

#include "BASE/Utils/public/MassflowMemory.h"
#include "BASE/Utils/public/RecItemList.h"
#include "BASE/include/EasyControlConfig.h"
#include "BASE/include/SystemConfig.h"
#include "BASE/include/LineConfig.h"
#include "BASE/include/LayoutConfig.h"
#include "BASE/include/RecorderConfig.h"
#include "BASE/include/RecorderDescription.h"
#include "BASE/include/DiagramConfig.h"


extern IDoseServerInterface* g_DoseServerInterface;
#define REFDOSESERVER (*g_DoseServerInterface)

extern remote::IRemoteServerInterface* g_RemoteServerInterface;
#define REFREMOTESERVER (*g_RemoteServerInterface)

extern remote::IRemoteInterface* g_RemoteInterface;
#define REMOTEREF (*g_RemoteInterface)

extern base::utils::CLogItemList* g_LogItemList;
#define LOGREF (*g_LogItemList)

extern base::utils::CUserRightsList* g_UserRightsList;
#define USERRIGHTSREF (*g_UserRightsList)

extern base::utils::CProductItemList* g_ProductDatabase;
#define PRODUCTDATABASEREF (*g_ProductDatabase)

extern base::CEasyControlConfig* g_EasyControlConfig;
#define EASYCONTROLREF (*g_EasyControlConfig)

extern base::utils::CMassflowMemory* g_MassflowMemory;
#define MASSFLOWREF (*g_MassflowMemory)

extern base::utils::CRecItemList* g_RecItemList;
#define RECORDERREF (*g_RecItemList)

extern base::CSystemConfig* g_SystemConfig;
#define SYSTEMCONFIGREF (*g_SystemConfig)

extern base::CLayoutConfig* g_LayoutConfig;
#define LAYOUTCONFIGREF (*g_LayoutConfig)

extern base::CDiagramConfig* g_DiagramConfig;
#define DIAGRAMCONFIGREF (*g_DiagramConfig)

extern base::CRecorderConfig* g_RecorderConfig;
#define RECORDERCONFIGREF (*g_RecorderConfig)



extern ITKBoardInterface* g_TKBoard;
#define TKB (*g_TKBoard)




