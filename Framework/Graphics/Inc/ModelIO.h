#pragma once

namespace SpringEngine::Graphics
{
	struct Model;
	namespace ModelIO
	{
		bool SaveModel(std::filesystem::path filePath, const Model& model);
		void LoadModel(std::filesystem::path filePath, Model& model);
	}
}