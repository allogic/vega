#ifndef VEGA_ENGINE_ASSET_MATERIAL_H
#define VEGA_ENGINE_ASSET_MATERIAL_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _material_asset_t
{
	string_t name;
	uint32_t material_index;
	uint8_t loaded;
	uint8_t is_loading;
	string_t base_color_ref;
} material_asset_t;

#endif // VEGA_ENGINE_ASSET_MATERIAL_H
