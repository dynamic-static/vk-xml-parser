
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
class Platform final
    : public Element
{
public:
    class Manifest;

    /**
    Creates an instance of Platform
    TODO : Documentation
    */
    inline Platform(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        compileGuard = attribute(xmlElement, "protect");
    }
};

/**
TODO : Documentation
*/
class Platform::Manifest final
    : public Element::Manifest<Platform>
{
public:
    /**
    Constructs an instance of Platform::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Platform::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        const auto& xmlElement = first_child_element(registryXmlElement, "platforms");
        auto pXmlElement = xmlElement.FirstChildElement();
        while (pXmlElement) {
            insert(Platform(*pXmlElement));
            pXmlElement = pXmlElement->NextSiblingElement();
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
