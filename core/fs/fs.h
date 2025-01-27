#ifndef VEGA_CORE_FS_H
#define VEGA_CORE_FS_H

#include <stdint.h>

#include <vega/core/std/string.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

uint8_t fs_read_text(uint8_t** buffer, uint64_t* buffer_size, char const* file_path);
uint8_t fs_read_binary(uint8_t** buffer, uint64_t* buffer_size, char const* file_path);

uint8_t fs_write_text(uint8_t* buffer, uint64_t buffer_size, char const* file_path);
uint8_t fs_write_binary(uint8_t* buffer, uint64_t buffer_size, char const* file_path);

string_t fs_resolve_file_stem_from_path(char const* file_path, uint64_t file_path_size);
string_t fs_resolve_file_ext_from_path(char const* file_path, uint64_t file_path_size);
string_t fs_resolve_file_root_from_path(char const* file_path, uint64_t file_path_size);

#endif // VEGA_CORE_FS_H
