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

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionLight(mDirectionalLight);

	ModelId modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character/Paladin/Paladin.model");
	mPaladin = CreateRenderGroup(modelId);

	modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character/Swat/Swat.model");
	mSwat = CreateRenderGroup(modelId);

	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_200x200.raw", 10.0f);

	mGround.meshBuffer.Initialize(mTerrain.GetMesh());
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_texture.jpg");
	mGround.normalMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_normal.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_spec.jpg");
	mGround.bumpMapId = TextureManager::Get()->LoadTexture(L"terrain/grass_2048.jpg");

	Math::Vector3 spawnPos = Vector3::Zero;
	spawnPos.x = 10.f;
	spawnPos.z = 10.f;
	spawnPos.y = mTerrain.GetHeight(spawnPos);
	SetRenderGroundPosition(mPaladin, spawnPos);

	spawnPos.x = 15.f;
	spawnPos.z = 5.f;
	spawnPos.y = mTerrain.GetHeight(spawnPos);
	SetRenderGroundPosition(mSwat, spawnPos);
}

void GameState::Terminate()
{
	mGround.Terminate();
	CleanupRenderGroup(mSwat);
	CleanupRenderGroup(mPaladin);
	mShadowEffect.Terminate();
	mTerrainEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	mShadowEffect.Begin();
		DrawRenderGroup(mShadowEffect, mSwat);
		DrawRenderGroup(mShadowEffect, mPaladin);
	mShadowEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();

	mStandardEffect.Begin();
		DrawRenderGroup(mStandardEffect, mSwat);
		DrawRenderGroup(mStandardEffect, mPaladin);
	mStandardEffect.End();
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
	
	mStandardEffect.DebugUI();
	mTerrainEffect.DebugUI();
	mShadowEffect.DebugUI();

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