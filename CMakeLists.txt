cmake_minimum_required (VERSION 3.8)

project (vega LANGUAGES C)

###############################################################
# Set C standard
###############################################################

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)

###############################################################
# Set SDK paths
###############################################################

set (ASSIMP_SDK "${CMAKE_SOURCE_DIR}/assimp")
set (TRACY_SDK "${CMAKE_SOURCE_DIR}/tracy")
set (VULKAN_SDK "C:/VulkanSDK/1.4.304.0")

###############################################################
# Set default configuration
###############################################################

if (NOT CMAKE_BUILD_TYPE)

	set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type (Debug, Release)" FORCE)

endif ()

###############################################################
# Set build specifics
###############################################################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")

	add_compile_definitions (VEGA_DEBUG)
	#add_compile_definitions (TRACY_ENABLE)

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")

	#add_compile_definitions (TRACY_ENABLE)

else ()

	message (FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")

endif ()

###############################################################
# Detect platform
###############################################################

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

	add_compile_definitions (VEGA_OS_WINDOWS)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

	add_compile_definitions (VEGA_OS_LINUX)

elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")

	add_compile_definitions (VEGA_OS_DARWIN)

else ()

	message (FATAL_ERROR "Unknown platform: ${CMAKE_SYSTEM_NAME}")

endif ()

###############################################################
# Detect SIMD support
###############################################################

include (CheckCCompilerFlag)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")

	check_c_compiler_flag ("/arch:SSE2" HAS_SSE2)
	check_c_compiler_flag ("/arch:AVX" HAS_AVX)
	check_c_compiler_flag ("/arch:AVX2" HAS_AVX2)
	check_c_compiler_flag ("/arch:AVX512" HAS_AVX512)

	if (HAS_SSE2)

		set (SIMD_FLAGS "${SIMD_FLAGS} /arch:SSE2")

		add_compile_definitions (VEGA_SSE_SUPPORT)
		add_compile_definitions (VEGA_SSE2_SUPPORT)

	endif ()

	if (HAS_AVX)

		set (SIMD_FLAGS "${SIMD_FLAGS} /arch:AVX")

		add_compile_definitions (VEGA_AVX_SUPPORT)

	endif ()

	if (HAS_AVX2)

		set (SIMD_FLAGS "${SIMD_FLAGS} /arch:AVX2")

		add_compile_definitions (VEGA_AVX_SUPPORT)
		add_compile_definitions (VEGA_AVX2_SUPPORT)

	endif ()

	if (HAS_AVX512)

		set (SIMD_FLAGS "${SIMD_FLAGS} /arch:AVX512")

		add_compile_definitions (VEGA_AVX_SUPPORT)
		add_compile_definitions (VEGA_AVX512_SUPPORT)

	endif()

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")

	check_c_compiler_flag ("-msse2" HAS_SSE2)
	check_c_compiler_flag ("-mavx" HAS_AVX)
	check_c_compiler_flag ("-mavx2" HAS_AVX2)
	check_c_compiler_flag ("-mavx512f" HAS_AVX512F)

	if (HAS_SSE2)

		set (SIMD_FLAGS "${SIMD_FLAGS} -msse2")

		add_compile_definitions (VEGA_SSE_SUPPORT)
		add_compile_definitions (VEGA_SSE2_SUPPORT)

	endif ()

	if (HAS_AVX)

		set (SIMD_FLAGS "${SIMD_FLAGS} -mavx")

		add_compile_definitions (VEGA_AVX_SUPPORT)

	endif ()

	if (HAS_AVX2)

		set (SIMD_FLAGS "${SIMD_FLAGS} -mavx2")

		add_compile_definitions (VEGA_AVX_SUPPORT)
		add_compile_definitions (VEGA_AVX2_SUPPORT)

	endif ()

	if (HAS_AVX512F)

		set (SIMD_FLAGS "${SIMD_FLAGS} -mavx512f")

		add_compile_definitions (VEGA_AVX_SUPPORT)
		add_compile_definitions (VEGA_AVX512_SUPPORT)

	endif()

else ()

	message (FATAL_ERROR "Unknown compiler: ${CMAKE_C_COMPILER_ID}")

endif ()

###############################################################
# Set compiler flags
###############################################################

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")

	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od ${SIMD_FLAGS}")
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Oi /Ot /GL /fp:fast ${SIMD_FLAGS}")

	set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")

elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")

	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og ${SIMD_FLAGS}")
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -ffast-math -ffp-contract=fast -march=native -flto ${SIMD_FLAGS}")

    set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
    set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -Wl,--gc-sections -Wl,--icf=safe")

else ()

	message (FATAL_ERROR "Unknown compiler: ${CMAKE_C_COMPILER_ID}")

endif ()

###############################################################
# Set global includes paths
###############################################################

include_directories ("${CMAKE_SOURCE_DIR}/..")
include_directories ("${TRACY_SDK}/public")

###############################################################
# Set global link paths
###############################################################

link_directories ("${CMAKE_SOURCE_DIR}/library")

###############################################################
# Set global link libraries
###############################################################

link_libraries ("TracyClient.lib")

###############################################################
# Include sub-projects
###############################################################

add_subdirectory (core)
add_subdirectory (engine)
add_subdirectory (game)
