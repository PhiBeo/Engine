#include "Precompiled.h"
#include "Animation.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time)
	{
		float t = (time - startTime) / (endTime - startTime);
		return t;
	}
}

Transform Animtion::GetTransform(float time) const
{
	Transform transform;
	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);
	return transform;
}

float Animtion::GetDuration() const
{
	return mDuration;
}

Math::Vector3 Animtion::GetPosition(float time) const
{
	for (uint32_t i = 0; i < mPositionsKeys.size(); ++i)
	{
		if (time < mPositionsKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mPositionsKeys[i - 1].time, mPositionsKeys[i].time, time);
				return Math::Lerp(mPositionsKeys[i - 1].key, mPositionsKeys[i].key, t);
			}
			return mPositionsKeys[i].key;
		}
	}

	if (!mPositionsKeys.empty())
	{
		return mPositionsKeys.back().key;
	}

	return Math::Vector3::Zero;
}

Math::Quaternion Animtion::GetRotation(float time) const
{
	for (uint32_t i = 0; i < mRotationKeys.size(); ++i)
	{
		if (time < mRotationKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time);
				return Math::Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, t);
			}
			return mRotationKeys[i].key;
		}
	}

	if (!mRotationKeys.empty())
	{
		return mRotationKeys.back().key;
	}

	return Math::Quaternion::Identity;
}

Math::Vector3 Animtion::GetScale(float time) const
{
	for (uint32_t i = 0; i < mScaleKeys.size(); ++i)
	{
		if (time < mScaleKeys[i].time)
		{
			if (i > 0)
			{
				float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time);
				return Math::Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, t);
			}
			return mScaleKeys[i].key;
		}
	}

	if (!mScaleKeys.empty())
	{
		return mScaleKeys.back().key;
	}

	return Math::Vector3::One;
}
