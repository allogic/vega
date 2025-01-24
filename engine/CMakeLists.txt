file (GLOB SRC_FILES "*.c")

add_library (engine STATIC ${SRC_FILES})

add_subdirectory (platform)
add_subdirectory (vulkan)

target_include_directories (engine PUBLIC "${ASSIMP_SDK}/Include")
target_include_directories (engine PUBLIC "${VULKAN_SDK}/include")

target_link_directories (engine PUBLIC "${VULKAN_SDK}/Lib")

target_link_libraries (engine PUBLIC "vulkan-1.lib")
target_link_libraries (engine PUBLIC core)
