#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,3.0f,-10.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);
	ParticleSystemInfo info;

	info.spawnPosition = {0.0f, 0.0f, 10.0f};
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.systemLifeTime = 99999.0f;
	info.minParticlePerEmit = 1;
	info.maxParticlePerEmit = 3;
	info.minTimeBetweenEmit = 0.5f;
	info.maxTimeBetweenEmit = 2.0f;
	info.minSpawnAngle = -10.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 10.0f * 3.141592 / 180.0f;
	info.minSpeed = 10.0f;
	info.maxSpeed = 15.0f;
	info.minLifeTime = 3.0f;
	info.maxLifeTime = 5.0f;
	info.minStartColor = Colors::White;
	info.maxStartColor = Colors::White;
	info.minEndColor = Colors::Cyan;
	info.maxEndColor = Colors::Purple;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = Math::Vector3::One;
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticle = 50;
	info.particleTextureId = TextureManager::Get()->LoadTexture("Images/bullet1.png");
	
	//mParticleSystem.Initialize(info);
	//mParticleSystem.SetCamera(mCamera);

	mFirework.Initialize(info);
	mFirework.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mFirework.Terminate();
	//mParticleSystem.Terminate();
	mParticleEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	//mParticleSystem.Update(deltaTime);
	mFirework.Update(deltaTime);
	UpdateCameraControl(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(10.f, Colors::White);
	SimpleDraw::Render(mCamera);

	mParticleEffect.Begin();
		mFirework.Render(mParticleEffect);
		//mParticleSystem.Render(mParticleEffect);
	mParticleEffect.End();
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

	//mParticleSystem.DebugUI();
	Physics::PhysicsWorld::Get()->DebugUI();

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