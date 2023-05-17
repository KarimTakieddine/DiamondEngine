#pragma once

#include <cassert>
#include <cstdlib>
#include <stdexcept>

namespace diamond_engine
{
	template<typename T, size_t A>
	class AlignedAllocator
	{
		static_assert(!(A& (A - 1)));
		static_assert(sizeof(T) % A == 0);

	public:
		void Allocate(size_t count)
		{
			Free();

			const size_t allocationSize = (count * sizeof(T)) + A;

			const uintptr_t unalignedStart = reinterpret_cast<uintptr_t>(malloc(allocationSize));

			const size_t mask				= A - 1;
			const uintptr_t misalignment	= (unalignedStart & mask);
			const ptrdiff_t adjustment		= A - misalignment;
			const uintptr_t alignedStart	= unalignedStart + adjustment;

			assert(adjustment <= 255);

			unsigned char* const pAlignedStart	= reinterpret_cast<unsigned char*>(alignedStart);
			pAlignedStart[-1]					= static_cast<unsigned char>(adjustment);

			m_size				= allocationSize;
			m_alignedData		= reinterpret_cast<T*>(pAlignedStart);
			m_topBufferBounds	= m_alignedData + count;
			m_topObjectBounds	= m_alignedData;
			m_current			= m_alignedData;
		}

		void Expand(size_t count)
		{
			T* const updatedTopObjectBounds		= m_topObjectBounds + count;
			const ptrdiff_t bufferBoundsOffset	= updatedTopObjectBounds - m_topBufferBounds;

			if (bufferBoundsOffset > 0)
			{
				throw std::runtime_error("Attempt to expand past maximum buffer size");
			}

			m_topObjectBounds = updatedTopObjectBounds;
		}

		void Free(size_t count)
		{
			T* updatedToObjectBounds			= m_topObjectBounds - count;
			const ptrdiff_t bufferBottomOffset	= updatedToObjectBounds - m_alignedData;

			if (bufferBottomOffset < 0)
			{
				updatedToObjectBounds = m_alignedData;
			}

			const ptrdiff_t currentOffset = m_current - updatedToObjectBounds;

			if (currentOffset > 0)
			{
				m_current = updatedToObjectBounds;
			}

			m_topObjectBounds = updatedToObjectBounds;
		}

		void Free()
		{
			if (m_alignedData == nullptr)
			{
				return;
			}

			const unsigned char* pAlignedStart	= reinterpret_cast<unsigned char*>(m_alignedData);
			const ptrdiff_t adjustment			= static_cast<ptrdiff_t>(pAlignedStart[-1]);

			const uintptr_t alignedStart	= reinterpret_cast<uintptr_t>(m_alignedData);
			const uintptr_t unalignedStart	= alignedStart - adjustment;

			free(reinterpret_cast<void*>(unalignedStart));

			m_size				= 0;
			m_alignedData		= nullptr;
			m_topBufferBounds	= nullptr;
			m_topObjectBounds	= nullptr;
			m_current			= nullptr;
		}

		T* Get()
		{
			T* const current						= m_current;
			T* const updatedCurrent					= current + 1;
			const ptrdiff_t topObjectBoundsOffset	= updatedCurrent - m_topObjectBounds;

			if (topObjectBoundsOffset > 0)
			{
				return nullptr;
			}
			
			m_current = updatedCurrent;

			return current;
		}

		~AlignedAllocator()
		{
			Free();
		}

	private:
		size_t m_size{ 0 };
		T* m_alignedData{ nullptr };
		T* m_topBufferBounds{ nullptr };
		T* m_topObjectBounds{ nullptr };
		T* m_current{ nullptr };
	};
}
