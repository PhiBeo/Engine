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
	SpringEngine::Graphics::Camera mCamera;
	SpringEngine::Graphics::Mesh mSphere;
	SpringEngine::Graphics::RenderObject mEarth;
	SpringEngine::Graphics::SimpleEffect mEffect;
};