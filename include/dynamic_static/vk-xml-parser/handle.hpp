
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
#include <set>
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
                parents.insert(parent);
            }
        } else {
            name = attribute(xmlElement, "name");
            alias = attribute(xmlElement, "alias");
        }
        if (name == "VkSwapchainKHR") {
            parents.insert("VkDevice");
        }
    }

    std::set<std::string> parents;          //!< TODO : Documentation
    std::set<std::string> createInfos;      //!< TODO : Documentation
    std::set<std::string> createFunctions;  //!< TODO : Documentation
    std::set<std::string> destroyFunctions; //!< TODO : Documentation
    bool dispatchable { false };            //!< TODO : Documentation
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
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
