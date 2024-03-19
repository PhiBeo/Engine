#include "Precompiled.h"
#include "PostProcessingEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

namespace
{
	const char* gModeNames[] =
	{
		"None",
		"Monochrome",
		"Invert",
		"Mirror",
		"Blur",
		"Combine2",
		"MotionBlur",
		"ChromaticAberration"
	};
}

void SpringEngine::Graphics::PostProcessingEffect::Initialize(const std::filesystem::path& filePath)
{
	mVertexShader.Initialize<VertexPX>(filePath);
	mPixelShader.Initialize(filePath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);

	mPostProcessBuffer.Initialize();
}

void SpringEngine::Graphics::PostProcessingEffect::Terminate()
{
	mPostProcessBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void SpringEngine::Graphics::PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	for (uint32_t i = 0; i < mTexture.size(); ++i)
	{
		if (mTexture[i] != nullptr)
		{
			mTexture[i]->BindPS(i);
		}
	}

	PostProcessData data;
	data.mode = static_cast<int>(mMode);
	switch (mMode)
	{
	case PostProcessingEffect::Mode::None:
	case PostProcessingEffect::Mode::Monochrome:
	case PostProcessingEffect::Mode::Invert:
		break;
	case PostProcessingEffect::Mode::Mirror:
	{
		data.params0 = mMirrorScaleX;
		data.params1 = mMirrorScaleY;
	}
		break;
	case PostProcessingEffect::Mode::Blur:
	{
		GraphicsSystem* gs = GraphicsSystem::Get();
		const float screenWidth = gs->GetBackBufferWidth();
		const float screenHeight = gs->GetBackBufferHeight();
		data.params0 = mBlurStrength / screenWidth;
		data.params1 = mBlurStrength / screenHeight;
	}
		break;
	case PostProcessingEffect::Mode::Combine2:break;
	case PostProcessingEffect::Mode::MotionBlur:
	{
		GraphicsSystem* gs = GraphicsSystem::Get();
		const float screenWidth = gs->GetBackBufferWidth();
		const float screenHeight = gs->GetBackBufferHeight();
		data.params0 = mBlurStrength / screenWidth;
		data.params1 = mBlurStrength / screenHeight;
	}
		break;
	case PostProcessingEffect::Mode::ChromaticAberration:
	{
		data.params0 = mAberrationValue;
	}
		break;
	default:
		break;
	}

	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);
}

void SpringEngine::Graphics::PostProcessingEffect::End()
{
	for (uint32_t i = 0; i < mTexture.size(); ++i)
	{
		Texture::UnbindPS(i);
	}

}

void SpringEngine::Graphics::PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void SpringEngine::Graphics::PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTexture.size(), "PostProcessinEffect: invalid slot index");
	mTexture[slot] = texture;
}

void SpringEngine::Graphics::PostProcessingEffect::SetMode(Mode mode)
{
	mMode = mode;
}

void SpringEngine::Graphics::PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = static_cast<int>(mMode);
		if(ImGui::Combo("Mode", &currentMode, gModeNames, std::size(gModeNames)))
		{
			mMode = static_cast<Mode>(currentMode);
		}

		if (mMode == Mode::Mirror)
		{
			ImGui::DragFloat("MirrorScaleX", &mMirrorScaleX, 0.1f, -1.0f, 1.0f);
			ImGui::DragFloat("MirrorScaleY", &mMirrorScaleY, 0.1f, -1.0f, 1.0f);
		}
		else if (mMode == Mode::Blur || mMode == Mode::MotionBlur)
		{
			ImGui::DragFloat("BlurStrength", &mBlurStrength, 1.0f, 0.0f, 100.0f);
		}
		else if (mMode == Mode::ChromaticAberration)
		{
			ImGui::DragFloat("AberrationValue", &mAberrationValue, 0.001f, 0.001f, 1.0f);
		}
		
	}
}
