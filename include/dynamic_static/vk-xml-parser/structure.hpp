
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/vk-xml-parser/element.hpp"
#include "dynamic_static/vk-xml-parser/parameter.hpp"

#include "tinyxml2.h"

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Structure final
    : public Element
{
public:
    class Manifest;

    /**
    Constructs an instance of Enumeration
    TODO : Documentation
    */
    inline Structure(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        alias = attribute(xmlElement, "alias");
        isUnion = attribute(xmlElement, "category") == "union";
        process_child_elements(
            xmlElement,
            "member",
            [&](const tinyxml2::XMLElement& memberXmlElement)
            {
                members.emplace_back(memberXmlElement);
                if (members.back().name == "sType") {
                    vkStructureType = attribute(memberXmlElement, "values");
                }
            }
        );
    }

    std::string vkStructureType;    //!< TODO : Documentation
    std::vector<Parameter> members; //!< TODO : Documentation
    bool isUnion { false };         //!< TODO : Documentation
};

/**
TODO : Documentation
*/
class Structure::Manifest final
    : public Element::Manifest<Structure>
{
public:
    /**
    Constructs an instance of Structure::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Structure::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        const auto& xmlElement = first_child_element(registryXmlElement, "types");
        auto pXmlElement = xmlElement.FirstChildElement();
        while (pXmlElement) {
            auto category = attribute(*pXmlElement, "category");
            if (category == "struct" || category == "union") {
                insert(Structure(*pXmlElement));
            }
            pXmlElement = pXmlElement->NextSiblingElement();
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
