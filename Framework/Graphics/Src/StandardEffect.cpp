#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void SpringEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	mVertexShader.Initialize<Vertex>(filePath);
	mPixShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SpringEngine::Graphics::StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixShader.Terminate();
	mVertexShader.Terminate();
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

	mSampler.BindPS(0);
}

void SpringEngine::Graphics::StandardEffect::End()
{
}

void SpringEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	TransformData data;
	data.wvp = Transpose(matWorld * matView * matProj);
	data.world = Transpose(matWorld);
	data.viewPosition = mCamera->GetPosition();

	SettingsData settingsData;
	settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
	settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
	settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
	settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0;
	settingsData.bumpWeight = mSettingsData.bumpWeight;

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

void SpringEngine::Graphics::StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
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
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.1f, 0.0f, 2.0f);
	}
}
