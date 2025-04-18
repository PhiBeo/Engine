#include "Precompiled.h"
#include "SoundEffectManager.h"
#include "AudioSystem.h"

using namespace SpringEngine;
using namespace SpringEngine::Audio;
using namespace DirectX;

namespace
{
	std::unique_ptr<SoundEffectManager> sSoundEffectManager;
}

void SoundEffectManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sSoundEffectManager == nullptr, "SoundEffectManager: is already initialized");
	sSoundEffectManager = std::make_unique<SoundEffectManager>();
	sSoundEffectManager->SetRootPath(root);
}

void SoundEffectManager::StaticTerminate()
{
	if (sSoundEffectManager != nullptr)
	{
		sSoundEffectManager->Clear();
		sSoundEffectManager.reset();
	}
}

SoundEffectManager* SoundEffectManager::Get()
{
	ASSERT(sSoundEffectManager != nullptr, "SoundEffectManager: is not initialized");
	return sSoundEffectManager.get();
}

SoundEffectManager::~SoundEffectManager()
{
	ASSERT(mInventory.empty(), "SoundEffectManager: clear must be called");
}

void SoundEffectManager::SetRootPath(const std::filesystem::path& fileName)
{
	mRoot = fileName;
}

SoundId SoundEffectManager::Load(const std::filesystem::path& fileName)
{
	std::filesystem::path fullPath = mRoot / fileName;
	std::size_t soundId = std::filesystem::hash_value(fullPath);
	auto [iter, success] = mInventory.insert({ soundId, nullptr });
	if (success)
	{
		AudioSystem* as = AudioSystem::Get();
		auto& soundEventPtr = iter->second;
		soundEventPtr = std::make_unique<Entry>();
		soundEventPtr->effect = std::make_unique<SoundEffect>(as->mAudioEngine, fullPath.wstring().c_str());
		soundEventPtr->instance = soundEventPtr->effect->CreateInstance();
	}
	return soundId;
}

void SoundEffectManager::Clear()
{
	AudioSystem::Get()->Suspend();
	for (auto& entry : mInventory)
	{
		if (entry.second != nullptr)
		{
			entry.second->instance->Stop();
			entry.second->instance.reset();
			entry.second->effect.reset();
			entry.second.reset();
		}
	}

	mInventory.clear();
}

void SoundEffectManager::Play(SoundId id, bool loop)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->instance->Stop();
		iter->second->instance->Play(loop);
	}
}

void SoundEffectManager::Stop(SoundId id)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->instance->Stop();
	}
}

void SoundEffectManager::SetVolume(SoundId id, float v)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->instance->SetVolume(v);
	}
}
