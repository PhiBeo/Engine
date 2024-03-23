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

	mPostProcessingEffect.Initialize(L"../../Assets/Shaders/PostProcess.fx");
	mPostProcessingEffect.SetTexture(&mRenderTarget);
	mPostProcessingEffect.SetTexture(&mGaussianBlurEffect.GetResultTexture(), 1);

	mGaussianBlurEffect.Initialize();
	mGaussianBlurEffect.SetSourceTexture(mBlurRenderTarget);

	GraphicsSystem* gs = GraphicsSystem::Get();
	const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
	mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	ModelId modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character/Paladin/Paladin.model");
	mOlivia = CreateRenderGroup(modelId);

	modelId = ModelManager::Get()->LoadModel(L"../../Assets/Models/Character/Swat/Swat.model");
	mAmy = CreateRenderGroup(modelId);

	for (auto& renderObject : mOlivia)
	{
		renderObject.transform.position.x -= 1.0f;
	}

	for (auto& renderObject : mAmy)
	{
		renderObject.transform.position.x += 1.0f;
	}

	Mesh groundMesh = MeshBuilder::CreateGroundPlane(20, 20, 1.0f);
	mGround.meshBuffer.Initialize(groundMesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"misc/concrete.jpg");

	MeshPX screenMesh = MeshBuilder::CreateScreenQuad();
	mScreenQuad.meshBuffer.Initialize(screenMesh);


}

void GameState::Terminate()
{
	mScreenQuad.Terminate();
	mGround.Terminate();
	mRenderTarget.Terminate();
	mBlurRenderTarget.Terminate();
	CleanupRenderGroup(mAmy);
	CleanupRenderGroup(mOlivia);
	mGaussianBlurEffect.Terminate();
	mPostProcessingEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	mRenderTarget.BeginRender();
		mStandardEffect.Begin();
			DrawRenderGroup(mStandardEffect, mOlivia);
			DrawRenderGroup(mStandardEffect, mAmy);
			mStandardEffect.Render(mGround);
		mStandardEffect.End();
	mRenderTarget.EndRender();

	mBlurRenderTarget.BeginRender({0.0f,0.0f,0.0f,0.0f});
		mStandardEffect.Begin();
			DrawRenderGroup(mStandardEffect, mOlivia);
		mStandardEffect.End();
	mBlurRenderTarget.EndRender();

	mGaussianBlurEffect.Begin();
		mGaussianBlurEffect.Render(mScreenQuad);
	mGaussianBlurEffect.End();

	mPostProcessingEffect.Begin();
		mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	
	mStandardEffect.DebugUI();
	mPostProcessingEffect.DebugUI();
	mGaussianBlurEffect.DebugUI();

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