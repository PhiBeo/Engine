#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
	diffuseMapId = 0;
	normalMapId = 0;
	specMapId = 0;
	bumpMapId = 0;
}

RenderGroup SpringEngine::Graphics::CreateRenderGroup(ModelId id)
{
	const Model* model = ModelManager::Get()->GetModel(id);
	ASSERT(model != nullptr, "RenderGroup: ModelId %d is not loaded", id);
	RenderGroup renderGroup = CreateRenderGroup(*model);
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.modelId = id;
	}
	return renderGroup;
}

RenderGroup Graphics::CreateRenderGroup(const Model& model)
{
	auto TryLoadTexture = [](const auto& textureName) ->TextureId
		{
			if (textureName.empty())
			{
				return 0;
			}
			return TextureManager::Get()->LoadTexture(textureName, false);
		};

	RenderGroup renderGroup;
	renderGroup.resize(model.meshData.size());
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if (meshData.materialIndex < model.materialData.size())
		{
			const Model::MaterialData& materialData = model.materialData[meshData.materialIndex];
			renderObject.material = materialData.material;
			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseMapName);
			renderObject.specMapId = TryLoadTexture(materialData.specularMapName);
			renderObject.normalMapId = TryLoadTexture(materialData.normalMapName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpMapName);
		}
	}
	return renderGroup;
}

void Graphics::CleanupRenderGroup(RenderGroup& renderGroup)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.Terminate();
	}
}

void SpringEngine::Graphics::SetRenderGroundPosition(RenderGroup& renderGroup, const Math::Vector3& position)
{
	for (RenderObject& renderObject : renderGroup)
	{
		renderObject.transform.position = position;
	}
}