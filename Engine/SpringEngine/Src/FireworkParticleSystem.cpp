#include "Precompiled.h"
#include "FireworkParticleSystem.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void FireworkParticle::Initialize()
{
	Particle::Initialize();

	ParticleSystemInfo info;

	info.spawnDelay = 0.0f;
	info.systemLifeTime = 0.0;
	info.minParticlePerEmit = 30;
	info.maxParticlePerEmit = 50;
	info.minTimeBetweenEmit = 0.1f;
	info.maxTimeBetweenEmit = 0.3f;
	info.minSpawnAngle = -180.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 180.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 20.0f;
	info.minLifeTime = 0.5f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::Red;
	info.maxStartColor = Colors::OrangeRed;
	info.minEndColor = Colors::White;
	info.maxEndColor = Colors::LightYellow;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = { 1.5f,1.5f, 1.5f };
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticle = 50;
	info.particleTextureId = TextureManager::Get()->LoadTexture("Images/bullet2.png");

	mExplosionEffect.Initialize(info);
}

void FireworkParticle::Terminate()
{
	Particle::Terminate();
	mExplosionEffect.Terminate();
}

void FireworkParticle::Activate(const ParticleActivateData& data)
{
	Particle::Activate(data);
}

void FireworkParticle::Update(float deltaTime)
{
	if (mLifeTime > 0.0f)
	{
		Particle::Update(deltaTime);
		if (mLifeTime < 0.0f)
		{
			OnDeath();
		}
	}

	if (mExplosionEffect.IsActive())
	{
		mExplosionEffect.Update(deltaTime);
	}
}

void FireworkParticle::SetCamera(const Graphics::Camera& camera)
{
	mExplosionEffect.SetCamera(camera);
}

void FireworkParticle::OnDeath()
{
	mExplosionEffect.SetPosition(mTransform.position);
	mExplosionEffect.Play(0.001f);
}

void FireWorkParticleSystem::SetCamera(const Graphics::Camera& camera)
{
	ParticleSystem::SetCamera(camera);
	for (auto& p : mParticles)
	{
		FireworkParticle* fp = static_cast<FireworkParticle*>(p.get());
		fp->SetCamera(camera);
	}
}

void FireWorkParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndexes.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndexes[i] = i;
		mParticles[i] = std::make_unique<Particle>();
		mParticles[i]->Initialize();
	}
}