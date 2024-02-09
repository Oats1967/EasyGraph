//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Declaration of module DiagramConfigXml
///
/// @file   DiagramConfigXml.h
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------
#pragma once

#include "BASE/Utils/public/xml/XmlTypes.h"
#include "BASE/Utils/public/xml/XmlContainer.h"

#include "BASE/include/BooleanMap.h"


namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
	class XMLNode;
};

namespace base
{
namespace xml
{

class CDiagramConfigXml : public XmlContainer< CDiagramConfig >
{
	const CYesNoMap				c_YesNoMap;

private:
	void ReadItem(const tinyxml2::XMLElement& config, const base::eMassflowSelect iSelect, CAxisScale& rAxisScale) const;
	void SlpitAttributes(const xmlentry& attriblist, const base::eMassflowSelect iSelect, CAxisScale& rAxisScale) const;
	void SetDefault();

public:
	CDiagramConfigXml() : XmlContainer()
	{}
	CDiagramConfigXml(const CDiagramConfigXml& _Config) : XmlContainer(_Config)
	{}
	CDiagramConfigXml(const CDiagramConfig& _Config) : XmlContainer(_Config)
	{}
	~CDiagramConfigXml() override = default;

	BOOL Load(const std::string& _szPath2Device) final;
	BOOL Save(const std::string& _szPath2Device = std::string("")) final;
};
};
};


