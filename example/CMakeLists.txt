project(node-addon-utils)

include(node_addon_template.cmake)

set(srcs
    module.cpp
    TestClass.cpp
)

node_addon_target(addon_utils)
target_sources(addon_utils PRIVATE ${srcs})
