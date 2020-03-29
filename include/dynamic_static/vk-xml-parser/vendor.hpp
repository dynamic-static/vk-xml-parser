
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
#include "dynamic_static/vk-xml-parser/utilities.hpp"

#include "tinyxml2.h"

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Vendor final
    : public Element
{
public:
    class Manifest;

    /**
    Creates an instance of Vendor
    TODO : Documentation
    */
    inline Vendor(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
    }
};

/**
TODO : Documentation
*/
class Vendor::Manifest final
    : public Element::Manifest<Vendor>
{
public:
    /**
    Constructs an instance of Vendor::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Vendor::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        const auto& xmlElement = first_child_element(registryXmlElement, "tags");
        auto pXmlElement = xmlElement.FirstChildElement();
        while (pXmlElement) {
            insert(Vendor(*pXmlElement));
            pXmlElement = pXmlElement->NextSiblingElement();
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
