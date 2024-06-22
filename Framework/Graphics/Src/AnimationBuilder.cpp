#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float t, EaseType easeType)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= t, "AnimationBuilder: Cannot add key frames back in time");
		keyframes.emplace_back(value, t, easeType);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Math::Vector3& pos, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionsKeys, pos, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotaionKey(const Math::Quaternion& rot, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mRotationKeys, rot, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Math::Vector3& scale, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mScaleKeys, scale, time, easeType);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddEventKey(AnimationCallback cb, float time)
{
	PushKey(mWorkingCopy.mEventKeys, cb, time, EaseType::Linear);
	mWorkingCopy.mDuration = Math::Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionsKeys.empty() ||
		!mWorkingCopy.mRotationKeys.empty() ||
		!mWorkingCopy.mScaleKeys.empty(),
		"AnimationBuilder: no animation keys are present");
	return mWorkingCopy;
}
