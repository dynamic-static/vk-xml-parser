
# ==========================================
#   Copyright (c) 2020 dynamic_static
#     Patrick Purcell
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

# dynamic_static.Graphics CMake configuration
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(dynamic_static.vk-xml-parser VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(dynamic_static "${CMAKE_CURRENT_LIST_DIR}/../")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")

################################################################################
#### include("${dynamic_static}/dynamic_static.CMake/dynamic_static.ex.cmake")
#### DstAddSubdirectory("dynamic_static.Core")
include("${CMAKE_CURRENT_LIST_DIR}/../cmake/dynamic_static.cmake")
dst_add_subdirectory(core)
# add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../core/" "${CMAKE_CURRENT_BINARY_DIR}/dynamic_static/core/")
################################################################################

# External dependencies
set(externalDirectory "${CMAKE_CURRENT_LIST_DIR}/external/")
find_package(Vulkan REQUIRED)
set(Vulkan_SDK_DIR "${Vulkan_INCLUDE_DIRS}/../")
set(Vulkan_XML "${Vulkan_SDK_DIR}/share/vulkan/registry/vk.xml")
include(tinyxml2)

# dynamic_static.VkXmlParser library
set(includeDirectory "${CMAKE_CURRENT_LIST_DIR}/include/")
set(includePath "${includeDirectory}/dynamic_static/vk-xml-parser/")
set(sourcePath "${CMAKE_CURRENT_LIST_DIR}/source/dynamic_static/vk-xml-parser/")
dst_add_static_library(
    target
        dynamic_static.vk-xml-parser
    linkLibraries
        dynamic_static.core
        tinyxml2
    includeDirectories
        "${includeDirectory}"
    includeFiles
        "${includePath}/element.hpp"
        "${includePath}/enumeration.hpp"
        "${includePath}/extension.hpp"
        "${includePath}/function.hpp"
        "${includePath}/handle.hpp"
        "${includePath}/manifest.hpp"
        "${includePath}/parameter.hpp"
        "${includePath}/platform.hpp"
        "${includePath}/structure.hpp"
        "${includePath}/utilities.hpp"
        "${includePath}/vendor.hpp"
        "${includeDirectory}/dynamic_static/vk-xml-parser.hpp"
    sourceFiles
        "${CMAKE_CURRENT_LIST_DIR}/dynamic_static.vk-xml-parser.cpp"
    compileDefinitions
        DYNAMIC_STATIC_VK_XML_FILE_PATH="${Vulkan_XML}"
)

# vk-api-manifest-dump executable
dst_add_executable(
    target vk-api-manifest-dump
    folder dynamic_static.vk-xml-parser/
    linkLibraries dynamic_static.vk-xml-parser
    sourceFiles "${CMAKE_CURRENT_LIST_DIR}/vk-api-manifest-dump.cpp"
)

# dynamic_static.cpp-generator.tests
if(DST_CORE_BUILD_TESTS)
    set(testsPath "${dynamic_static.vk-xml-parser_SOURCE_DIR}/tests/")
    dst_add_test_suite(
        target
            dynamic_static.vk-xml-parser
        sourceFiles
            "${testsPath}/todo.cpp"
    )
endif()
