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
}

void GameState::Terminate()
{
	CleanupRenderGroup(mAmy);
	CleanupRenderGroup(mOlivia);
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.f, Colors::White);
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
	DrawRenderGroup(mStandardEffect, mOlivia);
	DrawRenderGroup(mStandardEffect, mAmy);
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
	if (ImGui::CollapsingHeader("Quaternion", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool updateCharacter = false;
		if (ImGui::DragFloat("Yaw", &mYaw, 0.01f))
		{
			updateCharacter = true;
		}
		if (ImGui::DragFloat("Pitch", &mPitch, 0.01f))
		{
			updateCharacter = true;
		}
		if (ImGui::DragFloat("Roll", &mRoll, 0.01f))
		{
			updateCharacter = true;
		}

		if (updateCharacter)
		{
			Quaternion rot = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
			for (auto& r : mAmy)
			{
				r.transform.rotation = rot;
			}
		}
	}


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