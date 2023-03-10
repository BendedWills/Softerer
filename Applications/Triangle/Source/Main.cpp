#define _TETHER_STATIC
#include <Tether/Tether.hpp>
#include <Tether/Platform/Win32Window.hpp>

#include <Windows.h>
#include <Softerer/Platform/Win32Surface.hpp>

#include <cstdlib>

int main()
{
	Tether::Platform::Win32Window window(1280, 720, L"garbage triangle", true);

	Softerer::Platform::Win32Surface surface(window.GetHINSTANCE(), window.GetHWND());
	Softerer::Scope<Softerer::FramebufferImage> framebuffer = 
		surface.CreateFramebufferImage(1280, 720);

	while (!window.IsCloseRequested())
	{
		Tether::Application::Get().PollEvents();

		size_t pixelCount = framebuffer->GetWidth() * framebuffer->GetHeight();
		for (size_t i = 0; i < pixelCount; i++)
		{
			uint8_t brightness = (uint8_t)(rand() % 255);
			framebuffer->GetData()[i] = { brightness, brightness, brightness, 255 };
		}

		surface.Present(*framebuffer);
	}

	return 0;
}