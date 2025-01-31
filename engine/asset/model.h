#ifndef VEGA_ENGINE_ASSET_MODEL_H
#define VEGA_ENGINE_ASSET_MODEL_H

#include <stdint.h>

#include <vega/core/core.h>

typedef struct _model_asset_t
{
	string_t name;
	string_t file_stem;
	string_t file_ext;
	string_t file_root;
	string_t file_path;
	uint8_t loaded;
	uint8_t is_loading;
	vector_t texture_refs;
	vector_t material_refs;
	vector_t mesh_refs;
} model_asset_t;

#endif // VEGA_ENGINE_ASSET_MODEL_H
