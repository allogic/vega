file (GLOB SRC_FILES "*.c")

add_library (engine STATIC ${SRC_FILES})

add_subdirectory (asset)
add_subdirectory (component)
add_subdirectory (entity)
add_subdirectory (platform)
add_subdirectory (vulkan)

target_compile_definitions (engine PUBLIC "_CRT_SECURE_NO_WARNINGS")
target_compile_definitions (engine PUBLIC "VEGA_ROOT_DIR=\"${CMAKE_SOURCE_DIR}\"")

target_include_directories (engine PUBLIC "${VULKAN_SDK_DIR}/include")

target_link_directories (engine PUBLIC "${VULKAN_SDK_DIR}/Lib")

target_link_libraries (engine PUBLIC "vulkan-1.lib")
target_link_libraries (engine PUBLIC "assimp-vc143-mt.lib")
target_link_libraries (engine PUBLIC "zlibstatic.lib")
target_link_libraries (engine PUBLIC core)
