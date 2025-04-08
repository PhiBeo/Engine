#pragma once

#include "BlockAllocator.h"

namespace SpringEngine::Core
{
	template<class DataType>
	class TypeAllocator : private BlockAllocator
	{
	public:
		TypeAllocator(const char* name, size_t capacity)
			: BlockAllocator(name, sizeof(DataType), capacity)
		{

		}

		template<class... Args>
		DataType* New(Args&&... args)
		{
			DataType* instance = static_cast<DataType*>(Allocator());
			new(instance) DataType(std::forward<Args>(args)...);
			return instance;
		}

		void Delete(DataType* ptr)
		{
			if (ptr == nullptr)
			{
				return;
			}
			ptr->DataType();
			Free(ptr);
		}
	};
}