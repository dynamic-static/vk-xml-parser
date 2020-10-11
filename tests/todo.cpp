
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "dynamic_static/vk-xml-parser.hpp"

#include "catch2/catch.hpp"

#include <iostream>

namespace dst {
namespace cppgen {
namespace tests {

TEST_CASE("vk::xml::Parser")
{
    using namespace dst::vk::xml;
    std::string vkXmlFilePath = DYNAMIC_STATIC_VK_XML_FILE_PATH;
    tinyxml2::XMLDocument vkXml;
    auto result = vkXml.LoadFile(vkXmlFilePath.c_str());
    if (result == tinyxml2::XML_SUCCESS) {
        Manifest vkXmlManifest(vkXml);
        std::vector<const Handle*> handles;
        std::unordered_set<const Handle*> processedHandles;
        handles.reserve(vkXmlManifest.handles.size());
        size_t count = 0;
        vkXmlManifest.handles.enumerate_in_dependency_order(
            [&](const Handle& handle)
            {
                static size_t sCount;
                std::cout << sCount++ << " : " << handle.name << std::endl;
            }
        );
    }
}

} // namespace tests
} // namespace cppgen
} // namespace dst
