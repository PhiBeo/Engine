#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}

void SpringEngine::Graphics::ModelManager::StatticInitialize()
{
	ASSERT(sModelManager == nullptr, "ModelManager: Is already initialize");
	sModelManager = std::make_unique<ModelManager>();
}

void SpringEngine::Graphics::ModelManager::StaticTerminate()
{
	sModelManager.reset();
}

ModelManager* SpringEngine::Graphics::ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager: was not initialize");
	return sModelManager.get();
}

ModelId SpringEngine::Graphics::ModelManager::GetModelId(const std::filesystem::path& filePath)
{
	return std::filesystem::hash_value(filePath);
}

ModelId SpringEngine::Graphics::ModelManager::LoadModel(const std::filesystem::path& filePath)
{
	const ModelId modelId = GetModelId(filePath);
	auto [iter, success] = mInventory.insert({ modelId, nullptr });
	if (success)
	{
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(filePath, *modelPtr);
		ModelIO::LoadMaterial(filePath, *modelPtr);
		ModelIO::LoadSkeleton(filePath, *modelPtr);
		ModelIO::LoadAnimations(filePath, *modelPtr);
	}
	return modelId;
}

void ModelManager::AddAnimation(ModelId id, const std::filesystem::path& filePath)
{
	auto model = mInventory.find(id);
	ASSERT(model != mInventory.end(), "ModelManager: model was not loaded for animation");
	ModelIO::LoadAnimations(filePath, *model->second);
}

const Model* SpringEngine::Graphics::ModelManager::GetModel(ModelId id)
{
	auto model = mInventory.find(id);
	if (model != mInventory.end())
	{
		return model->second.get();
	}
	return nullptr;
}
