#ifndef VEGA_CORE_HEAP_H
#define VEGA_CORE_HEAP_H

#include <stdint.h>

#ifdef VEGA_DEBUG
extern uint64_t g_heap_allocated_bytes;
#endif // VEGA_DEBUG

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void* heap_alloc(uint64_t size);
void* heap_realloc(void* block, uint64_t size);
void heap_free(void* block);

#endif // VEGA_CORE_HEAP_H
