
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
#include "dynamic_static/vk-xml-parser/utilities.hpp"

#include "tinyxml2.h"

#include <string>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Parameter final
{
public:
    /**
    TODO : Documentation
    */
    enum FlagBits
    {
        Const           = 1,      //!< TODO : Documentation
        Pointer         = 1 << 1, //!< TODO : Documentation
        Array           = 1 << 2, //!< TODO : Documentation
        String          = 1 << 3, //!< TODO : Documentation
        DynamicArray    = 1 << 4, //!< TODO : Documentation
        StaticArray     = 1 << 5, //!< TODO : Documentation
        PointerArray    = 1 << 6, //!< TODO : Documentation
        StringArray     = 1 << 7, //!< TODO : Documentation
        FunctionPointer = 1 << 8, //!< TODO : Documentation
    };

    /**
    TODO : Documentation
    */
    using Flags = uint32_t;

    /**
    Constructs an instance of Parameter
    */
    Parameter(const tinyxml2::XMLElement& xmlElement)
    {
        unqualifiedType = first_child_element_text(xmlElement, "type");
        type = unqualifiedType;
        auto pNameXmlElement = xmlElement.FirstChildElement("name");
        if (pNameXmlElement) {
            name = text(*pNameXmlElement);
        }
        length = attribute(xmlElement, "len");
        if (!length.empty()) {
            length = string::split(length, ',')[0];
            if (length == "ceil(rasterizationSamples / 32)") {
                length = "(rasterizationSamples + 31) / 32";
            }
        }
        process_child_nodes(
            xmlElement,
            [&](const tinyxml2::XMLNode& node)
            {
                auto pValue = node.Value();
                if (pValue) {
                    std::string value = pValue;
                    if (string::contains(value, "* const")) {
                        flags |= Const | Pointer;
                        type += "* const";
                    } else {
                        if (string::contains(value, "const")) {
                            flags |= Const;
                            if (string::contains(type, "const")) {
                                type += " const";
                            } else {
                                type = "const " + type;
                            }
                        }
                        if (string::contains(value, '*')) {
                            flags |= Pointer;
                            type += "*";
                            if (string::contains(value, "**")) {
                                flags |= PointerArray;
                                type += "*";
                            }
                        }
                    }
                }
            }
        );
        if (!length.empty()) {
            flags |= Array;
            if (length.front() == '[' && length.back() == ']') {
                length = string::remove(string::remove(length, '['), ']');
                flags |= StaticArray;
            } else {
                flags |= DynamicArray;
            }
            if (unqualifiedType == "char") {
                if (flags & PointerArray) {
                    flags |= StringArray;
                } else {
                    flags |= String;
                }
            }
        }
        if (string::contains(unqualifiedType, "PFN_")) {
            flags |= Pointer | FunctionPointer;
        }
    }

    std::string type;            //!< TODO : Documentation
    std::string name;            //!< TODO : Documentation
    std::string length;          //!< TODO : Documentation
    std::string unqualifiedType; //!< TODO : Documentation
    Flags flags { };             //!< TODO : Documentation
};

} // namespace xml
} // namespace vk
} // namespace dst
