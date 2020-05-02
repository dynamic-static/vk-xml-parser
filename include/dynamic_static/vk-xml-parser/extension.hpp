
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

#include "tinyxml2.h"

#include <map>
#include <string>
#include <vector>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Extension final
    : public Element
{
public:
    class Manifest;

    /**
    TODO : Documentation
    */
    enum class Type
    {
        Instance, //!< TODO : Documentation
        Device,   //!< TODO : Documentation
    };

    /**
    Constructs an instance of Extension
    TODO : Documentation
    */
    inline Extension(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        extension = name;
        auto nameTokens = string::split(name, '_');
        if (nameTokens.size() > 1) {
            vendor = nameTokens[1];
        }
        author = attribute(xmlElement, "author");
        platform = attribute(xmlElement, "platform");
        type = attribute(xmlElement, "type") == "instance" ? Type::Instance : Type::Device;
        supported = attribute(xmlElement, "supported") == "vulkan";
        deprecatedBy = attribute(xmlElement, "deprecatedby");
        obsoletedBy = attribute(xmlElement, "obsoletedby");
        promotedTo = attribute(xmlElement, "promotedto");
        requirments = string::split(attribute(xmlElement, "requires"), ',');
        auto pRequireXmlElement = xmlElement.FirstChildElement("require");
        while (pRequireXmlElement) {
            process_child_elements(*pRequireXmlElement, "enum",
                [&](const tinyxml2::XMLElement& enumXmlElement)
                {
                    auto extends = attribute(enumXmlElement, "extends");
                    if (!extends.empty()) {
                        enumerations[extends].emplace_back(enumXmlElement);
                    }
                }
            );
            process_child_elements(*pRequireXmlElement, "type",
                [&](const tinyxml2::XMLElement& typeXmlElement)
                {
                    types.push_back(attribute(typeXmlElement, "name"));
                    assert(!types.back().empty() && "TODO : Documentation");
                }
            );
            process_child_elements(*pRequireXmlElement, "command",
                [&](const tinyxml2::XMLElement& commandXmlElement)
                {
                    functions.push_back(attribute(commandXmlElement, "name"));
                    assert(!functions.back().empty() && "TODO : Documentation");
                }
            );
            pRequireXmlElement = pRequireXmlElement->NextSiblingElement("require");
        }
    }

    /**
    TODO : Documentation
    */
    inline bool empty() const
    {
        return requirments.empty() && enumerations.empty() && types.empty() && functions.empty();
    }

    std::string author;                                          //!< TODO : Documentation
    std::string platform;                                        //!< TODO : Documentation
    Type type { Type::Instance };                                //!< TODO : Documentation
    bool supported { true };                                     //!< TODO : Documentation
    std::string deprecatedBy;                                    //!< TODO : Documentation
    std::string obsoletedBy;                                     //!< TODO : Documentation
    std::string promotedTo;                                      //!< TODO : Documentation
    std::vector<std::string> requirments;                        //!< TODO : Documentation
    std::map<std::string, std::vector<Enumerator>> enumerations; //!< TODO : Documentation
    std::vector<std::string> types;                              //!< TODO : Documentation
    std::vector<std::string> functions;                          //!< TODO : Documentation
};

/**
TODO : Documentation
*/
class Extension::Manifest final
    : public Element::Manifest<Extension>
{
public:
    /**
    Constructs an instance of Extension::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Extension::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        process_child_elements(
            first_child_element(registryXmlElement, "extensions"), "extension",
            [&](const tinyxml2::XMLElement& childXmlElement)
            {
                Extension extension(childXmlElement);
                if (!string::contains(extension.name, "RESERVED_DO_NOT_USE")) {
                    insert(extension);
                }
            }
        );
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
