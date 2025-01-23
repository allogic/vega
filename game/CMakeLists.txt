file (GLOB SRC_FILES "*.c")

add_executable (game ${SRC_FILES})

target_link_libraries (game PRIVATE engine)
