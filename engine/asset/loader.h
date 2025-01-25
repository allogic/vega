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

#endif // VEGA_ENGINE_ASSET_LOADER_H