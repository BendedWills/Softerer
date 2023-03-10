#pragma once

#include <Softerer/Surface.hpp>
#include <Softerer/Queue.hpp>
#include <Softerer/Semaphore.hpp>

namespace Softerer
{
	class Context
	{
	public:
		Context();

		void Submit(
			const Softerer::FramebufferImage& framebuffer,
			Semaphore** ppWaitSemaphores = nullptr,
			size_t waitSemaphoreCount = 0,
			Semaphore** ppSignalSemaphores = nullptr,
			size_t signalSemaphoreCount = 0
		);

		void EnqueuePresent(
			Surface& surface,
			const Softerer::FramebufferImage& framebuffer,
			Semaphore** ppWaitSemaphores = nullptr,
			size_t waitSemaphoreCount = 0,
			Semaphore** ppSignalSemaphores = nullptr,
			size_t signalSemaphoreCount = 0
		);

		void WaitIdle();
	private:
		Queue& FindOpenQueue();

		std::vector<Queue> m_Queues;
	};
}