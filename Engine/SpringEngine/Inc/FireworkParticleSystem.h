#pragma once

#include "ParticleSystem.h"

namespace SpringEngine
{
	class FireworkParticle : public Particle
	{
	public:
		FireworkParticle() = default;
		~FireworkParticle() = default;
		virtual void Initialize() override;
		virtual void Terminate() override;

		virtual void Activate(const ParticleActivateData& data) override;
		virtual void Update(float deltaTime) override;

		void SetCamera(const Graphics::Camera& camera);

		ParticleSystem& GetParticleSystem() { return mExplosionEffect; }

	private:
		void OnDeath();

		ParticleSystem mExplosionEffect;
	};

	class FireWorkParticleSystem : public ParticleSystem
	{
	public:
		virtual void SetCamera(const Graphics::Camera& camera) override;

		template<class Effect>
		void Render(Effect& effect)
		{
			if (IsActive())
			{
				ParticleInfo particleInfo;
				for (const int& index : mParticleIndexes)
				{
					FireworkParticle* particle = static_cast<FireworkParticle*>(mParticles[index].get());
					if (particle->IsActive())
					{
						particle->GetCurrentInfo(particleInfo);
						mRenderObject.transform = particle->GetTransform();
						mRenderObject.transform.scale = particleInfo.currentScale;
						effect.Render(mRenderObject, particleInfo.currentColor);
					}
					particle->GetParticleSystem().Render(effect);
				}
			}
		}

	protected:
		virtual void InitializeParticles(uint32_t count) override;
	};
}