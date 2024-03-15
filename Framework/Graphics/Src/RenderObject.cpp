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

RenderGroup Graphics::CreateRenderGroup(const Model& model)
{
	RenderGroup renderGroup;
	renderGroup.resize(model.meshData.size());
	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderGroup.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
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
