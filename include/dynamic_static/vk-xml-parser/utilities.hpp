
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/core/defines.hpp"

#include "tinyxml2.h"

#include <string>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
inline std::string attribute(
    const tinyxml2::XMLElement& xmlElement,
    const std::string& childElementName
)
{
    assert(!childElementName.empty() && "TODO : Documentation");
    auto pAttribute = xmlElement.Attribute(childElementName.c_str());
    return pAttribute ? pAttribute : std::string();
}

/**
TODO : Documentation
*/
inline std::string text(const tinyxml2::XMLElement& xmlElement)
{
    auto pText = xmlElement.GetText();
    return pText ? pText : std::string();
}

/**
TODO : Documentation
*/
inline const tinyxml2::XMLElement& first_child_element(
    const tinyxml2::XMLNode& xmlNode,
    const std::string& childElementName
)
{
    assert(!childElementName.empty() && "TODO : Documentation");
    auto pXmlElement = xmlNode.FirstChildElement(childElementName.c_str());
    assert(pXmlElement && "TODO : Documentation");
    return *pXmlElement;
}

/**
TODO : Documentation
*/
inline const std::string first_child_element_text(
    const tinyxml2::XMLNode& xmlNode,
    const std::string& childElementName
)
{
    assert(!childElementName.empty() && "TODO : Documentation");
    auto pXmlElement = xmlNode.FirstChildElement(childElementName.c_str());
    return pXmlElement ? text(*pXmlElement) : std::string();
}

/**
TODO : Documenation
*/
template <typename ProcessElementFunctionType>
inline void process_child_elements(
    const tinyxml2::XMLElement& xmlElement,
    const std::string& childElementName,
    ProcessElementFunctionType processElement
)
{
    assert(!childElementName.empty() && "TODO : Documentation");
    for (
        auto pChildXmlElement = xmlElement.FirstChildElement(childElementName.c_str());
        pChildXmlElement;
        pChildXmlElement = pChildXmlElement->NextSiblingElement(childElementName.c_str())
    ) {
        processElement(*pChildXmlElement);
    }
}

/**
TODO : Documentation
*/
template <typename ProcessNodeFunctionType>
inline void process_child_nodes(
    const tinyxml2::XMLElement& xmlElement,
    ProcessNodeFunctionType processNode
)
{
    for (
        auto pChildXmlNode = xmlElement.FirstChild();
        pChildXmlNode;
        pChildXmlNode = pChildXmlNode->NextSibling()
    ) {
        processNode(*pChildXmlNode);
    }
}

/**
TODO : Documentation
*/
template <typename ProcessNodeFunctionType>
inline void process_sibling_nodes(
    const tinyxml2::XMLNode& xmlNode,
    ProcessNodeFunctionType processNode
)
{
    for (
        auto pSiblingXmlNode = xmlNode.NextSibling();
        pSiblingXmlNode;
        pSiblingXmlNode = pSiblingXmlNode->NextSibling()
    ) {
        processNode(*pSiblingXmlNode);
    }
}

} // namespace xml
} // namespace vk
} // namespace dst
