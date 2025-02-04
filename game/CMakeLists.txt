file (GLOB SRC_FILES "*.cpp")

add_executable (game ${SRC_FILES})

target_link_libraries (game PRIVATE engine)
