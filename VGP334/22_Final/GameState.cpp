#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;
using namespace SpringEngine::Input;
using namespace SpringEngine::Audio;

#define ANIMATION_MAX_TIME 45
#define CAMERA_MAX_PHASE 4

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,4.0f,-10.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f,1.0f });
	mDirectionalLight.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
	mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

	mParticleEffect.Initialize();
	mParticleEffect.SetCamera(mCamera);
	ParticleSystemInfo info;

	info.spawnPosition = Math::Vector3::Zero;
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnDelay = 0.0f;
	info.systemLifeTime = 99999.0f;
	info.minParticlePerEmit = 5;
	info.maxParticlePerEmit = 10;
	info.minTimeBetweenEmit = 0.1f;
	info.maxTimeBetweenEmit = 0.3f;
	info.minSpawnAngle = -30.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 30.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minLifeTime = 0.3f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::Orange;
	info.maxStartColor = Colors::Red;
	info.minEndColor = Colors::OrangeRed;
	info.maxEndColor = Colors::Yellow;
	info.minStartScale = Vector3::One;
	info.maxStartScale = Vector3::One;
	info.minEndScale = Vector3::One;
	info.maxEndScale = Vector3::One;
	info.maxParticle = 100;
	info.particleTextureId = TextureManager::Get()->LoadTexture("misc/Flare.png");
	info.spawnPosition = Vector3({ 3.5f,0.0f,-3.5f });

	mSoundId = SoundEffectManager::Get()->Load("Refresh.wav");

	mFlair1.Initialize(info);
	mFlair1.SetCamera(mCamera);

	info.spawnPosition = Vector3({ -3.5f,0.0f,-3.5f });
	mFlair2.Initialize(info);
	mFlair2.SetCamera(mCamera);

	Mesh ground = MeshBuilder::CreateGroundPlane(10, 10, 1.0f);
	mGround.meshBuffer.Initialize(ground);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture("Wood.jpg");

	mCharacterId = ModelManager::Get()->LoadModel("../../Assets/Models/Character/MaleDancer/MaleDancer.model");
	ModelManager::Get()->AddAnimation(mCharacterId, "../../Assets/Models/Character/MaleDancer/MaleBboy.model");
	mCharacter = CreateRenderGroup(mCharacterId);
	mCharacterAnimator.Initialize(mCharacterId);
	mCharacterAnimator.PlayAnimation(1, true);

	AnimationCallback changeCamera = [&]() {ChangeCameraPhase(); };

	mCameraAnimation = AnimationBuilder()
		.AddPositionKey({ -3.0f, 5.0f, -10.0f }, 0.0f, EaseType::EaseInOutQuad)
		.AddPositionKey({ -3.0f, 5.0f, -10.5f }, 2.5f, EaseType::Linear)
		.AddPositionKey({ 0.0f, 5.0f, -11.0f }, 5.0f, EaseType::Linear)
		.AddPositionKey({ 3.0f, 5.0f, -10.5f }, 7.5f, EaseType::Linear)
		.AddPositionKey({ 3.0f, 5.0f, -10.0f }, 10.0f, EaseType::Linear)
		.AddEventKey(changeCamera, 10.0f)
		.AddPositionKey({10.0f, 1.5f, -7.0f}, 10.1f)
		.AddPositionKey({10.0f, 1.5f, 6.0f}, 22.0f)
		.AddEventKey(changeCamera, 22.0f)
		.AddPositionKey({ 0.0f,5.0f,10.0f }, 22.1f)
		.AddPositionKey({ 0.0f,1.0f,10.0f }, 35.0f)
		.AddEventKey(changeCamera, 35.0f)
		.AddPositionKey({ 0.0f,4.0f,-10.0f }, 35.1f )
		.AddPositionKey({ 0.0f,4.0f,-10.0f }, 45.1f )
		.Build();

	mAnimationTime = 0.0f;
	mCameraPhase = 1;

	SoundEffectManager::Get()->SetVolume(mSoundId, 0.1f);
	SoundEffectManager::Get()->Play(mSoundId);
}

void GameState::Terminate()
{
	CleanupRenderGroup(mCharacter);
	mGround.Terminate();
	mFlair2.Terminate();
	mFlair1.Terminate();
	mParticleEffect.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(const float deltaTime)
{
	mTime += deltaTime;
	if (mTime > 50.0f)
	{
		mTime = 0.0f;
	}
	mFlair2.Update(deltaTime);
	mFlair1.Update(deltaTime);

	UpdateCameraControl(deltaTime);
	mCharacterAnimator.Update(deltaTime);

	float prevTime = mAnimationTime;
	mAnimationTime += deltaTime;
	while (mAnimationTime > mCameraAnimation.GetDuration())
	{
		mAnimationTime -= mCameraAnimation.GetDuration();
	}
	mCameraAnimation.PlayEvents(prevTime, mAnimationTime);

	mCamera.SetPosition(mCameraAnimation.GetTransform(mAnimationTime).position);
	Vector3 position = Vector3::Zero;

	switch (mCameraPhase)
	{
	case 1:
		mCamera.SetLookAt({ 0.0f,0.0f,0.0f });
		break;
	case 2:
		position = mCameraAnimation.GetTransform(mAnimationTime).position;
		mCamera.SetLookAt({ 0.0f,1.5f,position.z});
		break;
	case 3:
		position = mCameraAnimation.GetTransform(mAnimationTime).position;
		mCamera.SetLookAt({ 0.0f, position.y, 5.0f });
		mCamera.Yaw(-deltaTime);
		break;
	case 4:
		mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
		break;
	default:
		break;
	}
	
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);

	mStandardEffect.Begin();
	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(mCharacterId, boneTransforms, &mCharacterAnimator);
		AnimationUtil::DrawSkeleton(mCharacterId, boneTransforms);
	}
	else
	{
		DrawRenderGroup(mStandardEffect, mCharacter);
	}
		mStandardEffect.Render(mGround);
	mStandardEffect.End();

	mParticleEffect.Begin();
		mFlair1.Render(mParticleEffect);
		mFlair2.Render(mParticleEffect);
	mParticleEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Time: ");
	ImGui::SameLine();
	ImGui::Text("%0.2f", mTime);

	ImGui::Text("CurrentPhase: %d", mCameraPhase);

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

	ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
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

void GameState::ChangeCameraPhase()
{
	mCameraPhase++;
	if (mCameraPhase > CAMERA_MAX_PHASE)
	{
		mCameraPhase = 1;
	}
}


