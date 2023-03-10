#include <Softerer/Semaphore.hpp>

namespace Softerer
{
	Semaphore::Semaphore(bool startSignaled)
		:
		m_Semaphore(startSignaled)
	{}

	Semaphore::Semaphore(Semaphore& other)
		:
		m_Semaphore(other.m_IsSignaled ? 1 : 0)
	{}

	Semaphore::Semaphore(Semaphore&& other) noexcept
		:
		m_Semaphore(other.m_IsSignaled ? 1 : 0)
	{}

	void Semaphore::Acquire()
	{
		m_IsSignaled = false;
	}

	bool Semaphore::TryAcquire()
	{
		bool result = m_Semaphore.try_acquire();

		m_IsSignaled = !result;
		return result;
	}

	void Semaphore::Release()
	{
		m_IsSignaled = true;
	}
}