//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DoseTypeMap
///
/// @file   DoseTypeMap.h
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
#include "BASE/types.h"
#include "BASE/include/StringEnumMap.h"
#include "BASE/include/MassflowSelect.h"

class CMassflowSelectMap : public base:: CStringEnumMap< base::eMassflowSelect >
{
    public:
        CMassflowSelectMap() :CStringEnumMap(
            {
                { base::eMassflowSelect::eVIEWMASSFLOW, "Durchsatz" },
                { base::eMassflowSelect::eVIEWSETPOINT, "Sollwert" },
                { base::eMassflowSelect::eVIEWDRIVECOMMAND, "Stellbefehl" },
                { base::eMassflowSelect::eVIEWWEIGHT, "Nettogewicht" },
                { base::eMassflowSelect::eVIEWDOSEPERFORMANCE, "Dosierperformanz" },
                { base::eMassflowSelect::eVIEWROTSPEED, "Drehzahl" },
                { base::eMassflowSelect::eVIEWTOTALIZER, "Verbrauch" }
            })
        {
            assert(m_Map.size() == _S32(base::eMassflowSelect::eVIEWMAX));
        }

        CMassflowSelectMap(const CMassflowSelectMap& rM) : CStringEnumMap(rM)
        {}
};
