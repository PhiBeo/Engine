#pragma once

#include "Model.h"

namespace SpringEngine::Graphics
{
	using ModelId = std::size_t;

	class ModelManager final
	{
	public:
		static void StatticInitialize();
		static void StaticTerminate();
		static ModelManager* Get();

		ModelManager() = default;
		~ModelManager() = default;

		ModelManager(const ModelManager&) = delete;
		ModelManager(const ModelManager&&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&&) = delete;

		ModelId GetModelId(const std::filesystem::path& filePath);
		ModelId LoadModel(const std::filesystem::path& filePath);
		const Model* GetModel(ModelId id);

	private:
		using ModelCache = std::map<ModelId, std::unique_ptr<Model>>;
		ModelCache mInventory;
	};
}