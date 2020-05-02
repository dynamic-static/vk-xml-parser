
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

#include <set>
#include <string>
#include <vector>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Enumerator final
{
public:
    /**
    TODO : Documentation
    */
    Enumerator() = default;

    /**
    Constructs an instance of Enumerator
    TODO : Documentation
    */
    inline Enumerator(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        value = attribute(xmlElement, "value");
        if (value.empty()) {
            value = attribute(xmlElement, "bitpos");
        }
        alias = attribute(xmlElement, "alias");
    }

    std::string name;         //!< TODO : Documentation
    std::string value;        //!< TODO : Documentation
    std::string alias;        //!< TODO : Documentation
    std::string compileGuard; //!< TODO : Documentation
};

/**
TODO : Documentation
*/
bool operator==(const Enumerator& lhs, const Enumerator& rhs)
{
    return
        lhs.name == rhs.name &&
        lhs.value == rhs.value &&
        lhs.alias == rhs.alias &&
        lhs.compileGuard == rhs.compileGuard;
}

/**
TODO : Documentation
*/
bool operator!=(const Enumerator& lhs, const Enumerator& rhs)
{
    return !(lhs == rhs);
}

/**
TODO : Documentation
*/
bool operator<(const Enumerator& lhs, const Enumerator& rhs)
{
    auto tie =
        [](const Enumerator& obj)
        {
            return std::tie(
                obj.value,
                obj.name,
                obj.alias,
                obj.compileGuard
            );
        };
    return tie(lhs) < tie(rhs);
}

/**
TODO : Documentation
*/
class Enumeration final
    : public Element
{
public:
    class Manifest;

    /**
    Constructs an instance of Enumeration
    TODO : Documentation
    */
    inline Enumeration(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        bitMask = attribute(xmlElement, "type") == "bitmask";
        auto pXmlElement = xmlElement.FirstChildElement();
        while (pXmlElement) {
            enumerators.insert(*pXmlElement);
            pXmlElement = pXmlElement->NextSiblingElement();
        }
    }

    std::set<Enumerator> enumerators; //!< TODO : Documentation
    bool bitMask { false };           //!< TODO : Documentation
};

/**
TODO : Documentation
*/
class Enumeration::Manifest final
    : public Element::Manifest<Enumeration>
{
public:
    /**
    Constructs an instance of Enumeration::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Enumeration::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement & registryXmlElement)
    {
        // TODO : Figure out how to post process enumerations to get aliases,
        //  compile guards, etc...
        const auto& xmlElement = first_child_element(registryXmlElement, "enums");
        auto pXmlElement = registryXmlElement.FirstChildElement("enums");
        while (pXmlElement) {
            Enumeration enumeration(*pXmlElement);
            if (enumeration.name != "API Constants") {
                insert(enumeration);
            }
            pXmlElement = pXmlElement->NextSiblingElement("enums");
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
