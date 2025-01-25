file (GLOB SRC_FILES "*.c")

add_library (core STATIC ${SRC_FILES})

add_subdirectory (debug)
add_subdirectory (fs)
add_subdirectory (math)
add_subdirectory (std)
