#ifndef VEGA_ENGINE_RENDERER_MATERIAL_H
#define VEGA_ENGINE_RENDERER_MATERIAL_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/image.h>

typedef enum _material_type_t
{
	MATERIAL_TYPE_PBR,
} material_type_t;

typedef struct _pbr_material_t
{
	image_t* base_color;
} pbr_material_t;

#endif // VEGA_ENGINE_RENDERER_MATERIAL_H
