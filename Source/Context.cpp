#include <Softerer/Context.hpp>

#include <iostream>

namespace Softerer
{
	Context::Context()
		:
		m_Queues(std::thread::hardware_concurrency() / 2.0f == 0 ? 1 
			   : std::thread::hardware_concurrency() / 2.0f)
	{}

	void Context::Submit(
		const Softerer::FramebufferImage& framebuffer,
		Semaphore** pWaitSemaphores,
		size_t waitSemaphoreCount,
		Semaphore** pSignalSemaphores,
		size_t signalSemaphoreCount
	)
	{
		size_t waitSemaphoresSize = sizeof(Semaphore*) * waitSemaphoreCount;
		size_t signalSemaphoresSize = sizeof(Semaphore*) * waitSemaphoreCount;

		Semaphore** waitSemaphores = (Semaphore**)malloc(waitSemaphoresSize);
		Semaphore** signalSemaphores = (Semaphore**)malloc(signalSemaphoresSize);
		memcpy(waitSemaphores, pWaitSemaphores, waitSemaphoresSize);
		memcpy(signalSemaphores, pSignalSemaphores, signalSemaphoresSize);

		FindOpenQueue().AddWork([&, waitSemaphoreCount, signalSemaphoreCount,
			pWaitSemaphores, pSignalSemaphores]()
		{
			for (size_t i = 0; i < waitSemaphoreCount; i++)
				waitSemaphores[i]->Acquire();

			Softerer::Image::Pixel* framebufferData = framebuffer.GetData();
			size_t pixelCount = framebuffer.GetWidth() * framebuffer.GetHeight();

			for (size_t i = 0; i < pixelCount; i++)
			{
				uint8_t brightness = (uint8_t)(rand() % 255);
				framebufferData[i] = { brightness, brightness, brightness, 255 };
			}

			for (size_t i = 0; i < signalSemaphoreCount; i++)
				signalSemaphores[i]->Release();

			free(waitSemaphores);
			free(signalSemaphores);
		});
	}

	void Context::EnqueuePresent(
		Surface& surface, 
		const Softerer::FramebufferImage& framebuffer,
		Semaphore** pWaitSemaphores,
		size_t waitSemaphoreCount,
		Semaphore** pSignalSemaphores,
		size_t signalSemaphoreCount
	)
	{
		FindOpenQueue().AddWork([&, waitSemaphoreCount, signalSemaphoreCount,
			pWaitSemaphores, pSignalSemaphores]()
		{
			for (size_t i = 0; i < waitSemaphoreCount; i++)
				pWaitSemaphores[i].Acquire();

			surface.Present(framebuffer);

			for (size_t i = 0; i < signalSemaphoreCount; i++)
				pSignalSemaphores[i].Release();
		});
	}

	void Context::WaitIdle()
	{
		for (Queue& queue : m_Queues)
			queue.WaitIdle();
	}

	Queue& Context::FindOpenQueue()
	{
		for (Queue& queue : m_Queues)
			if (!queue.IsInUse())
				return queue;

		return m_Queues[0];
	}
}