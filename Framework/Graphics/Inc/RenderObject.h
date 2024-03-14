#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Texture.h"

namespace SpringEngine::Graphics
{
	class RenderObject
	{
	public:
		void Terminate()
		{
			meshBuffer.Terminate();
			mDiffuseTexture.Terminate();
		}

		Transform transform;
		MeshBuffer meshBuffer;
		Texture mDiffuseTexture;
	};
}