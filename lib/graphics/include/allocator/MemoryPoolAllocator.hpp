#pragma once

#include <cstdlib>
#include <stdexcept>

#include <glm/mat4x4.hpp>

#include "UniformMemory.hpp"

namespace diamond_engine
{
	template<size_t A>
	class MemoryPoolAllocator
	{
		static_assert( !( A & ( A - 1 ) ) ); // Power of 2

	public:
		void allocate(size_t sizeInBytes)
		{
			release();

			const uintptr_t unalignedStart = reinterpret_cast<uintptr_t>(malloc(sizeInBytes + A));

			const size_t mask				= A - 1;
			const uintptr_t misalignment	= (unalignedStart & mask);
			const ptrdiff_t adjustment		= A - misalignment;
			const uintptr_t alignedStart	= unalignedStart + adjustment;

			assert(adjustment <= 0xFF);

			unsigned char* const pAlignedStart = reinterpret_cast<unsigned char*>(alignedStart);
			pAlignedStart[-1] = static_cast<unsigned char>(adjustment);

			m_memoryPool	= static_cast<void*>(pAlignedStart);
			m_position		= m_memoryPool;
			m_seek			= m_position;
			m_bounds		= static_cast<void*>(reinterpret_cast<unsigned char*>(m_position) + sizeInBytes);

			memset(m_memoryPool, NULL, sizeInBytes);
		}

		template<typename T>
		T* requestMemory(const T& value = T())
		{
			static_assert( sizeof(T) % A == 0 );

			const size_t sizeInBytes = sizeof(T);

			unsigned char * const nextPosition = reinterpret_cast<unsigned char*>(m_position) + sizeInBytes;

			if (nextPosition > m_bounds)
				throw std::runtime_error("Attempt to access memory past MemoryPoolAllocator bounds");

			T* result = new (m_position) T(value);

			m_position = static_cast<void*>(nextPosition);

			return result;
		}

		void advanceSeek(size_t sizeInBytes)
		{
			unsigned char* nextPosition = reinterpret_cast<unsigned char*>(m_seek) + sizeInBytes;

			if (nextPosition > m_bounds)
			{
				nextPosition = reinterpret_cast<unsigned char*>(m_bounds);
			}

			m_seek = nextPosition;
		}

		void rewindSeek(size_t sizeInBytes)
		{
			unsigned char* nextPosition = reinterpret_cast<unsigned char*>(m_seek) - sizeInBytes;

			if (nextPosition < m_memoryPool)
			{
				nextPosition = reinterpret_cast<unsigned char*>(m_memoryPool);
			}

			m_seek = nextPosition;
		}

		void freeMemory(size_t sizeInBytes)
		{
			unsigned char* nextPosition = reinterpret_cast<unsigned char*>(m_position) - sizeInBytes;

			if (nextPosition < m_memoryPool)
			{
				nextPosition = reinterpret_cast<unsigned char*>(m_memoryPool);
			}

			m_position = nextPosition;
		}

		void release()
		{
			if (m_memoryPool == nullptr)
			{
				return;
			}

			const unsigned char* pAlignedStart	= reinterpret_cast<unsigned char*>(m_memoryPool);
			const ptrdiff_t adjustment			= static_cast<ptrdiff_t>(pAlignedStart[-1]);

			const uintptr_t alignedStart	= reinterpret_cast<uintptr_t>(m_memoryPool);
			const uintptr_t unalignedStart	= alignedStart - adjustment;

			free(reinterpret_cast<void*>(unalignedStart));

			m_memoryPool	= nullptr;
			m_seek			= nullptr;
			m_bounds		= nullptr;
			m_position		= nullptr;
		}

		ptrdiff_t getSizeInBytes()
		{
			return reinterpret_cast<unsigned char*>(m_bounds) - reinterpret_cast<unsigned char*>(m_memoryPool);
		}

		ptrdiff_t getLiveSizeInBytes()
		{
			return reinterpret_cast<unsigned char*>(m_position) - reinterpret_cast<unsigned char*>(m_memoryPool);
		}

		ptrdiff_t getInactiveSizeInBytes()
		{
			return reinterpret_cast<unsigned char*>(m_bounds) - reinterpret_cast<unsigned char*>(m_position);
		}

		void* peek() const
		{
			return m_seek;
		}

		void* getBlockStart() const
		{
			return m_position;
		}

		void seekToStart()
		{
			m_seek = m_memoryPool;
		}

		~MemoryPoolAllocator()
		{
			release();
		}

	private:
		void* m_bounds{ nullptr };
		void* m_seek{ nullptr };
		void* m_position{ nullptr };
		void* m_memoryPool{ nullptr };
	};

	using GraphicsMemoryPool = MemoryPoolAllocator<8>;
}
