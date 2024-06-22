#pragma once

#include "ParticleSystem.h"

namespace SpringEngine
{
	class Event;

	class FireworkParticle : public Particle
	{
	public:
		~FireworkParticle() = default;

		void SetParentId(uint32_t id);
		virtual void Update(float deltaTime) override;

	private:
		void OnDeath();
		uint32_t mParentId = 0;
	};

	class FireworkParticleSystem : public ParticleSystem
	{
	public:
		FireworkParticleSystem();
		virtual void Terminate() override;
		virtual void Update(float deltaTime) override;
		virtual void SetCamera(const Graphics::Camera& camera) override;

		template<class Effect>
		void Render(Effect& effect)
		{
			ParticleSystem::Render(effect);
			for (auto& e : mExplosionEffects)
			{
				e.Render(effect);
			}
		}
	protected:
		virtual void InitializeParticles(uint32_t count) override;
		void OnExplosionEvent(const Event* event);

		uint32_t mUniqueId = 0;
		uint32_t mNextAvailableEffect = 0;
		ParticleSystem mExplosionEffects[10];
	};
}