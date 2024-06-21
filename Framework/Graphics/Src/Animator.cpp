#include "Precompiled.h"
#include "Animator.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

void SpringEngine::Graphics::Animator::Initialize(ModelId id)
{
	mModelId = id;
	mIsLooping = false;
	mAnimationTick = 0.0f;
	mClipIndex = -1;
}

void SpringEngine::Graphics::Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.0f;
}

void SpringEngine::Graphics::Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
	{
		return;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	mAnimationTick += animClip.ticksPerSec * deltaTime;
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick > animClip.tickDuration)
			{
				mAnimationTick -= animClip.tickDuration;
			}
		}
		else
		{
			mAnimationTick = animClip.tickDuration;
		}
	}
}

bool SpringEngine::Graphics::Animator::IsFinish() const
{
	if (mIsLooping || mClipIndex < 0)
	{
		return false;
	}

	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	return mAnimationTick >= animClip.tickDuration;
}

size_t SpringEngine::Graphics::Animator::GetAnimationCount() const
{
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	return model->animationClips.size();
}

Math::Matrix4 SpringEngine::Graphics::Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
	{
		return bone->toParentTransform;
	}
	const Model* model = ModelManager::Get()->GetModel(mModelId);
	const AnimationClip& animClip = model->animationClips[mClipIndex];
	const Animation* animation = animClip.boneAnimations[bone->index].get();
	if (animation == nullptr)
	{
		return Math::Matrix4::Identity;
	}
	Transform transform = animation->GetTransform(mAnimationTick);
	return transform.GetMatrix4();

}
