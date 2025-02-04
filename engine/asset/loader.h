#ifndef VEGA_ENGINE_ASSET_LOADER_H
#define VEGA_ENGINE_ASSET_LOADER_H

#include <stdint.h>

#include <vega/core/core.h>

#include <vega/engine/asset/material.h>
#include <vega/engine/asset/mesh.h>
#include <vega/engine/asset/model.h>
#include <vega/engine/asset/texture.h>

VEGA_EXTERN_C_BEGIN

typedef struct _model_asset_thread_args_t
{
	model_asset_t* model_asset;
} model_asset_thread_args_t;

typedef struct _texture_asset_thread_args_t
{
	texture_asset_t* texture_asset;
} texture_asset_thread_args_t;

typedef struct _material_asset_thread_args_t
{
	model_asset_t* model_asset;
	material_asset_t* material_asset;
	struct aiScene const* assimp_scene;
} material_asset_thread_args_t;

typedef struct _mesh_asset_thread_args_t
{
	model_asset_t* model_asset;
	mesh_asset_t* mesh_asset;
	struct aiScene const* assimp_scene;
} mesh_asset_thread_args_t;

extern map_t g_asset_loader_models;
extern map_t g_asset_loader_textures;
extern map_t g_asset_loader_materials;
extern map_t g_asset_loader_meshes;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void asset_loader_alloc(void);
void asset_loader_free(void);

void asset_loader_load_model(char const* name, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path);
void asset_loader_load_texture(char const* name, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void asset_loader_print_stats(struct aiScene const* assimp_scene);

vector_t asset_loader_material_tokenize_property_key(struct aiMaterialProperty const* assimp_material_property);

void asset_loader_build_gpu_resources(void);

VEGA_EXTERN_C_END

#endif // VEGA_ENGINE_ASSET_LOADER_H