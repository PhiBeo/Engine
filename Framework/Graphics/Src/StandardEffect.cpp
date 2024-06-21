#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "AnimationUtil.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void SpringEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(Math::Matrix4));

	mVertexShader.Initialize<Vertex>(filePath);
	mPixShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SpringEngine::Graphics::StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixShader.Terminate();
	mVertexShader.Terminate();
	mBoneTransformBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void SpringEngine::Graphics::StandardEffect::Begin()
{
	ASSERT(mCamera != nullptr, "StandardEffect: no camera set");
	ASSERT(mDirectionalLight != nullptr, "StandardEffect: no light set");
	mVertexShader.Bind();
	mPixShader.Bind();

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mBoneTransformBuffer.BindVS(4);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
}

void SpringEngine::Graphics::StandardEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void SpringEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	SettingsData settingsData;
	settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
	settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
	settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
	settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0;
	settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
	settingsData.useSkinning = mSettingsData.useSkinning > 0 && renderObject.skeleton != nullptr;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	settingsData.depthBias = mSettingsData.depthBias;

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	data.world = Transpose(matWorld);
	data.viewPosition = mCamera->GetPosition();
	if (settingsData.useShadowMap)
	{
		const auto& matLightView = mLightCamera->GetViewMatrix();
		const auto& matLightProj = mLightCamera->GetProjectionMatrix();
		data.lwvp = Transpose(matWorld * matLightView * matLightProj);

		mShadowMap->BindPS(4);
	}

	if (settingsData.useSkinning > 0)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(renderObject.modelId, boneTransforms, renderObject.animator);
		AnimationUtil::ApplyBoneOffset(renderObject.modelId, boneTransforms);
		for (Math::Matrix4& m : boneTransforms)
		{
			m = Transpose(m);
		}
		boneTransforms.resize(MaxBoneCount);
		mBoneTransformBuffer.Update(boneTransforms.data());
	}

	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settingsData);
	
	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.normalMapId, 1);
	tm->BindPS(renderObject.specMapId, 2);
	tm->BindVS(renderObject.bumpMapId, 3);
	renderObject.meshBuffer.Render();
}

void SpringEngine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void SpringEngine::Graphics::StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void SpringEngine::Graphics::StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void SpringEngine::Graphics::StandardEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
}

void SpringEngine::Graphics::StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuse = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuse", &useDiffuse))
		{
			mSettingsData.useDiffuseMap = useDiffuse ? 1 : 0;
		}
		bool useNormal = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormal", &useNormal))
		{
			mSettingsData.useNormalMap = useNormal ? 1 : 0;
		}
		bool useSpec = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("useSpec", &useSpec))
		{
			mSettingsData.useSpecMap = useSpec ? 1 : 0;
		}
		bool useBump = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("useBump", &useBump))
		{
			mSettingsData.useBumpMap = useBump ? 1 : 0;
		}
		bool useShadow = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("useShadow", &useShadow))
		{
			mSettingsData.useShadowMap = useShadow ? 1 : 0;
		}
		bool useSkinning = mSettingsData.useSkinning > 0;
		if (ImGui::Checkbox("useSkinning", &useSkinning))
		{
			mSettingsData.useSkinning = useSkinning ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.1f, 0.0f, 2.0f);
		ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
	}
}
