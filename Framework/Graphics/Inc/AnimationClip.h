#pragma once

#include "Animation.h"

namespace SpringEngine::Graphics
{
	struct AnimationClip
	{
		std::string name;
		float tickDuration = 0.0f;
		float ticksPerSec = 0.0f;
		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}