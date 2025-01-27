#ifndef VEGA_ENGINE_ASSET_MATERIAL_H
#define VEGA_ENGINE_ASSET_MATERIAL_H

#include <stdint.h>

#include <vega/core/core.h>

typedef enum _asset_material_property_type_t
{
	ASSET_MATERIAL_PROPERTY_TYPE_DOUBLE,
	ASSET_MATERIAL_PROPERTY_TYPE_INTEGER,
	ASSET_MATERIAL_PROPERTY_TYPE_STRING,
	ASSET_MATERIAL_PROPERTY_TYPE_BUFFER,
} asset_material_property_type_t;

typedef struct _asset_material_property_t
{
	asset_material_property_type_t buffer_type;
	void* buffer;
} asset_material_property_t;

typedef struct _asset_material_t
{
	map_t properties;
} asset_material_t;

#endif // VEGA_ENGINE_ASSET_MATERIAL_H
