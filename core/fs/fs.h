#ifndef VEGA_CORE_FS_H
#define VEGA_CORE_FS_H

#include <stdint.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

uint8_t fs_read_text(uint8_t** buffer, uint64_t* buffer_size, char const* file_path);
uint8_t fs_read_binary(uint8_t** buffer, uint64_t* buffer_size, char const* file_path);

uint8_t fs_write_text(uint8_t* buffer, uint64_t buffer_size, char const* file_path);
uint8_t fs_write_binary(uint8_t* buffer, uint64_t buffer_size, char const* file_path);

#endif // VEGA_CORE_FS_H
