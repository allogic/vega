#ifndef VEGA_ENGINE_ASSET_LOADER_H
#define VEGA_ENGINE_ASSET_LOADER_H

#include <vega/core/core.h>

typedef struct _asset_t
{
	void* dummy;
} asset_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

asset_t asset_loader_load(char const* file_path);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void asset_load_materials(struct aiScene const* scene);
void asset_load_meshes(struct aiScene const* scene);

void asset_print_stats_raw(struct aiScene const* scene);

#endif // VEGA_ENGINE_ASSET_LOADER_H