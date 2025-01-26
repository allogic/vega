file (GLOB SRC_FILES "*.c")

add_library (engine STATIC ${SRC_FILES})

add_subdirectory (asset)
add_subdirectory (platform)
add_subdirectory (vulkan)

target_include_directories (engine PUBLIC "${VULKAN_SDK}/include")

target_link_directories (engine PUBLIC "${VULKAN_SDK}/Lib")

target_link_libraries (engine PUBLIC "vulkan-1.lib")
target_link_libraries (engine PUBLIC "assimp-vc143-mt.lib")
target_link_libraries (engine PUBLIC "zlibstatic.lib")
target_link_libraries (engine PUBLIC core)
