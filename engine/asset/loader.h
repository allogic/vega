#ifndef VEGA_ENGINE_ASSET_LOADER_H
#define VEGA_ENGINE_ASSET_LOADER_H

#include <stdint.h>

#include <vega/core/core.h>

typedef enum _asset_buffer_type_t
{
	ASSET_BUFFER_TYPE_MODEL,
	ASSET_BUFFER_TYPE_TEXTURE,
	ASSET_BUFFER_TYPE_MATERIAL,
	ASSET_BUFFER_TYPE_MESH,
} asset_buffer_type_t;

typedef struct _asset_t
{
	struct _asset_t* parent;
	vector_t children;
	asset_buffer_type_t buffer_type;
	void* buffer;
	string_t name;
	string_t file_stem;
	string_t file_ext;
	string_t file_root;
	string_t file_path;
	uint8_t loaded;
	uint8_t is_loading;
	void* thread_handle;
} asset_t;

typedef struct _asset_model_thread_args_t
{
	asset_t* asset;
} asset_model_thread_args_t;

typedef struct _asset_texture_thread_args_t
{
	asset_t* asset;
} asset_texture_thread_args_t;

typedef struct _asset_material_thread_args_t
{
	asset_t* asset;
	struct aiScene const* assimp_scene;
	uint32_t material_index;
} asset_material_thread_args_t;

typedef struct _asset_mesh_thread_args_t
{
	asset_t* asset;
	struct aiScene const* assimp_scene;
	uint32_t mesh_index;
} asset_mesh_thread_args_t;

///////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////

void asset_loader_alloc(void);
void asset_loader_free(void);

asset_t* asset_loader_load_model(asset_t* parent, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path);
asset_t* asset_loader_load_texture(asset_t* parent, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path);

///////////////////////////////////////////////////////////////
// Internal API
///////////////////////////////////////////////////////////////

void asset_loader_load_materials(asset_t* parent, struct aiScene const* assimp_scene);
void asset_loader_load_meshes(asset_t* parent, struct aiScene const* assimp_scene);

void asset_loader_print_stats(struct aiScene const* assimp_scene);

void asset_loader_collect_thread_handles_recursive(asset_t* parent, vector_t* assets);
void asset_loader_wait_sub_assets(asset_t* asset);

#endif // VEGA_ENGINE_ASSET_LOADER_H