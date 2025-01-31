#ifndef VEGA_ENGINE_ASSET_MATERIAL_H
#define VEGA_ENGINE_ASSET_MATERIAL_H

#include <stdint.h>

#include <vega/core/core.h>

typedef enum _material_asset_property_type_t
{
	MATERIAL_ASSET_PROPERTY_TYPE_DOUBLE,
	MATERIAL_ASSET_PROPERTY_TYPE_INTEGER,
	MATERIAL_ASSET_PROPERTY_TYPE_STRING,
	MATERIAL_ASSET_PROPERTY_TYPE_BUFFER,
} material_asset_property_type_t;

typedef struct _material_asset_property_t
{
	material_asset_property_type_t buffer_type;
	void* buffer;
} material_asset_property_t;

typedef struct _material_asset_t
{
	string_t name;
	uint32_t material_index;
	uint8_t loaded;
	uint8_t is_loading;
	map_t properties;
} material_asset_t;

#endif // VEGA_ENGINE_ASSET_MATERIAL_H
