#define _TETHER_STATIC
#include <Tether/Tether.hpp>
#include <Tether/Common/Stopwatch.hpp>
#include <Tether/Platform/Win32Window.hpp>

#include <Windows.h>
#include <Softerer/Platform/Win32Surface.hpp>

#include <Softerer/Context.hpp>

#include <cstdlib>

static constexpr const uint32_t FRAMES_IN_FLIGHT = 4;

int main()
{
	Tether::Platform::Win32Window window(1280, 720, L"garbage triangle", false);
	window.SetResizable(false);
	window.SetVisible(true);

	Softerer::Platform::Win32Surface surface(window.GetHINSTANCE(), window.GetHWND());
	Softerer::Context context;

	std::vector<Softerer::Semaphore> imageAvailableSemaphores;
	std::vector<Softerer::Semaphore> renderFinishedSemaphores;
	std::vector<Softerer::Semaphore> inFlightSemaphores;
	std::vector<Softerer::FramebufferImage> framebuffers;

	imageAvailableSemaphores.resize(FRAMES_IN_FLIGHT, true);
	renderFinishedSemaphores.resize(FRAMES_IN_FLIGHT, true);
	inFlightSemaphores.resize(FRAMES_IN_FLIGHT, true);
	framebuffers.reserve(FRAMES_IN_FLIGHT);

	for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++)
		framebuffers.emplace_back(1280, 720);

	uint32_t currentFrame = 0;
	Tether::Stopwatch deltaTimer;
	Tether::Stopwatch printFpsTimer;
	while (!window.IsCloseRequested())
	{
		float delta = deltaTimer.GetElapsedSeconds();
		deltaTimer.Set();

		if (printFpsTimer.GetElapsedSeconds() >= 1.0f)
		{
			std::cout << "FPS: " << 1.0f / delta << '\n';
			printFpsTimer.Set();
		}

		Tether::Application::Get().PollEvents();

		Softerer::FramebufferImage& framebuffer = framebuffers[currentFrame];

		inFlightSemaphores[currentFrame].Acquire();
		
		Softerer::Semaphore* waitSemaphores[] =
		{
			&imageAvailableSemaphores[currentFrame]
		};

		Softerer::Semaphore* signalSemaphores[] =
		{
			&imageAvailableSemaphores[currentFrame],
			&inFlightSemaphores[currentFrame],
		};

		context.Submit(framebuffer, waitSemaphores, 1, signalSemaphores, 2);
		context.EnqueuePresent(surface, framebuffer, signalSemaphores, 2,
			waitSemaphores, 1);

		currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
	}

	context.WaitIdle();

	return 0;
}