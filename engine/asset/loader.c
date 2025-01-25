#include <string.h>

#include <assimp/cfileio.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vega/engine/asset/loader.h>

#ifndef TRACY_ZONE_BEGIN
	#define TRACY_ZONE_BEGIN TracyCZoneC(ctx, TRACY_COLOR_BLUE, 1U)
#endif // TRACY_ZONE_BEGIN

#ifndef TRACY_ZONE_END
	#define TRACY_ZONE_END TracyCZoneEnd(ctx)
#endif // TRACY_ZONE_END

asset_t asset_loader_load(char const* file_path)
{
	TRACY_ZONE_BEGIN

	asset_t asset;
	memset(&asset, 0, sizeof(asset_t));

	const struct aiScene* scene = aiImportFile(file_path, aiProcess_ValidateDataStructure | aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_PopulateArmatureData);

	if (scene)
	{
		if (scene->mRootNode && (scene->mFlags & ~(AI_SCENE_FLAGS_INCOMPLETE)))
		{

		}

		aiReleaseImport(scene);
	}

	TRACY_ZONE_END

	return asset;
}
