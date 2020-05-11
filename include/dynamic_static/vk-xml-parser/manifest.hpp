
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

#include <map>
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
    inline Manifest(const tinyxml2::XMLDocument& vkXml)
    {
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
        post_process_handle();
        post_process_object_types();
        post_process_structure_types();
    }

    Platform::Manifest platforms;                      //!< TODO : Documentation
    Vendor::Manifest vendors;                          //!< TODO : Documentation
    Handle::Manifest handles;                          //!< TODO : Documentation
    Enumeration::Manifest enumerations;                //!< TODO : Documentation
    Structure::Manifest structures;                    //!< TODO : Documentation
    Function::Manifest functions;                      //!< TODO : Documentation
    Extension::Manifest extensions;                    //!< TODO : Documentation
    std::map<std::string, std::string> objectTypes;    //!< TODO : Documentation
    std::map<std::string, std::string> structureTypes; //!< TODO : Documentation

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
            for (auto& extensionEnumeration : extension.enumerations) {
                auto enumerationItr = enumerations.find(extensionEnumeration.first);
                assert(enumerationItr != enumerations.end());
                auto& enumeration = enumerationItr->second;
                for (auto& extensionEnumerator : extensionEnumeration.second) {
                    extensionEnumerator.compileGuard = extension.compileGuard;
                    enumeration.enumerators.insert(extensionEnumerator);
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

    inline void post_process_handle()
    {
        for (const auto& vkFunctionItr : functions) {
            auto const& vkFunction = vkFunctionItr.second;
            if (vkFunction.parameters.size() > 1) {
                switch (vkFunction.type) {
                case Function::Type::Create: {
                    const auto& vkHandleParameter = vkFunction.parameters.back();
                    auto vkHandleItr = handles.find(vkHandleParameter.unqualifiedType);
                    if (vkHandleItr != handles.end()) {
                        vkHandleItr->second.createFunctions.insert(vkFunction.name);
                        for (const auto& vkParameter : vkFunction.parameters) {
                            if (structures.count(vkParameter.unqualifiedType) && vkParameter.unqualifiedType != "VkAllocationCallbacks") {
                                vkHandleItr->second.createInfos.insert(vkParameter.unqualifiedType);
                            }
                        }
                    }
                } break;
                case Function::Type::Destroy: {
                    auto vkHandleParameterIndex = vkFunction.parameters.size() - 2;
                    const auto& vkHandleParameter = vkFunction.parameters[vkHandleParameterIndex];
                    auto vkHandleItr = handles.find(vkHandleParameter.unqualifiedType);
                    if (vkHandleItr != handles.end()) {
                        vkHandleItr->second.destroyFunctions.insert(vkFunction.name);
                    }
                } break;
                }
            }
        }
    }

    inline void post_process_object_types()
    {
        // TODO :
    }

    inline void post_process_structure_types()
    {
        auto vkStructureTypeEnumerationItr = enumerations.find("VkStructureType");
        assert(vkStructureTypeEnumerationItr != enumerations.end());
        const auto& vkStructureTypeEnumeration = vkStructureTypeEnumerationItr->second;
        for (const auto& vkStructureTypeEnumerator : vkStructureTypeEnumeration.enumerators) {
            std::string vkStructureTypeName = "Vk";
            for (auto token : string::split_snake_case(string::remove(vkStructureTypeEnumerator.name, "VK_STRUCTURE_TYPE_"))) {
                if (!token.empty() && !vendors.count(token)) {
                    token = string::to_lower(token);
                    token[0] = string::to_upper(token[0]);
                }
                vkStructureTypeName += token;
            }
            structureTypes.insert({ vkStructureTypeName, vkStructureTypeEnumerator.name });
            structureTypes.insert({ vkStructureTypeEnumerator.name, vkStructureTypeName });
        }
    }
};

} // namespace xml
} // namespace vk
} // namespace dst
