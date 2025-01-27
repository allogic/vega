#ifndef VEGA_ENGINE_ASSET_TEXTURE_H
#define VEGA_ENGINE_ASSET_TEXTURE_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _asset_texture_t
{
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	void* buffer;
	uint64_t buffer_size;
} asset_texture_t;

#endif // VEGA_ENGINE_ASSET_TEXTURE_H
