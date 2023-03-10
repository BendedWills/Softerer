#pragma once

#include <semaphore>
#include <atomic>
#include <thread>

namespace Softerer
{
	class Semaphore
	{
	public:
		Semaphore(bool startSignaled);
		Semaphore(Semaphore& other);
		Semaphore(Semaphore&& other) noexcept;
		Semaphore& operator=(Semaphore& other) = delete;
		Semaphore& operator=(Semaphore&& other) = delete;

		void Acquire();
		bool TryAcquire();
		void Release();
	private:
		std::atomic<bool> m_IsSignaled;
		std::binary_semaphore m_Semaphore;
	};
}