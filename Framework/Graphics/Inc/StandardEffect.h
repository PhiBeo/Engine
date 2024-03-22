#pragma once

#include "ConstantBuffer.h"
#include "LightTypes.h"
#include "Material.h"
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
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionLight);
		void SetShadowMap(const Texture& shadowMap);

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 lwvp;
			Math::Matrix4 world;
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useNormalMap = 1;
			int useSpecMap = 1;
			int useBumpMap = 1;
			int useShadowMap = 1;
			float bumpWeight = 0.02f;
			float depthBias = 0.0f;
			float padding[1];
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;
		Sampler mSampler;
		VertexShader mVertexShader;
		PixelShader mPixShader;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}