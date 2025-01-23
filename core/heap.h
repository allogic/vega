#ifndef VEGA_CORE_HEAP_H
#define VEGA_CORE_HEAP_H

#include <stdint.h>

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void* heap_alloc(uint64_t size);
void* heap_realloc(void* block, uint64_t size);
void heap_free(void* block);

#endif // VEGA_CORE_HEAP_H
