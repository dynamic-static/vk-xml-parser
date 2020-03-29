
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
#include "dynamic_static/vk-xml-parser/parameter.hpp"

#include "tinyxml2.h"

#include <string>
#include <vector>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Function final
    : public Element
{
public:
    class Manifest;

    /**
    TODO : Documentation
    */
    enum class Type
    {
        Common,  //!< TODO : Documentation
        Cmd,     //!< TODO : Documentation
        Create,  //!< TODO : Documentation
        Destroy, //!< TODO : Documentation
    };

    /**
    Constructs an instance of Function
    TODO : Documentation
    */
    inline Function(const tinyxml2::XMLElement& xmlElement)
    {
        name = attribute(xmlElement, "name");
        alias = attribute(xmlElement, "alias");
        if (name.empty() || alias.empty()) {
            assert(name.empty() && alias.empty() && "TODO : Documentation");
            successCodes = string::split(attribute(xmlElement, "successcodes"), ',');
            errorCodes = string::split(attribute(xmlElement, "errorcodes"), ',');
            const auto& protoXmlElement = first_child_element(xmlElement, "proto");
            returnType = first_child_element_text(protoXmlElement, "type");
            name = first_child_element_text(protoXmlElement, "name");
            process_child_elements(xmlElement, "param",
                [&](const tinyxml2::XMLElement& paramXmlElement)
                {
                    parameters.emplace_back(paramXmlElement);
                }
            );
        }
        if (string::starts_with(name, "vkCmd")) {
            type = Type::Cmd;
        } else
        if (string::starts_with(name, "vkCreate") ||
            string::starts_with(name, "vkAllocate")) {
            type = Type::Create;
            if (alias.empty()) {
                assert(!parameters.empty() && "TODO : Documentation");
                target = parameters.back().unqualifiedType;
            }
        } else
        if (string::starts_with(name, "vkDestroy") ||
            string::starts_with(name, "vkFree")) {
            type = Type::Destroy;
            if (alias.empty()) {
                assert(!parameters.empty() && "TODO : Documentation");
                for (auto itr = parameters.rbegin(); itr != parameters.rend(); ++itr) {
                    if (itr->unqualifiedType != "VkAllocationCallbacks") {
                        target = itr->unqualifiedType;
                        break;
                    }
                }
            }
        }
    }

    Type type { Type::Common };            //!< TODO : Documentation
    std::string target;                    //!< TODO : Documentation
    std::string returnType;                //!< TODO : Documentation
    std::vector<std::string> successCodes; //!< TODO : Documentation
    std::vector<std::string> errorCodes;   //!< TODO : Documentation
    std::vector<Parameter> parameters;     //!< TODO : Documentation
};

/**
TODO : Documentation
*/
class Function::Manifest final
    : public Element::Manifest<Function>
{
public:
    /**
    Constructs an instance of Function::Manifest
    */
    inline Manifest() = default;

    /**
    Constructs an instance of Function::Manifest
    TODO : Documentation
    */
    inline Manifest(const tinyxml2::XMLElement& registryXmlElement)
    {
        process_child_elements(
            first_child_element(registryXmlElement, "commands"), "command",
            [&](const tinyxml2::XMLElement& childXmlElement)
            {
                insert(Function(childXmlElement));
            }
        );
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
