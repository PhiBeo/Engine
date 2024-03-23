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
	mOlivia = CreateRenderGroup(modelId);

	modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character/Swat/Swat.model");
	mAmy = CreateRenderGroup(modelId);

	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_200x200.raw", 10.0f);

	mGround.meshBuffer.Initialize(mTerrain.GetMesh());
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"terrain/dirt_seamless.jpg");
	mGround.normalMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_normal.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"mountain/mountain_spec.jpg");
	mGround.bumpMapId = TextureManager::Get()->LoadTexture(L"terrain/grass_2048.jpg");

	mSwatPosition.x = 10.f;
	mSwatPosition.z = 10.f;
	mSwatPosition.y = mTerrain.GetHeight(mSwatPosition);
	SetRenderGroundPosition(mOlivia, mSwatPosition);

	mPaladinPosition.x = 15.f;
	mPaladinPosition.z = 5.f;
	mPaladinPosition.y = mTerrain.GetHeight(mPaladinPosition);
	SetRenderGroundPosition(mAmy, mPaladinPosition);
}

void GameState::Terminate()
{
	mGround.Terminate();
	CleanupRenderGroup(mAmy);
	CleanupRenderGroup(mOlivia);
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
		DrawRenderGroup(mShadowEffect, mAmy);
		DrawRenderGroup(mShadowEffect, mOlivia);
	mShadowEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();

	mStandardEffect.Begin();
		DrawRenderGroup(mStandardEffect, mAmy);
		DrawRenderGroup(mStandardEffect, mOlivia);
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

	Vector2 terrainSize;
	terrainSize.x = mTerrain.GetWidth();
	terrainSize.y = mTerrain.GetHeight();

	if (ImGui::CollapsingHeader("PaladinPosition", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("PosX##Paladin", &mPaladinPosition.x, 0.1f, 0.0f, terrainSize.x))
		{
			mPaladinPosition.y = mTerrain.GetHeight(mPaladinPosition);
			SetRenderGroundPosition(mOlivia, mPaladinPosition);
		}
		if (ImGui::DragFloat("PosZ##Paladin", &mPaladinPosition.z, 0.1f, 0.0f, terrainSize.y))
		{
			mPaladinPosition.y = mTerrain.GetHeight(mPaladinPosition);
			SetRenderGroundPosition(mOlivia, mPaladinPosition);
		}
	}

	if (ImGui::CollapsingHeader("SwatPosition", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("PosX##Swat", &mSwatPosition.x, 0.1f, 0.0f, terrainSize.x))
		{
			mSwatPosition.y = mTerrain.GetHeight(mSwatPosition);
			SetRenderGroundPosition(mAmy, mSwatPosition);
		}
		if (ImGui::DragFloat("PosZ##Swat", &mSwatPosition.z, 0.1f, 0.0f, terrainSize.y))
		{
			mSwatPosition.y = mTerrain.GetHeight(mSwatPosition);
			SetRenderGroundPosition(mAmy, mSwatPosition);
		}
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