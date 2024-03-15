#pragma once

#include "Material.h"
#include "MeshBuffer.h"
#include "Transform.h"
#include "TextureManager.h"

namespace SpringEngine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate()
		{
			meshBuffer.Terminate();
			diffuseMapId = 0;
			normalMapId = 0;
			specMapId = 0;
			bumpMapId = 0;
		}

		Transform transform;
		MeshBuffer meshBuffer;

		Material material;
		TextureId diffuseMapId;
		TextureId normalMapId;
		TextureId specMapId;
		TextureId bumpMapId;
	};
}