
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
#include "dynamic_static/core/string.hpp"
#include "dynamic_static/vk-xml-parser/enumeration.hpp"
#include "dynamic_static/vk-xml-parser/extension.hpp"
#include "dynamic_static/vk-xml-parser/function.hpp"
#include "dynamic_static/vk-xml-parser/handle.hpp"
#include "dynamic_static/vk-xml-parser/platform.hpp"
#include "dynamic_static/vk-xml-parser/structure.hpp"
#include "dynamic_static/vk-xml-parser/utilities.hpp"
#include "dynamic_static/vk-xml-parser/vendor.hpp"

#include "tinyxml2.h"

#include <string>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Manifest
{
public:
    /**
    TODO : Documentation
    */
    inline Manifest(const std::string& vkXmlFilePath)
    {
        tinyxml2::XMLDocument vkXml;
        auto result = vkXml.LoadFile(!vkXmlFilePath.empty() ? vkXmlFilePath.c_str() : nullptr);
        if (result == tinyxml2::XML_SUCCESS) {
            const auto& registryXmlElement = first_child_element(vkXml, "registry");
            platforms = Platform::Manifest(registryXmlElement);
            vendors = Vendor::Manifest(registryXmlElement);
            handles = Handle::Manifest(registryXmlElement);
            enumerations = Enumeration::Manifest(registryXmlElement);
            structures = Structure::Manifest(registryXmlElement);
            functions = Function::Manifest(registryXmlElement);
            extensions = Extension::Manifest(registryXmlElement);
            process_element_vendors(handles);
            process_element_vendors(enumerations);
            process_element_vendors(structures);
            process_element_vendors(functions);
            process_element_vendors(extensions);
            process_extensions();
        }
    }

    Platform::Manifest platforms;       //!< TODO : Documentation
    Vendor::Manifest vendors;           //!< TODO : Documentation
    Handle::Manifest handles;           //!< TODO : Documentation
    Enumeration::Manifest enumerations; //!< TODO : Documentation
    Structure::Manifest structures;     //!< TODO : Documentation
    Function::Manifest functions;       //!< TODO : Documentation
    Extension::Manifest extensions;     //!< TODO : Documentation

private:
    template <typename ElementManifestType>
    inline void process_element_vendors(ElementManifestType& elementManifest)
    {
        for (auto& itr : elementManifest) {
            auto& element = itr.second;
            if (element.vendor.empty()) {
                auto tokens = string::split_camel_case(element.name);
                if (!tokens.empty() && vendors.find(tokens.back()) != vendors.end()) {
                    element.vendor = tokens.back();
                }
            }
        }
    }

    inline void process_extensions()
    {
        for (auto& extensionItr : extensions) {
            auto& extension = extensionItr.second;
            const auto& platformItr = platforms.find(extension.platform);
            if (platformItr != platforms.end()) {
                extension.compileGuard = platformItr->second.compileGuard;
            }
            for (const auto& extensionEnumeration : extension.enumerations) {
                auto& enumerationItr = enumerations.find(extensionEnumeration.first);
                assert(enumerationItr != enumerations.end());
                auto& enumeration = enumerationItr->second;
                for (const auto& extensionEnumerator : extensionEnumeration.second) {
                    Enumerator enumerator { };
                    enumerator.name = extensionEnumerator;
                    enumerator.compileGuard = extension.compileGuard;
                    enumeration.enumerators.push_back(enumerator);
                }
            }
            for (const auto& extensionType : extension.types) {
                if (apply_extension_to_element(extension, extensionType, handles) ||
                    apply_extension_to_element(extension, extensionType, enumerations) ||
                    apply_extension_to_element(extension, extensionType, structures)) {
                    continue;
                }
            }
            for (const auto& extensionFunction : extension.functions) {
                if (apply_extension_to_element(extension, extensionFunction, functions)) {
                    continue;
                }
            }
        }
    }

    template <typename ElementManifestType>
    inline bool apply_extension_to_element(
        const Extension& extension,
        const std::string& elementName,
        ElementManifestType& elementManifest
    )
    {
        auto itr = elementManifest.find(elementName);
        if (itr != elementManifest.end()) {
            auto& element = itr->second;
            element.extension = extension.name;
            element.compileGuard = extension.compileGuard;
            return true;
        }
        return false;
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
