
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "dynamic_static/vk-xml-parser.hpp"

#include <iomanip>
#include <iostream>

/**
TODO : Documentation
*/
inline void print_help()
{
    std::cout << "TODO : Documentation" << std::endl;
}

/**
TODO : Documentation
*/
inline std::string tab(size_t tabCount)
{
    return std::string(tabCount * 4, ' ');
}

/**
TODO : Documentation
*/
inline void print_value(const std::string& value, size_t tabCount)
{
    if (!value.empty()) {
        std::cout << tab(tabCount) << value << std::endl;
    }
}

/**
TODO : Documentation
*/
inline void print_named_value(const std::string& name, const std::string& value, size_t tabCount)
{
    if (!name.empty() && !value.empty()) {
        std::cout << tab(tabCount) << name << " : " << value << std::endl;
    }
}

/**
TODO : Documentation
*/
inline void print_named_value(const std::string& name, bool value, size_t tabCount)
{
    if (!name.empty()) {
        std::cout << tab(tabCount) << name << " : " << (value ? "true" : "false") << std::endl;
    }
}

/**
TODO : Documentation
*/
inline void print_named_collection(const std::string& name, const std::vector<std::string>& collection, size_t tabCount)
{
    if (!name.empty() && !collection.empty()) {
        std::cout << tab(tabCount) << name << " :" << std::endl;
        for (const auto& entry : collection) {
            print_value(entry, tabCount + 1);
        }
    }
}

/**
TODO : Documentation
*/
inline void print_parameter(const dst::vk::xml::Parameter& parameter, size_t tabCount)
{
    std::cout << tab(tabCount) << parameter.type << " " << parameter.name;
    if (parameter.flags) {
        std::cout << " { ";
        using namespace dst::vk::xml;
        auto printParamaterFlagBit =
        [&](Parameter::FlagBits flagBit, const std::string& text)
        {
            if (parameter.flags & flagBit) {
                std::cout << text << " ";
            }
        };
        printParamaterFlagBit(Parameter::Const, "Const");
        printParamaterFlagBit(Parameter::Pointer, "Pointer");
        printParamaterFlagBit(Parameter::Array, "Array");
        printParamaterFlagBit(Parameter::String, "String");
        printParamaterFlagBit(Parameter::DynamicArray, "DynamicArray");
        printParamaterFlagBit(Parameter::StaticArray, "StaticArray");
        printParamaterFlagBit(Parameter::PointerArray, "PointerArray");
        printParamaterFlagBit(Parameter::StringArray, "StringArray");
        printParamaterFlagBit(Parameter::FunctionPointer, "FunctionPointer");
        if (!parameter.length.empty()) {
            std::cout << "[" << parameter.length << "] ";
        }
        std::cout << "}";
    }
    std::cout << std::endl;
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Element& element, size_t tabCount)
{
    print_value(element.name, tabCount);
    print_named_value("compileGuard", element.compileGuard, tabCount);
    print_named_value("vendor", element.vendor, tabCount);
    print_named_value("alias", element.alias, tabCount);
    print_named_value("extension", element.extension, tabCount);
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Handle& handle, size_t tabCount)
{
    print_element((const dst::vk::xml::Element&)handle, tabCount);
    print_named_value("dispatchable", handle.dispatchable, tabCount);
    print_named_collection("parents", handle.parents, tabCount);
    print_named_collection("children", handle.children, tabCount);
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Enumerator& enumerator, size_t tabCount)
{
    print_named_value(enumerator.name, enumerator.value, tabCount);
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Enumeration& enumeration, size_t tabCount)
{
    print_element((const dst::vk::xml::Element&)enumeration, tabCount);
    print_named_value("bitMask", enumeration.bitMask, tabCount);
    for (const auto& enumerator : enumeration.enumerators) {
        print_element(enumerator, tabCount + 1);
    }
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Structure& structure, size_t tabCount)
{
    print_element((const dst::vk::xml::Element&)structure, tabCount);
    print_named_value("VkStructureType", structure.vkStructureType, tabCount);
    for (const auto& member : structure.members) {
        print_parameter(member, tabCount + 1);
    }
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Function& function, size_t tabCount)
{
    print_element((const dst::vk::xml::Element&)function, tabCount);
    using namespace dst::vk::xml;
    auto printFunctionType =
    [&](Function::Type functionType, const std::string& text)
    {
        if (function.type == functionType) {
            print_named_value("type", text, tabCount);
        }
    };
    printFunctionType(Function::Type::Common, "Common");
    printFunctionType(Function::Type::Cmd, "Cmd");
    printFunctionType(Function::Type::Create, "Create/Allocate");
    printFunctionType(Function::Type::Destroy, "Destroy/Free");
    print_named_value("target", function.target, tabCount);
    print_named_value("returnType", function.returnType, tabCount);
    auto printReturnCodes =
    [&](const std::string& name, const std::vector<std::string>& returnCodes)
    {
        if (!returnCodes.empty()) {
            std::cout << tab(tabCount) << name << " { ";
            for (const auto& returnCode : returnCodes) {
                std::cout << returnCode << " ";
            }
            std::cout << "}" << std::endl;
        }
    };
    printReturnCodes("successCodes", function.successCodes);
    printReturnCodes("errorCodes", function.errorCodes);
    for (const auto& parameter : function.parameters) {
        print_parameter(parameter, tabCount + 1);
    }
}

/**
TODO : Documentation
*/
inline void print_element(const dst::vk::xml::Extension& extension, size_t tabCount)
{
    print_element((const dst::vk::xml::Element&)extension, tabCount);
    using namespace dst::vk::xml;
    print_named_value("author", extension.author, tabCount);
    print_named_value("platform", extension.platform, tabCount);
    std::string typeStr = extension.type == Extension::Type::Instance ? "Instance" : "Device";
    print_named_value("type", typeStr, tabCount);
    print_named_value("supported", extension.supported, tabCount);
    print_named_value("deprecatedBy", extension.deprecatedBy, tabCount);
    print_named_value("obsoletedBy", extension.obsoletedBy, tabCount);
    print_named_value("promotedTo", extension.promotedTo, tabCount);
    print_named_collection("requirements", extension.requirments, tabCount);
    if (!extension.enumerations.empty()) {
        print_value("enumerations", tabCount + 1);
        for (auto const& enumeration : extension.enumerations) {
            print_value(enumeration.first, tabCount + 2);
            for (auto const& enumerator : enumeration.second) {
                print_element(enumerator, tabCount + 3);
            }
        }
    }
    print_named_collection("types", extension.types, tabCount);
    print_named_collection("functions", extension.functions, tabCount);
}

/**
TODO : Documentation
*/
template <typename ElementManifestType>
inline void print_element_manifest(
    const std::string& elementManifestName,
    const ElementManifestType& elementManifest
)
{
    std::cout << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    std::cout << elementManifestName << " : " << elementManifest.size() << std::endl;
    for (const auto& itr : elementManifest) {
        std::cout << tab(1) << std::string(40, '-') << std::endl;
        print_element(itr.second, 1);
    }
}

int main(int argc, char* argv[])
{
    std::string vkXmlFilePath = argc > 1 ? argv[1] : DYNAMIC_STATIC_VK_XML_FILE_PATH;
    tinyxml2::XMLDocument vkXml;
    auto result = vkXml.LoadFile(vkXmlFilePath.c_str());
    if (result == tinyxml2::XML_SUCCESS) {
        dst::vk::xml::Manifest vkXmlManifest(vkXml);
        std::cout << std::endl;
        std::cout << "Vulkan XML Manifest" << std::endl;
        print_element_manifest("platforms", vkXmlManifest.platforms);
        print_element_manifest("vendors", vkXmlManifest.vendors);
        print_element_manifest("handles", vkXmlManifest.handles);
        print_element_manifest("enumerations", vkXmlManifest.enumerations);
        print_element_manifest("structures", vkXmlManifest.structures);
        print_element_manifest("functions", vkXmlManifest.functions);
        print_element_manifest("extensions", vkXmlManifest.extensions);
    }
    return 0;
}
