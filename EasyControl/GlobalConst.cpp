//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LwfAlarmConfigPage.cpp
///
/// @file   LwfAlarmConfigPage.cpp
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
#include "EasyControl.h"
#include "GlobalConst.h"
#include "BASE/include/MassflowSelect.h"

namespace globalconst
{

	static_assert(_S32(base::eDoseType::DOSETYPE_NONE) == 0, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_LWS) == 1, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_LWV) == 2, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_LWP) == 3, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_LWB) == 4, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_GWB) == 5, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_ICB) == 6, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_WBF) == 7, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_PID) == 8, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_ICP) == 9, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_WBM) == 10, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_VOL) == 11, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_LCL) == 12, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_IFL) == 13, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_IFSO) == 14, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_IFSU) == 15, "Invalid eDoseType configuration");
	static_assert(_S32(base::eDoseType::DOSETYPE_MAX) == 16, "Invalid eDoseType configuration");


	// als extern 
	const int32_t g_DoseTitleIDIndex[_S32(base::cDoseTypeMax)] =
	{ 0, IDS_LWSTITLE, IDS_LWVTITLE, IDS_LWPTITLE, IDS_LWBTITLE, IDS_GWBTITLE, IDS_ICBTITLE, IDS_WBFTITLE,
		 IDS_PIDTITLE,IDS_ICPTITLE, IDS_WBMTITLE, IDS_VOLTITLE, IDS_LCLTITLE, IDS_IFLTITLE, IDS_IFOSTITLE, IDS_IFUSTITLE
	};

	const int32_t g_MassflowView[base::cMassflowSelectMax] =
	{ IDS_MASSFLOW_KGH,	IDS_SETPOINT_KGH,IDS_DRIVECOMMAND_PERCENTAGE, IDS_WEIGHT_KG,
	   IDS_DOSEPERFORMANCE_KGH,	IDS_TOTALIZER_KG,	IDS_ROTSPEED_UPM
	};
};

