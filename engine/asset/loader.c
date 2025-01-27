#include <assert.h>
#include <stdio.h>
#include <string.h>

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
#include <vega/engine/asset/material.h>
#include <vega/engine/asset/mesh.h>
#include <vega/engine/asset/model.h>
#include <vega/engine/asset/texture.h>

#include <vega/engine/renderer/vertex.h>

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

static LRESULT WINAPI asset_loader_load_physical_model_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_physical_texture_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_virtual_material_thread(PVOID user_param);
static LRESULT WINAPI asset_loader_load_virtual_mesh_thread(PVOID user_param);

static HANDLE s_asset_mutex_handle = 0;

void asset_loader_alloc(void)
{
	TRACY_ZONE_BEGIN

	s_asset_mutex_handle = CreateMutexA(NULL, 0, NULL);

	TRACY_ZONE_END
}
void asset_loader_free(void)
{
	TRACY_ZONE_BEGIN

	CloseHandle(s_asset_mutex_handle);

	TRACY_ZONE_END
}
asset_t* asset_loader_load_model(asset_t* parent, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path)
{
	TRACY_ZONE_BEGIN

	asset_t* asset = (asset_t*)heap_alloc(sizeof(asset_t));
	memset(asset, 0, sizeof(asset_t));

	asset->parent = parent;
	asset->children = std_vector_alloc(sizeof(asset_t*));
	asset->buffer_type = ASSET_BUFFER_TYPE_MODEL;
	asset->buffer = heap_alloc(sizeof(asset_model_t));
	asset->name = std_string_from(file_stem, strlen(file_stem));
	asset->file_stem = std_string_from(file_stem, strlen(file_stem));
	asset->file_ext = std_string_from(file_ext, strlen(file_ext));
	asset->file_root = std_string_from(file_root, strlen(file_root));
	asset->file_path = std_string_from(file_path, strlen(file_path));
	asset->loaded = 0;
	asset->is_loading = 1;

	if (parent)
	{
		WaitForSingleObject(s_asset_mutex_handle, INFINITE);
		std_vector_push(&parent->children, &asset);
		ReleaseMutex(s_asset_mutex_handle);
	}

	asset_model_thread_args_t* args = (asset_model_thread_args_t*)heap_alloc(sizeof(asset_model_thread_args_t));

	args->asset = asset;

	asset->thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_physical_model_thread, args, 0, NULL);

	TRACY_ZONE_END

	return asset;
}
asset_t* asset_loader_load_texture(asset_t* parent, char const* file_stem, char const* file_ext, char const* file_root, char const* file_path)
{
	TRACY_ZONE_BEGIN

	asset_t* asset = (asset_t*)heap_alloc(sizeof(asset_t));
	memset(asset, 0, sizeof(asset_t));

	asset->parent = parent;
	asset->children = std_vector_alloc(sizeof(asset_t*));
	asset->buffer_type = ASSET_BUFFER_TYPE_TEXTURE;
	asset->buffer = heap_alloc(sizeof(asset_texture_t));
	asset->name = std_string_from(file_stem, strlen(file_stem));
	asset->file_stem = std_string_from(file_stem, strlen(file_stem));
	asset->file_ext = std_string_from(file_ext, strlen(file_ext));
	asset->file_root = std_string_from(file_root, strlen(file_root));
	asset->file_path = std_string_from(file_path, strlen(file_path));
	asset->loaded = 0;
	asset->is_loading = 1;

	if (parent)
	{
		WaitForSingleObject(s_asset_mutex_handle, INFINITE);
		std_vector_push(&parent->children, &asset);
		ReleaseMutex(s_asset_mutex_handle);
	}

	asset_texture_thread_args_t* args = (asset_texture_thread_args_t*)heap_alloc(sizeof(asset_texture_thread_args_t));

	args->asset = asset;

	asset->thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_physical_texture_thread, args, 0, NULL);

	TRACY_ZONE_END

	return asset;
}
void asset_loader_load_virtual_materials(asset_t* parent, struct aiScene const* assimp_scene)
{
	TRACY_ZONE_BEGIN

	uint32_t material_index = 0;
	while (material_index < assimp_scene->mNumMaterials)
	{
		struct aiMaterial const* assimp_material = assimp_scene->mMaterials[material_index];
		struct aiMaterialProperty const* first_assimp_material_property = assimp_material->mProperties[0];

		VEGA_ASSERT(strcmp(first_assimp_material_property->mKey.data + 1, "mat.name") == 0, "First material property key must be \"mat.name\"");
		VEGA_ASSERT(first_assimp_material_property->mSemantic == 0, "First material property semantic must be \"0\"");
		VEGA_ASSERT(first_assimp_material_property->mIndex == 0, "First material property texture index must be \"0\"");
		VEGA_ASSERT(first_assimp_material_property->mType == aiPTI_String, "First material property type must be \"aiPTI_String\"");

		asset_t* asset = (asset_t*)heap_alloc(sizeof(asset_t));
		memset(asset, 0, sizeof(asset_t));

		asset->parent = parent;
		asset->children = std_vector_alloc(sizeof(asset_t*));
		asset->buffer_type = ASSET_BUFFER_TYPE_MATERIAL;
		asset->buffer = heap_alloc(sizeof(asset_material_t));
		asset->name = std_string_from(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, strlen(first_assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES));
		asset->file_stem = std_string_alloc();
		asset->file_ext = std_string_alloc();
		asset->file_root = std_string_alloc();
		asset->file_path = std_string_alloc();
		asset->loaded = 0;
		asset->is_loading = 1;

		if (parent)
		{
			WaitForSingleObject(s_asset_mutex_handle, INFINITE);
			std_vector_push(&parent->children, &asset);
			ReleaseMutex(s_asset_mutex_handle);
		}

		asset_material_thread_args_t* args = (asset_material_thread_args_t*)heap_alloc(sizeof(asset_material_thread_args_t));

		args->asset = asset;
		args->assimp_scene = assimp_scene;
		args->material_index = material_index;

		asset->thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_virtual_material_thread, args, 0, NULL);

		material_index++;
	}

	TRACY_ZONE_END
}
void asset_loader_load_virtual_meshes(asset_t* parent, struct aiScene const* assimp_scene)
{
	TRACY_ZONE_BEGIN

	uint32_t mesh_index = 0;
	while (mesh_index < assimp_scene->mNumMeshes)
	{
		struct aiMesh const* assimp_mesh = assimp_scene->mMeshes[mesh_index];

		asset_t* asset = (asset_t*)heap_alloc(sizeof(asset_t));
		memset(asset, 0, sizeof(asset_t));

		asset->parent = parent;
		asset->children = std_vector_alloc(sizeof(asset_t*));
		asset->buffer_type = ASSET_BUFFER_TYPE_MESH;
		asset->buffer = heap_alloc(sizeof(asset_mesh_t));
		asset->name = std_string_from(assimp_mesh->mName.data, assimp_mesh->mName.length);
		asset->file_stem = std_string_alloc();
		asset->file_ext = std_string_alloc();
		asset->file_root = std_string_alloc();
		asset->file_path = std_string_alloc();
		asset->loaded = 0;
		asset->is_loading = 1;

		if (parent)
		{
			WaitForSingleObject(s_asset_mutex_handle, INFINITE);
			std_vector_push(&parent->children, &asset);
			ReleaseMutex(s_asset_mutex_handle);
		}

		asset_mesh_thread_args_t* args = (asset_mesh_thread_args_t*)heap_alloc(sizeof(asset_mesh_thread_args_t));

		args->asset = asset;
		args->assimp_scene = assimp_scene;
		args->mesh_index = mesh_index;

		asset->thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)asset_loader_load_virtual_mesh_thread, args, 0, NULL);

		mesh_index++;
	}

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
void asset_loader_collect_thread_handles_recursive(asset_t* parent, vector_t* assets)
{
	TRACY_ZONE_BEGIN

	uint64_t child_index = 0;
	uint64_t child_count = std_vector_count(&parent->children);
	while (child_index < child_count)
	{
		asset_t* child = *(asset_t**)std_vector_at(&parent->children, child_index);

		if (child->thread_handle)
		{
			std_vector_push(assets, &child->thread_handle);
		}

		asset_loader_collect_thread_handles_recursive(child, assets);

		child_index++;
	}

	TRACY_ZONE_END
}
void asset_loader_wait_sub_assets(asset_t* asset)
{
	TRACY_ZONE_BEGIN

	vector_t thread_handles = std_vector_alloc(sizeof(void*));

	asset_loader_collect_thread_handles_recursive(asset, &thread_handles);

	WaitForMultipleObjects((uint32_t)std_vector_count(&thread_handles), std_vector_buffer(&thread_handles), 1, INFINITE);

	std_vector_free(&thread_handles);

	TRACY_ZONE_END
}
static LRESULT WINAPI asset_loader_load_physical_model_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Model")
	TRACY_ZONE_BEGIN

	asset_model_thread_args_t* args = (asset_model_thread_args_t*)user_param;
	asset_t* asset = (asset_t*)args->asset;
	asset_model_t* asset_model = (asset_model_t*)asset->buffer;

	memset(asset_model, 0, sizeof(asset_model_t));

	uint32_t flags =
		aiProcess_ValidateDataStructure |
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PopulateArmatureData;

	struct aiScene const* assimp_scene = aiImportFile(std_string_buffer(&asset->file_path), flags);

	if (assimp_scene)
	{
		if (assimp_scene->mRootNode && (assimp_scene->mFlags & ~(AI_SCENE_FLAGS_INCOMPLETE)))
		{
			asset_loader_load_virtual_materials(asset, assimp_scene);
			asset_loader_load_virtual_meshes(asset, assimp_scene);
		}

		asset_loader_wait_sub_assets(asset);

		aiReleaseImport(assimp_scene);
	}

	asset->loaded = 1;
	asset->is_loading = 0;

	WaitForSingleObject(s_asset_mutex_handle, INFINITE);
	printf("Loaded Model %s\n", std_string_buffer(&asset->name));
	ReleaseMutex(s_asset_mutex_handle);

	heap_free(args);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_physical_texture_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Texture")
	TRACY_ZONE_BEGIN

	asset_texture_thread_args_t* args = (asset_texture_thread_args_t*)user_param;
	asset_t* asset = (asset_t*)args->asset;
	asset_texture_t* asset_texture = (asset_texture_t*)asset->buffer;

	memset(asset_texture, 0, sizeof(asset_texture_t));

	asset_texture->buffer = stbi_load(std_string_buffer(&asset->file_path), &asset_texture->width, &asset_texture->height, &asset_texture->channels, 0);

	asset->loaded = 1;
	asset->is_loading = 0;

	WaitForSingleObject(s_asset_mutex_handle, INFINITE);
	printf("Loaded Texture %s\n", std_string_buffer(&asset->name));
	ReleaseMutex(s_asset_mutex_handle);

	heap_free(args);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_virtual_material_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Material")
	TRACY_ZONE_BEGIN

	asset_material_thread_args_t* args = (asset_material_thread_args_t*)user_param;
	asset_t* asset = (asset_t*)args->asset;
	asset_material_t* asset_material = (asset_material_t*)asset->buffer;

	memset(asset_material, 0, sizeof(asset_material_t));

	asset_material->properties = std_map_alloc();

	struct aiMaterial const* assimp_material = args->assimp_scene->mMaterials[args->material_index];

	uint32_t property_index = 1;
	while (property_index < assimp_material->mNumProperties)
	{
		struct aiMaterialProperty const* assimp_material_property = assimp_material->mProperties[property_index];

		asset_material_property_t asset_material_property;
		memset(&asset_material_property, 0, sizeof(asset_material_property_t));

		switch (assimp_material_property->mType)
		{
			case aiPTI_Float:
			{
				double value = *((float*)assimp_material_property->mData);

				asset_material_property.buffer_type = ASSET_MATERIAL_PROPERTY_TYPE_DOUBLE;
				asset_material_property.buffer = heap_alloc(sizeof(double));

				memcpy(asset_material_property.buffer, &value, sizeof(double));

				break;
			}
			case aiPTI_Double:
			{
				double value = *((double*)assimp_material_property->mData);

				asset_material_property.buffer_type = ASSET_MATERIAL_PROPERTY_TYPE_DOUBLE;
				asset_material_property.buffer = heap_alloc(sizeof(double));

				memcpy(asset_material_property.buffer, &value, sizeof(double));

				break;
			}
			case aiPTI_String:
			{
				if ((assimp_material_property->mSemantic > aiTextureType_NONE) && (assimp_material_property->mSemantic <= AI_TEXTURE_TYPE_MAX) && (assimp_material_property->mSemantic != aiTextureType_UNKNOWN))
				{
					string_t file_path = std_string_from(std_string_buffer(&asset->parent->file_root), std_string_size(&asset->parent->file_root));

					if (asset->parent)
					{
						std_string_append(&file_path, "\\", 1);
						std_string_append(&file_path, assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, assimp_material_property->mDataLength - ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES);
					}

					string_t file_stem = fs_resolve_file_stem_from_path(std_string_buffer(&file_path), std_string_size(&file_path));
					string_t file_ext = fs_resolve_file_ext_from_path(std_string_buffer(&file_path), std_string_size(&file_path));

					asset_loader_load_texture(asset->parent, std_string_buffer(&file_stem), std_string_buffer(&file_ext), std_string_buffer(&asset->parent->file_root), std_string_buffer(&file_path));

					std_string_free(&file_stem);
					std_string_free(&file_ext);
					std_string_free(&file_path);

					break;
				}

				asset_material_property.buffer_type = ASSET_MATERIAL_PROPERTY_TYPE_STRING;
				asset_material_property.buffer = heap_alloc(assimp_material_property->mDataLength - ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES);

				memcpy(asset_material_property.buffer, &assimp_material_property->mData + ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES, assimp_material_property->mDataLength - ASSIMP_MATERIAL_PROPERTY_DATA_ERROR_BYTES);

				break;
			}
			case aiPTI_Integer:
			{
				int32_t value = *((int32_t*)assimp_material_property->mData);

				asset_material_property.buffer_type = ASSET_MATERIAL_PROPERTY_TYPE_INTEGER;
				asset_material_property.buffer = heap_alloc(sizeof(int32_t));

				memcpy(asset_material_property.buffer, &value, sizeof(int32_t));

				break;
			}
			case aiPTI_Buffer:
			{
				asset_material_property.buffer_type = ASSET_MATERIAL_PROPERTY_TYPE_BUFFER;
				asset_material_property.buffer = heap_alloc(assimp_material_property->mDataLength);

				memcpy(asset_material_property.buffer, &assimp_material_property->mData, assimp_material_property->mDataLength);

				break;
			}
		}

		std_map_insert(&asset_material->properties, assimp_material_property->mKey.data, assimp_material_property->mKey.length, &asset_material_property, sizeof(asset_material_property_t));

		property_index++;
	}

	asset->loaded = 1;
	asset->is_loading = 0;

	WaitForSingleObject(s_asset_mutex_handle, INFINITE);
	printf("Loaded Material %s\n", std_string_buffer(&asset->name));
	ReleaseMutex(s_asset_mutex_handle);

	heap_free(args);

	TRACY_ZONE_END

	return 0;
}
static LRESULT WINAPI asset_loader_load_virtual_mesh_thread(PVOID user_param)
{
	TRACY_THREAD_NAME("Load Mesh")
	TRACY_ZONE_BEGIN

	asset_mesh_thread_args_t* args = (asset_mesh_thread_args_t*)user_param;
	asset_t* asset = (asset_t*)args->asset;
	asset_mesh_t* asset_mesh = (asset_mesh_t*)asset->buffer;

	memset(asset_mesh, 0, sizeof(asset_mesh_t));

	asset_mesh->pbr_vertices = std_vector_alloc(sizeof(pbr_vertex_t));
	asset_mesh->pbr_indices = std_fvector32_alloc();

	struct aiMesh const* assimp_mesh = args->assimp_scene->mMeshes[args->mesh_index];

	uint32_t face_index = 0;
	while (face_index < assimp_mesh->mNumFaces)
	{
		struct aiFace const* assimp_mesh_face = &assimp_mesh->mFaces[face_index];

		pbr_vertex_t pbr_vertex;

		uint32_t element_index = 0;
		while (element_index < assimp_mesh_face->mNumIndices)
		{
			uint32_t vertex_index = assimp_mesh_face->mIndices[element_index];

			memset(&pbr_vertex, 0, sizeof(pbr_vertex_t));

			if (assimp_mesh->mVertices)
			{
				pbr_vertex.position.x = assimp_mesh->mVertices[vertex_index].x;
				pbr_vertex.position.y = assimp_mesh->mVertices[vertex_index].y;
				pbr_vertex.position.z = assimp_mesh->mVertices[vertex_index].z;
				pbr_vertex.position.w = 0.0;
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
				pbr_vertex.texture_channel_0.x = assimp_mesh->mTextureCoords[0][vertex_index].x;
				pbr_vertex.texture_channel_0.y = assimp_mesh->mTextureCoords[0][vertex_index].y;
			}

			std_vector_push(&asset_mesh->pbr_vertices, &pbr_vertex);
			std_fvector32_push(&asset_mesh->pbr_indices, vertex_index);

			element_index++;
		}

		face_index++;
	}

	asset->loaded = 1;
	asset->is_loading = 0;

	WaitForSingleObject(s_asset_mutex_handle, INFINITE);
	printf("Loaded Mesh %s\n", std_string_buffer(&asset->name));
	ReleaseMutex(s_asset_mutex_handle);

	heap_free(args);

	TRACY_ZONE_END

	return 0;
}
