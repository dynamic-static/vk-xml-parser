
# ==========================================
#   Copyright (c) 2020-2021 dynamic_static
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

include(FetchContent)
FetchContent_Declare(
    dynamic_static.build
    GIT_REPOSITORY "https://github.com/dynamic-static/dynamic_static.build.git"
    GIT_TAG trunk
    GIT_PROGRESS TRUE
    FETCHCONTENT_UPDATES_DISCONNECTED
)
FetchContent_MakeAvailable(dynamic_static.build)
include("${dynamic_static.build_SOURCE_DIR}/dynamic_static.cmake")
