
# ==========================================
#   Copyright (c) 2020 Dynamic_Static
#     Patrick Purcell
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

#### set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
#### set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
#### add_subdirectory("${externalDirectory}/tinyxml2/")
#### set(tinyXmlFolder "external/tinyxml2/")
#### set_target_properties(tinyxml2 PROPERTIES FOLDER "${tinyXmlFolder}")
#### set_target_properties(uninstall PROPERTIES FOLDER "${tinyXmlFolder}")

include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    tinyxml2 GIT_REPOSITORY
    "https://github.com/leethomason/tinyxml2.git"
    GIT_TAG 8.0.0
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tinyxml2)
set_target_properties(tinyxml2 PROPERTIES FOLDER "external/")
