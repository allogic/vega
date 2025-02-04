#include <assert.h>
#include <stdio.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#define NO_GUID_DEFS
#include <windows.h>

#include <assimp/cfileio.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

#include <vega/core/std/map.h>

#include <vega/engine/asset/loader.h>

#include <vega/engine/vulkan/vertex.h>

#ifndef TRACY_THREAD_NAME
	#define TRACY_THREAD_NAME(NAME) TracyCSetThreadName(NAME);
#endif // TRACY_THREAD_NAME

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_GREEN, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

#ifndef ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES
	#define ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES (4ULL)
#endif // ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES

// TODO: remove PBR related stuff outside this class..

static LRESULT WINAPI asset_loader_load_model_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_texture_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_material_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_mesh_thread(PVOID user_param);

map_t g_asset_loader_models = { 0 };
map_t g_asset_loader_textures = { 0 };
map_t g_asset_loader_materials = { 0 };
map_t g_asset_loader_meshes = { 0 };

static HANDLE s_asset_loader_mutex = 0;

static map_t s_asset_loader_staging_models = { 0 };
static map_t s_asset_loader_staging_textures = { 0 };
static map_t s_asset_loader_staging_materials = { 0 };
static map_t s_asset_loader_staging_meshes = { 0 };

void asset_loader_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_asset_loader_mutex = CreateMutexA(NULL, 0, NULL);

	g_asset_loader_models = std_map_alloc();
	g_asset_loader_textures = std_map_alloc();
	g_asset_loader_materials = std_map_alloc();
	g_asset_loader_meshes = std_map_alloc();

	s_asset_loader_staging_models = std_map_alloc();
	s_asset_loader_staging_textures = std_map_alloc();
	s_asset_loader_staging_materials = std_map_alloc();
	s_asset_loader_staging_meshes = std_map_alloc();

	TRACY_ZONE_END
}
void asset_loader_free(void)
{
	TRACY_ZONE_BEGIN

	map_iter_t model_iter = std_map_begin(&g_asset_loader_models);
	while (std_map_end(&model_iter) == 0)
	{
		model_asset_t* model_asset = (model_asset_t*)std_map_value(&model_iter, 0);

		std_string_free(&model_asset->name);
		std_string_free(&model_asset->file_stem);
		std_string_free(&model_asset->file_ext);
		std_string_free(&model_asset->file_root);
		std_string_free(&model_asset->file_path);

		uint64_t texture_ref_index = 0;
		uint64_t texture_ref_count = std_vector_count(&model_asset->texture_refs);
		while (texture_ref_index < texture_ref_count)
		{
			string_t* texture_ref = (string_t*)std_vector_get(&model_asset->texture_refs, texture_ref_index);

			std_string_free(texture_ref);

			texture_ref_index++;
		}

		uint64_t material_ref_index = 0;
		uint64_t material_ref_count = std_vector_count(&model_asset->material_refs);
		while (material_ref_index < material_ref_count)
		{
			string_t* material_ref = (string_t*)std_vector_get(&model_asset->material_refs, material_ref_index);

			std_string_free(material_ref);

			material_ref_index++;
		}

		uint64_t mesh_ref_index = 0;
		uint64_t mesh_ref_count = std_vector_count(&model_asset->mesh_refs);
		while (mesh_ref_index < mesh_ref_count)
		{
			string_t* mesh_ref = (string_t*)std_vector_get(&model_asset->mesh_refs, mesh_ref_index);

			std_string_free(mesh_ref);

			mesh_ref_index++;
		}

		std_vector_free(&model_asset->texture_refs);
		std_vector_free(&model_asset->material_refs);
		std_vector_free(&model_asset->mesh_refs);
	}

	map_iter_t texture_iter = std_map_begin(&g_asset_loader_textures);
	while (std_map_end(&texture_iter) == 0)
	{
		texture_asset_t* texture_asset = (texture_asset_t*)std_map_value(&texture_iter, 0);

		std_string_free(&texture_asset->name);
		std_string_free(&texture_asset->file_stem);
		std_string_free(&texture_asset->file_ext);
		std_string_free(&texture_asset->file_root);
		std_string_free(&texture_asset->file_path);

		heap_free(texture_asset->buffer);

		vulkan_image_free(&texture_asset->image);
	}

	map_iter_t material_iter = std_map_begin(&g_asset_loader_materials);
	while (std_map_end(&material_iter) == 0)
	{
		material_asset_t* material_asset = (material_asset_t*)std_map_value(&material_iter, 0);

		std_string_free(&material_asset->name);
		std_string_free(&material_asset->base_color_ref);
	}

	map_iter_t mesh_iter = std_map_begin(&g_asset_loader_meshes);
	while (std_map_end(&mesh_iter) == 0)
	{
		mesh_asset_t* mesh_asset = (mesh_asset_t*)std_map_value(&mesh_iter, 0);

		std_string_free(&mesh_asset->name);
		std_string_free(&mesh_asset->material_ref);

		std_vector_free(&mesh_asset->pbr_vertices);
		std_fvector32_free(&mesh_asset->pbr_indices);

		vulkan_buffer_free(&mesh_asset->pbr_vertex_buffer);
		vulkan_buffer_free(&mesh_asset->pbr_index_buffer);
	}

	std_map_free(&g_asset_loader_models);
	std_map_free(&g_asset_loader_textures);
	std_map_free(&g_asset_loader_materials);
	std_map_free(&g_asset_loader_meshes);

	std_map_free(&s_asset_loader_staging_models);
	std_map_free(&s_asset_loader_staging_textures);
	std_map_free(&s_asset_loader_staging_materials);
	std_map_free(&s_asset_loader_staging_meshes);

	CloseHandle(s_asset_loader_mutex);

	TRACY_ZONE_END
}
void asset_loader_load_model(char const* name, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path)
{
	TRACY_ZONE_BEGIN

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	model_asset_t* model_asset = (model_asset_t*)std_map_insert(&s_asset_loader_staging_models, name, strlen(name), 0, sizeof(model_asset_t));
	model_asset_thread_args_t* model_asset_thread_args = (model_asset_thread_args_t*)heap_alloc(sizeof(model_asset_thread_args_t));

	ReleaseMutex(s_asset_loader_mutex);

	model_asset->name = std_string_from(file_stem, strlen(file_stem));
	model_asset->file_stem = std_string_from(file_stem, strlen(file_stem));
	model_asset->file_ext = std_string_from(file_ext, strlen(file_ext));
	model_asset->file_root = std_string_from(file_root, strlen(file_root));
	model_asset->file_path = std_string_from(file_path, strlen(file_path));
	model_asset->is_loading = 1;
	model_asset->texture_refs = std_vector_alloc(sizeof(string_t));
	model_asset->material_refs = std_vector_alloc(sizeof(string_t));
	model_asset->mesh_refs = std_vector_alloc(sizeof(string_t));

	model_asset_thread_args->model_asset = model_asset;

	uint64_t thread_handle = (uint64_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_model_thread, model_asset_thread_args, 0, NULL);

	WaitForSingleObject((HANDLE)thread_handle, INFINITE);

	asset_loader_build_gpu_resources();

	TRACY_ZONE_END
}
void asset_loader_load_texture(char const* name, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path)
{
	TRACY_ZONE_BEGIN

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	texture_asset_t* texture_asset = (texture_asset_t*)std_map_insert(&s_asset_loader_staging_textures, name, strlen(name), 0, sizeof(texture_asset_t));
	texture_asset_thread_args_t* texture_asset_thread_args = (texture_asset_thread_args_t*)heap_alloc(sizeof(texture_asset_thread_args_t));

	ReleaseMutex(s_asset_loader_mutex);

	texture_asset->name = std_string_from(file_stem, strlen(file_stem));
	texture_asset->file_stem = std_string_from(file_stem, strlen(file_stem));
	texture_asset->file_ext = std_string_from(file_ext, strlen(file_ext));
	texture_asset->file_root = std_string_from(file_root, strlen(file_root));
	texture_asset->file_path = std_string_from(file_path, strlen(file_path));
	texture_asset->is_loading = 1;

	texture_asset_thread_args->texture_asset = texture_asset;

	uint64_t thread_handle = (uint64_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_texture_thread, texture_asset_thread_args, 0, NULL);

	WaitForSingleObject((HANDLE)thread_handle, INFINITE);

	asset_loader_build_gpu_resources();

	TRACY_ZONE_END
}
void asset_loader_print_stats(struct aiScene const* assimp_scene)
{
	TRACY_ZONE_BEGIN

	uint32_t material_index = 0;
	while (material_index < assimp_scene->mNumMaterials)
	{
		struct aiMaterial const* assimp_material = assimp_scene->mMaterials[material_index];

		printf("Material_%u\n", material_index);

		uint32_t property_index = 0;
		while (property_index < assimp_material->mNumProperties)
		{
			struct aiMaterialProperty const* assimp_material_property = assimp_material->mProperties[property_index];

			printf("\tProperty_%u\n", property_index);
			printf("\t\tKey %s\n", assimp_material_property->mKey.data + 1);
			printf("\t\tSemantic %u\n", assimp_material_property->mSemantic);
			printf("\t\tTexture Index %u\n", assimp_material_property->mIndex);
			printf("\t\tValue ");

			switch (assimp_material_property->mType)
			{
				case aiPTI_Float:
				{
					printf("%f", *((float*)assimp_material_property->mData));

					break;
				}
				case aiPTI_Double:
				{
					printf("%f", *((double*)assimp_material_property->mData));

					break;
				}
				case aiPTI_String:
				{
					uint32_t data_index = ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES;
					while (data_index < assimp_material_property->mDataLength - ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES)
					{
						printf("%c", assimp_material_property->mData[data_index]);

						data_index++;
					}

					break;
				}
				case aiPTI_Integer:
				{
					printf("%d", *((int32_t*)assimp_material_property->mData));

					break;
				}
				case aiPTI_Buffer:
				{
					uint32_t data_index = 0;
					while (data_index < assimp_material_property->mDataLength)
					{
						printf("%02X ", assimp_material_property->mData[data_index]);

						data_index++;
					}

					break;
				}
			}

			printf("\n");

			property_index++;
		}

		printf("\n");

		material_index++;
	}

	uint32_t texture_index = 0;
	while (texture_index < assimp_scene->mNumTextures)
	{
		struct aiTexture const* assimp_texture = assimp_scene->mTextures[texture_index];

		printf("Texture_%u\n", texture_index);
		printf("\tFile Name %s\n", assimp_texture->mFilename.data);
		printf("\tWidth %u\n", assimp_texture->mWidth);
		printf("\tHeight %u\n", assimp_texture->mHeight);
		printf("\n");

		texture_index++;
	}

	uint32_t mesh_index = 0;
	while (mesh_index < assimp_scene->mNumMeshes)
	{
		struct aiMesh const* assimp_mesh = assimp_scene->mMeshes[mesh_index];

		printf("Mesh_%u\n", mesh_index);
		printf("\tName %s\n", assimp_mesh->mName.data);
		printf("\tVertices %u\n", assimp_mesh->mNumVertices);
		printf("\tFaces %u\n", assimp_mesh->mNumFaces);
		printf("\tBones %u\n", assimp_mesh->mNumBones);
		printf("\tMaterial Index %u\n", assimp_mesh->mMaterialIndex);
		printf("\n");

		mesh_index++;
	}

	uint32_t animation_index = 0;
	while (animation_index < assimp_scene->mNumAnimations)
	{
		struct aiAnimation const* assimp_animation = assimp_scene->mAnimations[animation_index];

		printf("Animation_%u\n", animation_index);
		printf("\tName %s\n", assimp_animation->mName.data);
		printf("\tDuration %f\n", assimp_animation->mDuration);
		printf("\tTicks Per Second %f\n", assimp_animation->mTicksPerSecond);
		printf("\n");

		animation_index++;
	}

	uint32_t skeleton_index = 0;
	while (skeleton_index < assimp_scene->mNumSkeletons)
	{
		struct aiSkeleton const* assimp_skeleton = assimp_scene->mSkeletons[skeleton_index];

		printf("Skeleton_%u\n", skeleton_index);
		printf("\tName %s\n", assimp_skeleton->mName.data);
		printf("\tBones %u\n", assimp_skeleton->mNumBones);
		printf("\n");

		skeleton_index++;
	}

	TRACY_ZONE_END
}
vector_t asset_loader_material_tokenize_property_key(struct aiMaterialProperty const* assimp_material_property)
{
	TRACY_ZONE_BEGIN

	vector_t tokens = std_vector_alloc(sizeof(string_t));

	char const* start = assimp_material_property->mKey.data + 1;
	char const* end = assimp_material_property->mKey.data + 1;

	while (*end)
	{
		if (*end == '|')
		{
			string_t token = std_string_from(start, end - start);

			std_vector_push(&tokens, &token);

			start = end + 1;
		}

		end++;
	}

	string_t token = std_string_from(start, end - start);

	std_vector_push(&tokens, &token);

	TRACY_ZONE_END

	return tokens;
}
void asset_loader_build_gpu_resources(void)
{
	TRACY_ZONE_BEGIN

	map_iter_t model_iter = std_map_begin(&s_asset_loader_staging_models);
	while (std_map_end(&model_iter) == 0)
	{
		uint64_t model_name_size = 0;

		char* model_name = (char*)std_map_key(&model_iter, &model_name_size);
		model_asset_t* model_asset = (model_asset_t*)std_map_value(&model_iter, 0);

		std_map_insert(&g_asset_loader_models, model_name, model_name_size, model_asset, sizeof(model_asset_t));
	}

	map_iter_t texture_iter = std_map_begin(&s_asset_loader_staging_textures);
	while (std_map_end(&texture_iter) == 0)
	{
		uint64_t texture_name_size = 0;

		char* texture_name = (char*)std_map_key(&texture_iter, &texture_name_size);
		texture_asset_t* texture_asset = (texture_asset_t*)std_map_value(&texture_iter, 0);

		switch (texture_asset->channels)
		{
			case 1: texture_asset->image = vulkan_image_2d_r_alloc(texture_asset->buffer, texture_asset->width, texture_asset->height, VK_FORMAT_R8_UNORM); break;
			case 2: texture_asset->image = vulkan_image_2d_rg_alloc(texture_asset->buffer, texture_asset->width, texture_asset->height, VK_FORMAT_R8G8_UNORM); break;
			case 3: texture_asset->image = vulkan_image_2d_rgb_alloc(texture_asset->buffer, texture_asset->width, texture_asset->height, VK_FORMAT_R8G8B8_UNORM); break;
			case 4: texture_asset->image = vulkan_image_2d_rgba_alloc(texture_asset->buffer, texture_asset->width, texture_asset->height, VK_FORMAT_R8G8B8A8_UNORM); break;
		}

		std_map_insert(&g_asset_loader_textures, texture_name, texture_name_size, texture_asset, sizeof(texture_asset_t));
	}

	map_iter_t material_iter = std_map_begin(&s_asset_loader_staging_materials);
	while (std_map_end(&material_iter) == 0)
	{
		uint64_t material_name_size = 0;

		char* material_name = (char*)std_map_key(&material_iter, &material_name_size);
		material_asset_t* material_asset = (material_asset_t*)std_map_value(&material_iter, 0);

		std_map_insert(&g_asset_loader_materials, material_name, material_name_size, material_asset, sizeof(material_asset_t));
	}

	map_iter_t mesh_iter = std_map_begin(&s_asset_loader_staging_meshes);
	while (std_map_end(&mesh_iter) == 0)
	{
		uint64_t mesh_name_size = 0;

		char* mesh_name = (char*)std_map_key(&mesh_iter, &mesh_name_size);
		mesh_asset_t* mesh_asset = (mesh_asset_t*)std_map_value(&mesh_iter, 0);

		mesh_asset->pbr_vertex_buffer = vulkan_buffer_vertex_alloc(std_vector_buffer(&mesh_asset->pbr_vertices), std_vector_size(&mesh_asset->pbr_vertices));
		mesh_asset->pbr_index_buffer = vulkan_buffer_index_alloc(std_fvector32_buffer(&mesh_asset->pbr_indices), std_fvector32_size(&mesh_asset->pbr_indices));

		std_map_insert(&g_asset_loader_meshes, mesh_name, mesh_name_size, mesh_asset, sizeof(texture_asset_t));
	}

	std_map_clear(&s_asset_loader_staging_models);
	std_map_clear(&s_asset_loader_staging_textures);
	std_map_clear(&s_asset_loader_staging_materials);
	std_map_clear(&s_asset_loader_staging_meshes);

	TRACY_ZONE_END
}
static LRESULT WINAPI asset_loader_load_model_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Model")
	TRACY_ZONE_BEGIN

	model_asset_thread_args_t* model_asset_thread_args = (model_asset_thread_args_t*)user_param;
	model_asset_t* model_asset = model_asset_thread_args->model_asset;

	fvector64_t thread_handles = std_fvector64_alloc();

	uint32_t flags =
		aiProcess_ValidateDataStructure |
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_Triangulate |
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PopulateArmatureData;

	struct aiScene const* assimp_scene = aiImportFile(std_string_buffer(&model_asset->file_path), flags);

	if (assimp_scene)
	{
		if (assimp_scene->mRootNode && (assimp_scene->mFlags & ~(AI_SCENE_FLAGS_INCOMPLETE)))
		{
			asset_loader_print_stats(assimp_scene);

			uint32_t material_index = 0;
			while (material_index < assimp_scene->mNumMaterials)
			{
				struct aiMaterial const* assimp_material = assimp_scene->mMaterials[material_index];
				struct aiMaterialProperty const* first_assimp_material_property = assimp_material->mProperties[0];

				VEGA_ASSERT(strcmp(first_assimp_material_property->mKey.data + 1, "mat.name") == 0, "First material property key must be \"mat.name\"");
				VEGA_ASSERT(first_assimp_material_property->mSemantic == 0, "First material property semantic must be \"0\"");
				VEGA_ASSERT(first_assimp_material_property->mIndex == 0, "First material property texture index must be \"0\"");
				VEGA_ASSERT(first_assimp_material_property->mType == aiPTI_String, "First material property type must be \"aiPTI_String\"");

				string_t name = std_string_from(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, strlen(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES));
				string_t name_ref = std_string_from(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, strlen(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES));

				WaitForSingleObject(s_asset_loader_mutex, INFINITE);
				std_vector_push(&model_asset->material_refs, &name_ref);
				material_asset_t* material_asset = (material_asset_t*)std_map_insert(&s_asset_loader_staging_materials, std_string_buffer(&name), std_string_size(&name), 0, sizeof(material_asset_t));
				material_asset_thread_args_t* material_asset_thread_args = (material_asset_thread_args_t*)heap_alloc(sizeof(material_asset_thread_args_t));
				ReleaseMutex(s_asset_loader_mutex);

				material_asset->name = name;
				material_asset->material_index = material_index;
				material_asset->is_loading = 1;

				material_asset_thread_args->model_asset = model_asset;
				material_asset_thread_args->material_asset = material_asset;
				material_asset_thread_args->assimp_scene = assimp_scene;

				uint64_t thread_handle = (uint64_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_material_thread, material_asset_thread_args, 0, NULL);

				std_fvector64_push(&thread_handles, thread_handle);

				material_index++;
			}

			uint32_t mesh_index = 0;
			while (mesh_index < assimp_scene->mNumMeshes)
			{
				struct aiMesh const* assimp_mesh = assimp_scene->mMeshes[mesh_index];

				string_t name = std_string_from(assimp_mesh->mName.data, assimp_mesh->mName.length);
				string_t name_ref = std_string_from(assimp_mesh->mName.data, assimp_mesh->mName.length);

				WaitForSingleObject(s_asset_loader_mutex, INFINITE);
				std_vector_push(&model_asset->mesh_refs, &name_ref);
				mesh_asset_t* mesh_asset = (mesh_asset_t*)std_map_insert(&s_asset_loader_staging_meshes, std_string_buffer(&name), std_string_size(&name), 0, sizeof(mesh_asset_t));
				mesh_asset_thread_args_t* mesh_asset_thread_args = (mesh_asset_thread_args_t*)heap_alloc(sizeof(mesh_asset_thread_args_t));
				ReleaseMutex(s_asset_loader_mutex);

				mesh_asset->name = name;
				mesh_asset->mesh_index = mesh_index;
				mesh_asset->is_loading = 1;
				mesh_asset->pbr_vertices = std_vector_alloc(sizeof(pbr_vertex_t));
				mesh_asset->pbr_indices = std_fvector32_alloc();

				mesh_asset_thread_args->model_asset = model_asset;
				mesh_asset_thread_args->mesh_asset = mesh_asset;
				mesh_asset_thread_args->assimp_scene = assimp_scene;

				uint64_t thread_handle = (uint64_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_mesh_thread, mesh_asset_thread_args, 0, NULL);

				std_fvector64_push(&thread_handles, thread_handle);

				mesh_index++;
			}
		}
	}

	WaitForMultipleObjects((uint32_t)std_fvector64_count(&thread_handles), (HANDLE const*)std_fvector64_buffer(&thread_handles), 1, INFINITE);

	if (assimp_scene)
	{
		aiReleaseImport(assimp_scene);
	}

	std_fvector64_free(&thread_handles);

	model_asset->loaded = 1;
	model_asset->is_loading = 0;

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	heap_free(model_asset_thread_args);

#ifdef VEGA_ASSET_LOADER_VERBOSE_ASYNC
	printf("Model loaded %s\n", std_string_buffer(&model_asset->file_path));
#endif // VEGA_ASSET_LOADER_VERBOSE_ASYNC

	ReleaseMutex(s_asset_loader_mutex);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_texture_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Texture")
	TRACY_ZONE_BEGIN

	texture_asset_thread_args_t* texture_asset_thread_args = (texture_asset_thread_args_t*)user_param;
	texture_asset_t* texture_asset = texture_asset_thread_args->texture_asset;

	fvector64_t thread_handles = std_fvector64_alloc();

	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t channels = 0;

	uint8_t* src_buffer = stbi_load(std_string_buffer(&texture_asset->file_path), &width, &height, &channels, 0);

	if (src_buffer)
	{
		WaitForSingleObject(s_asset_loader_mutex, INFINITE);
		uint8_t* dst_buffer = (uint8_t*)heap_alloc(width * height * 4);
		ReleaseMutex(s_asset_loader_mutex);

		uint64_t buffer_index = 0;
		uint64_t buffer_count = width * height;
		while (buffer_index < buffer_count)
		{
			switch (channels)
			{
				case 1:
				{
					uint64_t index_c4 = buffer_index * 4;
					uint64_t index_c1 = buffer_index * 1;

					dst_buffer[index_c4 + 0] = src_buffer[index_c1 + 0];
					dst_buffer[index_c4 + 1] = 0x0;
					dst_buffer[index_c4 + 2] = 0x0;
					dst_buffer[index_c4 + 3] = 0xFF;

					break;
				}
				case 2:
				{
					uint64_t index_c4 = buffer_index * 4;
					uint64_t index_c2 = buffer_index * 2;

					dst_buffer[index_c4 + 0] = src_buffer[index_c2 + 0];
					dst_buffer[index_c4 + 1] = src_buffer[index_c2 + 1];
					dst_buffer[index_c4 + 2] = 0x0;
					dst_buffer[index_c4 + 3] = 0xFF;

					break;
				}
				case 3:
				{
					uint64_t index_c4 = buffer_index * 4;
					uint64_t index_c3 = buffer_index * 3;

					dst_buffer[index_c4 + 0] = src_buffer[index_c3 + 0];
					dst_buffer[index_c4 + 1] = src_buffer[index_c3 + 1];
					dst_buffer[index_c4 + 2] = src_buffer[index_c3 + 2];
					dst_buffer[index_c4 + 3] = 0xFF;

					break;
				}
				case 4:
				{
					uint64_t index_c4 = buffer_index * 4;

					dst_buffer[index_c4 + 0] = src_buffer[index_c4 + 0];
					dst_buffer[index_c4 + 1] = src_buffer[index_c4 + 1];
					dst_buffer[index_c4 + 2] = src_buffer[index_c4 + 2];
					dst_buffer[index_c4 + 3] = src_buffer[index_c4 + 3];

					break;
				}
			}

			buffer_index++;
		}

		texture_asset->width = width;
		texture_asset->height = height;
		texture_asset->channels = 4;
		texture_asset->buffer = dst_buffer;
	}

	WaitForMultipleObjects((uint32_t)std_fvector64_count(&thread_handles), (HANDLE const*)std_fvector64_buffer(&thread_handles), 1, INFINITE);

	if (src_buffer)
	{
		stbi_image_free(src_buffer);
	}

	std_fvector64_free(&thread_handles);

	texture_asset->loaded = 1;
	texture_asset->is_loading = 0;

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	heap_free(texture_asset_thread_args);

#ifdef VEGA_ASSET_LOADER_VERBOSE_ASYNC
	printf("Texture loaded %s\n", std_string_buffer(&texture_asset->file_path));
#endif // VEGA_ASSET_LOADER_VERBOSE_ASYNC

	ReleaseMutex(s_asset_loader_mutex);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_material_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Material")
	TRACY_ZONE_BEGIN

	material_asset_thread_args_t* material_asset_thread_args = (material_asset_thread_args_t*)user_param;
	material_asset_t* material_asset = material_asset_thread_args->material_asset;
	model_asset_t* model_asset = material_asset_thread_args->model_asset;
	struct aiScene const* assimp_scene = material_asset_thread_args->assimp_scene;

	fvector64_t thread_handles = std_fvector64_alloc();

	struct aiMaterial const* assimp_material = assimp_scene->mMaterials[material_asset->material_index];

	uint32_t property_index = 1;
	while (property_index < assimp_material->mNumProperties)
	{
		struct aiMaterialProperty const* assimp_material_property = assimp_material->mProperties[property_index];

		/* TODO
		vector_t tokens = asset_loader_material_tokenize_property_key(assimp_material_property);

		uint64_t token_index = 0;
		uint64_t token_count = std_vector_count(&tokens);
		while (token_index < token_count)
		{
			string_t* token = (string_t*)std_vector_get(&tokens, token_index);

			printf("Token %s\n", std_string_buffer(token));

			token_index++;
		}
		*/

		// TODO: cleanup and map invalid material properties to future PBR material..

		switch (assimp_material_property->mType)
		{
			case aiPTI_Float:
			{
				break;
			}
			case aiPTI_Double:
			{
				break;
			}
			case aiPTI_String:
			{
				if (assimp_material_property->mSemantic == aiTextureType_UNKNOWN)
				{

				}
				else if ((assimp_material_property->mSemantic > aiTextureType_NONE) && (assimp_material_property->mSemantic <= AI_TEXTURE_TYPE_MAX))
				{
					string_t file_root = std_string_from(std_string_buffer(&model_asset->file_root), std_string_size(&model_asset->file_root));
					string_t file_path = std_string_from(std_string_buffer(&model_asset->file_root), std_string_size(&model_asset->file_root));

					std_string_append(&file_path, "/", 1);
					std_string_append(&file_path, assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, assimp_material_property->mDataLength - ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES);

					string_t file_stem = fs_resolve_file_stem_from_path(std_string_buffer(&file_path), std_string_size(&file_path));
					string_t file_ext = fs_resolve_file_ext_from_path(std_string_buffer(&file_path), std_string_size(&file_path));

					string_t name = std_string_from(std_string_buffer(&file_stem), std_string_size(&file_stem));
					string_t name_ref = std_string_from(std_string_buffer(&file_stem), std_string_size(&file_stem));

					WaitForSingleObject(s_asset_loader_mutex, INFINITE);
					std_vector_push(&model_asset->texture_refs, &name_ref);
					texture_asset_t* texture_asset = (texture_asset_t*)std_map_insert(&s_asset_loader_staging_textures, std_string_buffer(&name), std_string_size(&name), 0, sizeof(texture_asset_t));
					texture_asset_thread_args_t* texture_asset_thread_args = (texture_asset_thread_args_t*)heap_alloc(sizeof(texture_asset_thread_args_t));
					ReleaseMutex(s_asset_loader_mutex);

					texture_asset->name = name;
					texture_asset->file_stem = file_stem;
					texture_asset->file_ext = file_ext;
					texture_asset->file_root = file_root;
					texture_asset->file_path = file_path;
					texture_asset->is_loading = 1;

					texture_asset_thread_args->texture_asset = texture_asset;

					uint64_t thread_handle = (uint64_t)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_texture_thread, texture_asset_thread_args, 0, NULL);

					material_asset->base_color_ref = std_string_from(std_string_buffer(&name), std_string_size(&name));

					std_fvector64_push(&thread_handles, thread_handle);
				}
				else
				{
					material_asset->base_color_ref = std_string_alloc();
				}

				break;
			}
			case aiPTI_Integer:
			{
				break;
			}
			case aiPTI_Buffer:
			{
				break;
			}
		}

		property_index++;
	}

	WaitForMultipleObjects((uint32_t)std_fvector64_count(&thread_handles), (HANDLE const*)std_fvector64_buffer(&thread_handles), 1, INFINITE);

	std_fvector64_free(&thread_handles);

	material_asset->loaded = 1;
	material_asset->is_loading = 0;

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	heap_free(material_asset_thread_args);

#ifdef VEGA_ASSET_LOADER_VERBOSE_ASYNC
	printf("Material loaded %s\n", std_string_buffer(&material_asset->file_path));
#endif // VEGA_ASSET_LOADER_VERBOSE_ASYNC

	ReleaseMutex(s_asset_loader_mutex);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_mesh_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Mesh")
	TRACY_ZONE_BEGIN

	mesh_asset_thread_args_t* mesh_asset_thread_args = (mesh_asset_thread_args_t*)user_param;
	mesh_asset_t* mesh_asset = mesh_asset_thread_args->mesh_asset;
	model_asset_t* model_asset = mesh_asset_thread_args->model_asset;
	struct aiScene const* assimp_scene = mesh_asset_thread_args->assimp_scene;

	fvector64_t thread_handles = std_fvector64_alloc();

	struct aiMesh const* assimp_mesh = assimp_scene->mMeshes[mesh_asset->mesh_index];

	uint64_t vertex_index = 0;
	while (vertex_index < assimp_mesh->mNumVertices)
	{
		pbr_vertex_t pbr_vertex = { 0 };

		if (assimp_mesh->mVertices)
		{
			pbr_vertex.position.x = assimp_mesh->mVertices[vertex_index].x;
			pbr_vertex.position.y = assimp_mesh->mVertices[vertex_index].y;
			pbr_vertex.position.z = assimp_mesh->mVertices[vertex_index].z;
			pbr_vertex.position.w = 1.0;
		}

		if (assimp_mesh->mNormals)
		{
			pbr_vertex.normal.x = assimp_mesh->mNormals[vertex_index].x;
			pbr_vertex.normal.y = assimp_mesh->mNormals[vertex_index].y;
			pbr_vertex.normal.z = assimp_mesh->mNormals[vertex_index].z;
			pbr_vertex.normal.w = 0.0;
		}

		if (assimp_mesh->mTangents)
		{
			pbr_vertex.tangent.x = assimp_mesh->mTangents[vertex_index].x;
			pbr_vertex.tangent.y = assimp_mesh->mTangents[vertex_index].y;
			pbr_vertex.tangent.z = assimp_mesh->mTangents[vertex_index].z;
			pbr_vertex.tangent.w = 0.0;
		}

		if (assimp_mesh->mBitangents)
		{
			pbr_vertex.bitangent.x = assimp_mesh->mBitangents[vertex_index].x;
			pbr_vertex.bitangent.y = assimp_mesh->mBitangents[vertex_index].y;
			pbr_vertex.bitangent.z = assimp_mesh->mBitangents[vertex_index].z;
			pbr_vertex.bitangent.w = 0.0;
		}

		if (assimp_mesh->mColors[0])
		{
			pbr_vertex.color_channel_0.x = assimp_mesh->mColors[0][vertex_index].r;
			pbr_vertex.color_channel_0.y = assimp_mesh->mColors[0][vertex_index].g;
			pbr_vertex.color_channel_0.z = assimp_mesh->mColors[0][vertex_index].b;
			pbr_vertex.color_channel_0.w = assimp_mesh->mColors[0][vertex_index].a;
		}

		if (assimp_mesh->mTextureCoords[0])
		{
			pbr_vertex.texcoord_channel_0.x = assimp_mesh->mTextureCoords[0][vertex_index].x;
			pbr_vertex.texcoord_channel_0.y = assimp_mesh->mTextureCoords[0][vertex_index].y;
		}

		std_vector_push(&mesh_asset->pbr_vertices, &pbr_vertex);

		vertex_index++;
	}

	uint32_t face_index = 0;
	while (face_index < assimp_mesh->mNumFaces)
	{
		struct aiFace const* assimp_mesh_face = &assimp_mesh->mFaces[face_index];

		uint32_t element_index = 0;
		while (element_index < assimp_mesh_face->mNumIndices)
		{
			std_fvector32_push(&mesh_asset->pbr_indices, assimp_mesh_face->mIndices[element_index]);

			element_index++;
		}

		face_index++;
	}

	struct aiMaterial const* assimp_material = assimp_scene->mMaterials[assimp_mesh->mMaterialIndex];
	struct aiMaterialProperty const* first_assimp_material_property = assimp_material->mProperties[0];

	VEGA_ASSERT(strcmp(first_assimp_material_property->mKey.data + 1, "mat.name") == 0, "First material property key must be \"mat.name\"");
	VEGA_ASSERT(first_assimp_material_property->mSemantic == 0, "First material property semantic must be \"0\"");
	VEGA_ASSERT(first_assimp_material_property->mIndex == 0, "First material property texture index must be \"0\"");
	VEGA_ASSERT(first_assimp_material_property->mType == aiPTI_String, "First material property type must be \"aiPTI_String\"");

	mesh_asset->material_ref = std_string_from(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, strlen(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES));

	WaitForMultipleObjects((uint32_t)std_fvector64_count(&thread_handles), (HANDLE const*)std_fvector64_buffer(&thread_handles), 1, INFINITE);

	std_fvector64_free(&thread_handles);

	mesh_asset->loaded = 1;
	mesh_asset->is_loading = 0;

	WaitForSingleObject(s_asset_loader_mutex, INFINITE);

	heap_free(mesh_asset_thread_args);

#ifdef VEGA_ASSET_LOADER_VERBOSE_ASYNC
	printf("Mesh loaded %s\n", std_string_buffer(&mesh_asset->file_path));
#endif // VEGA_ASSET_LOADER_VERBOSE_ASYNC

	ReleaseMutex(s_asset_loader_mutex);

	TRACY_ZONE_END

	return 0;
}
