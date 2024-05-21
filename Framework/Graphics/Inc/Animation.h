#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace SpringEngine::Graphics
{
	class Animtion
	{
	public:
		Transform GetTransform(float time) const;
		float GetDuration() const;

	private:
		Math::Vector3 GetPosition(float time) const;
		Math::Quaternion GetRotation(float time) const;
		Math::Vector3 GetScale(float time) const;

		friend class AnimationBuilder;

		PositionKeys mPositionsKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}