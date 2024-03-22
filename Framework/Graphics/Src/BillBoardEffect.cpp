#include "Precompiled.h"
#include "BillBoardEffect.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "TextureManager.h"
#include "VertexTypes.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void BillBoardEffect::Initialize()
{
	std::filesystem::path filePath = L"../../Assets/Shaders/DoBillboard.fx";
	mVertexShader.Initialize<Vertex>(filePath);
	mPixelShader.Initialize(filePath);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mBlendState.Initialize(BlendState::Mode::AlphaBlend);

}

void BillBoardEffect::Terminate()
{
	mBlendState.Terminate();
	mSampler.Terminate();
	mConstantBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void BillBoardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mBlendState.Set();
	mConstantBuffer.BindVS(0);
	mSampler.BindPS(0);
}

void BillBoardEffect::End()
{
	Texture::UnbindPS(0);
	BlendState::ClearState();
}

void BillBoardEffect::Render(const RenderObject& renderObject)
{
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matWV = Math::Matrix4::Translation(Math::TransformCoord(renderObject.transform.position, matView));
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	Math::Matrix4 matFinal = matWV * matProj;
	Math::Matrix4 wvp = Math::Transpose(matFinal);

	mConstantBuffer.Update(&wvp);
	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	renderObject.meshBuffer.Render();
}

void BillBoardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
