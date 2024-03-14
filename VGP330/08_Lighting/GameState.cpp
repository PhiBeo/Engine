#include "GameState.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
	mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

	mSphere = MeshBuilder::CreateSphere(60, 60, 1.0f);

	mEarth.meshBuffer.Initialize(mSphere);
	mEarth.mDiffuseTexture.Initialize(L"../../Assets/Textures/earth.jpg");

	mEffect.Initialize();
	mEffect.SetCamera(mCamera);
}

void GameState::Terminate()
{
	mEffect.Terminate();
	mEarth.Terminate();
}

void GameState::Update(const float deltaTime)
{

}

void GameState::Render()
{
	mEffect.Begin();
	mEffect.Render(mEarth);
	mEffect.End();
}

void GameState::DebugUI()
{

}