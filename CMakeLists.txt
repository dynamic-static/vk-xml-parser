
# ==========================================
#   Copyright (c) 2020 Dynamic_Static
#     Patrick Purcell
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

# Dynamic_Static.Graphics CMake configuration
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(Dynamic_Static.VkXmlParser VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")
set(externalDirectory "${CMAKE_CURRENT_LIST_DIR}/external/")

# External dependencies
add_subdirectory("${externalDirectory}/Dynamic_Static.Core/")
include(tinyxml2)
enable_testing()
# Dynamic_Static.VkXmlParser library
set(includeDirectory "${CMAKE_CURRENT_LIST_DIR}/include/")
set(includePath "${includeDirectory}/dynamic_static/vk-xml-parser/")
set(sourcePath "${CMAKE_CURRENT_LIST_DIR}/source/dynamic_static/vk-xml-parser/")
DstAddStaticLibrary(
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
)

# vk-api-manifest-dump executable
DstAddExecutable(
    target vk-api-manifest-dump
    linkLibraries dynamic_static.vk-xml-parser
    sourceFiles "${CMAKE_CURRENT_LIST_DIR}/vk-api-manifest-dump.cpp"
)
