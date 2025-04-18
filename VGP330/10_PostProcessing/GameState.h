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

	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::DirectionalLight mDirectionalLight;
	SpringEngine::Graphics::StandardEffect mStandardEffect;
	SpringEngine::Graphics::PostProcessingEffect mPostProcessingEffect;
	SpringEngine::Graphics::GaussianBlurEffect mGaussianBlurEffect;

	SpringEngine::Graphics::RenderGroup mOlivia;
	SpringEngine::Graphics::RenderGroup mAmy;
	SpringEngine::Graphics::RenderObject mGround;
	SpringEngine::Graphics::RenderObject mScreenQuad;

	SpringEngine::Graphics::RenderTarget mRenderTarget;
	SpringEngine::Graphics::RenderTarget mBlurRenderTarget;
};