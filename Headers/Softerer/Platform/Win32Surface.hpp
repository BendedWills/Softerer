#pragma once

#include <Softerer/Surface.hpp>

#ifdef __INTELLISENSE__
#include <Windows.h>
#endif

namespace Softerer::Platform
{
	class Win32Surface : public Surface
	{
	public:
		Win32Surface(HINSTANCE hinst, HWND hwnd);
		~Win32Surface();

		void Present(const FramebufferImage& framebuffer) override;
	private:
		HINSTANCE m_Hinst = nullptr;
		HWND m_Hwnd = nullptr;

		HDC m_Hdc = nullptr;
	};
}