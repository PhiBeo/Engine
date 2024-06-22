#pragma once

namespace SpringEngine
{
	struct ParticleActivateData
	{
		float lifeTime = 0.0f;
		Color colorStart = Colors::White;
		Color colorEnd = Colors::White;
		Math::Vector3 scaleStart = Math::Vector3::One;
		Math::Vector3 scaleEnd = Math::Vector3::One;
		Math::Vector3 position = Math::Vector3::Zero;
		Math::Vector3 velocity = Math::Vector3::Zero;
	};

	struct ParticleInfo
	{
		Color currentColor = Colors::White;
		Math::Vector3 currentScale = Math::Vector3::One;
		
	};

	class Particle
	{
	public:
		Particle() = default;
		
		void Initialize();
		void Terminate();

		void Activate(const ParticleActivateData& data);
		void Update(float deltaTime);

		bool IsActive() const;
		void GetCurrentInfo(ParticleInfo& info) const;
		const Graphics::Transform& GetTransform() const;

	private:
		Graphics::Transform mTransform;
		Physics::RigidBody mRigidBody;
		Physics::CollisionShape mCollisionShape;

		ParticleActivateData mData;
		float mLifeTime = 0.0f;
	};
}