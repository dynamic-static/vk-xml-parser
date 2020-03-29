
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/core/string.hpp"
#include "dynamic_static/vk-xml-parser/element.hpp"
#include "dynamic_static/vk-xml-parser/utilities.hpp"

#include "tinyxml2.h"

#include <map>
#include <vector>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Handle final
    : public Element
{
public:
    class Manifest;

    /**
    Constructs an instance of Handle
    */
    Handle() = default;

    /**
    Constructs an instance of Handle
    TODO : Documentation
    */
    inline Handle(const tinyxml2::XMLElement& xmlElement)
    {
        name = first_child_element_text(xmlElement, "name");
        if (!name.empty()) {
            dispatchable = first_child_element_text(xmlElement, "type") == "VK_DEFINE_HANDLE";
            for (const auto& parent : string::split(attribute(xmlElement, "parent"), ',')) {
                parents.push_back(parent);
            }
        } else {
            name = attribute(xmlElement, "name");
            alias = attribute(xmlElement, "alias");
        }
    }

    std::vector<std::string> parents;  //!< TODO : Documentation
    std::vector<std::string> children; //!< TODO : Documentation
    std::string createFunction;        //!< TODO : Documentation
    std::string destroyFunction;       //!< TODO : Documentation
    bool dispatchable { false };       //!< TODO : Documentation
};

/**
TODO : Documentation
*/
class Handle::Manifest final
    : public Element::Manifest<Handle>
{
public:
    /**
    Constructs an instance of Handle::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Handle::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        const auto& xmlElement = first_child_element(registryXmlElement, "types");
        auto pXmlElement = xmlElement.FirstChildElement();
        while (pXmlElement) {
            if (attribute(*pXmlElement, "category") == "handle") {
                insert(Handle(*pXmlElement));
            }
            pXmlElement = pXmlElement->NextSiblingElement();
        }
        for (auto handleItr : *this) {
            for (const auto& parent : handleItr.second.parents) {
                auto parentItr = find(parent);
                assert(parentItr != end() && "TODO : Documentation");
                parentItr->second.children.push_back(handleItr.second.name);
            }
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
