#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace SpringEngine::Graphics
{
	class Camera;
	class RenderObject;
	class Texture;

	class StandardEffect
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		
		void SetCamera(const Camera& camera);

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;

		TransformBuffer mTransformBuffer;
		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixShader;

		const Camera* mCamera = nullptr;
	};
}