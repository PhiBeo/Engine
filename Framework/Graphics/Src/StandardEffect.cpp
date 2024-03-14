#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void SpringEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& filePath)
{
	mTransformBuffer.Initialize();

	mVertexShader.Initialize<Vertex>(filePath);
	mPixShader.Initialize(filePath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void SpringEngine::Graphics::StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixShader.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
}

void SpringEngine::Graphics::StandardEffect::Begin()
{
}

void SpringEngine::Graphics::StandardEffect::End()
{
}

void SpringEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
}

void SpringEngine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void SpringEngine::Graphics::StandardEffect::DebugUI()
{
}
