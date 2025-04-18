#pragma once

#include <SpringEngine/Inc/SpringEngine.h>

class GameState : public SpringEngine::AppState
{
public:
	~GameState() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(const float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCameraControl(float deltaTime);

	void ChangeCameraPhase();

	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::DirectionalLight mDirectionalLight;
	SpringEngine::Graphics::StandardEffect mStandardEffect;

	SpringEngine::Graphics::RenderObject mSky;
	SpringEngine::Graphics::RenderObject mGround;
	SpringEngine::Graphics::ModelId mMaleCharacterId;
	SpringEngine::Graphics::RenderGroup mMaleCharacter;
	SpringEngine::Graphics::Animator mMaleAnimator;

	SpringEngine::Graphics::ModelId mFemaleCharacterId;
	SpringEngine::Graphics::RenderGroup mFemaleCharacter;
	SpringEngine::Graphics::Animator mFemaleAnimator;

	SpringEngine::Graphics::ParticleEffect mParticleEffect;
	SpringEngine::ParticleSystem mFlair1;
	SpringEngine::ParticleSystem mFlair2;

	SpringEngine::Audio::SoundId mSoundId;



	SpringEngine::Graphics::Animation mCameraAnimation;
	float mAnimationTime = 0.0f;

	bool mDrawSkeleton = false;
	float mTime = 0.0f;
	int mCameraPhase = 0;
	int mLastframePhase = 0;
};