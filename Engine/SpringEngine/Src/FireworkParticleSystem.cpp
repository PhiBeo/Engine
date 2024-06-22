#include "Precompiled.h"
#include "FireworkParticleSystem.h"

#include "EventManager.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void SpringEngine::FireworkParticle::SetParentId(uint32_t id)
{
	mParentId = id;
}

void SpringEngine::FireworkParticle::Update(float deltaTime)
{
	if (mLifeTime > 0.0f)
	{
		Particle::Update(deltaTime);
		if (mLifeTime < 0.0f)
		{
			OnDeath();
		}
	}
}

void SpringEngine::FireworkParticle::OnDeath()
{
	FireworkExplodeEvent feEvent;
	feEvent.fireworkId = mParentId;
	feEvent.position = mTransform.position;
	EventManager::Broadcast(&feEvent);
}

static uint32_t gFireworkId = 0;
SpringEngine::FireworkParticleSystem::FireworkParticleSystem() :
	mUniqueId(++gFireworkId)
{
}

void SpringEngine::FireworkParticleSystem::Terminate()
{
	ParticleSystem::Terminate();
	for (auto& ps : mExplosionEffects)
	{
		ps.Terminate();
	}
}

void SpringEngine::FireworkParticleSystem::Update(float deltaTime)
{
	ParticleSystem::Update(deltaTime);
	for (auto& ps : mExplosionEffects)
	{
		ps.Update(deltaTime);
	}
}

void SpringEngine::FireworkParticleSystem::SetCamera(const Graphics::Camera& camera)
{
	ParticleSystem::SetCamera(camera);
	for (auto& ep : mExplosionEffects)
	{
		ep.SetCamera(camera);
	}
}

void SpringEngine::FireworkParticleSystem::InitializeParticles(uint32_t count)
{
	mParticleIndexes.resize(count);
	mParticles.resize(count);
	for (uint32_t i = 0; i < count; ++i)
	{
		mParticleIndexes[i] = i;
		mParticles[i] = std::make_unique<FireworkParticle>();
		mParticles[i]->Initialize();
	}

	ParticleSystemInfo info;

	info.spawnDelay = 0.0f;
	info.systemLifeTime = 0.0f;
	info.minParticlePerEmit = 20;
	info.maxParticlePerEmit = 30;
	info.minSpawnAngle = -180.0f * 3.141592 / 180.0f;
	info.maxSpawnAngle = 180.0f * 3.141592 / 180.0f;
	info.minSpeed = 5.0f;
	info.maxSpeed = 10.0f;
	info.minLifeTime = 0.5f;
	info.maxLifeTime = 1.0f;
	info.minStartColor = Colors::White;
	info.maxStartColor = Colors::Yellow;
	info.minEndColor = Colors::White;
	info.maxEndColor = Colors::LightBlue;
	info.minStartScale = Math::Vector3::One;
	info.maxStartScale = { 1.5f,1.5f, 1.5f };
	info.minEndScale = Math::Vector3::Zero;
	info.maxEndScale = Math::Vector3::Zero;
	info.maxParticle = 30;
	info.particleTextureId = TextureManager::Get()->LoadTexture("Images/bullet2.png");

	const uint32_t maxExplosionEffects = 10;
	for (uint32_t i = 0; i < maxExplosionEffects; ++i)
	{
		mExplosionEffects[i].Initialize(info);
	}

	EventManager::Get()->AddListener(EventType::FireworkExplode, std::bind(&FireworkParticleSystem::OnExplosionEvent, this, std::placeholders::_1));
}

void SpringEngine::FireworkParticleSystem::OnExplosionEvent(const Event* event)
{
	FireworkExplodeEvent* feEvent = (FireworkExplodeEvent*)event;
	if (feEvent->fireworkId == mUniqueId)
	{
		auto& nextPS = mExplosionEffects[mNextAvailableEffect];
		mNextAvailableEffect = (mNextAvailableEffect + 1) % 10;

		nextPS.SetPosition(feEvent->position);
		nextPS.SpawnParticles();
	}
}
