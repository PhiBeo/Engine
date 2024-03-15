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
}

void SpringEngine::Graphics::ModelManager::StaticTerminate()
{
}

ModelManager* SpringEngine::Graphics::ModelManager::Get()
{
	return nullptr;
}

ModelId SpringEngine::Graphics::ModelManager::GetModelId(const std::filesystem::path& filePath)
{
	return ModelId();
}

ModelId SpringEngine::Graphics::ModelManager::LoadModel(const std::filesystem::path& filePath)
{
	return ModelId();
}

const Model* SpringEngine::Graphics::ModelManager::GetModel(ModelId id)
{
	return nullptr;
}
