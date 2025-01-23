cmake_minimum_required (VERSION 3.8)

project (vega LANGUAGES C)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)

set (VULKAN_SDK "C:/VulkanSDK/1.4.304.0")
set (TRACY_SDK "C:/Users/mialb/Downloads/tracy_latest/tracy")

if (NOT CMAKE_BUILD_TYPE)

	set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type (e.g. Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)

endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

	# TODO

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")

	# TODO

elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

	# TODO

elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")

	# TODO

else ()

	# TODO

endif()

if (MSVC)

	# Multi-Processor Compilation
	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")

	# /O2          : Full optimizations (favor speed over size)
	# /Oi          : Enable intrinsics (replace standard library calls with faster intrinsics)
	# /Ot          : Favor fast code over small code
	# /GL          : Enable link-time code generation (LTCG) for better cross-module optimizations
	# /fp:fast     : Use fast floating-point optimizations (sacrifices strict IEEE compliance for speed)
	# /arch:AVX512 : Enable AVX512 support
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Oi /Ot /GL /fp:fast /arch:AVX512")

	# /LTCG        : Link Time Code Generation
	# /OPT:REF     : Eliminate unreferenced functions/data
	# /OPT:ICF     : Fold identical COMDATs (functions and data that are identical into a single instance)
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")

endif ()

include_directories ("${CMAKE_SOURCE_DIR}/..")
include_directories ("${TRACY_SDK}/public")

link_directories ("${CMAKE_SOURCE_DIR}/library")

link_libraries ("TracyClient.lib")

add_compile_definitions (VEGA_DEBUG)
#add_compile_definitions (TRACY_ENABLE)

add_subdirectory (core)
add_subdirectory (engine)
add_subdirectory (game)
