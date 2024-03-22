#pragma once

#include "BlendState.h"
#include "ConstantBuffer.h"
#include "PixelShader.h"`
#include "VertexShader.h"
#include "Sampler.h"

namespace SpringEngine::Graphics
{
	class Camera;
	class RenderObject;

	class BillBoardEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();
		void Render(const RenderObject& renderObject);
		void SetCamera(const Camera& camera);

	private:
		const Camera* mCamera = nullptr;

		ConstantBuffer mConstantBuffer;
		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		BlendState mBlendState;
	};
}