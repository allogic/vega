#ifndef VEGA_ENGINE_ASSET_TEXTURE_H
#define VEGA_ENGINE_ASSET_TEXTURE_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/vulkan/image.h>

VEGA_EXTERN_C_BEGIN

typedef struct _texture_asset_t
{
	string_t name;
	string_t file_stem;
	string_t file_ext;
	string_t file_root;
	string_t file_path;
	uint8_t loaded;
	uint8_t is_loading;
	uint32_t width;
	uint32_t height;
	uint32_t channels;
	uint8_t* buffer;
	image_t image;
} texture_asset_t;

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_ASSET_TEXTURE_H
