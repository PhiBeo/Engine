#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mSphere = MeshBuilder::CreateSphere(60, 60, 1.0f);

	mWall.meshBuffer.Initialize(mSphere);
	mWall.diffuseMapId = TextureManager::Get()->LoadTexture("brick_wall.jpg");
	mWall.normalMapId = TextureManager::Get()->LoadTexture("brick_wall_normal.jpg");
	mWall.specMapId = TextureManager::Get()->LoadTexture("brick_wall_spec.jpg");
	mWall.bumpMapId = TextureManager::Get()->LoadTexture("brick_wall_bump.jpg");
	mWall.material.emissive = { 0.2f, 0.2f, 0.2f, 0.2f };
	mWall.material.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mWall.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mWall.material.specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	mWall.material.power = 10.0f;

	mCelShadingEffect.Initialize(L"../../Assets/Shaders/CelShader.fx");
	mCelShadingEffect.SetCamera(mCamera);
	mCelShadingEffect.SetDirectionalLight(mDirectionalLight);

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mRenderTargetCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f,0.0f,0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, Texture::Format::RGBA_U8);
}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mCelShadingEffect.Terminate();
	mStandardEffect.Terminate();
	mWall.Terminate();
}

void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	mStandardEffect.SetCamera(mRenderTargetCamera);
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			mStandardEffect.Render(mWall);
		mStandardEffect.End();
	mRenderTarget.EndRender();

	if (!mUseCelShading)
	{
		mStandardEffect.SetCamera(mCamera);
		mStandardEffect.Begin();
		mStandardEffect.Render(mWall);
		mStandardEffect.End();
	}
	else
	{
		mCelShadingEffect.Begin();
		mCelShadingEffect.Render(mWall);
		mCelShadingEffect.End();
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::ColorEdit4("Emissive##Material", &mWall.material.emissive.r);
		ImGui::ColorEdit4("Ambient##Material", &mWall.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mWall.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mWall.material.specular.r);
		ImGui::DragFloat("Power##Material", &mWall.material.power, 1.0f, 0.0f);
	}
	ImGui::Separator();
	ImGui::Text("RenderTarget");
	ImGui::Image(mRenderTarget.GetRawData(),
		{ 128,128 },
		{ 0,0 },
		{ 1,1 },
		{ 1,1,1,1 },
		{ 1,1,1,1 });
	ImGui::Separator();
	ImGui::Checkbox("UseCelShader", &mUseCelShading);
	mStandardEffect.DebugUI();

	ImGui::End();
}

void GameState::UpdateCameraControl(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;

	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	else if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}
	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}