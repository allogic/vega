#include <stdio.h>
#include <string.h>

#include <assimp/cfileio.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vega/engine/asset/loader.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_BLUE, 1U);
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx);
#endif // TRACY_ZONE_END

asset_t asset_loader_load(char const* file_path)
{
	TRACY_ZONE_BEGIN

	asset_t asset;
	memset(&asset, 0, sizeof(asset_t));

	uint32_t flags =
		aiProcess_ValidateDataStructure |
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_PopulateArmatureData;

	const struct aiScene* scene = aiImportFile(file_path, flags);

	if (scene)
	{
		if (scene->mRootNode && (scene->mFlags & ~(AI_SCENE_FLAGS_INCOMPLETE)))
		{
			asset_print_stats_raw(scene);
		}

		aiReleaseImport(scene);
	}

	TRACY_ZONE_END

	return asset;
}
void asset_load_materials(struct aiScene const* scene)
{
	TRACY_ZONE_BEGIN

	uint32_t material_index = 0;
	while (material_index < scene->mNumMaterials)
	{
		struct aiMaterial const* assimpMaterial = scene->mMaterials[material_index];

		//mAsyncImageResources[resourceIndex + 0] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_DIFFUSE); });
		//mAsyncImageResources[resourceIndex + 1] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_SPECULAR); });
		//mAsyncImageResources[resourceIndex + 2] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_AMBIENT); });
		//mAsyncImageResources[resourceIndex + 3] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_EMISSIVE); });
		//mAsyncImageResources[resourceIndex + 4] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_HEIGHT); });
		//mAsyncImageResources[resourceIndex + 5] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_NORMALS); });
		//mAsyncImageResources[resourceIndex + 6] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_SHININESS); });
		//mAsyncImageResources[resourceIndex + 7] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_OPACITY); });
		//mAsyncImageResources[resourceIndex + 8] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_DISPLACEMENT); });
		//mAsyncImageResources[resourceIndex + 9] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_LIGHTMAP); });
		//mAsyncImageResources[resourceIndex + 10] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_REFLECTION); });
		//mAsyncImageResources[resourceIndex + 11] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_BASE_COLOR); });
		//mAsyncImageResources[resourceIndex + 12] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_NORMAL_CAMERA); });
		//mAsyncImageResources[resourceIndex + 13] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_EMISSIVE); }); // TODO
		//mAsyncImageResources[resourceIndex + 14] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_METALNESS); });
		//mAsyncImageResources[resourceIndex + 15] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_DIFFUSE_ROUGHNESS); });
		//mAsyncImageResources[resourceIndex + 16] = std::async(std::launch::async, [=]() { return LoadImageOfType(Scene, assimpMaterial, aiTextureType_AMBIENT_OCCLUSION); });

		material_index++;
	}

	TRACY_ZONE_END
}
void asset_load_meshes(struct aiScene const* scene)
{
	TRACY_ZONE_BEGIN



	TRACY_ZONE_END
}
void asset_print_stats_raw(struct aiScene const* scene)
{
	TRACY_ZONE_BEGIN

	uint32_t material_index = 0;
	while (material_index < scene->mNumMaterials)
	{
		struct aiMaterial const* assimp_material = scene->mMaterials[material_index];

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
					uint32_t data_index = 1;
					while (data_index < assimp_material_property->mDataLength)
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
					uint32_t data_index = 1;
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
	while (texture_index < scene->mNumTextures)
	{
		struct aiTexture const* assimp_texture = scene->mTextures[texture_index];

		printf("Texture_%u\n", texture_index);
		printf("\tFile Name %s\n", assimp_texture->mFilename.data);
		printf("\tWidth %u\n", assimp_texture->mWidth);
		printf("\tHeight %u\n", assimp_texture->mHeight);
		printf("\n");

		texture_index++;
	}

	uint32_t mesh_index = 0;
	while (mesh_index < scene->mNumMeshes)
	{
		struct aiMesh const* assimp_mesh = scene->mMeshes[mesh_index];

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
	while (animation_index < scene->mNumAnimations)
	{
		struct aiAnimation const* assimp_animation = scene->mAnimations[animation_index];

		printf("Animation_%u\n", animation_index);
		printf("\tName %s\n", assimp_animation->mName.data);
		printf("\tDuration %f\n", assimp_animation->mDuration);
		printf("\tTicks Per Second %f\n", assimp_animation->mTicksPerSecond);
		printf("\n");

		animation_index++;
	}

	uint32_t skeleton_index = 0;
	while (skeleton_index < scene->mNumSkeletons)
	{
		struct aiSkeleton const* assimp_skeleton = scene->mSkeletons[skeleton_index];

		printf("Skeleton_%u\n", skeleton_index);
		printf("\tName %s\n", assimp_skeleton->mName.data);
		printf("\tBones %u\n", assimp_skeleton->mNumBones);
		printf("\n");

		skeleton_index++;
	}

	TRACY_ZONE_END
}
